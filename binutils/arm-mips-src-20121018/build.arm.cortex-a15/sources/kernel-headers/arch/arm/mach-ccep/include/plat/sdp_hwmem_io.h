/*********************************************************************************************
 *
 *	sdp_hwmem_io.h (Samsung Soc DMA memory allocation)
 *
 *	author : tukho.kim@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
 * Date 	author		Description
 * ----------------------------------------------------------------------------------------
// Jul,09,2010 	tukho.kim	created
 ********************************************************************************************/

#ifndef __SDP_HWMEM_IO_H__
#define __SDP_HWMEM_IO_H__

#define CMD_HWMEM_GET		(0x11)
#define CMD_HWMEM_RELEASE	(0x12)

//#define CMD_HWMEM_INV		(0x21)	 
#define CMD_HWMEM_CLEAN		(0x22)
#define CMD_HWMEM_FLUSH		(0x23)

#define CMD_HWMEM_INV_RANGE			(0x28)
#define CMD_HWMEM_CLEAN_RANGE		(0x29)
#define CMD_HWMEM_FLUSH_RANGE		(0x2A)

#define CMD_HWMEM_GET_REVISION_ID	(0x2B)

typedef struct sdp_get_hwmem_t {
	int			node;	
	int			uncached;
	size_t		size;

// return value
	unsigned long 	phy_addr;
	unsigned long 	vir_addr;
}SDP_GET_HWMEM_T;


typedef struct sdp_rel_hwmem_t {
	unsigned long 	phy_addr;
	unsigned long 	vir_addr;
	size_t		size;
}SDP_REL_HWMEM_T;


typedef SDP_REL_HWMEM_T SDP_CACHE_HWMEM_T;

#endif /* __SDP_HWMEM_IO_H__ */

