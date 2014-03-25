#include "EmpEngineTalk.h"
#include "EmpDebug.h"

#include <sstream>



namespace sef {

//using namespace gloox;       
//using std::string;

CGloox* 	m_pGloox[MAX_ACCOUNT_NUM]; /*global instance for media sharing */


// in order to convert the charset
//#include <iconv.h>



/**
* @brief		Use this function to send data(string) to widget
*			   
* @param [in]	command - javascript function name to process this call
* @param [in]	argv - arguement(string) to send
* @param [out]	pData - widget handler to receive this function call
*
* @return		void
*
* @version 1.0
*
*/
void JS_Command(char* command, char* argv, void* pData)
{
	SEF_LOG(Emp, DEBUGGING, "command=%s, argv=%s\n", command, argv);

	CEmpEngineTalk* pEmpEngineTalk = (CEmpEngineTalk*)pData;

	pEmpEngineTalk->SendEvent(0, command, argv);
}

CEmpEngineTalk::CEmpEngineTalk(void)
{
	SEF_LOG(Emp, DEBUGGING, "CEmpEngineTalk Constructor Called!!!\n");
	m_bStartClient = false;

	//char command[MAX_FILE_NAME_LENGTH+1] = "";
	//snprintf(command,MAX_FILE_NAME_LENGTH,"rm -rf %s/%s.*", LOGFILE_PATH, VCARD_PHOTOFILE_NAME);
	//system(command);
	//system("sync");
	//system("sync");
}

CEmpEngineTalk::~CEmpEngineTalk(void)
{
	SEF_LOG(Emp, DEBUGGING, "CEmpEngineTalk Destructor Called!!!\n");

	int i;

	if(m_bStartClient) //this instance invoked login (media sharing does not invoke login)
	{
		for(i=0;i<MAX_ACCOUNT_NUM;i++)
		{
			if(m_pGloox[i]!=NULL) 
			{
				m_pGloox[i]->logOut();
				delete m_pGloox[i];
				m_pGloox[i] = NULL;
			}
		}

		m_bStartClient = false;
	}

	//char command[MAX_FILE_NAME_LENGTH+1] = "";
	//snprintf(command,MAX_FILE_NAME_LENGTH,"rm -rf %s/%s.*", LOGFILE_PATH, VCARD_PHOTOFILE_NAME);
	//system(command);
	//system("sync");
	//system("sync");	
}

bool CEmpEngineTalk::Execute(string& strReturn, const string& strCmd,
		const string& strParam1, const string& strParam2, const string& strParam3,
		const string& strParam4)
{		
	if( COMPARE_EXECUTE_CMD("startMessengerClient") )
	{
		SEF_LOG(Emp, DEBUGGING, "glooxid:%s id:%s pw:%s ft_port:%s\n\n" ,strParam1.c_str(),strParam2.c_str(), strParam3.c_str(), strParam4.c_str());
		int glooxid = atoi(strParam1.c_str());
		char* id = const_cast<char*>(strParam2.c_str());
		char* pw = const_cast<char*>(strParam3.c_str());
		int ft_port = atoi(strParam4.c_str());

		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}		

		if(id==NULL||pw==NULL)
		{
			SET_RET_VAL_INT(0);
			return true;
		}

		if(m_pGloox[glooxid]!=NULL) 
		{
			delete m_pGloox[glooxid];
		}

		m_pGloox[glooxid] = new CGloox();

		if(m_pGloox[glooxid]==NULL) 
		{
			SEF_LOG(Emp, DEBUGGING, "Malloc failed!(m_pGloox)\n");
			SET_RET_VAL_INT(0);
			return true;
		}

		m_bStartClient = true; //this instance invoked login, and this instance has to delete memory later.

		if(m_pGloox[glooxid]->startGloox(glooxid, id, pw, (void*)this, ft_port)==true)
		{
			SEF_LOG(Emp, DEBUGGING, "startGloox() successful!!!\n");
			SET_RET_VAL_INT(1);
		}
		else
		{
			SEF_LOG(Emp, DEBUGGING, "startGloox() failed!!!\n");
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("sendMessageReserve") )
	{
		SEF_LOG(Emp, DEBUGGING, "glooxid:%s pJidBare:%s nMsgType:%s pMessage:%s\n\n" ,strParam1.c_str(),strParam2.c_str(), strParam3.c_str(), strParam4.c_str());
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());
		int nMsgType = atoi(strParam3.c_str());
		char* pMessage = const_cast<char*>(strParam4.c_str());	

		//SMsg_Inf *	pInf;
		
		if (!pJidBare ||!pMessage)
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->sendMessageReserve(pJidBare, nMsgType, pMessage)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		else 
		{
			SET_RET_VAL_INT(0);
		}

	}
	else if( COMPARE_EXECUTE_CMD("sendMessage") )
	{
		SEF_LOG(Emp, DEBUGGING, "glooxid:%s pJidBare:%s nMsgType:%s pMessage:%s\n\n" ,strParam1.c_str(),strParam2.c_str(), strParam3.c_str(), strParam4.c_str());
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());
		int nMsgType = atoi(strParam3.c_str());
		char* pMessage = const_cast<char*>(strParam4.c_str());			
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		
		if( pJidBare==NULL || nMsgType < CHAT_MSG_TYPE_DATA || nMsgType > CHAT_MSG_TYPE_CHAT)
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->sendMessage(pJidBare, nMsgType, pMessage)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		else 
		{
			SET_RET_VAL_INT(0);
		}

	}
	else if( COMPARE_EXECUTE_CMD("addBuddy") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());
		char* pJidName = const_cast<char*>(strParam3.c_str());
		char* pJidGroup = const_cast<char*>(strParam4.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->addBuddy(pJidBare, pJidName, pJidGroup)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("delBuddy") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->delBuddy(pJidBare)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("ackSubsReq") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());
		char* pJidName = const_cast<char*>(strParam3.c_str());
		char* pJidGroup = const_cast<char*>(strParam4.c_str());			
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->ackSubsReq(pJidBare, pJidName, pJidGroup)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("unsubscribe") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->unsubscribe(pJidBare)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("logOut") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
			
		if(m_pGloox[glooxid]==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->logOut()==true)
		{
			delete m_pGloox[glooxid];
			m_pGloox[glooxid] = NULL;
			SET_RET_VAL_INT(1);
		}
		else 
		{
			SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]: m_pGloox->logOut() failed!\n", __FILE__, __FUNCTION__, __LINE__);
			delete m_pGloox[glooxid];
			m_pGloox[glooxid] = NULL;	
			SET_RET_VAL_INT(0);
		}

	}
	else if( COMPARE_EXECUTE_CMD("setRosterFill") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->setRosterFill()==true) 
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("setPresence") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int presence = atoi(strParam2.c_str());
		int priority = atoi(strParam3.c_str());
		char* msg = const_cast<char*>(strParam4.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->setPresence(presence, priority, msg)==true) 
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("setEditedRoster") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* id = const_cast<char*>(strParam2.c_str());
		char* newNickname = const_cast<char*>(strParam3.c_str());
		char* newGroup = const_cast<char*>(strParam4.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		
		if(m_pGloox[glooxid]==NULL || id==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->setEditedRoster(id, newNickname, newGroup)==true) 
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("requestVCard") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->requestVCard((void*)this, pJidBare)==true) //To be modified
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("joinMUCRoom") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* roomname = const_cast<char*>(strParam2.c_str());	
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || roomname==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->joinMUCRoom(roomname)==true) 
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("leaveMUCRoom") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* roomname = const_cast<char*>(strParam2.c_str());	
		char* msg = const_cast<char*>(strParam3.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || roomname==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->leaveMUCRoom(roomname, msg)==true) 
		{
			SET_RET_VAL_INT(1);
		}
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("sendMUCMessage") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* roomname = const_cast<char*>(strParam2.c_str());	
		char* msg = const_cast<char*>(strParam3.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || roomname==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->sendMUCMessage(roomname, msg)==true) 
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("inviteMUCRoom") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* roomname = const_cast<char*>(strParam2.c_str());
		char* jid = const_cast<char*>(strParam3.c_str());
		char* msg = const_cast<char*>(strParam4.c_str());

		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || roomname==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->inviteMUCRoom(roomname, jid, msg)==true) 
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}

	}
	else if( COMPARE_EXECUTE_CMD("getMyFullJID") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char fulljid[256] = "";	

		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		m_pGloox[glooxid]->getMyFullJID(fulljid);
		
		SET_RET_VAL_ASCII(fulljid);
	}
	else if( COMPARE_EXECUTE_CMD("getMyPWD") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char pwd[256] = "";
		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		m_pGloox[glooxid]->getMyPWD(pwd);
			
		SET_RET_VAL_ASCII(pwd);
	}
	else if( COMPARE_EXECUTE_CMD("getConnected") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		return SET_RET_VAL_INT(m_pGloox[glooxid]->getConnected());
	}
	else if( COMPARE_EXECUTE_CMD("requestBlockList") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->requestBlockList()==true) //To be modified
		{
		SET_RET_VAL_INT(1);
	}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("setBlock") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());	
		char* pJidBare = const_cast<char*>(strParam2.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->setBlock(pJidBare)==true) //To be modified
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("unsetBlock") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* pJidBare = const_cast<char*>(strParam2.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || pJidBare==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->unsetBlock(pJidBare)==true) //To be modified
		{
			SET_RET_VAL_INT(1);
		}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("setSharedStatus") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int sharedpresence = atoi(strParam2.c_str());
		char* sharedstatus = const_cast<char*>(strParam3.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || sharedstatus==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]->setSharedStatus(sharedpresence, sharedstatus)==true) //To be modified
		{
		SET_RET_VAL_INT(1);
	}
			
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}	
	else if( COMPARE_EXECUTE_CMD("getSharedStatus") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->getSharedStatus()==true) //To be modified
		{
			SET_RET_VAL_INT(1);
		}
		
		else 
		{
			SET_RET_VAL_INT(0);
	}
	}
