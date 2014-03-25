/**
* @defgroup TalkCore [Talk plugin]
* @brief the Talk widget plugin for Solution Hub Project.
* @note	Talk is a widget to send messages to buddy or receives messages from buddy.
* @note	and it can manage buddy list and groups.
* @note Talk is basic widget for Social Network Service and required for Media Sharing, Alarm widget.
* @note User widget is necessary to login Hub Site.
*/

#include <locale.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ios>
#include <algorithm>
#include <cctype>
#include <dirent.h>

#include <string.h>			// added by k2h 0527
#include <signal.h>

#include "TalkCore.h"


using namespace gloox;       
using std::string;
using namespace std;


#define MESSENGER_DEBUG 		(printf)

#define MD_FULLDIR		("/mtd_rwarea/_Mediasharing")
#define MD_HOMEDIR		("_Mediasharing")
#define PRESENCE_FILE	("_Presence.info")
#define MDTITLE_WORD	("MDSHARE_TITLE")
#define MDDESC_WORD 	("MDSHARE_DESC")

#define EPG_WORD		("EPG:")
#define PRIVATE_WORD	("PRIVATE:")
#define MDSHARE_WORD	("MDSHARE:")


namespace sef {


extern void JS_Command(char* command, char* argv, void* pData);



	
// Gloox
/**
* @brief		constructor of the CGloox
*			   
* @version 1.0
*
*/
CGloox::CGloox()
{
	MESSENGER_DEBUG("[%s][%s][%d] called\n", __FILE__, __FUNCTION__, __LINE__);


	resetAll();

	pthread_mutex_init(&m_tSendMsgMutex, NULL);
	
}


/**
* @brief		destructor of the CGloox
*			   
* @version 1.0
*
*/
CGloox::~CGloox()
{

	MESSENGER_DEBUG("[%s][%s][%d] called\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pClient!=NULL) 
	{
		logOut();
	}


	while(m_aBuddyList.size()>0) 
	{
		LPUSER_INFO usrinfo;
		usrinfo = m_aBuddyList.front();
		SAFE_DEL(usrinfo);
		m_aBuddyList.pop_front(); //remove all
	}	

	
	pthread_mutex_destroy(&m_tSendMsgMutex);


}



/**
* @brief		reset all member variables for constructor, logOut()
*			   
* @version 1.0
*
*/
void CGloox::resetAll(void)
{
	m_nGlooxID = -1;
	m_pClient = NULL;
	
	memset(&m_tSendJobInfo, 0, sizeof(ThreadJob_Inf));
	memset(&m_tMainJobInfo, 0, sizeof(ThreadJob_Inf));
	
	memset(m_sPWD, 0, MAX_USER_PWD_LENGTH+1);
	
	m_pSendMsgSession = NULL;
	m_pHandlerData = NULL;


	m_pVCardHandlerData = NULL;
	
	m_session = 0; //don't delete(free)
	m_messageEventFilter = 0; //warning : memory leak?
	m_chatStateFilter = 0; //warning: memory leak?
	m_VCardManager = NULL;
	m_PrivacyManager = NULL;
	
	m_eSubscription = SUBSCRIPTION_TYPE_BUDDYADD;
	m_nVCardCount = 0;
	m_bConnected = false;
	m_bDisconnected = false;
	
	m_tBuddy_it = m_aBuddyList.begin();
	
	
	m_MyJId.setJID(std::string(""));	

#ifdef TALK_MDSHARE	
	m_pMSHandlerData = NULL;
	m_pAlarmHandlerData = NULL;
	
	/* file transfer */
	m_nFTPort = DEFAULT_FT_PORT;
	m_nFTID = 0;
	m_pS5BServer = NULL;
	m_pSIProfile = NULL;
	
	/* initialize values */
	m_bEPG = false;
	m_bPRIVATE = false;
	m_bMDShare = false;
	memset(m_sMDShareTitle,0,MAX_MDSHARE_TITLE);
	memset(m_sMDShareDesc,0,MAX_MDSHARE_DESC);
	
	
	strncpy(m_sDownloadDir, LOGFILE_PATH, MAX_DOWNLOAD_PATH_LENGTH);
#endif	

}


/**
* @brief		Use this function to start communications with server
*			   
* @param [in]	glooxid - instance id
* @param [in]	id - id pointer of the buddy
* @param [in]	password - password pointer of the buddy
* @param [in]	pHandlerData - widget handler of Talk
* @param [in]	ft_port - file transfer proxy port (default=7777)
*
* @return		bool - true if successful, false if failed
* 
* @version 1.0
*
*/
bool CGloox::startGloox(int glooxid, char* id, char* password, void* pHandlerData, int ft_port)
{  
	int i;
	
	MESSENGER_DEBUG("[%s][%s][%d]: id:%s pw:%s [%x]\n", __FILE__, __FUNCTION__, __LINE__, id, password, (unsigned int)m_pClient);

	m_session = 0;
	m_messageEventFilter = 0;
	m_chatStateFilter = 0;
	m_eSubscription = SUBSCRIPTION_TYPE_BUDDYADD;
	m_bConnected = false;
	m_bDisconnected = false;
	m_nVCardCount = 0;
	m_nGlooxID = glooxid;
	m_pHandlerData = pHandlerData;
	m_tBuddy_it = m_aBuddyList.begin();

	SAFE_DEL(m_VCardManager);
	m_VCardManager = NULL;

	SAFE_DEL(m_PrivacyManager);
	m_PrivacyManager = NULL;	

	while(m_aBuddyList.size()>0) 
	{
		LPUSER_INFO usrinfo;
		usrinfo = m_aBuddyList.front();
		SAFE_DEL(usrinfo);
		m_aBuddyList.pop_front(); //remove all
	}
	
	m_MyJId.setJID(id);
	strncpy(m_sPWD, password, MAX_USER_PWD_LENGTH);

	if(m_pClient)	
	{
		delete (m_pClient);
		m_pClient = NULL;
	}

	m_pClient = new Client(m_MyJId, password);      
	if(m_pClient==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!\n", __FILE__, __FUNCTION__, __LINE__);	
		return false;
	}	

#if 0
/* .../include/gloox/ doesn't include config.h for these definitions (we have to do manually.) */
#if defined( HAVE_RES_QUERYDOMAIN ) && defined( HAVE_DN_SKIPNAME ) && defined( HAVE_RES_QUERY )
	MESSENGER_DEBUG("[%s][%s][%d]: SRV records can be used in this platform!!!\n", __FILE__, __FUNCTION__, __LINE__);
#else
	MESSENGER_DEBUG("[%s][%s][%d]: SRV records cannot be used !!!\n", __FILE__, __FUNCTION__, __LINE__);
	MESSENGER_DEBUG("[%s][%s][%d]: setServer(talk.google.com) !!!\n", __FILE__, __FUNCTION__, __LINE__);
	m_pClient->setServer( "talk.google.com" );
#endif
#endif
	
	m_pClient->registerConnectionListener(this);
	m_pClient->registerMessageSessionHandler(this, 0);
	m_pClient->rosterManager()->registerRosterListener(this, false); //fixed: false = asynchronous subscription
//	m_pClient->rosterManager()->registerRosterListener(this, true);

	m_pClient->disco()->setVersion("TalkWidget", GLOOX_VERSION, "Linux");
	m_pClient->disco()->setIdentity("client", "widget");
	m_pClient->disco()->addFeature(XMLNS_CHAT_STATES);
//	m_pClient->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);
	m_pClient->logInstance().registerLogHandler(LogLevelWarning, LogAreaAll, this);

	
	StringList ca;
	ca.push_back("/LOGFILE_PATH/to/cacert.crt");
	m_pClient->setCACerts(ca);
	
	
	if(m_VCardManager==NULL) 
	{
		m_VCardManager = new VCardManager(m_pClient);

		if(m_VCardManager==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: out of memory!\n", __FILE__, __FUNCTION__, __LINE__);	
			return false;
		}
		
	}

	if(m_PrivacyManager==NULL) 
	{
		m_PrivacyManager = new PrivacyManager(m_pClient);

		if(m_PrivacyManager==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: out of memory!\n", __FILE__, __FUNCTION__, __LINE__);	
			return false;
		}

		m_PrivacyManager->registerPrivacyListHandler( this );
		
	}	

#ifdef TALK_MDSHARE	
	m_pSIProfile = new SIProfileFT( m_pClient, this );
	if(m_pSIProfile==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		delete m_VCardManager;
		delete m_PrivacyManager;
		delete m_pClient;
		return false;
	}

	// you should obtain this using disco, really
	m_pSIProfile->addStreamHost( JID( HOST_SERVER_NAME ), HOST_SERVER_IP, m_nFTPort );

	int count = 0;
	int my_port = m_nFTPort;
	ConnectionError le = ConnNoError;
	
	do
	{
		if(m_pS5BServer) 
		{
			delete m_pS5BServer;
		}
		m_pS5BServer = new SOCKS5BytestreamServer( m_pClient->logInstance(), my_port++ );
		if(m_pS5BServer==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
			delete m_pSIProfile;
			delete m_VCardManager;
			delete m_PrivacyManager;
			delete m_pClient;
			return false;
		}
		
	
		le = m_pS5BServer->listen();
		
	} while(le!=ConnNoError&&++count<1000);

	if( le!= ConnNoError )
	{
		MESSENGER_DEBUG("[%s][%s][%d]: server listening failed!!! err = %d\n", __FILE__, __FUNCTION__, __LINE__, le);
		delete m_pS5BServer;
		delete m_pSIProfile;
		delete m_VCardManager;
		delete m_PrivacyManager;
		delete m_pClient;
		return false;
	}
	
	MESSENGER_DEBUG("[%s][%s][%d]: now SOCK5BytestreamServer listening...\n", __FILE__, __FUNCTION__, __LINE__);
	
	
	m_pSIProfile->registerSOCKS5BytestreamServer( m_pS5BServer );
	//m_pSIProfile->addStreamHost( JID( HOST_SERVER_NAME ), HOST_SERVER_IP, m_nFTPort );
#endif
	
	
	if(m_pClient->connect(false))
	{
		ConnectionError ce = ConnNoError;     

		MESSENGER_DEBUG("[%s][%s][%d]: waiting connection...\n", __FILE__, __FUNCTION__, __LINE__);	
       
		for(i=0;i<600&&!m_bConnected;i++) /* 1 minute wating... */
		{
			ce = m_pClient->recv(DEFAULT_SMALL_WAITTIME * 100);         
			if(ce == ConnNotConnected)
			{
				char temp[DEFAULT_MSGBUF_SIZE+1];
				snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||disconnect", m_nGlooxID);
				sendDisconnectToJS(temp);
	
				SAFE_DEL(m_VCardManager);
				m_VCardManager = NULL;

				SAFE_DEL(m_PrivacyManager);
				m_PrivacyManager = NULL;				
			
				SAFE_DEL(m_pClient);
				m_pClient = NULL;

				
				MESSENGER_DEBUG("[%s][%s][%d]: startGloox() failed!!!\n", __FILE__, __FUNCTION__, __LINE__);
				return false;
			}

		} 

		if(!m_bConnected)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: waiting connection is time-out!!!\n", __FILE__, __FUNCTION__, __LINE__);


			SAFE_DEL(m_VCardManager);
			m_VCardManager = NULL;

			SAFE_DEL(m_PrivacyManager);
			m_PrivacyManager = NULL;			
			
			SAFE_DEL(m_pClient);
			m_pClient = NULL;

			return false;
		}
		else
		{
			/* this connection just means server connection but not logged-in! */
			MESSENGER_DEBUG("[%s][%s][%d]: connection is successful!!!\n", __FILE__, __FUNCTION__, __LINE__);
		}


		m_tMainJobInfo.bStop = false;		
		m_tMainJobInfo.nId = pthread_create(&(m_tMainJobInfo.tThread), NULL, threadMain, (void *) this);
		if (m_tMainJobInfo.nId != 0)
		{
			m_tMainJobInfo.bRun = false;
			MESSENGER_DEBUG("[%s][%s][%d]: Thread Creation Failed..\n",__FILE__,__FUNCTION__,__LINE__);

			SAFE_DEL(m_VCardManager);
			m_VCardManager = NULL;

			SAFE_DEL(m_PrivacyManager);
			m_PrivacyManager = NULL;			
						
			SAFE_DEL(m_pClient);
			m_pClient = NULL;
			
			return false;
		}
		

		/* bug fixed: don't wait "m_tMainJobInfo.bRun==true" forever */
		/* disconnection may cause dead-lock */
		
		usleep(DEFAULT_SMALL_WAITTIME * 500); /* chance to run thread */

#ifdef TALK_MDSHARE
		if(glooxid==0) //this instance is master.
		{
			if(getShareInfoFile(m_MyJId.bare().c_str())==false)
			{
				MESSENGER_DEBUG("[%s][%s][%d]: getShareInfoFile() failed!!!\n", __FILE__, __FUNCTION__, __LINE__);
	
				//check and make user directory
				checkUserDir(m_MyJId.bare().c_str());
		
				//create _Presence.info file
				updatePresenceFile(m_MyJId.bare().c_str());
			}
		
			char* presence_msg;
			int epg, icast, mdshare, bufsize;
			
			epg = (m_bEPG==true)?1:0;
			icast = (m_bPRIVATE==true)?1:0;
			mdshare = (m_bMDShare==true)?1:0;
			bufsize = MAX_MDSHARE_TITLE+MAX_MDSHARE_DESC+strlen(EPG_WORD)+strlen(PRIVATE_WORD)+strlen(MDSHARE_WORD)+12;
			presence_msg = (char*)malloc(bufsize);

			if(presence_msg==NULL)
			{
				MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
				return false;
			}
			
			snprintf(presence_msg,bufsize,"0||%d||%d||%d||%s||%s", epg, icast, mdshare, m_sMDShareTitle, m_sMDShareDesc);
		
			sendPresenceInfoToJS(presence_msg);
			free(presence_msg);
						
			signal(SIGPIPE, SIG_IGN); //signal "SIGPIPE" will be ignored to prevent system halt (it can be happen when cancelft())
		}
#endif
		
        MESSENGER_DEBUG("[%s][%s][%d]: startGloox successful!!!\n", __FILE__,__FUNCTION__,__LINE__);
		return true;
	}


	MESSENGER_DEBUG("[%s][%s][%d]: startGloox() failed!!!\n", __FILE__, __FUNCTION__, __LINE__);
	return false;
}




/**
* @brief	Use this function to reserve messages for sending;
*			   
* @param [in]	pJidBare - bare id pointer of the buddy
* @param [in]	nMsgType - message type
* @param [in]	pMessage - The message to send 
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMessageReserve( char * pJidBare, int nMsgType, char * pMessage)
{
	SMsg_Inf *	pInf;
	
	MESSENGER_DEBUG("[%s][%s][%d] called (JID = %s, msg = %s)\n", __FILE__, __FUNCTION__, __LINE__, pJidBare, pMessage);

	if (!pJidBare || !pMessage) 
	{
		return false;
	}

	pInf = new SMsg_Inf;
	if(pInf==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d] out of memory!!\n",__FILE__,__FUNCTION__,__LINE__);
		return false;
	}
	
	memset(pInf, 0x00, sizeof(SMsg_Inf));

	pInf->nMsgType = nMsgType;
	strncpy(pInf->sJidBare, pJidBare, MAX_JID_LENGTH);
	
	pInf->pszSendMsg = (char*)malloc(strlen(pMessage)+1);
	if(pInf->pszSendMsg==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d] out of memory!!\n",__FILE__,__FUNCTION__,__LINE__);
		delete pInf;
		return false;
	}
	
	strncpy(pInf->pszSendMsg, pMessage, strlen(pMessage)+1); //fixed: include NULL

	pthread_mutex_lock(&m_tSendMsgMutex);
	m_aSendMsgList.push_back(pInf); //push message to queue
	pthread_mutex_unlock(&m_tSendMsgMutex);

	if(m_tSendJobInfo.bRun==true) 
	{
		MESSENGER_DEBUG("[%s][%s][%d] previous sendmessage failed???..\n",__FILE__,__FUNCTION__,__LINE__);
	}
	
	
	
	m_tSendJobInfo.bStop = false;
	m_tSendJobInfo.nId = pthread_create(&(m_tSendJobInfo.tThread), NULL, sendMsgFunc, (void *) this);
	if (m_tSendJobInfo.nId != 0)
	{
		m_tSendJobInfo.bRun = false;
		MESSENGER_DEBUG("[%s][%s][%d] Thread Creation Failed..\n",__FILE__,__FUNCTION__,__LINE__);
	}
	
	return true;
}



/**
* @brief		Removes the given MessageSession from the  list of MessageSessions and deletes it.
*			   
* @param [in]	none
*
* @return		bool - true if successful, false if failed
* 
* @version 1.0
*
*/
bool CGloox::removeSendSession()
{
	MESSENGER_DEBUG("[%s][%s][%d] called\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pClient==NULL) 
	{
		return false;
	}	

	if(m_pSendMsgSession)
	{
		m_pClient->disposeMessageSession(m_pSendMsgSession);
		m_pSendMsgSession = NULL;
	}

	return true;
}



/**
* @brief		Use this function to send a message and count the message;
*			   
* @param [in]	pJidBare - bare id pointer of the buddy
* @param [in]	nMsgType - message type
* @param [in]	pMessage - The message to send 
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMessage(char* pJidBare, int nMsgType, char* pMessage)
{
	char temp[] = "";
	JID tJid(pJidBare);
	//bool bAdded = false;
	MESSENGER_DEBUG("[%s][%s][%d] called. (JID = %s, Msg = %s)\n", __FILE__, __FUNCTION__, __LINE__, pJidBare, pMessage);
	if(m_pClient==NULL || nMsgType < CHAT_MSG_TYPE_DATA || nMsgType > CHAT_MSG_TYPE_CHAT)
	{
		return false;
	}

	if(pMessage==NULL) 
	{
		pMessage = temp;
	}

	if(!m_pSendMsgSession)
	{
		m_pSendMsgSession = new MessageSession(m_pClient, tJid);
	}

	if(!m_pSendMsgSession)
	{
		MESSENGER_DEBUG("[%s][%s][%d] MessageSession error\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
		

	switch(nMsgType)
	{
		case CHAT_MSG_TYPE_DATA:
			m_pSendMsgSession->send(pMessage);
			break;
			
		case CHAT_MSG_TYPE_EVENT:
			break;
			
		case CHAT_MSG_TYPE_CHAT:
			break;

		default:
			break;
	}

	removeSendSession();
	
	MESSENGER_DEBUG("[%s][%s][%d] Send Message Finish \n", __FILE__, __FUNCTION__, __LINE__);
	return true;
} 


/**
* @brief		Use this function to add a contact to the roster and subscribe to a new JID.
*			   
* @param [in]	pJidBare - bare id pointer of the buddy
* @param [in]	pJidName - name pointer of the buddy
* @param [in]	pJidGroup - group pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::addBuddy( char* pJidBare, char* pJidName, char* pJidGroup)
{
	//This will triggers handleItemAdded() for once, and handleItemUpdated() for twice.
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	JID jid(pJidBare);
	StringList groups;	  
	groups.clear();
	groups.push_back(pJidGroup); 
	
	m_eSubscription = SUBSCRIPTION_TYPE_USERADD;
	
	//m_pClient->rosterManager()->add(jid, pJidName, groups); //subscribe() is super-set.
	m_pClient->rosterManager()->subscribe(jid, pJidName, groups, "");
	//m_pClient->rosterManager()->synchronize(); //don't need to do this.
	//m_pClient->rosterManager()->fill(); //fixed: handleItemAdded() will do this!

	return true;
}



/**
* @brief		Use this function to remove a contact from the roster
*			   

* @param [in]	pJidBare - bare id pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::delBuddy( char* pJidBare)
{
	//This will triggers handleItemUpdated() for twice, and handleItemRemoved() for once.
	//Be careful that when buddy removes me, it will not trigger handleItemRemoved().
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	JID jid(pJidBare); 	
	
	m_pClient->rosterManager()->remove(jid); 
	
	return true;
}

/**
* @brief		Use this function to accept a subscription request
*			   
* @param [in]	pJidBare - bare id pointer of the buddy
* @param [in]	pJidName - name pointer of the buddy
* @param [in]	pJidGroup - group pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/

bool CGloox::ackSubsReq( char* pJidBare, char* pJidName, char* pJidGroup)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);


	JID jid(pJidBare);    
	StringList groups;	
	groups.clear();
	groups.push_back(pJidGroup);

	//m_pClient->rosterManager()->add( jid, pJidName, groups );
	m_pClient->rosterManager()->subscribe(jid, pJidName, groups, "");
	m_pClient->rosterManager()->ackSubscriptionRequest(jid, true);		   
	m_pClient->rosterManager()->fill();

	return true;
}

/**
* @brief		Use this function to deny a subscription request
*			   
* @param [in]	pJidBare - bare id pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::unsubscribe( char* pJidBare)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	JID jid(pJidBare);    
	  
	m_pClient->rosterManager()->unsubscribe(jid, ""); 

	return true;
}


/**
* @brief		Disconnents from the server and Save the log & macro files 
*			   
* @param [in]	none
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::logOut()
{
	void* iRet;
	
	MESSENGER_DEBUG("[%s][%s][%d] called\n", __FILE__, __FUNCTION__, __LINE__);

		
	if(!m_pClient)
	{
		MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}		


	//stop message thread
	if (m_tSendJobInfo.bRun)
	{
		m_tSendJobInfo.nId	 = -1;
		m_tSendJobInfo.bStop = true;
		pthread_join(m_tSendJobInfo.tThread, (void **) &iRet);
		m_tSendJobInfo.bStop = false;
	}


#ifdef TALK_MDSHARE
	//file transfer is in progress
	std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
	while(sit != m_aSendList.end())
	{
		if((*sit)->m_pSIProfileFT)
		{

			MESSENGER_DEBUG("[%s][%s][%d] send FT remained!!!\n", __FILE__, __FUNCTION__, __LINE__);

			cancelFT((*sit)->m_nFTID);
		}
	}


	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit != m_aRecvList.end())
	{
		if((*rit)->m_pSIProfileFT)
		{
	
			MESSENGER_DEBUG("[%s][%s][%d] recv FT remained!!!\n", __FILE__, __FUNCTION__, __LINE__);
	
			cancelFT((*rit)->m_nFTID);
		}
	}


	SAFE_DEL(m_pSIProfile);
	m_pSIProfile = NULL;


	if(m_pS5BServer!=NULL) 
	{
		m_pS5BServer->stop();
	}

	SAFE_DEL(m_pS5BServer);
	m_pS5BServer = NULL;
#endif


	removeSendSession();

	if(m_bConnected)
	{
		m_pClient->disconnect();
	}


	while(m_aBuddyList.size()>0) 
	{
		LPUSER_INFO usrinfo;
		usrinfo = m_aBuddyList.front();
		SAFE_DEL(usrinfo);
		m_aBuddyList.pop_front(); //remove all
	}

#ifdef TALK_MDSHARE
	if(m_nGlooxID==0) //master instance
	{
		//check and make user directory
		checkUserDir(m_MyJId.bare().c_str());
		
		//save _Presence.info file
		updatePresenceFile(m_MyJId.bare().c_str());

		//don't enable below to prevent system halt
		//signal(SIGPIPE, SIG_DFL); //signal "SIGPIPE" will be restored to default action		
	}
#endif

	if(m_pClient)
	{
		delete (m_pClient);
		m_pClient = NULL;
	}
	
	resetAll(); //reset all values

	MESSENGER_DEBUG("[%s][%s][%d] logOut finished and all thread&resources cleaned!\n", __FILE__, __FUNCTION__, __LINE__);
	
	return true;	
}



/**
* @brief		Use this function to set the entity's presence. If status is 0, channel information is hidden
*			   
* @param [in]	presence - presence
* @param [in]	status - status of channel information sharing 
* @param [in]	channel - channel information pointer
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::setPresence( int presence, int priority, char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);


	m_pClient->setPresence((Presence)presence , priority, msg);		


	return true; 
}

bool CGloox::setSharedStatus(int shared_presence, char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);


	m_pClient->setSharedStatus(shared_presence, msg);		


	return true; 
}

bool CGloox::getSharedStatus()
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);


	m_pClient->getSharedStatus();		


	return true; 
}


/**
* @brief		Use this function to change the nickname of buddy.
*			   
* @param [in]	id - id pointer of the buddy
* @param [in]	newNickname - new nickname pointer of the buddy
* @param [in]	newGroup - new group pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::setEditedRoster( char* id, char* newNickname, char* newGroup)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(id==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: JID is NULL!!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

	MESSENGER_DEBUG("setEditedRoster: id = %s\n", id);

	if(newNickname)
	{
		MESSENGER_DEBUG("setEditedRoster: new Nickname = %s (strlen = %d)\n", newNickname, strlen(newNickname) );
	}

	if(newGroup)
	{
		MESSENGER_DEBUG("setEditedRoster: new Groupname = %s (strlen = %d)\n", newGroup, strlen(newGroup));
	}

	
	Roster* roster = m_pClient->rosterManager()->roster();	

	Roster::const_iterator it = roster->begin();
	
	for(; it != roster->end(); ++it)
	{
		char tempStr[MAX_JID_LENGTH+1];
		
		memset(tempStr, 0x00, MAX_JID_LENGTH+1);

		StringList g = (*it).second->groups();
		
		// get jid
		strncpy(tempStr, (*it).second->jid().c_str(), MAX_JID_LENGTH);
		
		if(!strcmp(tempStr, id))
		{
			MESSENGER_DEBUG("\n[%s][%s][%d]: roster found! change it!\n", __FILE__, __FUNCTION__, __LINE__);
		
			if(newNickname)
			{
				(*it).second->setName(newNickname);
			}

			if(newGroup)
			{
				g.clear();
				g.push_back(newGroup);
				
				(*it).second->setGroups(g);
			}

		}					
		
	}	   

	m_pClient->rosterManager()->synchronize();	
	m_pClient->rosterManager()->fill();
	
	return true;
}



/**
* @brief		This function does the initial filling of the roster with the current server-side roster
*			   
* @param [in]	none
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::setRosterFill()
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
	m_pClient->rosterManager()->fill();
	return true;
}




/**
* @brief		Use this function to request the vcard of jid.
*			   
* @param [in]	requester - widget handler to feed back result.
* @param [in]	jid - jid pointer of the request
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::requestVCard(void* requester,  char* jid_str)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	m_pVCardHandlerData = requester; //don't check null right now

	JID jid(jid_str);
	
	m_VCardManager->fetchVCard(jid, this);

	return true;
}


/**
* @brief		Use this function to request the block list of jid.
*			   
* @param [in]	requester - widget handler to feed back result.
* @param [in]	jid - jid pointer of the request
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::requestBlockList()
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
	
	m_PrivacyManager->requestBlockList();

	return true;
}

bool CGloox::setBlock(char* jid_str)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
	
	m_PrivacyManager->setBlock(jid_str);

	return true;
}

bool CGloox::unsetBlock(char* jid_str)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
	
	m_PrivacyManager->unsetBlock(jid_str);

	return true;
}

#ifdef TALK_MDSHARE
/**
* @brief		Use this function to request file transfer via socket5bytestream.
*			   
* @param [in]	jid - jid pointer of the request
* @param [in]	filename - filename to transfer
* @param [in]	filedesc -file description message
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
int CGloox::requestFT( char* jid, char* filename, char* filedesc)
{
	FT_Send_Info* pFT;
	
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	struct stat f_stat;
	if( stat( filename, &f_stat ) ) 
	{
		MESSENGER_DEBUG("[%s][%s][%d]: stat() failed!!!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	
	int m_size = f_stat.st_size;

	pFT = (FT_Send_Info*)new FT_Send_Info;
	
	if(pFT==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!\n", __FILE__, __FUNCTION__, __LINE__);
		pFT->m_FTSendFile.close();
		return false;
	}
	
//	memset(pFT, 0, sizeof(FT_Send_Info));

	memset(&pFT->m_FTThread, 0, sizeof(ThreadJob_Inf));

	pFT->m_nFTFileCurrent = 0;
	pFT->m_nFTFilePercent = 0;
	pFT->m_nFTFilePrevPercent = 0;
	pFT->m_pS5B = NULL;
	pFT->m_pS5BServer = NULL;
	pFT->m_pSIProfileFT = NULL;

	pFT->m_FTSendFile.open( filename, std::ios_base::in | std::ios_base::binary );
	if( pFT->m_FTSendFile.fail() ) 
	{
		MESSENGER_DEBUG("[%s][%s][%d]: ifstream failed!!!\n", __FILE__, __FUNCTION__, __LINE__);
		delete pFT;
		return false;
	}

	
	pFT->m_pParent = (void*)this;
	pFT->m_pSIProfileFT = m_pSIProfile;
	
	if(pFT->m_pSIProfileFT==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!\n", __FILE__, __FUNCTION__, __LINE__);
		pFT->m_FTSendFile.close();
		delete pFT;
		return false;
	}
	

	pFT->m_pS5BServer = m_pS5BServer;
	if(pFT->m_pS5BServer==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		pFT->m_FTSendFile.close();
		return false;
	}

/*	
	ConnectionError le = ConnNoError;

	if( ( le = pFT->m_pS5BServer->listen() ) != ConnNoError )
	{
		MESSENGER_DEBUG("[%s][%s][%d]: server listening failed!!! err = %d\n", __FILE__, __FUNCTION__, __LINE__, le);
		pFT->m_FTSendFile.close();
		return false;
	}

	MESSENGER_DEBUG("[%s][%s][%d]: now SOCK5BytestreamServer listening...\n", __FILE__, __FUNCTION__, __LINE__);
*/

	//m_pSIProfileFT = new SIProfileFT( m_pClient, this );
	//pFT->m_pSIProfileFT->registerSOCKS5BytestreamServer( pFT->m_pS5BServer );
	//m_pSIProfileFT->addStreamHost( JID( HOST_SERVER_NAME ), HOST_SERVER_IP, m_nFTPort );
	
	
	pFT->m_nFTFileTotal = m_size;

	if(m_nFTID>65535) 
	{
		m_nFTID = 0; //turn around
	}
	
	pFT->m_nFTID = ++m_nFTID;


	JID to(jid);
	std::string fullfilename(filename);
	std::string filestring;
	size_t found;
	found=fullfilename.find_last_of("/");
	filestring = fullfilename.substr(found+1);
	
	MESSENGER_DEBUG("[%s][%s][%d]: filename(without path) = %s\n", __FILE__, __FUNCTION__, __LINE__, filestring.c_str());

	m_aSendList.push_back(pFT);
	
	pFT->m_FTThread.nId = pthread_create(&(pFT->m_FTThread.tThread), NULL, threadSendFT, (void *)pFT);
	if (pFT->m_FTThread.nId != 0)
	{
		pFT->m_FTThread.bRun = false;
		MESSENGER_DEBUG("[%s][%s][%d] Thread Creation Failed..\n",__FILE__,__FUNCTION__,__LINE__);
	}
	
	pFT->m_sStreamID = pFT->m_pSIProfileFT->requestFT( to, filestring, m_size, "", std::string(filedesc) );
	
	return pFT->m_nFTID; //FT handle (>0)
}


/**
* @brief		Use this function to cancel all file transfer.
*			   
* @param [in]	ft_id - id of file transfer
*
* @return		void
*
* @version 1.0
*
*/
void CGloox::cancelFT(int ft_id)
{
	//void* iRet;
	
	MESSENGER_DEBUG("[%s][%s][%d]: called! ft_id = %d\n", __FILE__, __FUNCTION__, __LINE__, ft_id);
	
	if(m_aSendList.size()>0||m_aRecvList.size()>0)
	{
		std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
		while(sit != m_aSendList.end())
		{
			if((*sit)->m_nFTID==ft_id)
			{
				(*sit)->m_FTThread.bStop = true;
				
//				pthread_join((*sit)->m_FTThread.tThread, (void **) &iRet);

				MESSENGER_DEBUG("[%s][%s][%d]:	transfering is cancelled normally!\n", __FILE__, __FUNCTION__, __LINE__);
				
				return;
			}		
			++sit;
		}
		
		
		std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
		while(rit!= m_aRecvList.end())
		{
			if((*rit)->m_nFTID==ft_id)
			{
				(*rit)->m_FTThread.bStop = true;
				
//				pthread_join((*rit)->m_FTThread.tThread, (void **) &iRet);

				MESSENGER_DEBUG("[%s][%s][%d]:	transfering is cancelled normally!\n", __FILE__, __FUNCTION__, __LINE__);
				
				return; 		
			}		
			++rit;
		}

	}
	else
	{
		MESSENGER_DEBUG("[%s][%s][%d]:	no file transfer is in progress! do nothing!\n", __FILE__, __FUNCTION__, __LINE__);
	}
}
#endif


/**
* @brief		Use this function to join multi-user chat room (group chat).
*			   
* @param [in]	roomname - room name with nickname (ex: "kr|6_0@internetat.tv/hikarufan" )
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::joinMUCRoom(char* roomname)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(roomname==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: roomname is null!!!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

	MUCRoom_Inf* pRoom = new MUCRoom_Inf;
	
	if(pRoom==NULL) 
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

	strncpy(pRoom->pRoomName, roomname, MAX_MUCNAME_LENGTH);
	
    JID nick( roomname );

	pRoom->pRoomHandle = new MUCRoom(m_pClient, nick, this, 0);
	if(pRoom->pRoomHandle==NULL) 
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		delete pRoom;
		return false;
	}	

	pRoom->pRoomHandle->setRequestHistory(0, MUCRoom::HistoryMaxStanzas);

	pRoom->pRoomHandle->join();

	//pRoom->pRoomHandle->getRoomInfo();
	//pRoom->pRoomHandle->getRoomItems(); //you must do this after join MUC room
    
	m_MUCRoomList.push_back(pRoom);

	return true;

}


/**
* @brief		Use this function to leave multi-user chat room
*			   
* @param [in]	roomname - roomname to leave
* @param [in]	msg - leaving message to others
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::leaveMUCRoom(char* roomname, char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);


	if(roomname==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: invalide args\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}	


	std::list<MUCRoom_Inf*>::iterator it = m_MUCRoomList.begin();
	while(it != m_MUCRoomList.end())
	{
		if(!strcmp((*it)->pRoomName, roomname))
		{
			(*it)->pRoomHandle->leave(std::string(msg));
			delete (*it)->pRoomHandle;
			delete (*it);

			m_MUCRoomList.erase(it);
			return true;
		}
		
		++it;
	}
	
	return false;
}


/**
* @brief		Use this function to send a message to MUC chat users
*			   
* @param [in]	rooomname - room name to send a message
* @param [in]	msg - message to send
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::sendMUCMessage(char* roomname, char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
	
	if(roomname==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: invalide args\n", __FILE__, __FUNCTION__, __LINE__);
		return false;		
	}	
	
	list<MUCRoom_Inf*>::iterator it = m_MUCRoomList.begin();
	
	for(; it != m_MUCRoomList.end(); ++it)
	{
		if(!strcmp((*it)->pRoomName, roomname))
		{
			(*it)->pRoomHandle->send(std::string(msg));
			return true;
		}
	}
	
	return false;
}


/**
* @brief		Use this function to invite a buddy to multi-user chat room (group chat).
*			   
* @param [in]	jid - buddy to invite
* @param [in]	roomname - room name with nickname (ex: "kr|6_0@internetat.tv/hikarufan" )
* @param [in]	msg -invitation message to buddy (ex: "hello, it's me" )
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::inviteMUCRoom(char* roomname, char* jid, char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(roomname==NULL||jid==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: arguement is null!!!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

	list<MUCRoom_Inf*>::iterator it = m_MUCRoomList.begin();
	
	for(; it != m_MUCRoomList.end(); ++it)
	{
		if(!strcmp((*it)->pRoomName, roomname))
		{
			JID invitee(jid);
			
			(*it)->pRoomHandle->invite(invitee, std::string(msg), false);
			return true;
		}
	}

	return false;
}


#ifdef TALK_MDSHARE
/**
* @brief		Use this function to decide whether current file transfer is acceptable.
*			   
* @param [in]	jid - jid with resource
* @param [in]	filename - filename to file transfer
* @param [in]	filedesc - file descriptor message from buddy
* @param [in]	size - byte size of file
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::decideFTRequest(const char* jid, const char* filename, const char* filedesc, int size)
{

	std::list<LPUSER_INFO>::const_iterator it = m_aBuddyList.begin();

	if(!strncmp(filedesc, "ms://$thumbnail", 15))
	{
		return false;
	}
	
	for(; it != m_aBuddyList.end(); ++it)
	{
		
		if(!strcmp((*it)->sJidBare, jid))
		{
			MESSENGER_DEBUG("decideFTRequest: file sender is my buddy. it's ok!\n");
			return true;
		}
	}

	MESSENGER_DEBUG("decideFTRequest: file sender is not my buddy!!! rejected!!!\n");
	return false;
}


/**
* @brief		Use this function to set media sharing widget handler for event listen
*			   
* @param [in]	pHandlerData - widget handler for media sharing
* @param [in]	mode - set new one? or restore old one?
*
* @return		void
*
* @version 1.0
*
*/
void  CGloox::setMSWidgetHandle(void* pHandlerData, int mode)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(mode) 
	{
		m_pMSHandlerData = pHandlerData;
	}
	else 
	{
		m_pMSHandlerData = NULL;
	}
}


