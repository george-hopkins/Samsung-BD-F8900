#include "EmpAppTalk.h"
#include "EmpEngineTalk.h"
#include "SefCommon.h"
#include "EmpDebug.h"
#include "EmpMain.h"
#include "EmpCommonTalk.h"


using namespace std;

#if 0
int main(int argc, char* argv[])
{
	return sef::EmpMain(argc, argv, new sef::CEmpAppTalk);	
}
#endif

extern int gArgc;
extern char** gArgv;

int Main()
{
	return sef::EmpMain(gArgc, gArgv, new sef::CEmpAppTalk);	
}

namespace sef {

CEmpAppTalk::CEmpAppTalk(void)
{
	m_strName = EMP_INFO_APP_NAME;
	m_strOption = EMP_OPTION_SINGLETON;
	m_strVersion = EMP_INFO_VERSION;	
}
 
CEmpEngineBase* CEmpAppTalk::CreateEmpEngine(void)
{
	CEmpEngineBase* pEmpEngine = NULL;
	pEmpEngine = new CEmpEngineTalk;

	if( NULL == pEmpEngine )
	{
		SEF_LOG(Emp, FATAL, "NULL == pEmpEngine");
		return NULL;
	}
	
	return pEmpEngine;	
}

/**
 * @brief                       Validate emp version. this function is virtual. so each CEmpAppBase[Name] can redefine their condition.
 * @param               [in] strVersion Emp version
 * @return              true if valid, otherwise false
 */
bool CEmpAppTalk::CheckVersion(const string strVersion)
{
	SEF_LOG(Emp, FATAL, "CheckVersion(strVersion=[%s]), (m_strVersion=[%s]) ", strVersion.c_str(), m_strVersion.c_str());
/*	
  if( strVersion.compare(m_strVersion) > 0 )
  {
    SEF_LOG(Emp, FATAL, "CheckVersion(strVersion=[%s]), (m_strVersion=[%s]) ", strVersion.c_str(), m_strVersion.c_str());
    return false;
  }
*/
  return true;
}


}