#ifdef TALK_MDSHARE	
	else if( COMPARE_EXECUTE_CMD("requestFT") )
	{
		int nRet;
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* jid = const_cast<char*>(strParam2.c_str());
		char* filename = const_cast<char*>(strParam3.c_str());	
		char* filedesc = const_cast<char*>(strParam4.c_str());	
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL || jid==NULL || filename==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		nRet = m_pGloox[glooxid]->requestFT(jid, filename, filedesc);
		SET_RET_VAL_INT(nRet);
	}
	else if( COMPARE_EXECUTE_CMD("cancelFT") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int ft_id = atoi(strParam2.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM||ft_id<0) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		m_pGloox[glooxid]->cancelFT(ft_id);
			
		SET_RET_VAL_INT(1);
	}
	else if( COMPARE_EXECUTE_CMD("setDownloadDir") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		char* dirpath = const_cast<char*>(strParam2.c_str());		
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		
		if(m_pGloox[glooxid]==NULL || dirpath==NULL ) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->setDownloadDir(dirpath)==true)
		{
			SET_RET_VAL_INT(1);
			return true;
		}
		
		SET_RET_VAL_INT(0);
	}
	else if( COMPARE_EXECUTE_CMD("setMSWidgetHandle") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());	
		int mode = atoi(strParam2.c_str());

		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		m_pGloox[glooxid]->setMSWidgetHandle((void*)this, mode); //To be modified
			
		SET_RET_VAL_INT(1);
	}
	else if( COMPARE_EXECUTE_CMD("getFirstSharingBuddy") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int bufsize = atoi(strParam3.c_str());		
		char buddy[bufsize];	

		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->getFirstSharingBuddy(buddy, bufsize)==true) 
		{
			SET_RET_VAL_ASCII(buddy);
		}
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}	
	else if( COMPARE_EXECUTE_CMD("getSharingBuddy") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int bufsize = atoi(strParam3.c_str());
		char buddy[bufsize];		

		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->getSharingBuddy(buddy, bufsize)==true) 
		{
			SET_RET_VAL_ASCII(buddy);
		}
		else 
		{
			SET_RET_VAL_INT(0);
		}
	}
	else if( COMPARE_EXECUTE_CMD("setAlarmWidgetHandle") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int mode = atoi(strParam2.c_str());
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		m_pGloox[glooxid]->setAlarmWidgetHandle((void*)this, mode);  //To be modified
			
		SET_RET_VAL_INT(1);
	}	
	else if( COMPARE_EXECUTE_CMD("setPresenceInfo") )
	{
		SEF_LOG(Emp, DEBUGGING, "[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__);
		int glooxid = atoi(strParam1.c_str());
		int epg = 0;
		int icast = 0;
		int mdshare = 0;
		
		if(glooxid<0||glooxid>=MAX_ACCOUNT_NUM) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
		
		if(m_pGloox[glooxid]==NULL) 
		{
			SET_RET_VAL_INT(0);
			return true;
		}
			
		if(m_pGloox[glooxid]->setPresenceInfo(epg, icast, mdshare)==true)
		{
			ostringstream buffer;
			
			buffer<<epg;
			string strepg = buffer.str();
			buffer.str(""); 	
			
			buffer<<icast;
			string stricast = buffer.str();
			buffer.str(""); 	
			
			buffer<<mdshare;
			string strmdshare = buffer.str();
			buffer.str(""); 	
			
			string strRet = strepg + "~" + stricast + "~" + strmdshare;

		
			SET_RET_VAL_ASCII(strRet);
		}
			
			SET_RET_VAL_INT(0);
		}
#endif	
	else
	{
		return false;
	}

	return true;
}


}