/**
* @brief		Use this function to get first shared buddy (media sharing buddy list)
*			   
* @param [out]	buddy - first buddy who has sharing information
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::getFirstSharingBuddy(char* buddy, int bufsize)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_aBuddyList.size()==0)
	{
		buddy[0] = 0;
		return false;
	}
		
	m_tBuddy_it = m_aBuddyList.begin();

	for(; m_tBuddy_it != m_aBuddyList.end()&&(*m_tBuddy_it)->bMDShare==false; ++m_tBuddy_it) 
	{
		; /* NULL */
	}

	if(m_tBuddy_it==m_aBuddyList.end())
	{
		buddy[0] = 0;
		return false;
	}

	snprintf(buddy, bufsize, "%d||%s||%s||%s||%s", m_nGlooxID, (*m_tBuddy_it)->sJidBare, (*m_tBuddy_it)->sResrcId[0], (*m_tBuddy_it)->sMDShareTitle, (*m_tBuddy_it)->sMDShareDesc);

	m_tBuddy_it++;
	
	return true;
}


/**
* @brief		Use this function to get shared buddy (media sharing buddy list)
*			   
* @param [out]	buddy - buddy who has sharing information
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::getSharingBuddy(char* buddy, int bufsize)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_aBuddyList.size()==0)
	{
		buddy[0] = 0;
		return false;
	}

	for(; m_tBuddy_it != m_aBuddyList.end()&&(*m_tBuddy_it)->bMDShare==false; ++m_tBuddy_it)
	{
		; /* NULL */
	}
			
	if(m_tBuddy_it==m_aBuddyList.end()) 
	{
		buddy[0] = 0;
		return false;
	}
	
	snprintf(buddy, bufsize, "%d||%s||%s||%s||%s", m_nGlooxID, (*m_tBuddy_it)->sJidBare, (*m_tBuddy_it)->sResrcId[0], (*m_tBuddy_it)->sMDShareTitle, (*m_tBuddy_it)->sMDShareDesc);

	m_tBuddy_it++;

	return true;
}


