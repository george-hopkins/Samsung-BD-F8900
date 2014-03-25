#ifndef	_CSR_BT_MOBIS_APP_H
#define	_CSR_BT_MOBIS_APP_H
#include "csr_synergy.h"
#include "Bluetooth_Common.h"
//#include "csr_bt_core_stack_pmalloc.h"
#include "csr_types.h"
#include "Bluetooth_SD.h"
#include "Bluetooth_SC.h"
#include "Bluetooth_CM.h"
#include "Bluetooth_HF.h"
#include "Bluetooth_AV.h"
#include "Bluetooth_AVRC.h"
#include "Bluetooth_PBAC.h"
#include "Bluetooth_SPP.h"
#include "Bluetooth_HF.h"
#include "Bluetooth_Key.h"


typedef struct
{
	SdAppInst	*sdinstData;

	ScAppInst	*scinstData;

	CmAppInst	*cminstData;

	HfAppInst	*hfinstData;
	
	PbacAppInst *pbacinstData;

	Av2AppInst  *av2instData;

	AvrcAppInst  *avrcinstData;
	
	CsrSppAppGlobalInst  *sppinstData;
	
	KeyAppInst           *keyinstData;
} btAppInstance_t;

extern btAppInstance_t *btAppInstance;


#endif  //__BT_H_
