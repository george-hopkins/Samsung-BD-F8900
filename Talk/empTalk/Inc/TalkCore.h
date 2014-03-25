#ifndef _TALKCORE_H_
#define _TALKCORE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>


#include "client.h"
#include "messagesessionhandler.h"
#include "messageeventhandler.h"
#include "messageeventfilter.h"
#include "chatstatehandler.h"
#include "chatstatefilter.h"
#include "connectionlistener.h"   
#include "disco.h"
#include "discohandler.h"
#include "rostermanager.h"
#include "stanza.h"
#include "gloox.h"
#include "lastactivity.h"
#include "loghandler.h"
#include "logsink.h"
#include "connectiontcpclient.h"
#include "connectionsocks5proxy.h"
#include "connectionhttpproxy.h"   
#include "messagehandler.h"

/* vcard header */
#include "vcardhandler.h"
#include "vcardmanager.h"
#include "vcard.h"

/* file transfer header */
#include "siprofileft.h"
#include "siprofilefthandler.h"
#include "socks5bytestreamdatahandler.h"
#include "socks5bytestreamserver.h"


/* MUC header */
#include "mucroomhandler.h"
#include "mucroom.h"
#include "dataform.h"

/*Privacy List*/
#include "privacylisthandler.h"
#include "privacymanager.h"


using namespace gloox;



#define HOST_SERVER_NAME		("proxy.internetat.tv")	// Openfire Server is same with chgroup IP
#define HOST_SERVER_IP			("168.219.192.145") // Openfire Server IP

//#define HOST_SERVER_NAME		("proxy.internetat.tv")	// Openfire Server Domain Name
//#define HOST_SERVER_IP			("168.219.192.40") // Openfire Server IP


	
#ifdef _LINUX
#define LOGFILE_PATH		("/dtv")
#define MACRO_FILE		("/mtd_rwarea/macro.txt")
#else
#define LOGFILE_PATH		(".")
#define MACRO_FILE		("./macro.txt")
#endif

#define VCARD_PHOTOFILE_NAME	("vdata")

#define NOTICE_PREFIX  ("$NOTICE")   /* Alarm/Teasing message has this */
#define RECOMMEND_PREFIX ("$RECOMMEND") /* Alarm/Teasing recommend */
#define MDSHARE_PREFIX ("$MDSHARE/1/")  /* mdshare validity */

#define SAFE_DEL(X) 		if((X)!=NULL) delete (X)



#define LOG_FILE_EXT			("_log.txt")		// to predefine type of logfile
#define MAX_USER				(512) 										
#define MAX_USER_ID_LENGTH		(64)
#define MAX_USER_PWD_LENGTH 		(128)
#define MAX_JID_LENGTH			(128)
#define MACRO_NUM				(5)
#define MAX_MACRO_LENGTH		(128)
#define MAX_FILE_NAME_LENGTH	(255)
#define MAX_DOWNLOAD_PATH_LENGTH	(255)
#define MAX_FT_DESC_LENGTH		(255)
#define MAX_MESSAGE_LENGTH		(1024)
#define MAX_BLOCKLIST_LENGTH		(512)
#define MAX_PRESENCEDESC_LENGTH (255)
#define MAX_GROUPINFO_SIZE		(20)
#define MAX_RESRCINFO_SIZE		(20)
#define MAX_GROUP_NAME_LENGTH	(128)
#define BUDDYLIST_STRING_SIZE	(512)
#define MAX_USER_NAME_LENGTH (128)
#define MAX_RESRC_NAME_LENGTH (128)
#define MAX_VCARD_LENGTH		(512)
#define MAX_MUCNAME_LENGTH		(255)
#define FT_FILEBUFFER_SIZE		(16384)
#define MAX_MDSHARE_TITLE		(128) //spec. is 30
#define MAX_MDSHARE_DESC		(128) //spec. is 60

#define DEFAULT_MSGBUF_SIZE		(50) //javascript message size

#define DEFAULT_SMALL_WAITTIME (1000) //default time for recv/send

#define DEFAULT_FT_PORT 		(7777)  //default file transfer port

#define PCWEB_MS_MARKER  		(".MS") //presence marker of PC-Web Media Sharing 

#define LENGTH_PCWEB_MS_MARKER (3) //length of PCWEB_MS_MARKER