/**
* @brief		Use this function to decide which directory can be used for writing file
*			   
* @param [in]	dirpath - directory path to save downloaded files
*
* @return		void
*
* @version 1.0
*
*/
bool CGloox::setDownloadDir(char* dirpath)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	DIR*	pDir;
	
	if ((pDir = opendir(dirpath)) == NULL)
	{
		return false;
	}
	
	free (pDir);
	
	strncpy(m_sDownloadDir, dirpath, MAX_DOWNLOAD_PATH_LENGTH);
	
	return true;
}



/**
* @brief		Use this function to set alarm widget handler for event listen
*			   
* @param [in]	pHandlerData - widget handler for alarm
* @param [in]	mode - set new one? or restore old one?
*
* @return		void
*
* @version 1.0
*
*/
void  CGloox::setAlarmWidgetHandle(void* pHandlerData, int mode)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(mode) 
	{
		m_pAlarmHandlerData = pHandlerData;
	}
	else 
	{
		m_pAlarmHandlerData = NULL;
	}
}
#endif


/**
* @brief		Use this function to get my full JID (ex:test00@internetat.tv/TV-0)
*			   
* @param [out]	jid - my full JID
*
* @return		void
*
* @version 1.0
*
*/
void  CGloox::getMyFullJID(char* jid)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	strncpy(jid, m_MyJId.full().c_str(), MAX_JID_LENGTH);
}


/**
* @brief		Use this function to get my PWD 
*			   
* @param [out]	pwd - my password
*
* @return		void
*
* @version 1.0
*
*/
void  CGloox::getMyPWD(char* pwd)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	strncpy(pwd, m_sPWD, MAX_USER_PWD_LENGTH);
}


/**
* @brief		Use this function to get connection status 
*			   
* @param [in]	none
*
* @return		int
*
* @version 1.0
*
*/
int  CGloox::getConnected(void)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	return (int)m_bConnected;
}


#ifdef TALK_MDSHARE
/**
* @brief		Use this function to set presence information
*			   
* @param [in]	epg - epg flag information
* @param [in]	icast - icast flag information
* @param [in]	mdshare- media share flag information
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool  CGloox::setPresenceInfo(int epg, int icast, int mdshare)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	if(epg>=0&&epg<=1)
	{
		m_bEPG = (epg==1)?true:false;
	}

	if(icast>=0&&icast<=1)
	{
		m_bPRIVATE = (icast==1)?true:false;
	}

	if(mdshare>=0&&mdshare<=1)
	{
		m_bMDShare = (mdshare==1)?true:false;
	}

	//check and make user directory
	checkUserDir(m_MyJId.bare().c_str());

	//save presence info file
	updatePresenceFile(m_MyJId.bare().c_str());
	return true;
}




/**
* @brief		check received XMPP msg whether it is real chatting msg or notice msg
*			   
* @param [in]	str - received XMPP msg to check
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::checkNoticeMsg(const std::string &str)
{
	std::string uppers;

	std::string upperstr = str;

	std::transform(upperstr.begin(), upperstr.end(), upperstr.begin(),
				   (int(*)(int)) std::toupper);

	if(upperstr.find(std::string(NOTICE_PREFIX))!=std::string::npos||
		upperstr.find(std::string(RECOMMEND_PREFIX))!=std::string::npos)
	{
		return true;
	}
	else 
	{
		return false;
	}
}



/**
* @brief		read _Presence.Info file to get sharing information
*			   
* @param [in]	userID - My bare JID to read share file
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::getShareInfoFile(const char* userID)
{
	char*	pPresFName;
	FILE*	fd;
	char	buff[1024];
	char*	tok;
	int 	bufsize;

	/* initialize values */
	m_bEPG = false;
	m_bPRIVATE = false;
	m_bMDShare = false;
	memset(m_sMDShareTitle,0,MAX_MDSHARE_TITLE);
	memset(m_sMDShareDesc,0,MAX_MDSHARE_DESC);

	bufsize = strlen(MD_FULLDIR) + strlen(userID) + strlen(PRESENCE_FILE) + 5;
	pPresFName = (char*)malloc(bufsize);
	if(pPresFName==NULL)
	{
		MESSENGER_DEBUG("getShareInfoFile: out of memory!\n");
		return false;
	}
	
	snprintf (pPresFName, bufsize, "%s/%s/%s", MD_FULLDIR, userID, PRESENCE_FILE);

	if ((fd = fopen (pPresFName, "r+")) == NULL)
	{
		MESSENGER_DEBUG("getShareInfoFile: file open failed!\n");
		free(pPresFName);
		return false;
	}
	else
	{
		while (fgets(buff, 1024, fd) != NULL)
		{
			if (!strncmp(EPG_WORD, buff, strlen(EPG_WORD)))
			{
				tok = strchr (buff, ':');
				tok += 1;
				
				(tok[0]=='1')?m_bEPG=true:m_bEPG=false;
				
				MESSENGER_DEBUG("getShareInfoFile:m_bEPG = %d\n", m_bEPG); 	
			
			}

			if (!strncmp(PRIVATE_WORD, buff, strlen(PRIVATE_WORD)))
			{
				tok = strchr (buff, ':');
				tok += 1;
				
				(tok[0]=='1')?m_bPRIVATE=true:m_bPRIVATE=false;
				
				MESSENGER_DEBUG("getShareInfoFile:m_bPRIVATE = %d\n", m_bPRIVATE); 	
			
			}		
			if (!strncmp(MDSHARE_WORD, buff, strlen(MDSHARE_WORD)))
			{
				tok = strchr (buff, ':');
				tok += 1;
				
				(tok[0]=='1')?m_bMDShare=true:m_bMDShare=false;
				
				MESSENGER_DEBUG("getShareInfoFile:m_bMDShare = %d\n", m_bMDShare); 
			
			}
		
			if (!strncmp(MDTITLE_WORD, buff, strlen(MDTITLE_WORD)))
			{
				tok = strchr (buff, ':');
				tok += 1;
				
				MESSENGER_DEBUG("MS:1:shareTitle %s\n", tok); 	

				memset(m_sMDShareTitle, 0, MAX_MDSHARE_TITLE+1);
				strncpy((char*)m_sMDShareTitle, tok, strlen(tok)-1); //fixed: carriage return removal
				MESSENGER_DEBUG("MS:2:shareTitle %s\n", m_sMDShareTitle); 	

			}
			
			if (!strncmp(MDDESC_WORD, buff, strlen(MDDESC_WORD)))
			{
				tok = strchr(buff, ':');
				tok += 1;

				MESSENGER_DEBUG("MS:1:shareDesc %s\n", tok);	

				memset(m_sMDShareDesc, 0, MAX_MDSHARE_DESC+1);
				strncpy((char*)m_sMDShareDesc, tok, strlen(tok)-1); //fixed: carriage return removal

				MESSENGER_DEBUG("MS:2:shareDesc %s\n", m_sMDShareDesc);	
			}

			
		}
	}

	free (pPresFName);
	fclose(fd);

	return true;
}



/**
* @brief		update _Presence.Info file to save my sharing information
*			   
* @param [in]	userID - My bare JID to save information file
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::updatePresenceFile(const char* userID)
{
	char*	pPresFName;
	char*	pNewFName;
	FILE*	srcFd;
	FILE*	dstFd;
	char	buff[1024];
	//char	*tok;
	int 	bufsize;

	bufsize = strlen(MD_FULLDIR) + strlen(userID) + strlen(PRESENCE_FILE) + 5;
	pPresFName = (char*)malloc(bufsize);
	if(pPresFName==NULL)
	{
		MESSENGER_DEBUG("updatePresenceFile: out of memory\n");
		return false;
	}
	pNewFName = (char*)malloc(bufsize);
	if(pNewFName==NULL)
	{
		MESSENGER_DEBUG("updatePresenceFile: out of memory\n");
		free(pPresFName);
		return false;
	}
	
	snprintf (pPresFName, bufsize, "%s/%s/%s", MD_FULLDIR, userID, PRESENCE_FILE);
	snprintf (pNewFName, bufsize, "%s/%s/_tmp.info", MD_FULLDIR, userID);

	dstFd = fopen(pNewFName, "w+"); //temp file

	if(dstFd==NULL)
	{
		MESSENGER_DEBUG("updatePresenceFile: fopen('w+') failed! presence.info can't be saved.\n");
		free(pPresFName);
		free(pNewFName);
		return false;
	}
	
	fwrite (EPG_WORD, strlen(EPG_WORD), 1, dstFd);
//	fwrite (":", 1, 1, dstFd);
	(m_bEPG==true)?fwrite("1\n",2,1, dstFd):fwrite("0\n",2,1, dstFd);
	
	fwrite (PRIVATE_WORD, strlen(PRIVATE_WORD), 1, dstFd);
//	fwrite (":", 1, 1, dstFd);
	(m_bPRIVATE==true)?fwrite("1\n",2,1, dstFd):fwrite("0\n",2,1, dstFd);
	
	fwrite (MDSHARE_WORD, strlen(MDSHARE_WORD), 1, dstFd);
//	fwrite (":", 1, 1, dstFd);
	(m_bMDShare==true)?fwrite("1\n",2,1, dstFd):fwrite("0\n",2,1, dstFd);


	if ((srcFd = fopen(pPresFName, "r+")) == NULL)
	{
		// _Presence.info file not exist
		fflush(dstFd);
		fclose(dstFd);
	}
	else
	{
		// _Presence.info is existed
		
		while (fgets(buff, 1024, srcFd) != NULL)
		{
			if( strncmp(EPG_WORD, buff, strlen(EPG_WORD)) &&
				strncmp(PRIVATE_WORD, buff, strlen(PRIVATE_WORD)) &&
				strncmp(MDSHARE_WORD, buff, strlen(MDSHARE_WORD)))
			{
				fwrite(buff, strlen(buff), 1, dstFd); //write title&desc
			}
		}

		fflush(dstFd);
		fclose(srcFd);
		fclose(dstFd);
		
		remove(pPresFName); //remove old file
		
	}

	rename(pNewFName, pPresFName); //rename new file
		
	free (pPresFName);
	free (pNewFName);	

	return true;
}


/**
* @brief		check user directory for _Presence.info and make it if not exist.
*			   
* @param [in]	userID - My bare JID to save information file
*
* @return		int - return value of mkdir()
*
* @version 1.0
*
*/
int CGloox::checkUserDir(const char* userID)
{
	int 	retValue;
	DIR*	pDir;
	char*	userFullPath;
	int		bufSize;

	bufSize = strlen(MD_FULLDIR) + strlen(userID) + 5;
	userFullPath = (char*)malloc(bufSize);
	if(userFullPath==NULL)
	{
		return -1;
	}
	snprintf (userFullPath, bufSize, "%s/%s", MD_FULLDIR, userID);

	retValue = 1;
	if ((pDir = opendir(userFullPath)) == NULL)
	{
		// Create user directory
		retValue = mkdir(userFullPath, 0755);
	}

	free (userFullPath);
	free (pDir);

	return retValue;
}
#endif




// ConnectionListener
/**
* @brief		This function notifies about successful connections (called by Gloox)
*			   
* @version 1.0
*
*/

void CGloox::onConnect()
{    
	char temp[DEFAULT_MSGBUF_SIZE+1];
    MESSENGER_DEBUG("onConnect: connection event!!!\n");
      
    m_bConnected = true;

	memset(temp, 0, DEFAULT_MSGBUF_SIZE+1); 
	
	snprintf(temp,DEFAULT_MSGBUF_SIZE, "%d", m_nGlooxID);
	
	sendConnectToJS(temp);
}

// ConnectionListener

/**
* @brief		This function notifies about disconnection and its reason
*
* @param [in]	e - The reason for the disconnection.
*			   
* @version 1.0
*
*/
void CGloox::onDisconnect(ConnectionError e)
{
	char temp[DEFAULT_MSGBUF_SIZE+1];
	
	MESSENGER_DEBUG("onDisconnect: disconnect event: %d\n", e);            

	memset(temp, 0, DEFAULT_MSGBUF_SIZE+1); 

	if(e == ConnAuthenticationFailed)
	{
		MESSENGER_DEBUG("Auth. failed. reason: %d\n", m_pClient->authError());
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||authentication", m_nGlooxID);
		sendDisconnectToJS(temp);
	}
	else if(e == ConnStreamClosed)
	{
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||streamclose", m_nGlooxID);
		sendDisconnectToJS(temp);

	}
	else if(e == ConnIoError)
	{
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||connio", m_nGlooxID);
		sendDisconnectToJS(temp);

	}
	else if(e == ConnParseError)
	{
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||connparse", m_nGlooxID);
		sendDisconnectToJS(temp);

	}


	else if(e == ConnUserDisconnected)
	{
		//This can be happen with normal disconnect
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||user", m_nGlooxID);
		sendDisconnectToJS(temp);

	}
	else
	{
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||disconnect", m_nGlooxID);
		sendDisconnectToJS(temp);
	}

	m_bDisconnected = true;
}




// ConnectionListener
/**
* @brief		This function is called when the connection was SSL secured
*
* @param [in]		info - Comprehensive info on the certificate			   
*
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::onTLSConnect(const CertInfo& info)    
{    	
	MESSENGER_DEBUG("status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
		"from: %s\nto: %s\n",
		info.status, info.issuer.c_str(), info.server.c_str(),
		info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
		info.compression.c_str(), ctime((const time_t*)&info.date_from),
		ctime((const time_t*)&info.date_to));

	m_bConnected = true;
              
	return true;
}




//MessageHandler

/**
* @brief		Reimplement this function if you want to be notified about incoming messages.
*
* @param [in]		stanza - The complete Stanza. 		   
* @param [in]		session - If this MessageHandler is used with a MessageSession, this parameter holds a pointer to that MessageSession		   
*
* @version 1.0
*
*/
void CGloox::handleMessage(Stanza* stanza, MessageSession* /*session*/)
{    	
	char *str = NULL;
	char *str2 = NULL;
	//char str_bare[MAX_JID_LENGTH+6];
	JID temp = stanza->from();
	JID tojid = stanza->to();		// added by k2h 0602


	str = (char *)malloc(MAX_MESSAGE_LENGTH+1);
	if(str==NULL)
	{
		MESSENGER_DEBUG("handleMessage: out of memory!!!\n");
	
		return;
	}
	memset(str, 0x00, MAX_MESSAGE_LENGTH+1);	
	
	//memset(str_bare, 0, MAX_JID_LENGTH+6);
    	
	MESSENGER_DEBUG("handleMessage: type: %d, subject: %s, message: %s, thread id: %s", stanza->subtype(),
		stanza->subject().c_str(), stanza->body().c_str(), stanza->thread().c_str());   
	MESSENGER_DEBUG("handleMessage: from: %s\n", temp.bare().c_str());
	
	m_messageEventFilter->raiseMessageEvent(MessageEventDisplayed);
	m_messageEventFilter->raiseMessageEvent(MessageEventComposing);
	m_chatStateFilter->setChatState(ChatStateComposing);

	int count = 1;	
	
	while(strlen(stanza->body().c_str()) >= (unsigned)(MAX_MESSAGE_LENGTH*count))
	{
		count++;
		MESSENGER_DEBUG("MAX_MESSAGE_LENGTH*count : %d\n", MAX_MESSAGE_LENGTH*count);		
		str = (char *)realloc(str, MAX_MESSAGE_LENGTH*count + 1);
		if(str==NULL)
		{
			MESSENGER_DEBUG("handleMessage: out of memory!!!\n");
			
			return;
		}
	}	

	//snprintf(str,MAX_MESSAGE_LENGTH*count,"%d||%s",m_nGlooxID, stanza->body().c_str());
	snprintf(str,MAX_MESSAGE_LENGTH*count,"%d||%s||%s",m_nGlooxID, temp.bare().c_str(), stanza->body().c_str());

#ifdef TALK_MDSHARE
	// added by k2h 0603
	if (!strncmp(stanza->body().c_str(), "ms://$", 6))		// This is command for Mediasharing
	{
		processMSCommand(tojid, temp, stanza->body().c_str());
	}
	else
	{
		//snprintf(str_bare,MAX_JID_LENGTH+4, "%d||%s", m_nGlooxID, temp.bare().c_str());		

		//sendFromIdToJS(str_bare); 
	
		if(!checkNoticeMsg(stanza->body())) 
		{
#endif			
			str2 = convertIntoHTML(str);
		
			sendMessageToJS(str2); /* forward received msg to widget(chatting) */
			
#ifdef TALK_MDSHARE			
		}
		else
		{
			sendNoticeToJS(str); /* forward notice msg(special msg) to alarm/teasing widget */
		}
	}
#endif
	
	if(str) 
	{
		free(str);
	}
	if(str2)
	{
		free(str2);
	}
/*
	if(stanza->body() == "quit")
	{
		m_pClient->disconnect();
	}
*/
}



