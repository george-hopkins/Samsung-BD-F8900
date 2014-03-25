#ifndef __EMP_ENGINE_TALK_H__
#define __EMP_ENGINE_TALK_H__

#include "EmpEngine.h"
#include <string>

#include "TalkCore.h"


#define MAX_ACCOUNT_NUM 		(4)			//max. of my accounts (multi-instance)

namespace sef {

class CGloox;


class CEmpEngineTalk : public CEmpEngineBase
{
protected:
	bool m_bStartClient;
public:
	CEmpEngineTalk(void);
	virtual ~CEmpEngineTalk(void);	
protected:	
	bool Execute(string& strReturn, const string& strCmd,
		const string& strParam1, const string& strParam2, const string& strParam3,
		const string& strParam4);
};

}
#endif
