#ifndef	_CSR_BT_SAMSUNG_APP_H
#define	_CSR_BT_SAMSUNG_APP_H
#include "csr_synergy.h"
#include "csr_types.h"
#include "Bluetooth_SD.h"
#include "Bluetooth_SC.h"
#include "Bluetooth_CM.h"
#include "Bluetooth_AV.h"
#include "Bluetooth_AVRC.h"
#include "Bluetooth_HIDH.h"

typedef struct
{
	SdAppInst	*sdinstData;

	ScAppInst	*scinstData;

	CmAppInst	*cminstData;

	Av2AppInst  *av2instData;

	AvrcAppInst  *avrcinstData;

	HidhAppInst  *hidhinstData;
		
} btAppInstance_t;

//extern btAppInstance_t *btAppInstance;


#endif  //__BT_H_