#ifdef TALK_MDSHARE
// processMSCommand

/**
* @brief		process Mediasharing Message
*
*
* @version 1.0
*
*/

bool CGloox::processMSCommand(const JID& tojid, const JID& fromjid, const char* msg)
{
	// Todo later - check Shared status
	char* filedst = NULL;
	char* filedesc = NULL;

	char* tmpP1  = NULL;
	char* tmpP2 = NULL;
	char* last1;

	int bufsize = MAX_FILE_NAME_LENGTH + MAX_DOWNLOAD_PATH_LENGTH;

	filedst = (char*)malloc(bufsize);
	if(filedst==NULL)
	{
		MESSENGER_DEBUG("processMSCommand: out of memory!!!\n");
	
		return false;
	}

	filedesc = (char*)malloc(bufsize);
	if(filedesc==NULL)
	{
		MESSENGER_DEBUG("processMSCommand: out of memory!!!\n");
		free(filedst);
		return false;
	}


	if (1)
	{
		// request folder list
		if (!strncmp(msg, "ms://$requestfolder/", 20))
		{
			MESSENGER_DEBUG("processMSCommand: Request Folder...: %s\n", msg);
			snprintf(filedst, bufsize, "/mtd_rwarea/_Mediasharing/%s/_Folderlist.xml", tojid.bare().c_str());	

			MESSENGER_DEBUG("processMSCommand: Request Folder...: %s\n", filedst);

			requestFT(const_cast<char*>(fromjid.full().c_str()), filedst, const_cast<char*>("ms://$sendfolder/_Folderlist.xml"));
		}
		else if(!strncmp (msg, "ms://$requestfilelist/", 22))
		{
			MESSENGER_DEBUG("processMSCommand: Request Filelist...: %s\n", msg);
			tmpP1 = strchr(const_cast<char*>(msg), '"') + 1;
			tmpP2 = strtok_r(tmpP1, "\"", &last1);

			snprintf(filedst, bufsize, "%s/_Filelist.xml", tmpP2);

			MESSENGER_DEBUG("processMSCommand: Request Filelist...: %s\n", filedst);

			requestFT(const_cast<char*>(fromjid.full().c_str()), filedst, const_cast<char*>("ms://$sendfilelist/_Filelist.xml"));

		}
		else if (!strncmp (msg, "ms://$requestfile/", 18))
		{
			MESSENGER_DEBUG("processMSCommand: Request File... : %s\n", msg);
			tmpP1 = strchr(const_cast<char*>(msg), '"') + 1;
			tmpP2 = strtok_r(tmpP1, "\"", &last1);

			// get absolute path
			snprintf(filedst, bufsize, "%s", tmpP2);

			// get file name from absolute path
			tmpP1 = strrchr(tmpP2, '/') + 1;
			snprintf(filedesc, bufsize, "ms://$requestedfile/\"%s\"", tmpP1);

			MESSENGER_DEBUG("processMSCommand: Request File...: %s\n", filedst);
			MESSENGER_DEBUG("processMSCommand: Request File desc...: %s\n", filedesc);

			requestFT(const_cast<char*>(fromjid.full().c_str()), filedst, filedesc);	

		}
		else if (!strncmp (msg, "ms://$canceltransfer/", 21))
		{
			int ft_id;
			const char* tmpID;
			MESSENGER_DEBUG("processMSCommand: Cancel File Transfer... : %s\n", msg);
			tmpID = msg + 21;

			// get transfer id
			ft_id = atoi(tmpID);

			MESSENGER_DEBUG("processMSCommand: Cancel ID = %d\n", ft_id);

//			cancelFT(ft_id);
			//file transfer is in progress

			std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
			while(sit != m_aSendList.end())
			{
				if((*sit)->m_pSIProfileFT)
				{
			
					MESSENGER_DEBUG("[%s][%s][%d] send FT cancelled!!!\n", __FILE__, __FUNCTION__, __LINE__);
			
					cancelFT((*sit)->m_nFTID);
				}
				++sit;
			}
			
			
			std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
			while(rit != m_aRecvList.end())
			{
				if((*rit)->m_pSIProfileFT)
				{
			
					MESSENGER_DEBUG("[%s][%s][%d] recv FT cancelled!!!\n", __FILE__, __FUNCTION__, __LINE__);
			
					cancelFT((*rit)->m_nFTID);
				}
				++rit;
			}



		}		
		else 
		{
			MESSENGER_DEBUG("processMSCommand: Error MS command: %s\n", msg);
		}
	}

	if(filedst) 
	{
		free (filedst);
	}
	if(filedesc) 
	{
		free (filedesc);
	}

	return true;
}
#endif


// MessageEventHandler

/**
* @brief		Notifies the MessageEventHandler that an event has been raised by the remote contact.
*
* @param [in]		from - The originator of the Event. 	   
* @param [in]		event - The Event which has been raised. 
*
* @version 1.0
*
*/

void CGloox::handleMessageEvent(const JID& from, MessageEventType event)
{    	
	MESSENGER_DEBUG("received event: %d from: %s\n", event, from.full().c_str());      
}



// ChatStateHandler

/**
* @brief		Notifies the ChatStateHandler that a different chat state has been set by the remote contact
*
* @param [in]		from - The originator of the Event. 	   
* @param [in]		state - The chat state set by the remote entity.
*
* @version 1.0
*
*/
void CGloox::handleChatState(const JID& from, ChatStateType state)
{
	MESSENGER_DEBUG("received state: %d from: %s\n", state, from.full().c_str());
}

// MessageSessionHandler

/**
* @brief		Reimplement this function if you want to be notified about incoming messages by means of automatically created MessageSessions.
*               You receive ownership of the supplied session (not the stanza) and are responsible for deleting it at the end of its life.
*
* @param [in]		session - The new MessageSession. 	   
*
* @version 1.0
*
*/
void CGloox::handleMessageSession(MessageSession* session)
{
	MESSENGER_DEBUG("got new session\n");

	// this example can handle only one session. so we get rid of the old session
	m_pClient->disposeMessageSession(m_session);
	m_session = session;
	m_session->registerMessageHandler(this);
	m_messageEventFilter = new MessageEventFilter(m_session);
	m_messageEventFilter->registerMessageEventHandler(this);
	m_chatStateFilter = new ChatStateFilter(m_session);
	m_chatStateFilter->registerChatStateHandler(this);
}



// LogHandler

/**
* @brief		Reimplement this function if you want to receive the chunks of the conversation between gloox and server.
*
* @param [in]		level - The log message's severity. 	   
* @param [in]		area - The log message's origin.  	
* @param [in]		message - The log message	
*
* @version 1.0
*
*/
void CGloox::handleLog(LogLevel level, LogArea area, const std::string& message)
{
	MESSENGER_DEBUG("log: level: %d, area: %d, %s\n", level, area, message.c_str());      
}



// ConnectionListener

/**
* @brief		This function is called (by a Client object) if an error occurs while trying to bind a resource.
*
* @param [in]		error - Describes the error condition.   	   
*
* @version 1.0
*
*/    
void CGloox::onResourceBindError(ResourceBindError error)
{
	MESSENGER_DEBUG("onResourceBindError: %d\n", error);
}

// ConnectionListener

/**
* @brief		This function is called (by a Client object) if an error occurs while trying to establish a session.
*
* @param [in]		error - Describes the error condition.   	   
*
* @version 1.0
*
*/    
void CGloox::onSessionCreateError(SessionCreateError error)
{
	MESSENGER_DEBUG("onSessionCreateError: %d\n", error);
}

// RosterListener


/**
* @brief		Reimplement this function if you want to be notified about new items on the server-side roster
*				(items subject to a so-called Roster Push).
*				This function will be called regardless who added the item, either this resource or another.
*				However, it will not be called for JIDs for which presence is received without them being on the roster.
*
* @param [in]		jid - The new item's full address.  	   
*
* @version 1.0
*
*/
void CGloox::handleItemAdded(const JID& jid)
{    	
	MESSENGER_DEBUG("handleItemAdded: I added buddy or buddy removed me. (jid = %s)\n", jid.bare().c_str());

	//char temp[MAX_JID_LENGTH+1];

	//memset(temp, 0, MAX_JID_LENGTH+1);
	//strncpy(temp, jid.bare().c_str(),MAX_JID_LENGTH);      
	      
	if(m_eSubscription == SUBSCRIPTION_TYPE_BUDDYADD) //this buddy added(or removed) me!
	{
		//sendSubscriptionToJS(temp);
	}
	else if(m_eSubscription == SUBSCRIPTION_TYPE_USERADD) //I added this buddy
	{
		m_eSubscription = SUBSCRIPTION_TYPE_BUDDYADD; //this is default value.
	}


	//fixed: do nothing for buddylist because handleRoster() will do everything.

	//request roster
	m_pClient->rosterManager()->fill();
	
	MESSENGER_DEBUG("handleItemAdded: done!!!\n");
}



// RosterListener

/**
* @brief		Reimplement this function if you want to be notified about items which authorised subscription.
*
* @param [in]		jid - The authorising item's full address.		   
*
* @version 1.0
*
*/
void CGloox::handleItemSubscribed(const JID& jid)
{
	//when my request is accepted by buddy?
	MESSENGER_DEBUG("handleItemSubscribed: %s\n", jid.bare().c_str()); 
}


// RosterListener

/**
* @brief		Reimplement this function if you want to be notified about items which removed subscription authorization
*
* @param [in]		jid - The item's full address.  	   
*
* @version 1.0
*
*/
void CGloox::handleItemUnsubscribed(const JID& jid)
{
	//when my request is denied by buddy.
	MESSENGER_DEBUG("handleItemUnsubscribed: buddy denied my subscription request! (JID =  %s)\n", jid.bare().c_str());
}



// RosterListener

/**
* @brief		Reimplement this function if you want to be notified about items that were removed from the server-side roster
*				(items subject to a so-called Roster Push).
*				This function will be called regardless who deleted the item, either this resource or another
*
* @param [in]		jid - The removed item's full address.  	   
*
* @version 1.0
*
*/
void CGloox::handleItemRemoved(const JID& jid)
{
	//This will be triggered when I remove this buddy.
	//and when buddy removed me.
	MESSENGER_DEBUG("handleItemRemoved: buddy or I removed each other. (jid = %s)\n", jid.bare().c_str()); 

	//fixed: do nothing for buddy list because handleRoster() will do everything.

	//request roster
	m_pClient->rosterManager()->fill();
}



// RosterListener

/**
* @brief		Reimplement this function if you want to be notified about items that were modified on the server-side roster
*				(items subject to a so-called Roster Push).
*				A roster push is initiated if a second resource of this JID modifies an item stored on the server-side contact list.
*				This can include modifying the item's name, its groups, or the subscription status.
*				These changes are pushed by the server to all connected resources.
*				This is why this function will be called if you modify a roster item locally and synchronize it with the server.
*
* @param [in]		jid - The modified item's full address.  	   
*
* @version 1.0
*
*/
void CGloox::handleItemUpdated(const JID& jid)
{
	//This handle called for many times when I add this buddy
	//addBuddy() triggers twice and buddy's subscription triggers twice too.
	MESSENGER_DEBUG("handleItemUpdated: updated jid = %s\n", jid.bare().c_str()); 
}



// RosterListener

/**
* @brief		Reimplement this function if you want to receive the whole server-side roster on the initial roster push.
*				After successful authentication, RosterManager asks the server for the full server-side roster.
*				Invocation of this method announces its arrival. Roster item status is set to 'unavailable' until incoming presence info updates it.
*				A full roster push only happens once per connection.
*
* @param [in]		roster - The full roster  	   
*
* @version 1.0
*
*/
void CGloox::handleRoster(const Roster& roster)
{
	char * str = NULL;
	char * temp_str = NULL;
	char * retStr = NULL;
	LPUSER_INFO usrinfo = NULL;
	char idstr[DEFAULT_MSGBUF_SIZE+1];

	memset(idstr, 0, DEFAULT_MSGBUF_SIZE+1);

	if(roster.size()==0)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: rosterlist is empty!\n", __FILE__, __FUNCTION__, __LINE__);
		char tmpstr[DEFAULT_MSGBUF_SIZE+1];
		
		memset(tmpstr, 0, DEFAULT_MSGBUF_SIZE+1);
		
		snprintf(idstr,DEFAULT_MSGBUF_SIZE,"%d", m_nGlooxID);
		snprintf(tmpstr,DEFAULT_MSGBUF_SIZE,"%d$$", m_nGlooxID);
		sendRosterToJS(idstr, tmpstr);				
		return;
	}	

	temp_str = (char *)malloc(BUDDYLIST_STRING_SIZE+1);	
	if(temp_str==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: temp_str is NULL!\n", __FILE__, __FUNCTION__, __LINE__);
		return;
	}
	
	memset(temp_str, 0x00, BUDDYLIST_STRING_SIZE+1);
	
	//MESSENGER_DEBUG("roster arriving\nitems:\n");

	//clear buddy list.
	while(m_aBuddyList.size()>0) 
	{
		usrinfo = m_aBuddyList.front();
		SAFE_DEL(usrinfo);
		m_aBuddyList.pop_front(); //remove all
	}


	Roster::const_iterator it = roster.begin();

	for(; it != roster.end(); ++it)
	{	
		usrinfo = new USER_INFO;
		if(usrinfo==NULL)
		{
			MESSENGER_DEBUG("handleRoster: memory alloc failed!\n");
			if(temp_str)
			{
				free(temp_str);
			}
			if(retStr)
			{
				free(retStr);
			}
			if(str)
			{
				free(str);
			}

			return;
		}
		
		memset(usrinfo, 0x00, sizeof(USER_INFO));
		// get jid
		strncpy(usrinfo->sJidBare, (*it).second->jid().c_str(), MAX_JID_LENGTH);

		// get user nickname
		strncpy(usrinfo->sUsrName, (*it).second->name().c_str(), MAX_USER_NAME_LENGTH);
		
		// get user id
		std::string userid = (*it).second->jid().substr(0,(*it).second->jid().find(std::string("@")));
		
		strncpy(usrinfo->sUsrId, userid.c_str(), MAX_USER_ID_LENGTH);
		
		// get subscript
		usrinfo->nSubscript = (*it).second->subscription();		
				
		usrinfo->nChatCnt = 0;
				
			
		//MESSENGER_DEBUG("handleRoster: jid= %s, name= %s, subscription= %d\n", (*it).second->jid().c_str(), (*it).second->name().c_str(),
		//	(*it).second->subscription());

		if(usrinfo->nSubscript==RosterItem::S10nNone)
		{
			MESSENGER_DEBUG("handleRoster: subscription is zero! discard this buddy!\n");
			delete usrinfo;
			continue;
		}

		if((*it).second->subscription()==RosterItem::S10nFrom) //not yet subscribed for me!!
		{
			m_pClient->rosterManager()->subscribe((*it).second->jid(), (*it).second->name(), (*it).second->groups(), ""); //send subscription request
		}


		StringList g = (*it).second->groups();
		
//		MESSENGER_DEBUG("[%s][%s][%d]\n\n\n", __FILE__, __FUNCTION__, __LINE__);
		

		StringList::const_iterator it_g = g.begin();
		for(; it_g != g.end(); ++it_g)
		{
			strncpy(usrinfo->sGroupId[usrinfo->nGroupCnt], (*it_g).c_str(), MAX_GROUP_NAME_LENGTH);
			usrinfo->nGroupCnt++;
			
			MESSENGER_DEBUG("\tgroup: %s\n", (*it_g).c_str());
		}
		
		RosterItem::ResourceMap::const_iterator rit = (*it).second->resources().begin();
		for(; rit != (*it).second->resources().end(); ++rit)
		{
			strncpy(usrinfo->sResrcId[usrinfo->nResrcCnt], (*rit).first.c_str(), MAX_RESRC_NAME_LENGTH);
			usrinfo->nResrcCnt++;
			
			MESSENGER_DEBUG("resource: %s\n", (*rit).first.c_str());
		}

		//rosteritem's resources is not same with resource name.
		//actually resource name is not valid yet. presence will give you resource info.
		//buddy = "hikarufan@internetat.tv||hikarufan||Nickname||Friends||||8"
		snprintf(temp_str, BUDDYLIST_STRING_SIZE, "%s||%s||%s||%s||%s||%ld||", usrinfo->sJidBare, usrinfo->sUsrId, usrinfo->sUsrName,
				usrinfo->sGroupId[0], usrinfo->sResrcId[0], usrinfo->nSubscript); 

		

		int tempStringLength = 0;
			
		if(str)
		{
			tempStringLength = strlen(str) + strlen(temp_str);
		}
		else
		{					
			tempStringLength = strlen(temp_str);			
		}

		if(retStr)
		{
			free(retStr);
		}
		retStr = (char*)malloc(tempStringLength+DEFAULT_MSGBUF_SIZE); //to add glooxid
		if(retStr==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: retStr is NULL!\n", __FILE__, __FUNCTION__, __LINE__);
			if(usrinfo)
			{
				delete usrinfo;
			}
			if(temp_str)
			{
				free(temp_str);
			}

			if(str)
			{
				free(str);
			}			
			return;
		}
		
		memset(retStr, 0x00, tempStringLength+DEFAULT_MSGBUF_SIZE);

	
		if(str == NULL)
		{
			//first buddy is just gloox id (ex: "0$$buddy")
			snprintf(retStr,tempStringLength+DEFAULT_MSGBUF_SIZE,"%d$$%s", m_nGlooxID,temp_str);
		}
		else
		{
			//make "buddy$$buddy" string
			snprintf(retStr,tempStringLength+DEFAULT_MSGBUF_SIZE,"%s$$%s", str, temp_str);
			free(str);
		}		

		str = (char*)malloc(strlen(retStr)+1);
		if(str==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: str is NULL!\n", __FILE__, __FUNCTION__, __LINE__);
			if(temp_str)
			{
				free(temp_str);
			}
			if(retStr)
			{
				free(retStr);
			}
			
			if(usrinfo)
			{
				delete usrinfo;
			}
			return;
		}
		
		memset(str, 0x00, strlen(retStr)+1);

		strncpy(str, retStr, strlen(retStr)+1); //fixed: include NULL

//		MESSENGER_DEBUG("1111 strlen(str) : %d\n\n", strlen(str));

//		MESSENGER_DEBUG("2222 strlen(str) : %d\n\n", strlen(retStr));


		bool is_updated = false;
		
		std::list<LPUSER_INFO>::const_iterator it2 = m_aBuddyList.begin();
		
		for(; it2 != m_aBuddyList.end(); ++it2)
		{
			if(!strcmp((*it2)->sJidBare, usrinfo->sJidBare)) //is already there!
			{
				MESSENGER_DEBUG("[%s][%s][%d]: buddylist is updated with new info.\n", __FILE__, __FUNCTION__, __LINE__);
			
				strncpy((*it2)->sUsrName, usrinfo->sUsrName, MAX_USER_NAME_LENGTH); //copy for update
				strncpy((*it2)->sGroupId[0], usrinfo->sGroupId[0], MAX_GROUP_NAME_LENGTH); //copy for update
				strncpy((*it2)->sResrcId[0], usrinfo->sResrcId[0], MAX_RESRC_NAME_LENGTH); //copy for update
				(*it2)->nSubscript = usrinfo->nSubscript; //copy for update
				
				/* other information is maintained with old one (presence info,.. etc) */
				
				is_updated = true;
			}
		}

		if(is_updated==false) 
		{
			m_aBuddyList.push_back(usrinfo);
		}
		else if(usrinfo)
		{
			delete usrinfo;
			usrinfo = NULL;
		}
		
	}


				
