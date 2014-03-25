
#include <stdio.h>
#include <string.h>
#include "csr_samsung_log.h"
static int dbg_level = 0;




void csr_ss_log_set_level(int level)
{
    switch (level)
    {
      case CSR_SS_LOG_LEVEL_TEXT_CRITICAL:
	  	 dbg_level =CSR_LOG_LEVEL_TEXT_CRITICAL;
		  break;
	  case CSR_SS_LOG_LEVEL_TEXT_ERROR:
	  	  dbg_level =CSR_LOG_LEVEL_TEXT_ERROR|CSR_LOG_LEVEL_TEXT_CRITICAL;
		  break;
	  case CSR_SS_LOG_LEVEL_TEXT_WARNING:
          dbg_level =CSR_LOG_LEVEL_TEXT_WARNING|CSR_LOG_LEVEL_TEXT_ERROR|CSR_LOG_LEVEL_TEXT_CRITICAL;
		  break;
	   case CSR_SS_LOG_LEVEL_TEXT_INFO:
          dbg_level =CSR_LOG_LEVEL_TEXT_INFO|CSR_LOG_LEVEL_TEXT_WARNING|CSR_LOG_LEVEL_TEXT_ERROR|CSR_LOG_LEVEL_TEXT_CRITICAL;
		  break;
	   case CSR_SS_LOG_LEVEL_TEXT_DEBUG:
          dbg_level =CSR_LOG_LEVEL_TEXT_ALL;
		  break;
	   default:
	   	 dbg_level =CSR_LOG_LEVEL_TEXT_OFF;
	   	 break;
    } /* End switch */
}

void csr_ss_client_dbg_printf(int level, const char *fm, ...)
{
//	char buf[1024] = { 0, };
	if(dbg_level&level)
	{
	    va_list ap;


		va_start(ap,fm );

	    vprintf(fm,ap);
		
		va_end(ap);
	}
}