namespace sef {




/**
* @brief	Thread information structure with thread handle, flags.
*			   
* @version 1.0
*
*/
typedef struct ST_ThreadJob_Inf
{
	int 			nId;
	bool			bRun;
	bool			bStop;
	pthread_t		tThread;

} ThreadJob_Inf;





/**
* @brief	Chatting message information structure.
*			   
* @version 1.0
*
*/
typedef struct ST_Msg_Inf
{
	char		sJidBare[MAX_JID_LENGTH+1];
	char*		pszSendMsg;
	int 		nMsgType;

} SMsg_Inf;





/**
* @brief	multi-user chatting(MUC) control structure.
*
* @version 1.0
*/
typedef struct ST_MUC_Inf
{
	char pRoomName[MAX_MUCNAME_LENGTH+1];
	MUCRoom* pRoomHandle;
} MUCRoom_Inf;


enum CHAT_MSG_TYPE
{
	CHAT_MSG_TYPE_DATA = 0,
	CHAT_MSG_TYPE_EVENT,
	CHAT_MSG_TYPE_CHAT
};

enum XMPP_SUBSCRIPTION_TYPE
{
	SUBSCRIPTION_TYPE_USERADD = 0,
	SUBSCRIPTION_TYPE_BUDDYADD
};




/**
* @brief	user information structure (buddy info).
*
* @version 1.0
*/
typedef struct ST_USER_INFO
{
	char sJidBare[MAX_JID_LENGTH+1];
	char sUsrId[MAX_USER_ID_LENGTH+1];
	char sUsrName[MAX_USER_NAME_LENGTH+1];
	char sGroupId[MAX_GROUPINFO_SIZE][MAX_GROUP_NAME_LENGTH+1];
	char sResrcId[MAX_RESRCINFO_SIZE][MAX_RESRC_NAME_LENGTH+1];
	long nGroupCnt;
	long nResrcCnt; 	
	long nSubscript;
	int nChatCnt;
	Presence nPresence;
	char sPresenceDesc[MAX_PRESENCEDESC_LENGTH+1];
	char sPicture[MAX_FILE_NAME_LENGTH+1];
	char sMDShareTitle[MAX_PRESENCEDESC_LENGTH+1];
	char sMDShareDesc[MAX_PRESENCEDESC_LENGTH+1];	
	bool bMDShare; /* media share enable? */
} USER_INFO, *LPUSER_INFO;




/**
* @brief	file send transfer information structure.
*
* @version 1.0
*/
typedef struct ST_FT_Send_Info
{
	int m_nFTID;
	void* m_pParent;
//	bool m_bFTQuit; //file transfer quit flag
//	bool m_bFTOpen; //file transfer open flag
	int m_nFTFileTotal; //total file size
	int m_nFTFileCurrent; //current byte pos
	int m_nFTFilePercent; //current percentage
	int m_nFTFilePrevPercent; //previous percentage
//	char m_sFTFilePath[MAX_FILE_NAME_LENGTH+1]; //file path
	
	SIProfileFT* m_pSIProfileFT;
	SOCKS5Bytestream* m_pS5B; //socket handler
	SOCKS5BytestreamServer* m_pS5BServer;
	ThreadJob_Inf m_FTThread; //thread
	std::ifstream m_FTSendFile;
	std::string m_sStreamID;
} FT_Send_Info;




/**
* @brief	file receive transfer information structure.
*
* @version 1.0
*/
typedef struct ST_FT_Recv_Info
{
	int m_nFTID;
	void* m_pParent;
//	bool m_bFTQuit; //file transfer quit flag
//	bool m_bFTOpen; //file transfer open flag
	int m_nFTFileTotal; //total file size
	int m_nFTFileCurrent; //current byte pos
	int m_nFTFilePercent; //current percentage
	int m_nFTFilePrevPercent; //previous percentage
//	char m_sFTFilePath[MAX_FILE_NAME_LENGTH+1]; //file path
	
	SIProfileFT* m_pSIProfileFT;
	SOCKS5Bytestream* m_pS5B; //socket handler
	ThreadJob_Inf m_FTThread;
	std::ofstream m_FTRecvFile;
	std::string m_sStreamID;	
} FT_Recv_Info;




//TODO: multiple file transfer at once
typedef std::list<SMsg_Inf *>	 SendMsgList;
typedef std::list<LPUSER_INFO>	 BuddyInfoList;
typedef std::list<FT_Send_Info*>	FTSendList;
typedef std::list<FT_Recv_Info*>	FTRecvList;

class CGloox;




/**
* @brief	This is core of Talk plug-in and all functions are implemented in this class.
*			   
*/
class CGloox : public MessageSessionHandler, ConnectionListener, LogHandler, MessageEventHandler, MessageHandler, ChatStateHandler, RosterListener, VCardHandler, MUCRoomHandler, PrivacyListHandler
#ifdef TALK_MDSHARE
, SIProfileFTHandler, SOCKS5BytestreamDataHandler
#endif
{
public:
	CGloox(void);
	~CGloox(void);

public:

	//Interface
	bool  startGloox(int glooxid, char* id, char* password, void* pHandlerData, int ft_port = DEFAULT_FT_PORT);
	bool  removeSendSession(void); 
	bool  sendMessage(char* pJidBare, int nMsgType, char* pMessage); 
	bool  addBuddy(char* pJidBare, char* pJidName, char* pJidGroup); 
	bool  delBuddy(char* pJidBare);	
	bool  ackSubsReq(char* pJidBare, char* pJidName, char* pJidGroup);	
	bool  unsubscribe(char* pJidBare);
	bool  logOut(void);
	bool  setRosterFill(void);
	bool  setPresence(int presence, int priority, char* channel);
	bool  setSharedStatus(int shared_presence, char* msg);
	bool  getSharedStatus();
	bool  setEditedRoster(char* id, char* newNickname, char* newGroup);
	bool  sendMessageReserve(char* pJidBare, int nMsgType, char* pMessage);
	bool  requestVCard(void* requester, char* jid);
	bool  requestBlockList();
	bool  setBlock(char* jid_str);
	bool  unsetBlock(char* jid_str);		
	
#ifdef TALK_MDSHARE	
	int   requestFT(char* jid, char* filename, char* filedesc);
	void  cancelFT(int ft_id);
	bool  setDownloadDir(char* dirpath);	
	void  setMSWidgetHandle(void* pHandlerData, int mode);
	bool  getFirstSharingBuddy(char* buddy, int bufsize);
	bool  getSharingBuddy(char* buddy, int bufsize);
#endif

	bool  joinMUCRoom(char* roomname);
	bool  leaveMUCRoom(char* roomname, char* msg);
	bool  sendMUCMessage(char* roomname, char* msg);
	bool  inviteMUCRoom(char* roomname, char* jid,   char* msg);

#ifdef TALK_MDSHARE
	void  setAlarmWidgetHandle(void* pHandlerData, int mode);
	//set presence info
	bool  setPresenceInfo(int epg, int icast, int mdshare);
	
#endif

	void  getMyFullJID(char* jid);
	void  getMyPWD(char* pwd);

	int   getConnected(void);
	
	
	//Gloox Event
	virtual void onConnect(void);
	virtual void onDisconnect(ConnectionError e);
	virtual bool onTLSConnect(const CertInfo& info);  
	virtual void handleMessage(Stanza* stanza, MessageSession*);
	virtual void handleMessageEvent(const JID& from, MessageEventType event);
	virtual void handleChatState(const JID& from, ChatStateType state);
	virtual void handleMessageSession(MessageSession* session);
	virtual void handleLog(LogLevel level, LogArea area, const std::string& message);
	virtual void onResourceBindError(ResourceBindError error);
	virtual void onSessionCreateError(SessionCreateError error);
	virtual void handleItemSubscribed(const JID& jid);
	virtual void handleItemAdded(const JID& jid);
	virtual void handleItemUnsubscribed(const JID& jid);
	virtual void handleItemRemoved(const JID& jid);
	virtual void handleItemUpdated(const JID& jid);
	virtual void handleRoster(const Roster& roster);
	virtual void handleRosterError(Stanza*);
	virtual void handleRosterPresence(const RosterItem& item, const std::string& resource, Presence presence, const std::string& msg);
	virtual void handleSelfPresence(const RosterItem& item, const std::string& resource, Presence presence, const std::string&);
	virtual bool handleSubscriptionRequest(const JID& jid, const std::string&);
	virtual bool handleUnsubscriptionRequest(const JID& jid, const std::string&);
	virtual void handleNonrosterPresence(Stanza* stanza);

	//VCard handler from vcardmanager.cpp
	virtual void handleVCard( const JID& jid, VCard *vcard ) ;
	virtual void handleVCardResult( VCardContext context, const JID& jid, StanzaError se = StanzaErrorUndefined  );

	virtual void handlePrivacyListNames( const std::string& active, const std::string& def, const StringList& lists );
	virtual void handleBlockList( const StringList& lists );
	virtual void handlePrivacyList( const std::string& name, PrivacyList& items );
	virtual void handlePrivacyListChanged( const std::string& name );
	virtual void handlePrivacyListResult( const std::string& id, PrivacyListResult plResult );
	virtual void handleSharedStatus( int type, const std::string& status, const std::string& presence );

#ifdef TALK_MDSHARE	
	//file transfer handler
	virtual void handleFTRequest( const JID& from, const std::string& id, const std::string& sid,
									  const std::string& name, long size, const std::string& hash,
									  const std::string& date, const std::string& mimetype,
									  const std::string& desc, int /*stypes*/, long /*offset*/, long /*length*/ );
	virtual void handleFTRequestError( Stanza* /*stanza*/, const std::string& /*sid*/ );
	virtual void handleFTSOCKS5Bytestream( SOCKS5Bytestream* s5b );
	virtual void handleSOCKS5Data( SOCKS5Bytestream* /*s5b*/, const std::string& data );
	virtual void handleSOCKS5Error( SOCKS5Bytestream* /*s5b*/, Stanza* /*stanza*/ );
	virtual void handleSOCKS5Open( SOCKS5Bytestream* /*s5b*/ );
	virtual void handleSOCKS5Close( SOCKS5Bytestream* /*s5b*/ );
#endif

	/* Multi-User Chat handler */
    virtual void handleMUCParticipantPresence( MUCRoom * /*room*/, const MUCRoomParticipant participant,
                                            Presence presence );

    virtual void handleMUCMessage( MUCRoom* /*room*/, const std::string& nick, const std::string& message,
                                   bool history, const std::string& /*when*/, bool priv );

    virtual void handleMUCSubject( MUCRoom * /*room*/, const std::string& nick, const std::string& subject );

    virtual void handleMUCError( MUCRoom * /*room*/, StanzaError error );

    virtual void handleMUCInfo( MUCRoom * /*room*/, int features, const std::string& name,
                                    const DataForm *infoForm );

    virtual void handleMUCItems( MUCRoom * /*room*/, const StringMap& items );

    virtual void handleMUCInviteDecline( MUCRoom * /*room*/, const JID& invitee, const std::string& reason );

    virtual bool handleMUCRoomCreation( MUCRoom *room );

#ifdef TALK_MDSHARE
	//media sharing interface
	bool decideFTRequest(const char* jid, const char* filename, const char* filedesc, int size);
#endif


	//thread entry
	static void *sendMsgFunc(void *parent);
	static void *threadMain(void *parent);
	
#ifdef TALK_MDSHARE	
	static void *threadSendFT(void* parent);
	static void *threadRecvFT(void* parent);
#endif

	//internal
	void resetAll(void);
	
	
#ifdef TALK_MDSHARE	
	bool checkNoticeMsg(const std::string &str);

	// added by k2h - 0602 
	bool processMSCommand(const JID& tojid, const JID& fromjid, const char* msg);

	//read _Presence.Info file
	bool getShareInfoFile(const char* userID);
	
	//update(save) _Presence.Info file
	bool updatePresenceFile(const char* userID);
	
	//check and make directory for _Presence.info file
	int checkUserDir(const char* userID);
#endif

	//JS caller function
	bool sendEventToJS(void);
	bool sendMessageToJS(char* msg);

#ifdef TALK_MDSHARE
	bool sendNoticeToJS(char* msg);
#endif	
	
	bool sendFromIdToJS(char* msg);
	bool sendConnectToJS(char* msg);
	bool sendDisconnectToJS(char* msg);
	bool sendSubscriptionToJS(char* msg);
	bool sendRosterPresenceToJS(char* presence);
	bool sendSelfPresenceToJS(char* presence);	
	bool sendMacroDataToJS(char* macro);
	bool sendVCardToJS(char* msg);
	bool sendRosterToJS(char*msg1, char* msg2);

	bool sendBlockListToJS(char* blocklist);
	bool sendSharedStatusToJS(int type, char* msg);
		
	bool sendMUCPresenceToJS(char* msg);
	bool sendMUCMessageToJS(char* msg);
	bool sendMUCCreateToJS(char* msg);
	bool sendMUCErrorToJS(char* msg);

#ifdef TALK_MDSHARE
	bool sendFTRequestToJS(char* msg);
	bool sendFTProgressToJS(char* msg, bool is_send);
	bool sendFTOpenToJS(char* msg, bool is_send);
	bool sendFTCloseToJS(char* msg, bool is_send);
	bool sendFTErrorToJS(char* msg, bool is_send);

	bool sendPresenceInfoToJS(char* msg);
#endif	

	//set/get functions
	void setHandlerData(void* pData)
	{
		m_pHandlerData = pData;
	}

	VCardManager* getVCardManager(void) 
	{ 
		return m_VCardManager; 
	}

	JID* getMyJID(void)
	{
		return (&m_MyJId);
	}


	MessageSession* getMessageSession(void)
	{
		return m_session;
	}
	

	MessageEventFilter* getMessageEventFilter(void)
	{
		return m_messageEventFilter;
	}

	ChatStateFilter* getChatStateFilter(void)
	{
		return m_chatStateFilter;
	}


	gloox::Client* getClient(void)
	{
		return m_pClient;
	}


	void setClient(gloox::Client* pClient)
	{
		m_pClient = pClient;
	}
	
	
private:

	int	m_nGlooxID;
	JID m_MyJId;
	char m_sPWD[MAX_USER_PWD_LENGTH+1];

	gloox::Client* m_pClient;
	
 	SendMsgList         m_aSendMsgList;
	ThreadJob_Inf       m_tSendJobInfo;
	ThreadJob_Inf		m_tMainJobInfo;
	
#ifdef TALK_MDSHARE	
	ThreadJob_Inf		m_tFTJobInfo;
#endif

	pthread_mutex_t     m_tSendMsgMutex;
	MessageSession*		m_pSendMsgSession;

	void* 	m_pHandlerData; //default js handler
	
#ifdef TALK_MDSHARE	
	void* 	m_pMSHandlerData; //Media sharing handler
	void*	m_pAlarmHandlerData; //Alarm handler
#endif	
	
	void*	m_pVCardHandlerData; //which widget requested vcard?
	

	BuddyInfoList		m_aBuddyList;
	std::list<LPUSER_INFO>::iterator m_tBuddy_it;
	
#ifdef TALK_MDSHARE
	char m_sMDShareTitle[MAX_MDSHARE_TITLE+1];
	char m_sMDShareDesc[MAX_MDSHARE_DESC+1];

	bool m_bEPG; //share/unshare EPG info
	bool m_bPRIVATE; //share/unshare iCast info
	bool m_bMDShare; //share/unshare media share info
#endif
	
	MessageSession* m_session;
	MessageEventFilter* m_messageEventFilter;
	ChatStateFilter* m_chatStateFilter;
	VCardManager* m_VCardManager;
	int m_eSubscription;
	int m_nVCardCount;
	bool m_bConnected;
	bool m_bDisconnected;

#ifdef TALK_MDSHARE
	FTSendList m_aSendList;
	FTRecvList m_aRecvList;
	int m_nFTID;
	SIProfileFT* m_pSIProfile;
	SOCKS5BytestreamServer* m_pS5BServer;
#endif

	PrivacyManager* m_PrivacyManager;

#ifdef TALK_MDSHARE
	char m_sDownloadDir[MAX_DOWNLOAD_PATH_LENGTH+1];
	int m_nFTPort;
#endif
	
	std::list<MUCRoom_Inf*> m_MUCRoomList; /* group chat room list */
};


//utility function
char* convertIntoUTF8(char* msg);
char* convertIntoEUCKR(char* msg);
char* convertIntoHTML(char* msg);


}
#endif /* _TALKCORE_H_ */