//	MESSENGER_DEBUG("[%s][%s][%d] trying passRoster...\n", __FILE__, __FUNCTION__, __LINE__);

	snprintf(idstr,DEFAULT_MSGBUF_SIZE,"%d", m_nGlooxID);

	if(str!=NULL)
	{
		sendRosterToJS(idstr, str);
	}
	else
	{
		MESSENGER_DEBUG("[%s][%s][%d]: rosterlist is empty!\n", __FILE__, __FUNCTION__, __LINE__);
		char tmpstr[DEFAULT_MSGBUF_SIZE+1];
		snprintf(idstr,DEFAULT_MSGBUF_SIZE,"%d", m_nGlooxID);
		snprintf(tmpstr,DEFAULT_MSGBUF_SIZE,"%d$$", m_nGlooxID);
		sendRosterToJS(idstr, tmpstr);
	}
 
 	if(temp_str) 
 	{
 		free(temp_str);
 	}
	if(str) 
	{
		free(str);
	}
	if(retStr) 
	{
		free(retStr);
	}
	
}


/**
* @brief		This function is called if the server returned an error. 
*
* @param [in]		stanza - The error stanza
*
* @version 1.0
*
*/    
void CGloox::handleRosterError(Stanza* /*stanza*/)
{
	MESSENGER_DEBUG("an roster-related error occured\n");
}

// RosterListener

/**
* @brief		This function is called on every status change of an item in the roster.
*				If the presence is of type Unavailable, then the resource has already been removed from the RosterItem. 
*
* @param [in]		item - The roster item 
* @param [in]		resource - The resource that changed presence.
* @param [in]		presence - The item's new presence.
* @param [in]		msg - The status change message 
*
* @version 1.0
*
*/   
void CGloox::handleRosterPresence(const RosterItem& item, const std::string& resource, Presence presence, const std::string& msg)
{
	MESSENGER_DEBUG("handleRosterPresence: %s/%s -- %d, Msg: %s\n", item.jid().c_str(), resource.c_str(), presence, msg.c_str());
        
	char sPresence[MAX_MESSAGE_LENGTH+1];
	const char* pPresenceName;

	if(resource.length()>LENGTH_PCWEB_MS_MARKER)
	{
		std::string msmarker = resource.substr(resource.length()-LENGTH_PCWEB_MS_MARKER, LENGTH_PCWEB_MS_MARKER); //last 3 chars

		if(msmarker==std::string(PCWEB_MS_MARKER)) //This is PC-Web MS presence
		{
			MESSENGER_DEBUG("handleRosterPresence: Presence of PC-Web Media Sharing will be ignored.\n");
			return; //ignore this
		}
	}

	memset(sPresence, 0x00, MAX_MESSAGE_LENGTH+1);

	switch(presence)
	{
		case 0:
			pPresenceName ="UnKnown";
			break;
		case 1:
			pPresenceName = "Available";
			break;
		case 2:
			pPresenceName = "Chat";
			break;
		case 3:
			pPresenceName = "Away";
			break;
		case 4:
			pPresenceName = "Dnd";
			break;
		case 5:
			pPresenceName = "Xa";
			break;
		case 6:
			pPresenceName = "Unavailable";
			break;
/*
		case 7:
			pPresenceName = "Broadcasting";
			break;
		case 8:
			pPresenceName = "Watching";
			break;
*/			
		default:
			pPresenceName = "UnKnown";
			break;
	}


	snprintf(sPresence,MAX_MESSAGE_LENGTH,"%d||%s||%s||%s", m_nGlooxID,item.jid().c_str(), pPresenceName, msg.c_str());


	std::list<LPUSER_INFO>::const_iterator it = m_aBuddyList.begin();
	
	for(; it != m_aBuddyList.end(); ++it)
	{
		
		if(!strcmp((*it)->sJidBare, item.jid().c_str())) //found buddy in list
		{
			size_t prepos, nexpos;
			std::string uppermsg;
			
			MESSENGER_DEBUG("handleRosterPresence: buddy found in list! presence updated.\n");

			strncpy((*it)->sResrcId[0], resource.c_str(), MAX_RESRC_NAME_LENGTH);
			
			(*it)->nPresence = presence;
			
			strncpy((*it)->sPresenceDesc, msg.c_str(), MAX_PRESENCEDESC_LENGTH);

			uppermsg = msg;
			
			std::transform(uppermsg.begin(), uppermsg.end(), uppermsg.begin(),
						   (int(*)(int)) std::toupper);

			
			prepos = uppermsg.find(MDSHARE_PREFIX);


			if(prepos!=std::string::npos)
			{
				std::string tempstr;
				//found "$MDSHARE/1/"
				(*it)->bMDShare = true;
				prepos += std::string(MDSHARE_PREFIX).length();

				nexpos = msg.find(std::string(":$"), prepos); //find(pattern, index)
				tempstr = msg.substr(prepos, nexpos); //std::npos means end of string in this case

				std::string title = tempstr.substr(0, tempstr.find(std::string("/"))); //substr(index, length)
				std::string desc = tempstr.substr(tempstr.find(std::string("/"))+1);

				strncpy((*it)->sMDShareTitle, title.c_str(), MAX_MDSHARE_TITLE);
				strncpy((*it)->sMDShareDesc, desc.c_str(), MAX_MDSHARE_DESC);

				MESSENGER_DEBUG("handleRosterPresence: MDShare Found!!! (title=%s,desc=%s)\n", (*it)->sMDShareTitle, (*it)->sMDShareDesc);

			}
			else
			{
				(*it)->bMDShare = false;
				(*it)->sMDShareTitle[0] = 0;
				(*it)->sMDShareDesc[0] = 0;
			}
		}
	}


	sendRosterPresenceToJS(sPresence);

	
}



// RosterListener

/**
* @brief		This function is called on every status change of a JID that matches the Client's own JID.
*				If the presence is of type Unavailable, then the resource has already been removed from the RosterItem. 
*
* @param [in]		item - The self item 
* @param [in]		resource - The resource that changed presence.
* @param [in]		presence - The item's new presence.
* @param [in]		msg - The status change message 
*
* @version 1.0
*
*/
void CGloox::handleSelfPresence(const RosterItem& item, const std::string& resource, Presence presence, const std::string& msg)
{
	MESSENGER_DEBUG("self presence received: %s/%s -- %d, Msg: %s\n", item.jid().c_str(), resource.c_str(), presence, msg.c_str());

	char sPresence[MAX_MESSAGE_LENGTH+1];
	const char* pPresenceName;

	if(resource.length()>LENGTH_PCWEB_MS_MARKER)
	{
		std::string msmarker = resource.substr(resource.length()-LENGTH_PCWEB_MS_MARKER, LENGTH_PCWEB_MS_MARKER); //last 3 chars

		if(msmarker==std::string(PCWEB_MS_MARKER)) //This is PC-Web MS presence
		{
			MESSENGER_DEBUG("handleRosterPresence: Presence of PC-Web Media Sharing will be ignored.\n");
			return; //ignore this
		}
	}

	memset(sPresence, 0x00, MAX_MESSAGE_LENGTH+1);

	switch(presence)
	{
		case 0:
			pPresenceName ="UnKnown";
			break;
		case 1:
			pPresenceName = "Available";
			break;
		case 2:
			pPresenceName = "Chat";
			break;
		case 3:
			pPresenceName = "Away";
			break;
		case 4:
			pPresenceName = "Dnd";
			break;
		case 5:
			pPresenceName = "Xa";
			break;
		case 6:
			pPresenceName = "Unavailable";
			break;
/*
		case 7:
			pPresenceName = "Broadcasting";
			break;
		case 8:
			pPresenceName = "Watching";
			break;
*/			
		default:
			pPresenceName = "UnKnown";
			break;
	}


	snprintf(sPresence,MAX_MESSAGE_LENGTH,"%d||%s||%s||%s", m_nGlooxID,item.jid().c_str(), pPresenceName, msg.c_str());

	//sendSelfPresenceToJS(sPresence);
}



// RosterListener

/**
* @brief		This function is called when an entity wishes to subscribe to this entity's presence.
*				If the handler is registered as a asynchronous handler for subscription requests, the return value of this function is ignored.
*				In this case you should use RosterManager::ackSubscriptionRequest() to answer the request. 
*
* @param [in]		jid - The requesting item's address
* @param [in]		msg - A message sent along with the request
*
* @version 1.0
*
*/
bool CGloox::handleSubscriptionRequest(const JID& jid, const std::string& /*msg*/)
{
	/*  상대가 내 presence를 원하면 발생  */
	/*  버디가 먼저 나를 추가해도 발생하지만, 내가 먼저 버디를 추가해도 버디가 accept한 후 발생.   */
	//when buddy added me : handleSubscriptionRequest() ->handleItemAdded() -> handleRoster()
	char sSubscribe[MAX_MESSAGE_LENGTH+1];
	
	MESSENGER_DEBUG("handleSubscriptionRequest: buddy added me. subscription is requested. (jid = %s)\n", jid.bare().c_str());

	//m_pClient->rosterManager()->fill(); //this is no effect.
	memset(sSubscribe, 0, MAX_MESSAGE_LENGTH+1);

	snprintf(sSubscribe, MAX_MESSAGE_LENGTH, "%d||%s", m_nGlooxID, jid.bare().c_str());
	sendSubscriptionToJS(sSubscribe);
	
	return true; 
}


// RosterListener

/**
* @brief		This function is called when an entity unsubscribes from this entity's presence.
*				If the handler is registered as a asynchronous handler for subscription requests, the return value of this function is ignored.
*				In this case you should use RosterManager::unsubscribe() if you want to unsubscribe yourself from the contct's presence and to remove the contact from the roster. 
*
* @param [in]		jid - The item's address
* @param [in]		msg - A message sent along with the request
*
* @version 1.0
*
*/
bool CGloox::handleUnsubscriptionRequest(const JID& jid, const std::string& /*msg*/)
{
	//This is triggered when buddy removes me in buddylist.
	//And handleItemRemoved() will not be called!!!
	
	//when buddy removes me : handleItemUpdated() -> handleUnsubscriptionRequest() -> handleItemAdded() -> handleItemUnsubscribed()

	MESSENGER_DEBUG("handleUnsubscriptionRequest: buddy removed me (jid = %s)\n", jid.bare().c_str());
	
	//m_pClient->rosterManager()->fill();
	
	return true;
}



// RosterListener

/**
* @brief		  This function is called whenever presence from an entity is received which is not in the roster.
*
* @param [in]		stanza - The full presence stanza
*
* @version 1.0
*
*/
void CGloox::handleNonrosterPresence(Stanza* stanza)
{
	MESSENGER_DEBUG("received presence from entity not in the roster: %s\n", stanza->from().full().c_str());
}



/* VCard Handler */
/**
 * @brief		This function is called when a VCard has been successfully fetched.
 *		 		The VCardHandler becomes owner of the VCard object and is responsible for deleting it.
 *
 * @param[in] 	jid - The JID to which this VCard belongs.
 * @param[in] 	vcard - The fetched VCard. Zero if there is no VCard for this contact.
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleVCard( const JID& jid, VCard *vcard )
{
	char* vcard_msg;
	char photo_path[MAX_FILE_NAME_LENGTH+1] = "";
	char phone_number[DEFAULT_MSGBUF_SIZE+1] = "";
	VCard::TelephoneList::const_iterator it;
	
	++m_nVCardCount;

	
	if( !vcard ) 	
	{
		MESSENGER_DEBUG( "empty vcard!\n" );
		return;	  
	}
	
	MESSENGER_DEBUG( "received vcard for %s: %d\n", jid.full().c_str(), m_nVCardCount );
	
	
	vcard_msg = (char *)malloc(MAX_VCARD_LENGTH+1);
	if(vcard_msg==NULL)
	{
		MESSENGER_DEBUG("handleVCard: out of memory!!!\n");
		return;
	}

	memset(vcard_msg, 0, MAX_VCARD_LENGTH+1);

	if(!vcard->telephone().empty())
	{
		it = vcard->telephone().begin();
		snprintf(phone_number, DEFAULT_MSGBUF_SIZE, "%s", (*it).number.c_str());
	}

    if(!vcard->photo().type.empty())
	{
		if(!strcmp(vcard->photo().type.c_str(),"image/png"))
		{
			snprintf(photo_path,MAX_FILE_NAME_LENGTH,"%s/%s.%s.png", LOGFILE_PATH, VCARD_PHOTOFILE_NAME, jid.bare().c_str());
		}
		else if(!strcmp(vcard->photo().type.c_str(),"image/jpg")||!strcmp(vcard->photo().type.c_str(),"image/jpeg"))
		{
			snprintf(photo_path,MAX_FILE_NAME_LENGTH,"%s/%s.%s.jpg", LOGFILE_PATH, VCARD_PHOTOFILE_NAME, jid.bare().c_str());
		}
		else if(!strcmp(vcard->photo().type.c_str(),"image/gif"))
		{
			snprintf(photo_path,MAX_FILE_NAME_LENGTH,"%s/%s.%s.gif", LOGFILE_PATH, VCARD_PHOTOFILE_NAME, jid.bare().c_str());
		}
		else 
		{
			MESSENGER_DEBUG( "unknown photo type! type = %s", vcard->photo().type.c_str());
			snprintf(photo_path,MAX_FILE_NAME_LENGTH,"%s/%s.%s.dat", LOGFILE_PATH, VCARD_PHOTOFILE_NAME, jid.bare().c_str());
		}
		
		
		std::ofstream fout(photo_path,ios::binary|ios::trunc);
		fout.write((const char *)vcard->photo().binval.c_str(), vcard->photo().binval.size());
		fout.close();      
	}


	MESSENGER_DEBUG("handleVCard: %s, %s, %s, %s, %s, %s, %s\n",jid.bare().c_str(), vcard->nickname().c_str(), vcard->formattedname().c_str(), vcard->bday().c_str(), phone_number, vcard->url().c_str(), photo_path);
	snprintf(vcard_msg,MAX_VCARD_LENGTH,"%d||%s||%s||%s||%s||%s||%s||%s", m_nGlooxID, jid.bare().c_str(), vcard->nickname().c_str(), vcard->formattedname().c_str(), vcard->bday().c_str(), phone_number, vcard->url().c_str(), photo_path);


	std::list<LPUSER_INFO>::const_iterator it2 = m_aBuddyList.begin();
	
	for(; it2 != m_aBuddyList.end(); ++it2)
	{

		if(!strcmp((*it2)->sJidBare, jid.bare().c_str()))
		{
			MESSENGER_DEBUG("handleVCard: buddy found in list! update it.\n");
			strncpy((*it2)->sPicture, photo_path, MAX_FILE_NAME_LENGTH);
		}

/*
		MESSENGER_DEBUG("\nhandleVCard: buddy list status \n");
		MESSENGER_DEBUG("handleVCard: current buddy.sJidBare = %s\n", (*it2)->sJidBare);
		MESSENGER_DEBUG("handleVCard: current buddy.sUsrId = %s\n", (*it2)->sUsrId);
		MESSENGER_DEBUG("handleVCard: current buddy.sUsrName = %s\n", (*it2)->sUsrName); //different wit2h vcard nickname
		MESSENGER_DEBUG("handleVCard: current buddy.sGroupId = %s\n", (*it2)->sGroupId[0]);
		MESSENGER_DEBUG("handleVCard: current buddy.sResrcId = %s\n", (*it2)->sResrcId[0]);
		MESSENGER_DEBUG("handleVCard: current buddy.nGroupCnt = %d\n", (*it2)->nGroupCnt);
		MESSENGER_DEBUG("handleVCard: current buddy.sResrcCnt = %d\n", (*it2)->nResrcCnt);
		MESSENGER_DEBUG("handleVCard: current buddy.nSubscript = %d\n", (*it2)->nSubscript);
		MESSENGER_DEBUG("handleVCard: current buddy.nChatCnt = %d\n", (*it2)->nChatCnt);
		MESSENGER_DEBUG("handleVCard: current buddy.nPresence = %d\n", (*it2)->nPresence);
		MESSENGER_DEBUG("handleVCard: current buddy.sPresenceDesc = %s\n", (*it2)->sPresenceDesc);
		MESSENGER_DEBUG("handleVCard: current buddy.sPicture = %s\n\n", (*it2)->sPicture);
*/		
	}


	sendVCardToJS(vcard_msg);

	MESSENGER_DEBUG("[%s][%s][%d] handleVCard finished!\n", __FILE__, __FUNCTION__, __LINE__);

	if(vcard_msg) 
	{
		free(vcard_msg);
	}
	
}    



