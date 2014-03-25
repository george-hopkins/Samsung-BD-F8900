#ifndef __EMP_APP_TALK_H__
#define __EMP_APP_TALK_H__

#include "EmpApp.h"
#include "EmpEngine.h"

namespace sef {

class CEmpAppTalk : public CEmpAppBase
{
public:
	CEmpAppTalk(void);
	
protected:
	virtual CEmpEngineBase* CreateEmpEngine(void);
        virtual bool CheckVersion(const std::string strVersion);	
};

}


#endif