/**
 * @brief 	This function is called to indicate the result of a VCard store operation
 * 			or any error that occurs.
 *
 * @param[in]	context - The operation which yielded the result.
 * @param[in]	jid - The JID involved.
 * @param[in]	se - The error, if any. If equal to @c StanzaErrorUndefined no error occured.
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleVCardResult( VCardContext context, const JID& jid, StanzaError se)
{
	MESSENGER_DEBUG( "[%s][%s][%d]: vcard result: context: %d, jid: %s, error: %d\n",  __FILE__, __FUNCTION__, __LINE__, context, jid.full().c_str(), se );
	//m_VCardManager->fetchVCard( jid, this );

	char temp[DEFAULT_MSGBUF_SIZE+1];
	
	memset(temp, 0, DEFAULT_MSGBUF_SIZE+1);
	
	snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||%s||error", m_nGlooxID, jid.bare().c_str());
	sendVCardToJS(temp);
}

void CGloox::handlePrivacyListNames( const std::string& active, const std::string& def,
                                     const StringList& lists )
{
	MESSENGER_DEBUG( "[%s][%s][%d]: handlePrivacyListNames\n", __FILE__, __FUNCTION__, __LINE__);
}


void CGloox::handleBlockList( const StringList& lists )
{
	if(lists.size()==0)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: blocklist is empty!\n", __FILE__, __FUNCTION__, __LINE__);
		sendBlockListToJS(const_cast<char*>("||"));		
		return;
	}

	char * str = NULL;
	char * temp_str = NULL;
	char * retStr = NULL;
	
	temp_str = (char *)malloc(MAX_JID_LENGTH+1);	
	if(temp_str==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: temp_str is NULL!\n", __FILE__, __FUNCTION__, __LINE__);
		return;
	}
	
	memset(temp_str, 0x00, MAX_JID_LENGTH+1);

	printf( "received PL...\n" );
	StringList::const_iterator it = lists.begin();
	for( ; it != lists.end(); it++ )
	{
		snprintf(temp_str, MAX_JID_LENGTH, "%s||", (*it).c_str());

		int tempStringLength = 0;
			
		if(str)
		{
			tempStringLength = strlen(str) + strlen(temp_str);
		}
		else
		{					
			tempStringLength = strlen(temp_str);			
		}

		if(retStr)
		{
			free(retStr);
		}
		retStr = (char*)malloc(tempStringLength+1); //to add glooxid
		if(retStr==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: retStr is NULL!\n", __FILE__, __FUNCTION__, __LINE__);
			if(temp_str)
			{
				free(temp_str);
			}

			if(str)
			{
				free(str);
			}			
			return;
		}

		memset(retStr, 0x00, tempStringLength+1);


		if(str == NULL)
		{
			//first buddy is just gloox id (ex: "buddy")
			snprintf(retStr,tempStringLength+1,"%s", temp_str);
		}
		else
		{
			//make "buddy||buddy" string
			snprintf(retStr,tempStringLength+1,"%s%s", str, temp_str);
			free(str);
		}		

		str = (char*)malloc(strlen(retStr)+1);
		if(str==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: str is NULL!\n", __FILE__, __FUNCTION__, __LINE__);
			if(temp_str)
			{
				free(temp_str);
			}
			if(retStr)
			{
				free(retStr);
			}
			return;
		}

		memset(str, 0x00, strlen(retStr)+1);

		strncpy(str, retStr, strlen(retStr)+1); //fixed: include NULL		
		
	}
	sendBlockListToJS(str);	

	if(temp_str) 
	{
		free(temp_str);
	}
	if(str) 
	{
		free(str);
	}
	if(retStr) 
	{
		free(retStr);
	}
}

void CGloox::handlePrivacyList( const std::string& name, PrivacyList& items )
{
  MESSENGER_DEBUG( "[%s][%s][%d]: handlePrivacyListNames\n", __FILE__, __FUNCTION__, __LINE__);
}

void CGloox::handlePrivacyListChanged( const std::string& name )
{
  MESSENGER_DEBUG( "[%s][%s][%d]: handlePrivacyListNames\n", __FILE__, __FUNCTION__, __LINE__);
}


void CGloox::handlePrivacyListResult( const std::string& id, PrivacyListResult plResult )
{
  MESSENGER_DEBUG( "[%s][%s][%d]: result for id %s: %d", __FILE__, __FUNCTION__, __LINE__, id.c_str(), plResult);
}

void CGloox::handleSharedStatus( int type, const std::string& shared_status, const std::string& shared_presence )
{
	char msg[MAX_MESSAGE_LENGTH+1];
  char tmsg[MAX_MESSAGE_LENGTH+1];
  MESSENGER_DEBUG( "[%s][%s][%d]: handleSharedStatus: %s, %s\n", __FILE__, __FUNCTION__, __LINE__, shared_status.c_str(), shared_presence.c_str());
  
  memset(msg, 0, MAX_MESSAGE_LENGTH+1);
  memset(tmsg, 0, MAX_MESSAGE_LENGTH+1);
  
  /* snprintf() always writes null in its end. if there is no space, it will use bufsize-1 and write null in end. */
  snprintf(tmsg, MAX_MESSAGE_LENGTH-10, "%s", shared_status.c_str());
  snprintf(msg, MAX_MESSAGE_LENGTH,"%s||%s", tmsg, shared_presence.c_str());  
  sendSharedStatusToJS(type, msg);
}


#ifdef TALK_MDSHARE
/* file transfer handler */
/**
 * @brief	 handleFTRequest occurred when buddy requests file transfer to me
 *
 * @param[in]	from - sender JID
 * @param[in]	id - id string
 * @param[in]	sid - source id
 * @param[in]	name - file name
 * @param[in] 	size - file size
 * @param[in]	hash - hash string
 * @param[in]	date - date
 * @param[in]	mimetype - mimetype
 * @param[in]	desc - file description
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleFTRequest( const JID& from, const std::string& id, const std::string& sid,
								  const std::string& name, long size, const std::string& hash,
								  const std::string& date, const std::string& mimetype,
								  const std::string& desc, int /*stypes*/, long /*offset*/, long /*length*/ )
{
	
	char* temp;
	char* convertedStr = NULL;
	
	char savepath[MAX_FILE_NAME_LENGTH+1];
	int bufsize;

	FT_Recv_Info* pFT;
	
	MESSENGER_DEBUG("[%s][%s][%d]: file transfer request event!\n", __FILE__, __FUNCTION__, __LINE__);
	MESSENGER_DEBUG("handleFTRequest: received ft request from %s: %s (%ld bytes, sid: %s). hash: %s, date: %s, mime-type: %s\n"
			  "desc: %s\n",
			  from.full().c_str(), name.c_str(), size, sid.c_str(), hash.c_str(), date.c_str(),
			  mimetype.c_str(), desc.c_str());
			  
	bufsize = MAX_JID_LENGTH+MAX_FILE_NAME_LENGTH+MAX_FT_DESC_LENGTH+10;
	temp = (char*)malloc(bufsize);
	if(temp==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		
		return;
	}

	pFT = (FT_Recv_Info*)new FT_Recv_Info;
	if(pFT==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		
		return;
	}
	
//	memset(pFT, 0, sizeof(FT_Recv_Info));

	memset(&pFT->m_FTThread, 0, sizeof(ThreadJob_Inf));

	pFT->m_nFTFileCurrent = 0;
	pFT->m_nFTFilePercent = 0;
	pFT->m_nFTFilePrevPercent = 0;
	pFT->m_pS5B = NULL;
	pFT->m_pSIProfileFT = NULL;


	pFT->m_pParent = (void*)this;
	pFT->m_sStreamID = sid;
	pFT->m_pSIProfileFT = m_pSIProfile;
	
	if(pFT->m_pSIProfileFT==NULL)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!\n", __FILE__, __FUNCTION__, __LINE__);
		free(temp);
		return;
	}
	
	//pFT->m_pSIProfileFT->addStreamHost( JID( HOST_SERVER_NAME ), HOST_SERVER_IP, m_nFTPort );
	

	if(decideFTRequest(from.bare().c_str(), name.c_str(), desc.c_str(), size)!=true)
	{
		MESSENGER_DEBUG("[%s][%s][%d]: file transfer request declined!!!\n", __FILE__, __FUNCTION__, __LINE__);
		pFT->m_pSIProfileFT->declineFT(from, id, SIManager :: RequestRejected);
		free(temp);
		delete pFT;
		return;
	}

	if(m_nFTID>65535) 
	{
		m_nFTID = 0; //turn around
	}

	++m_nFTID;
	
	pFT->m_nFTID = m_nFTID;


	// Edited by k2h - 090527
	// _Folderlist.xml, _Filelist.xml files are save to 
	// _Mediasharing/{LoggedUserID}/{BuddyID}_XXX.xml
	//

	{
		char* extPtr;

		extPtr = strrchr(const_cast<char*>(name.c_str()), '.');

		MESSENGER_DEBUG("[%s][%s][%d]: File name:%s\n", __FILE__, __FUNCTION__, __LINE__, name.c_str());
		MESSENGER_DEBUG("[%s][%s][%d]: File ext:%s\n", __FILE__, __FUNCTION__, __LINE__, extPtr);

		// check xml files
		if (!strcmp(extPtr, ".xml"))
		{
			// Todo later - check the description command 




			snprintf(savepath,MAX_FILE_NAME_LENGTH,"/mtd_rwarea/_Mediasharing/%s/%s_%s", m_MyJId.bare().c_str(), from.username().c_str(), name.c_str());
		}

		else
		{
			snprintf(savepath,MAX_FILE_NAME_LENGTH,"%s/_MS_tmp/%s" ,m_sDownloadDir ,name.c_str());
		}

	}
	
	MESSENGER_DEBUG("[%s][%s][%d]: file save path = %s\n", __FILE__, __FUNCTION__, __LINE__, savepath);

	pFT->m_FTRecvFile.open(savepath,ios::binary|ios::trunc);
	
	if( pFT->m_FTRecvFile.fail() ) 
	{
		MESSENGER_DEBUG("[%s][%s][%d]: file open failed!!! file receive rejected!!!\n", __FILE__, __FUNCTION__, __LINE__);
		pFT->m_pSIProfileFT->declineFT(from, id, SIManager :: RequestRejected);
		snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, pFT->m_nFTID);
		sendFTErrorToJS(temp, false);
		free(temp);
		delete pFT;		
		return;
	}	

	
	pFT->m_nFTFileTotal = size;

	m_aRecvList.push_back(pFT);
	
	pFT->m_FTThread.nId = pthread_create(&(pFT->m_FTThread.tThread), NULL, threadRecvFT, (void *)pFT);
	if (pFT->m_FTThread.nId != 0)
	{
		pFT->m_FTThread.bRun = false;
		MESSENGER_DEBUG("[%s][%s][%d] Thread Creation Failed..\n",__FILE__,__FUNCTION__,__LINE__);
	}

	
	pFT->m_pSIProfileFT->acceptFT( from, id, SIProfileFT::FTTypeS5B ); /* TODO: you MUST accept carefully */

	MESSENGER_DEBUG("[%s][%s][%d]: file receive accepted!!! file receive begin!!!\n", __FILE__, __FUNCTION__, __LINE__);
	
	snprintf(temp,bufsize,"%d||%d||%s||%s||%s||%ld",m_nGlooxID, pFT->m_nFTID,from.bare().c_str(), name.c_str(), desc.c_str(), size);
	convertedStr = convertIntoHTML(temp);
	sendFTRequestToJS(convertedStr);
	if(convertedStr) 
	{
		free (convertedStr);
	}

	free(temp);
}




/**
 * @brief	when my requestFT() is rejected.
 *
 * @param[in]	stanza - error stanza
 * @param[in]	sid - source id
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleFTRequestError( Stanza* stanza, const std::string& sid )
{
	char temp[DEFAULT_MSGBUF_SIZE+1];
	//void* iRet;
	
	MESSENGER_DEBUG("[%s][%s][%d]: file transfer request error!\n", __FILE__, __FUNCTION__, __LINE__);
	MESSENGER_DEBUG("sid = %s\n", sid.c_str());
	
	std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
	while(sit != m_aSendList.end())
	{
		if((*sit)->m_sStreamID==sid)
		{
			(*sit)->m_FTThread.bStop = true;

//			pthread_join((*sit)->m_FTThread.tThread, (void**)&iRet);

			
			snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*sit)->m_nFTID);
			
			sendFTErrorToJS(temp, true);
	
			MESSENGER_DEBUG("[%s][%s][%d]:	FT request is denied!\n", __FILE__, __FUNCTION__, __LINE__);
				
			return;
		}		
		++sit;
	}
		
		
	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit!= m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==sid)
		{
			(*rit)->m_FTThread.bStop = true;
				
//			pthread_join((*rit)->m_FTThread.tThread, (void**)&iRet);
			
			snprintf(temp,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*rit)->m_nFTID);
			
			sendFTErrorToJS(temp, false);
				
			MESSENGER_DEBUG("[%s][%s][%d]:	FT request is denied!\n", __FILE__, __FUNCTION__, __LINE__);
				
			return; 		
		}		
		
		++rit;
	}
	
}



/**
 * @brief	when file transfer is accepted by both side and trasfer socket is connected.
 *
 * @param[in]	s5b - socket object
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleFTSOCKS5Bytestream( SOCKS5Bytestream* s5b )
{
	MESSENGER_DEBUG("[%s][%s][%d]: received socks5 bytestream\n", __FILE__, __FUNCTION__, __LINE__);

	s5b->registerSOCKS5BytestreamDataHandler( this );
	if( s5b->connect() )
	{
		MESSENGER_DEBUG( "handleFTSOCKS5Bytestream: ok! s5b connected to streamhost\n" );
	}				


	std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
	while(sit != m_aSendList.end())
	{
		if((*sit)->m_sStreamID==s5b->sid())
		{
			MESSENGER_DEBUG("[%s][%s][%d]:	FT sid matched!\n", __FILE__, __FUNCTION__, __LINE__);

			(*sit)->m_pS5B = s5b;
			return;
		}		
		++sit;
	}
		
		
	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit!= m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==s5b->sid())
		{
	
			MESSENGER_DEBUG("[%s][%s][%d]:	FT sid matched!\n", __FILE__, __FUNCTION__, __LINE__);

			(*rit)->m_pS5B = s5b;
			return; 		
		}		
		
		++rit;
	}

}


/**
 * @brief	while file transfer, this event is occurred to read received data.
 *
 * @param[in]	s5b - socket object
 * @param[in]	data - received data
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleSOCKS5Data( SOCKS5Bytestream* s5b, const std::string& data )
{
	char msg[DEFAULT_MSGBUF_SIZE+1];
	
//	MESSENGER_DEBUG("handleSOCKS5Data: received %d bytes of data\n", __FILE__, __FUNCTION__, __LINE__,data.length());

	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit!= m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==s5b->sid())
		{
	
			//MESSENGER_DEBUG("[%s][%s][%d]:	FT sid matched!\n", __FILE__, __FUNCTION__, __LINE__);
			(*rit)->m_FTRecvFile.write((const char *)data.c_str(), data.length());
			
			(*rit)->m_nFTFileCurrent += data.length();
			
			(*rit)->m_nFTFilePercent = ((*rit)->m_nFTFileCurrent * 100)/(*rit)->m_nFTFileTotal;
			
			if((*rit)->m_nFTFilePercent!=(*rit)->m_nFTFilePrevPercent||(*rit)->m_nFTFileCurrent==(*rit)->m_nFTFileTotal)
			{	
				snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d||%d",m_nGlooxID, (*rit)->m_nFTID, (*rit)->m_nFTFileCurrent);
				sendFTProgressToJS(msg, false);
				
				(*rit)->m_nFTFilePrevPercent = (*rit)->m_nFTFilePercent;
			}
	
			return; 
		}		
		
		++rit;
	}

}


/**
 * @brief	when socket is disconnected, this error is occurred.
 *
 * @param[in]	s5b - socket object
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleSOCKS5Error( SOCKS5Bytestream* s5b, Stanza* /*stanza*/ )
{
	//void* iRet;
	char msg[DEFAULT_MSGBUF_SIZE+1];
	MESSENGER_DEBUG("[%s][%s][%d]: socks5 stream error\n", __FILE__, __FUNCTION__, __LINE__);

	
	std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
	while(sit != m_aSendList.end())
	{
		if((*sit)->m_sStreamID==s5b->sid())
		{

			(*sit)->m_FTThread.bStop = true;

//			pthread_join((*sit)->m_FTThread.tThread, (void**)&iRet);
							
			snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*sit)->m_nFTID);
			
			sendFTErrorToJS(msg, true);
			
			MESSENGER_DEBUG("[%s][%s][%d]:	FT socket error!\n", __FILE__, __FUNCTION__, __LINE__);

			return;
		}		
		++sit;
	}
		
		
	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit!= m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==s5b->sid())
		{
			(*rit)->m_FTThread.bStop = true;

//			pthread_join((*rit)->m_FTThread.tThread, (void**)&iRet);
				
			
			snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*rit)->m_nFTID);
			
			sendFTErrorToJS(msg, false);
				
			MESSENGER_DEBUG("[%s][%s][%d]:	FT request is denied!\n", __FILE__, __FUNCTION__, __LINE__);
				
			return; 		

		}		
		
		++rit;
	}

}


/**
 * @brief	when socket is opened for file transfer
 *
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleSOCKS5Open( SOCKS5Bytestream* s5b )
{
	char msg[DEFAULT_MSGBUF_SIZE+1];
	MESSENGER_DEBUG("[%s][%s][%d]: stream opened\n", __FILE__, __FUNCTION__, __LINE__);

	
	std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
	while(sit != m_aSendList.end())
	{
		if((*sit)->m_sStreamID==s5b->sid())
		{
	
			snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*sit)->m_nFTID);
			sendFTOpenToJS(msg, true);	
			return;
		}		
		++sit;
	}
		
		
	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit!= m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==s5b->sid())
		{
			snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*rit)->m_nFTID);
			sendFTOpenToJS(msg, false);				
			return; 		
		}		
		
		++rit;
	}


}


/**
 * @brief	when socket is closed for file transfer
 *
 * @param[in]	s5b - socket object
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleSOCKS5Close( SOCKS5Bytestream* s5b )
{
	//void* iRet;
	char msg[DEFAULT_MSGBUF_SIZE+1];
	MESSENGER_DEBUG("[%s][%s][%d]: socket close event!\n", __FILE__, __FUNCTION__, __LINE__);


	std::list<FT_Send_Info*>::iterator sit = m_aSendList.begin();
	while(sit != m_aSendList.end())
	{
		if((*sit)->m_sStreamID==s5b->sid())
		{
			(*sit)->m_FTThread.bStop = true;

//			pthread_join((*sit)->m_FTThread.tThread, (void**)&iRet);

					
			snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*sit)->m_nFTID);
			
			sendFTCloseToJS(msg, true);			
						
			return;
		}		
		++sit;
	}


	std::list<FT_Recv_Info*>::iterator rit = m_aRecvList.begin();
	while(rit!= m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==s5b->sid())
		{
			(*rit)->m_FTThread.bStop = true;

//			pthread_join((*rit)->m_FTThread.tThread, (void**)&iRet);				

			snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d",m_nGlooxID, (*rit)->m_nFTID);
			
			sendFTCloseToJS(msg, false);
				
		
			return; 				
		}		
		
		++rit;
	}

}
#endif



/* Multi-User Chat Handler */
/**
 * @brief	notify participant presence for multi-user chat
 *
 * @param[in]	room - room object
 * @param[in]	participant - participant object
 * @param[in]	presence - presence value
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCParticipantPresence( MUCRoom* room, const MUCRoomParticipant participant,
                                            Presence presence )
{
	char sPresence[MAX_MESSAGE_LENGTH+1];
	const char* pPresenceName;
	if( presence == PresenceAvailable )
	{
		MESSENGER_DEBUG( "[%s][%s][%d]: %s is in the room, too\n", __FILE__, __FUNCTION__, __LINE__ ,participant.nick->resource().c_str() );
	}
	else if( presence == PresenceUnavailable )
	{
		MESSENGER_DEBUG( "[%s][%s][%d]: %s left the room\n", __FILE__, __FUNCTION__, __LINE__ ,participant.nick->resource().c_str() );
	}
	else
	{
		MESSENGER_DEBUG( "[%s][%s][%d]: Presence is %d of %s\n", __FILE__, __FUNCTION__, __LINE__ ,presence, participant.nick->resource().c_str() );
	}
	
	memset(sPresence, 0, MAX_MESSAGE_LENGTH+1);
	
	switch(presence)
	{
		case 0:
			pPresenceName = "UnKnown";
			break;
		case 1:
			pPresenceName = "Available";
			break;
		case 2:
			pPresenceName = "Chat";
			break;
		case 3:
			pPresenceName = "Away";
			break;
		case 4:
			pPresenceName = "Dnd";
			break;
		case 5:
			pPresenceName = "Xa";
			break;
		case 6:
			pPresenceName = "Unavailable";
			break;
/*
		case 7:
			pPresenceName = "Broadcasting";
			break;
		case 8:
			pPresenceName = "Watching";
			break;
*/			
		default:
			pPresenceName = "UnKnown";
			break;
	}

	snprintf(sPresence,MAX_MESSAGE_LENGTH,"%d||%s||%s||%s",m_nGlooxID, room->name().c_str(), participant.nick->resource().c_str(), pPresenceName);
	
	sendMUCPresenceToJS(sPresence);
	
}



/**
 * @brief	notify received message for multi-user chat(MUC)
 *
 * @param[in]	room - room object
 * @param[in]	nick - nickname for talking entity
 * @param[in]	message - received message
 * @param[in]	history - flag to indicate whether this is history(saved) message or not
 * @param[in] 	priv - flag to indicate private message
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCMessage( MUCRoom* room, const std::string& nick, const std::string& message,
                               bool history, const std::string& /*when*/, bool priv )
{
	char* msg, *msg2;
	MESSENGER_DEBUG( "[%s][%s][%d]: %s said: '%s' (history: %s, private: %s)\n", __FILE__, __FUNCTION__, __LINE__,nick.c_str(), message.c_str(),
			history ? "yes" : "no", priv ? "yes" : "no" );

	msg = (char*)malloc(message.size()+MAX_MUCNAME_LENGTH+MAX_USER_ID_LENGTH);
	if(msg==NULL)
	{
		MESSENGER_DEBUG( "[%s][%s][%d]: out of memory!!!", __FILE__, __FUNCTION__, __LINE__ );
	}
	else
	{	
		
		snprintf(msg, message.size()+MAX_MUCNAME_LENGTH+MAX_USER_ID_LENGTH,"%d||%s||%s||%d||%s", m_nGlooxID,room->name().c_str(), nick.c_str(), (int)history, message.c_str());

		msg2 = convertIntoHTML(msg);
		
		sendMUCMessageToJS(msg2);

		if(msg) 
		{
			free(msg);
		}
		if(msg2) 
		{
			free(msg2);
		}
	}
}



/**
 * @brief	notify MUC subject is changed
 *
 * @param[in]	room - room object
 * @param[in]	nick - participant object
 * @param[in]	subject - changed subject
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCSubject( MUCRoom* room, const std::string& nick, const std::string& subject )
{
	if( nick.empty() )
	{
	  MESSENGER_DEBUG( "[%s][%s][%d]: Subject: %s\n", __FILE__, __FUNCTION__, __LINE__ ,subject.c_str() );
	}
	else
	{
	  MESSENGER_DEBUG( "[%s][%s][%d]: %s has set the subject to: '%s'\n", __FILE__, __FUNCTION__, __LINE__ ,nick.c_str(), subject.c_str() );
	}
}



/**
 * @brief	notify MUC error
 *
 * @param[in]	room - room object
 * @param[in]	error - error object
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCError( MUCRoom* room, StanzaError error )
{
	char msg[MAX_MESSAGE_LENGTH+1];
	
	MESSENGER_DEBUG( "[%s][%s][%d]: got an error: %d", __FILE__, __FUNCTION__, __LINE__ ,error );
	
	snprintf(msg, MAX_MESSAGE_LENGTH,"%d||%s||%d",m_nGlooxID,room->name().c_str(), error);
	sendMUCErrorToJS(msg);
}



/**
 * @brief	notify MUC information
 *
 * @param[in]	room - room object
 * @param[in]	features - MUC feature information
 * @param[in]	name - name string
 * @param[in] 	infoForm - data form of information
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCInfo( MUCRoom* room, int features, const std::string& name,
                                const DataForm *infoForm )
{
	MESSENGER_DEBUG( "[%s][%s][%d]: features: %d, name: %s, form xml: %s\n", __FILE__, __FUNCTION__, __LINE__ , features, name.c_str(), infoForm->tag()->xml().c_str() );
}


/**
 * @brief	notify MUC items
 *
 * @param[in]	room - room object
 * @param[in]	items - items to notify
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCItems( MUCRoom* room, const StringMap& items )
{
	StringMap::const_iterator it = items.begin();
	for( ; it != items.end(); ++it )
	{
	  MESSENGER_DEBUG( "[%s][%s][%d]: %s -- %s is an item here\n", __FILE__, __FUNCTION__, __LINE__ , (*it).first.c_str(), (*it).second.c_str() );
	}
}


/**
 * @brief	notify MUC invitee has declined my invitation
 *
 * @param[in]	room - room object
 * @param[in]	invitee - invitee who declined
 * @param[in]	reason - reason for decline.
 *
 * @return	void
 *
 * @version 1.0
 *
 */
void CGloox::handleMUCInviteDecline( MUCRoom* room, const JID& invitee, const std::string& reason )
{
	MESSENGER_DEBUG( "[%s][%s][%d]: Invitee %s declined invitation. reason given: %s\n", __FILE__, __FUNCTION__, __LINE__ , invitee.full().c_str(), reason.c_str() );
}


/**
 * @brief	notify MUC room is created
 *
 * @param[in]	room - room object
 *
 * @return	bool - true if successful, false if failed
 *
 * @version 1.0
 *
 */
bool CGloox::handleMUCRoomCreation( MUCRoom* room )
{
	char msg[MAX_MESSAGE_LENGTH+1];
	MESSENGER_DEBUG( "[%s][%s][%d]: room %s didn't exist, beeing created.\n", __FILE__, __FUNCTION__, __LINE__ , room->name().c_str() );
	
	snprintf(msg,MAX_MESSAGE_LENGTH,"%d||%s",m_nGlooxID,room->name().c_str());
	
	sendMUCCreateToJS(msg);
	return true;
}





#ifdef TALK_MDSHARE
/**
* @brief		Show received notice msg by forwarding msg to alarm/teasing widget
*			   
* @param [in]	msg - The message to send 
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendNoticeToJS(char* msg)
{
	
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	

/*
	if(m_pAlarmHandlerData!=NULL) 
	{
		MESSENGER_DEBUG("sendNoticeToJS: Alarm handler is used!\n");
		JS_Command((char*)"showReceivedNotice", msg, m_pAlarmHandlerData);
	}
	else */
	{
		MESSENGER_DEBUG("sendNoticeToJS: Alarm handler disabled! Default handler used!\n");	
		JS_Command(const_cast<char*>("showReceivedNotice"), msg, m_pHandlerData);
	}

	return true; 
}
#endif


/**
* @brief		Show received msg by calling JS code
*			   
* @param [in]	msg - The message to send 
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMessageToJS(char* msg)
{
	
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	
	
	usleep(900000);
	
	MESSENGER_DEBUG("after usleep [%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	

	JS_Command(const_cast<char*>("showReceivedMessage"), msg, m_pHandlerData);

	return true; 
}



/**
* @brief		idReceiver function call of javascript
*			   
* @param [in]	msg - bare id pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendFromIdToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
	
	JS_Command(const_cast<char*>("idReceiver"), msg, m_pHandlerData);
	
	return true; 
}



/**
* @brief		displaySubscription function call of javascript
*			   
* @param [in]	msg - bare id pointer of the buddy
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendSubscriptionToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	JS_Command(const_cast<char*>("subscriptionReceiver"), msg, m_pHandlerData);

	return true; 
}


/**
* @brief		rosterPresence function call of javascript
*			   
* @param [in]	presence - presence information(id, presence, channel information)
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendRosterPresenceToJS(char* presence)
{
	MESSENGER_DEBUG("[%s][%s][%d]: pres_msg = %s\n", __FILE__, __FUNCTION__, __LINE__, presence);
	
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	
	
	usleep(300000);
	
	MESSENGER_DEBUG("after usleep [%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);		

	JS_Command(const_cast<char*>("rosterPresence"), presence, m_pHandlerData);
	
	return true; 
}


/**
* @brief		rosterPresence function call of javascript
*			   
* @param [in]	presence - presence information(id, presence, channel information)
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendSelfPresenceToJS(char* presence)
{
	MESSENGER_DEBUG("[%s][%s][%d]: pres_msg = %s\n", __FILE__, __FUNCTION__, __LINE__, presence);

	JS_Command(const_cast<char*>("selfPresence"), presence, m_pHandlerData);
	
	return true; 
}


/**
* @brief		connection notification function to javascript
*			   
* @param [in]	msg - glooxid
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendConnectToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	JS_Command(const_cast<char*>("connectReceiver"), msg, m_pHandlerData);	
	return true; 
}



/**
* @brief		disconnection notification function call of javascript
*			   
* @param [in]	msg - The reason for the disconnection.
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendDisconnectToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);

	JS_Command(const_cast<char*>("disconnectReceiver"), msg, m_pHandlerData);	
	return true; 
}




/**
* @brief		vcard receiver call for widget
*			   
* @param [in]	msg - The received vcard info
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendVCardToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] passVCard called!\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pVCardHandlerData!=NULL) 
	{
		JS_Command(const_cast<char*>("vcardReceiver"), msg, m_pVCardHandlerData);
	}

	else 
	{
		JS_Command(const_cast<char*>("vcardReceiver"), msg, m_pHandlerData);
	}

	return true;
}


/**
* @brief		Use this function to send roster infotmation to Widget
*			   
* @param [in]	msg1 - glooxid string
* @param [in]	msg2 - roster information
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendRosterToJS(char*msg1, char* msg2)
{
	MESSENGER_DEBUG("[%s][%s][%d] sendRosterToJS called!\n", __FILE__, __FUNCTION__, __LINE__);

	JS_Command(const_cast<char*>("clearRoster"), msg1, m_pHandlerData);
	JS_Command(const_cast<char*>("rosterReceiver"), msg2, m_pHandlerData);
	
	return true;
}

bool CGloox::sendBlockListToJS(char* blocklist)
{
	MESSENGER_DEBUG("[%s][%s][%d]: blocklist = %s\n", __FILE__, __FUNCTION__, __LINE__, blocklist);

	JS_Command(const_cast<char*>("blocklist"), blocklist, m_pHandlerData);
	
	return true; 
}

bool CGloox::sendSharedStatusToJS(int type, char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]: msg = %s\n", __FILE__, __FUNCTION__, __LINE__, msg);

	if(type == 1)
	{
		JS_Command(const_cast<char*>("getSharedStatus"), msg, m_pHandlerData);
	}
	else if(type ==2)
	{
		JS_Command(const_cast<char*>("setSharedStatus"), msg, m_pHandlerData);
	}
	else
	{
		MESSENGER_DEBUG("[%s][%s][%d]: wrong SharedStatus type\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	
	return true; 
}


/**
* @brief		Use this function to send MUC presence infotmation to Widget
*			   
* @param [in]	msg - presence information
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMUCPresenceToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);


	JS_Command(const_cast<char*>("mucPresenceReceiver"), msg, m_pHandlerData);

	return true;
}


/**
* @brief		Use this function to send MUC chat msg to Widget
*			   
* @param [in]	msg - MUC chat message
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMUCMessageToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);


	JS_Command(const_cast<char*>("showMUCMessage"), msg, m_pHandlerData);
	

	return true;
}



/**
* @brief		Use this function to send MUC event msg to Widget
*			   
* @param [in]	msg - MUC event msg
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMUCCreateToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);


	JS_Command(const_cast<char*>("mucCreateReceiver"), msg, m_pHandlerData);
	

	return true;
}


/**
* @brief		Use this function to send MUC error msg to Widget
*			   
* @param [in]	msg - MUC event msg
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendMUCErrorToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	JS_Command(const_cast<char*>("mucErrorReceiver"), msg, m_pHandlerData);

	return true;
}


#ifdef TALK_MDSHARE
/**
* @brief		Use this function to send file transfer request to Widget
*			   
* @param [in]	msg - file transfer message
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendFTRequestToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pMSHandlerData==NULL) //this is receive event
	{
		JS_Command(const_cast<char*>("ftRequestReceiver"), msg, m_pHandlerData);
	}
	else
	{
		JS_Command(const_cast<char*>("ftRequestReceiver"), msg, m_pMSHandlerData);
	}	

	return true;
}



/**
* @brief		Use this function to send file transfer progress to Widget
*			   
* @param [in]	msg - file transfer message
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendFTProgressToJS(char* msg, bool is_send)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pMSHandlerData==NULL || is_send==true) //mdshare has no concern with send event
	{
		JS_Command(const_cast<char*>("ftProgressReceiver"), msg, m_pHandlerData);
	}
	else 
	{
		JS_Command(const_cast<char*>("ftProgressReceiver"), msg, m_pMSHandlerData);
	}

	return true;
}


/**
* @brief		Use this function to notify file transfer socket open
*			   
* @param [in]	msg - message to send
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendFTOpenToJS(char* msg, bool is_send)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pMSHandlerData==NULL || is_send==true) 
	{
		JS_Command(const_cast<char*>("ftOpenReceiver"), msg, m_pHandlerData);
	}
	else
	{
		JS_Command(const_cast<char*>("ftOpenReceiver"), msg, m_pMSHandlerData);
	}
	
	return true;
}


/**
* @brief		Use this function to send file transfer close to Widget
*			   
* @param [in]	msg - message to send
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendFTCloseToJS(char* msg, bool is_send)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pMSHandlerData==NULL || is_send==true) 
	{
		JS_Command(const_cast<char*>("ftCloseReceiver"), msg, m_pHandlerData);
	}
	else
	{
		JS_Command(const_cast<char*>("ftCloseReceiver"), msg, m_pMSHandlerData);
	}
	
	return true;
}


/**
* @brief		Use this function to notify file transfer error
*			   
* @param [in]	msg - message to send
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendFTErrorToJS(char* msg, bool is_send)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	if(m_pMSHandlerData==NULL || is_send==true) 
	{
		JS_Command(const_cast<char*>("ftErrorReceiver"), msg, m_pHandlerData);
	}
	else
	{
		JS_Command(const_cast<char*>("ftErrorReceiver"), msg, m_pMSHandlerData);
	}
	
	return true;
}

/**
* @brief		Use this function to send presence.info data to Javascript
*			   
* @param [in]	msg - message to send
* 
* @return		bool - true if successful, false if failed
*
* @version 1.0
*
*/
bool CGloox::sendPresenceInfoToJS(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d] called!\n", __FILE__, __FUNCTION__, __LINE__);

	JS_Command(const_cast<char*>("presenceinfoReceiver"), msg, m_pHandlerData);
	
	return true;
}
#endif


/**
* @brief		messenger main thread for receive message, file transfer.
*			   
* @param [in]	parent - void pointer for arguement
* 
* @return		void* - return value of thread
*
* @version 1.0
*
*/
void *CGloox::threadMain(void* parent)
{
	ConnectionError result;
    
	CGloox *pGloox = (CGloox*)parent;
	
	MESSENGER_DEBUG("[%s][%s][%d] thread started!\n", __FILE__, __FUNCTION__, __LINE__);

	if(pGloox==NULL) 
	{
		return NULL;
	}
	
	pGloox->m_tMainJobInfo.bRun = true;
	
	while(!pGloox->m_tMainJobInfo.bStop)
	{	
		gloox::Client* pClient = pGloox->getClient();
//		printf(".");
		if(pClient==NULL)
		{
			MESSENGER_DEBUG("[%s][%s][%d] pClient is NULL!!! exit...\n", __FILE__, __FUNCTION__, __LINE__);
			break;
		}

		result = pClient->recv(DEFAULT_SMALL_WAITTIME * 10); //microsecond

//		if(result == ConnNotConnected)
		if(result != ConnNoError)
		{
			MESSENGER_DEBUG("[%s][%s][%d]: result = %d\n", __FILE__, __FUNCTION__, __LINE__, result);
			break;
		}

//	network checking routine		
/*
		if(!gethostbyname_r( "www.naver.com" ))
		{	
			MESSENGER_DEBUG("[%s][%s][%d][%d]\n", __FILE__, __FUNCTION__, __LINE__, gethostbyname_r( "www.naver.com" ));
			sendDisconnectToJS((char*)"ConnNetworkError");		
			break;
		}	
*/
		
	}

	pGloox->m_tMainJobInfo.bRun = false;
	

	MESSENGER_DEBUG("[%s][%s][%d] thread exit!\n", __FILE__, __FUNCTION__, __LINE__);	
	pthread_exit(0);
}


#ifdef TALK_MDSHARE
/**
* @brief		messenger file transfer thread (file send)
*			   
* @param [in]	parent - void pointer for arguement
* 
* @return		void* - return value of thread
*
* @version 1.0
*
*/
void *CGloox::threadSendFT(void* parent)
{
	ConnectionError result;
	ConnectionError se = ConnNoError;

	CGloox* pGloox;
	
	FT_Send_Info* pFT = (FT_Send_Info*)parent;
	if(pFT==NULL) 
	{
		return NULL;
	}		
	char msg[DEFAULT_MSGBUF_SIZE+1];
	char* input = NULL;

	
	MESSENGER_DEBUG("[%s][%s][%d] thread started!\n", __FILE__, __FUNCTION__, __LINE__);

	pGloox = (CGloox*)pFT->m_pParent;

	if(pGloox==NULL) 
	{
		return NULL;
	}
	
	pFT->m_FTThread.bRun = true;
	
	while(!pFT->m_FTThread.bStop)
	{	

		/* file send */
		if( pFT->m_pS5BServer!=NULL )
		{

			se = pFT->m_pS5BServer->recv( DEFAULT_SMALL_WAITTIME );
			if( se != ConnNoError )
			{
			  MESSENGER_DEBUG( "[%s][%s][%d]: SOCKS5BytestreamServer returned: %d\n",__FILE__, __FUNCTION__, __LINE__, se );
			  pFT->m_FTThread.bStop = true;
			  
			  snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d", pGloox->m_nGlooxID, pFT->m_nFTID);
			  pGloox->sendFTErrorToJS(msg, true);

			}
			else
			{
				//printf( "size = %d, eof = %d\n",pGloox->m_S5BList.size(), pGloox->m_FTSendFile.eof());
				if( pFT->m_pS5B!=NULL && !pFT->m_FTSendFile.eof() )
				{
					SOCKS5Bytestream* it = pFT->m_pS5B;
						
					//printf("s5b isOpen = %d\n", (*it)->isOpen());
						
					if( it->isOpen() )
					{
					  if(input==NULL)
					  {
						input = (char*)malloc(FT_FILEBUFFER_SIZE);
					  }
						  
					  if(input==NULL)
					  {
						MESSENGER_DEBUG( "[%s][%s][%d]: out of memory!!!\n",__FILE__, __FUNCTION__, __LINE__);
						pFT->m_FTThread.bStop = true;

						snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d", pGloox->m_nGlooxID, pFT->m_nFTID);
						pGloox->sendFTErrorToJS(msg, true);
					  }
					  else
					  {
						  
						  pFT->m_FTSendFile.read( input, FT_FILEBUFFER_SIZE );
						  std::string t( input, pFT->m_FTSendFile.gcount() );
							 
						  //printf( ".");
						  if( !it->send( t ) )
						  {
							MESSENGER_DEBUG( "[%s][%s][%d]: send failed!!!\n",__FILE__, __FUNCTION__, __LINE__);
							MESSENGER_DEBUG( "eof = %d\n", pFT->m_FTSendFile.eof());
							pFT->m_FTThread.bStop = true;
					
							snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d", pGloox->m_nGlooxID, pFT->m_nFTID);
							pGloox->sendFTErrorToJS(msg, true);

						  }
						  //printf("!");
							  
						  pFT->m_nFTFileCurrent += pFT->m_FTSendFile.gcount();
	
						  pFT->m_nFTFilePercent = (pFT->m_nFTFileCurrent * 100)/pFT->m_nFTFileTotal;
	
						  if(pFT->m_nFTFilePercent!=pFT->m_nFTFilePrevPercent||pFT->m_nFTFileCurrent==pFT->m_nFTFileTotal) //handle 1/100 progress
						  { 
							snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d||%d", pGloox->m_nGlooxID, pFT->m_nFTID, pFT->m_nFTFileCurrent);
							pGloox->sendFTProgressToJS(msg, true);
							
							pFT->m_nFTFilePrevPercent = pFT->m_nFTFilePercent;
						  }
				  
					  }
					}
					
					result = it->recv( DEFAULT_SMALL_WAITTIME );
					if(result!=ConnNoError)
					{
						MESSENGER_DEBUG("[%s][%s][%d] connection error (%d)!\n", __FILE__, __FUNCTION__, __LINE__, result);
						pFT->m_FTThread.bStop = true;
						
						snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d", pGloox->m_nGlooxID, pFT->m_nFTID);
						pGloox->sendFTErrorToJS(msg, true);
						
					}
				}
				else if( pFT->m_pS5B!=NULL )
				{
					MESSENGER_DEBUG("[%s][%s][%d] file send finished!\n", __FILE__, __FUNCTION__, __LINE__);
				
					pFT->m_FTThread.bStop = true;
				}
				else
				{
					//wait for a while (set-up needs time)
					usleep(DEFAULT_SMALL_WAITTIME * 10);
				}

			}
			
		}


	}


	pFT->m_FTThread.bRun = false;
	
	if(input) 
	{
		free(input); 
		input = NULL;
	}


	std::list<FT_Send_Info*>::iterator sit = pGloox->m_aSendList.begin();
	while(sit != pGloox->m_aSendList.end())
	{
		if((*sit)->m_nFTID==pFT->m_nFTID)
		{
			
			if((*sit)->m_pS5B!=NULL) 
			{
				(*sit)->m_pSIProfileFT->dispose((*sit)->m_pS5B);
				(*sit)->m_pS5B = NULL;
			}

			if((*sit)->m_FTSendFile.is_open()) 
			{
				(*sit)->m_FTSendFile.close();
			}
		
			delete (*sit);
			
			pGloox->m_aSendList.erase(sit);
			
			MESSENGER_DEBUG("[%s][%s][%d]:	ft resource free finished!\n", __FILE__, __FUNCTION__, __LINE__);

			break;
		}		
		++sit;
	}


	MESSENGER_DEBUG("[%s][%s][%d] thread exit!\n", __FILE__, __FUNCTION__, __LINE__);	
	pthread_exit(0);
}




/**
* @brief		messenger file transfer thread (file receive)
*			   
* @param [in]	parent - void pointer for arguement
* 
* @return		void* - return value of thread
*
* @version 1.0
*
*/
void *CGloox::threadRecvFT(void* parent)
{
	ConnectionError result;

	FT_Recv_Info* pFT = (FT_Recv_Info*)parent;
	if(pFT==NULL) 
	{
		return NULL;
	}	
	CGloox *pGloox = (CGloox*)pFT->m_pParent;
	SOCKS5Bytestream* it;

	char msg[DEFAULT_MSGBUF_SIZE+1];
	
	MESSENGER_DEBUG("[%s][%s][%d] thread started!\n", __FILE__, __FUNCTION__, __LINE__);

	if(pGloox==NULL) 
	{
		return NULL;
	}
	
	pFT->m_FTThread.bRun = true;

	
	while(!pFT->m_FTThread.bStop)
	{	

		/* file receive */		
		it = pFT->m_pS5B;
		if(it!=NULL)
		{
			
			result = it->recv( DEFAULT_SMALL_WAITTIME );
			if(result!=ConnNoError)
			{
				if(result==ConnStreamClosed)
				{
					MESSENGER_DEBUG("[%s][%s][%d] stream closed by server.\n", __FILE__, __FUNCTION__, __LINE__);
				}
				else
				{
					MESSENGER_DEBUG("[%s][%s][%d] connection error! (%d)\n", __FILE__, __FUNCTION__, __LINE__, result);
				}
				pFT->m_FTThread.bStop = true;

				snprintf(msg,DEFAULT_MSGBUF_SIZE,"%d||%d", pGloox->m_nGlooxID, pFT->m_nFTID);
				pGloox->sendFTErrorToJS(msg, false);
			}
		}
		else
		{
			//wait for a while
			usleep(DEFAULT_SMALL_WAITTIME * 10);
		}
	}

	pFT->m_FTThread.bRun = false;

	std::list<FT_Recv_Info*>::iterator rit = pGloox->m_aRecvList.begin();
	while(rit!= pGloox->m_aRecvList.end())
	{
		if((*rit)->m_sStreamID==pFT->m_sStreamID)
		{
		
			if((*rit)->m_pS5B!=NULL) 
			{
				(*rit)->m_pSIProfileFT->dispose((*rit)->m_pS5B);
				(*rit)->m_pS5B = NULL;
			}
			
			if((*rit)->m_FTRecvFile.is_open()) 
			{
				(*rit)->m_FTRecvFile.close();
			}
	
			delete (*rit);
	
			pGloox->m_aRecvList.erase(rit);
			
			MESSENGER_DEBUG("[%s][%s][%d]:	ft resource free finished!\n", __FILE__, __FUNCTION__, __LINE__);
			
			break;
		}		
		++rit;
	}

	MESSENGER_DEBUG("[%s][%s][%d] thread exit!\n", __FILE__, __FUNCTION__, __LINE__);	
	pthread_exit(0);
}
#endif


/**
* @brief	thread function for sending message
*			   
* @param [in]	parent - a pointer of the CGloox
*
* @return	void* - return value of thread
*
* @version 1.0
*
*/
void *CGloox::sendMsgFunc(void * parent)
{
	SMsg_Inf*	pInf;
	CGloox*    pMessenger = (CGloox *) parent;

	if (!pMessenger)
	{
		return NULL;
	}
	
	MESSENGER_DEBUG("[%s][%s][%d] thread started!\n", __FILE__, __FUNCTION__, __LINE__);

	pMessenger->m_tSendJobInfo.bRun = true;

	while (!pMessenger->m_tSendJobInfo.bStop)
	{
		pthread_mutex_lock(&(pMessenger->m_tSendMsgMutex));

		if (pMessenger->m_aSendMsgList.size() < 1)
		{
			pthread_mutex_unlock(&(pMessenger->m_tSendMsgMutex));
			break;
		}

		pInf = (SMsg_Inf *)pMessenger->m_aSendMsgList.front();
		if(pInf!=NULL)
		{
			if(pInf->pszSendMsg == NULL)
			{
				MESSENGER_DEBUG("[%s][%s][%d]: pInf->pszSendMsg == NULL!!!\n", __FILE__, __FUNCTION__, __LINE__);
				pthread_mutex_unlock(&(pMessenger->m_tSendMsgMutex));
				return NULL;
			}				
			MESSENGER_DEBUG("[%s][%s][%d]: string length = %d\n", __FILE__, __FUNCTION__, __LINE__, strlen(pInf->pszSendMsg));
			
			pMessenger->sendMessage(pInf->sJidBare, pInf->nMsgType, pInf->pszSendMsg);
			pMessenger->m_aSendMsgList.pop_front();
			if(pInf->pszSendMsg)
			{
				free(pInf->pszSendMsg);
			}
			delete pInf;
		}

		pthread_mutex_unlock(&(pMessenger->m_tSendMsgMutex));

		usleep(DEFAULT_SMALL_WAITTIME * 100);
	}

	MESSENGER_DEBUG("[%s][%s][%d] thread exit!\n", __FILE__, __FUNCTION__, __LINE__);

	pMessenger->m_tSendJobInfo.bRun = false;
	pthread_exit(0);
}




///////////////////////////////////////////////////////////////////////////////
//Plug-in application 기능 구현 - Utility APIs
///////////////////////////////////////////////////////////////////////////////

/**
* @brief	string find & replace utility function
*			   
* @param [in]	source - source of replace
* @param [in]	pattern - pattern to replace
* @param [in]	replace - replace string for pattern
*
* @return	string - result string
*
* @version 1.0
*
*/
static std::string string_replace_all( 
			const std::string& source, 
			const std::string& pattern, 
			const std::string& replace )
{
	std::string result = source;
	std::string::size_type pos = 0;
	std::string::size_type offset = 0;
	std::string::size_type pattern_len = pattern.size();
	std::string::size_type replace_len = replace.size();

	while ( ( pos = result.find( pattern, offset ) ) != std::string::npos )
	{
		result.replace( result.begin() + pos, 
					result.begin() + pos + pattern_len, 
					replace );
		offset = pos + replace_len;
	}
	return result;
}



/**
* @brief		Use this function to convert plain text to HTML text
*			   
* @param [in]	msg - text to convert
* 
* @return		char* - converted text
*
* @version 1.0
*
*/
char* convertIntoHTML(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	
	char *pHTML;
	std::string sPlain(msg);
	std::string sResult;

	//	sResult = string_replace_all(sPlain, std::string("<"), std::string("&lt"));
	//	sPlain = string_replace_all(sResult, std::string(">"), std::string("&gt"));
	sResult = string_replace_all(sPlain, std::string("\n"), std::string("<br>"));
	sPlain = string_replace_all(sResult, std::string("\""), std::string("&quot"));

	pHTML = (char*)malloc(sPlain.size()+1); //fixed: include NULL termination.

	if(pHTML == NULL)		
	{
		MESSENGER_DEBUG("[%s][%s][%d]: out of memory!!!\n", __FILE__, __FUNCTION__, __LINE__);
		return NULL;
	}

	strncpy(pHTML, sPlain.c_str(), sPlain.size()+1); //fixed: include NULL

	return pHTML;

}



/**
* @brief		Use this function to convert EUC-KR into UTF-8
*			   
* @param [in]	msg - date of EUC-KR type
* 
* @return		char*
*
* @version 1.0
*
*/
char* convertIntoUTF8(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	
	char *sUTF8;
#if 1 // HYOK: we don't need to convert if input is UTF-8 and CHARSET is UTF-8.
	if(msg==NULL) 
	{
		return NULL;
	}

	sUTF8 = strdup(msg);

#else // old version
	iconv_t it; 
	int ret = 0;		
	char *tempUTF8 = NULL, *tempEUCKR = NULL;	
	size_t in, out; 

	in = strlen(msg);
	out = in * 2 + 1;
	sUTF8 = (char *)malloc(out);

	tempUTF8 = sUTF8;
	tempEUCKR = msg;

	memset(tempUTF8, 0x00, out);		

	it = iconv_open("UTF-8", "UCS-2");
	//it = iconv_open("UTF-8", "EUC-KR");
	if(it == (iconv_t)(-1)) // input encoding is not EUC-KR or EUC-KR is not available.
	{	
		MESSENGER_DEBUG("[%s][%s][%d]: iconv_open() failed!!!\n", __FILE__, __FUNCTION__, __LINE__);
		MESSENGER_DEBUG("[%s][%s][%d]: EUC-KR to UTF-8 encoding failed. charset can be broken!!!\n", __FILE__, __FUNCTION__, __LINE__);
		strncpy(sUTF8, msg, );
		return sUTF8;	
	}	
	
	
	ret = iconv(it, (char **)&tempEUCKR, &in, (char **)&tempUTF8, &out);	
	if(ret == -1)		
	{
		MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	
	}
	
	iconv_close(it);	
#endif
	return sUTF8;

}



/**
* @brief		Use this function to convert UTF-8 into EUC-KR
*			   
* @param [in]	msg - date of UTF-8 type
* 
* @return		char*
*
* @version 1.0
*
*/
char* convertIntoEUCKR(char* msg)
{
	MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);		
	char* sEUCKR;
#if 1 // HYOK: we don't need to convert if input is UTF-8 and CHARSET is UTF-8.

	if(msg==NULL) 
	{
		return NULL;
	}
	sEUCKR = strdup(msg);

#else // old version
	iconv_t it; 
	int ret = 0;	
	char *tempUTF8 = NULL, *tempEUCKR = NULL;	
	size_t in, out; 

	if(msg == NULL)
	{
		return NULL;
	}
	
	in = strlen(msg);	
	out = in * 2 + 1;	
	sEUCKR = (char *)malloc(out);	

	tempUTF8 = msg; 
	tempEUCKR = sEUCKR; 

	memset(tempEUCKR, 0x00, out);	
	
	it = iconv_open("UCS-2", "UTF-8");
	//it = iconv_open("EUC-KR", "UTF-8"); 
	if(it == (iconv_t)(-1)) 
	{
		MESSENGER_DEBUG("[%s][%s][%d]: iconv_open() error!!!\n", __FILE__, __FUNCTION__, __LINE__);
		MESSENGER_DEBUG("[%s][%s][%d]: UTF-8 to UCS-2 encoding failed. charset can be broken!!!\n", __FILE__, __FUNCTION__, __LINE__);
		strncpy(sEUCKR, msg, );
		return sEUCKR;	
	}	

	
	ret = iconv(it, (char **)&tempUTF8, &in, (char **)&tempEUCKR, &out);	
	if(ret == -1)		
	{
		MESSENGER_DEBUG("[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);	
	}
	iconv_close(it);
	// convert the charset	]
#endif	
	return sEUCKR;
}



}

