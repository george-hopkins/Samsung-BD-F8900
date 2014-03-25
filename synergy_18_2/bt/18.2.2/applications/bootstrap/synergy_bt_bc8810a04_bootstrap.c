
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      1.3.29
FILE:          synergy_bt_bc8810a04_bootstrap.c
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_tm_bluecore_bootstrap.h"
//#include "csr_cmdif_util.h"
//#include "csr_hci_rfc.h"
//#include "csr_coal.h"
#include "csr_transport.h"

#include "csr_usr_config.h"
#include "csr_util.h"
#include "csr_bt_profiles.h"

#define CSR_BUILD_ID_6817                               ((CsrUint16)6817)

/* Force a hardware reset of the BlueCore Chip                      */
#define CSR_VARID_WARM_RESET                            ((CsrUint16)0x4002)


/* Set the bit rate */
#define CSR_PSKEY_UART_BITRATE                          ((CsrUint16)0x01EA)
static CsrUint32 bitRate =                              ((CsrUint32)115200);

/* Set the Crystal frequency                                        */
#define CSR_PSKEY_ANA_FREQ                              ((CsrUint16)0x01FE)
#define CSR_PSKEY_ANA_FREQ_13MHZ                        ((CsrUint16)0x328C)
#define CSR_PSKEY_ANA_FREQ_16MHZ                        ((CsrUint16)0x3E80)
#define CSR_PSKEY_ANA_FREQ_26MHZ                        ((CsrUint16)0x6590)
#define CSR_PSKEY_ANA_FREQ_32MHZ                        ((CsrUint16)0x7D00)
static CsrUint16 mhz =                                  ((CsrUint16)26000);

/* Setup Crystal frequency trim                 
 * Mimimum value is 0x0000                      
 * Maximum value is 0x003F                      
 */                                             
#define CSR_PSKEY_ANA_FTRIM                             ((CsrUint16)0x01F6)  
static CsrUint16 ana_ftrim =                            ((CsrUint16)0x0025);

/* Maximum HCI ACL packet length. The maximum acceptable length, in bytes, 
   of the data portion of HCI ACL data packets received from the host. 
   
   This supports the HCI_Read_Buffer_Size HCI command. It sometimes makes 
   sense to tune this value to obtain better device throughput, but the 
   product of the values of this key and of PSKEY_H_HC_FC_MAX_ACL_PKTS 
   should be kept constant as this effectively reserves a block of the 
   device's precious RAM for use by the host.

   Mimimum value is 0x0001                      
   Maximum value is 0x0200   
*/
#define CSR_PSKEY_H_HC_FC_MAX_ACL_PKT_LEN               ((CsrUint16)0x0011)
static CsrUint16 h_hc_fc_max_acl_pkt_len =              ((CsrUint16)0x0156);

/* Maximum number of HCI ACL packets. The maximum number of HCI ACL data 
   packets being sent to air that can be held within the chip at any time.
   This supports the HCI_Read_Buffer_Size HCI command. See the description 
   of CSR_PSKEY_H_HC_FC_MAX_ACL_PKT_LEN above.
   
   Mimimum value is 0x0001                      
   Maximum value is 0x000E   
*/
#define CSR_PSKEY_H_HC_FC_MAX_ACL_PKTS                  ((CsrUint16)0x0013)
static CsrUint16 h_hc_fc_max_acl_pkts =                 ((CsrUint16)0x0009);

/* Set the local's device Bluetooth address. This should be unique to this
   device - allocated during manufacturing.

   The type bdaddr can be viewed as a CsrUint16[4] array:

        1st CsrUint16: The top 8 bits of the LAP are in the bottom 8
                    bits of this word. The top 8 bits of this word
                    must be zero.

        2nd CsrUint16: lower 16 bits of the LAP

        3rd CsrUint16: The 8 bit UAP is in the bottom 8 bits of this
                    word. The top 8 bits of this word must be zero.

        4th CsrUint16: 16 bit NAP

   For example, the Bluetooth address 0x1234 0x56 0x789abc is encoded as
   0x0078, 0x9abc, 0x0056, 0x1234.

   The default value of this key is one of CSR's legal addresses:
   0x0002 0x5b 0x00a5a5.

   Bluetooth module manufacturers must obtain their own block of
   addresses from the Bluetooth SIG/IEEE.
*/
#define CSR_PSKEY_BDADDR                                ((CsrUint16)0x0001)
CsrUint16 bdaddr_data[4] = {                             0x0099, 0x9999, 0x005b, 0x0002};                   

/* Set hostio_map_sco_pcm.
 * If hostio_map_sco_pcm is TRUE then all attempts 
 * to open SCO connections map to PCM ports 
 *
 * NOTE: Dynamic PCM port selection works only 
 * if PSKEY_HOSTIO_MAP_SCO_PCM is set to FALSE!
 */
#define CSR_PSKEY_HOSTIO_MAP_SCO_PCM                    ((CsrUint16)0x01AB)
#define CSR_PSKEY_HOSTIO_MAP_SCO_PCM_TRUE               ((CsrUint16)0x0001)
#define CSR_PSKEY_HOSTIO_MAP_SCO_PCM_FALSE              ((CsrUint16)0x0000)
static CsrUint16 hostio_map_sco_pcm =                   ((CsrUint16)TRUE);


/* Set hostio_map_sco_codec.
 * If hostio_map_sco_codec is TRUE, and if hostio_map_sco_pcm is also
 * TRUE, then all SCO connections are routed through the built-in
 * audio codec rather than though the normal PCM interface.
 */
#define CSR_PSKEY_HOSTIO_MAP_CODEC                      ((CsrUint16)0x01B0)    
#define CSR_PSKEY_HOSTIO_MAP_CODEC_TRUE                 ((CsrUint16)0x0001)
#define CSR_PSKEY_HOSTIO_MAP_CODEC_FALSE                ((CsrUint16)0x0000)
static CsrUint16 hostio_map_sco_codec =                 ((CsrUint16)TRUE);
                                                        
/* PIO to enable when built-in codec is                 
 * enabled (PSKEY_CODEC_PIO)                            
 * Mimimum value is 0x0000                              
 * Maximum value is 0x000F  */                          
#define CSR_PSKEY_CODEC_PIO                             ((CsrUint16)0x01B9)
static CsrUint16 codec_pio =                            ((CsrUint16)0x0008);

/* Minimum CPU clock speed with PCM port running    */
#define CSR_PSKEY_PCM_MIN_CPU_CLOCK                     ((CsrUint16)0x024D)
#define CSR_PSKEY_PCM_MIN_CPU_CLOCK_CPU_FAST            ((CsrUint16)0x0000)    
#define CSR_PSKEY_PCM_MIN_CPU_CLOCK_CPU_SLOW_4M         ((CsrUint16)0x0001)
#define CSR_PSKEY_PCM_MIN_CPU_CLOCK_CPU_SLOW_2M         ((CsrUint16)0x0002)
#define CSR_PSKEY_PCM_MIN_CPU_CLOCK_CPU_SLOW_1P024M     ((CsrUint16)0x0003)
static CsrUint16 pcm_min_cpu_clock =                    ((CsrUint16)0x0000);

/* Setup the maximum number of SCO links            */
#define CSR_PSKEY_MAX_SCOS                              ((CsrUint16)0x000E)
#define CSR_PSKEY_MAX_SCOS_NONE                         ((CsrUint16)0x0000)
#define CSR_PSKEY_MAX_SCOS_ONE                          ((CsrUint16)0x0001)
#define CSR_PSKEY_MAX_SCOS_TWO                          ((CsrUint16)0x0002)
#define CSR_PSKEY_MAX_SCOS_THREE                        ((CsrUint16)0x0003)
static CsrUint16 max_scos =                             ((CsrUint16)CSR_BT_MAX_NUM_SCO_CONNS);

/* Setup host transport                             */
#define CSR_PSKEY_HOST_INTERFACE                        ((CsrUint16)0x01F9)
#define CSR_PSKEY_HOST_INTERFACE_BCSP                   ((CsrUint16)0x0001)
#define CSR_PSKEY_HOST_INTERFACE_H2_USB                 ((CsrUint16)0x0002)
#define CSR_PSKEY_HOST_INTERFACE_H4DS                   ((CsrUint16)0x0007)
#define CSR_PSKEY_HOST_INTERFACE_SDIO                   ((CsrUint16)0x0009)
static CsrUint16 host_interface =                       ((CsrUint16)0x0001);

/* VM disable                                       */
#define CSR_PSKEY_VM_DISABLE                            ((CsrUint16)0x025D)
#define CSR_PSKEY_VM_DISABLE_TRUE                       ((CsrUint16)0x0001)
#define CSR_PSKEY_VM_DISABLE_FALSE                      ((CsrUint16)0x0000)
static CsrUint16 vm_disable =                           ((CsrUint16)CSR_PSKEY_VM_DISABLE_TRUE);


/* Setup the friendly name for the device
 * Maximum Length is defined in CSR_BT_MAX_FRIENDLY_NAME_LEN
 * (or 40 if MAX_FRINDLY_NAME_LEN is longer)
 */
#define CSR_PSKEY_DEVICE_NAME                           ((CsrUint16)0x0108)
static CsrBtDeviceName friendly_name =                  "CSR-SYNERGY-BC8810 A04 ROM";

/*************************************************************************************
 *                    Generic helper functions 
 ************************************************************************************/
void setBootStrapHostInterface(CsrUint16 transportType)
{ /* Setting the host interface                             */
    switch(transportType)
    {
        case TRANSPORT_TYPE_BCSP:
            {
                host_interface = CSR_PSKEY_HOST_INTERFACE_BCSP;
                break;
            }
        case TRANSPORT_TYPE_USB:
            {
                host_interface = CSR_PSKEY_HOST_INTERFACE_H2_USB;
                break;
            }
        case TRANSPORT_TYPE_H4_DS:
            {
                host_interface = CSR_PSKEY_HOST_INTERFACE_H4DS;
                break;
            }
        case TRANSPORT_TYPE_TYPE_A:
            {
                host_interface = CSR_PSKEY_HOST_INTERFACE_SDIO;
                break;
            }
        default:
            printf("***Unhandled transport type: %d\n", transportType);
            exit(1);
            break;
    }
}
void setBootstrapBaudrate(CsrUint32 inBitRate)
{ 
    bitRate = inBitRate;
}

void setBootstrapFrequency(CsrUint32 freq)
{ /* Setting the Crystal frequency                  */
    mhz =(CsrUint16) freq & 0x0000ffff;
}

void setBootstrapFtrim(CsrUint16 ftrimIn)
{ /* Setting Crystal frequency trim                 */
    ana_ftrim = ftrimIn;
}

void setBootstrapLocalDeviceBluetoothAddress(CsrBtDeviceAddr *bdAddr)
{ /* Setting the local device's Bluetooth Adress */
    bdaddr_data[3] = bdAddr->nap;
    bdaddr_data[2] = bdAddr->uap;
    bdaddr_data[1] = (CsrUint16) bdAddr->lap & 0xffff;
    bdaddr_data[0] = (CsrUint16) (bdAddr->lap >> 16);
}

void setBootstrapLocalDeviceFriendlyName(char* name)
{ /* Setting local device's "user friendly" name */  
    CsrUint16 length = CSR_BT_MAX_FRIENDLY_NAME_LEN;
    if (length>40) /* we can only have 40 chars in friendly name */ 
    {
        length=40;
    }
    CsrStrNCpy(friendly_name,name,length);
    friendly_name[length] = '\0';
}


/*************************************************************************************
 *                    Generic Patches
 ************************************************************************************/
/* [DISABLED]

#
# Disabled generic keys for all builds
# File revision: 1

# PSKEY_LMP_REMOTE_VERSION
#
# If the value from usr_config.h should be used, these lines must be uncommented:
 */
/* #define CSR_PSKEY_GENERIC_PATCH_0                                  ((CsrUint16)0x010e) */
	/* static const CsrUint16 generic_patch_0[] = {
	BCHS_BT_VERSION }; */


/*************************************************************************************/
/*                    Patches for buildId 6817                                       */
/*************************************************************************************/

/* # =============================================================================
# 
# CSR tracking information, report this in case of problems:
#   Revision: #3 
#   Built Mon Aug 23 16:31:08 2010 GMT
#   Checked-in DateTime: 2010/08/23 17:33:05 
#   File: #depot/bc/gemini-3/dev/patches/6817/pb-90.psr 
# 
# Patch is for build 6817 gem_5hci_rom_bt3.0_gemini-3_1003261729_encr128
# 
# Issues patched: B-80744, B-81790, B-82370, B-82487, B-82777, B-83530,
#                 B-87000
# 
# Documentation from pb-90.pb
# ---------------------------
# 
# General purpose patch and PSKEY bundle for BC8810/BC8815 A04 CSP Packages.
# 
# Documentation from bt_pskeys.pb
# -------------------------------
# 
# Issues patched: B-81790, B-82370, B-82487
# 
# Non-default PSKEY settings aimed at improving Bluetooth performance.
# These are recommended for all users of this device regardless of
# package or application.
# 
# Note that some of these settings may depend on the presence of other
# PSKEYs and/or patches. 
# 
# Documentation from patch_lna_bias_c.c
# -------------------------------------
# 
# Issues patched: B-82777
# 
# Receiver desensitisation observed on channels that are multiples of the 
# reference (crystal) frequency on BC8810.
# 
# Documentation from patch_lna_bias_asm.xap
# -----------------------------------------
# 
# Issues patched: B-82777
# 
# Receiver desensitisation observed on channels that are multiples of the 
# reference (crystal) frequency on BC8810.
# 
# Documentation from patch_lo_injection.xap
# -----------------------------------------
# 
# Issues patched: B-83530
# 
# Use High LO injection side for transmit on channel 0.
# 
# Documentation from patch_radio_trim.xap
# ---------------------------------------
# 
# Issues patched: B-87000
# 
# On BlueCore 7 disabling tx with bccmd caused higher than expected power
# consumption. This was due to the BlueCore continually attempting to trim the
# bluetooth radio but not being able to beacuse tx was disabled.
# 
# Documentation from patch_pcm_reset.xap
# --------------------------------------
# 
# Issues patched: B-80744
# 
# The PCM hardware block was not being properly reset for either PCM port. 
# 
# Version information for input files
# -----------------------------------
# 
# Created by patchmaker.pl Revision: #1  Date: 2009/12/07 
# 
# Input file: pb-90.pb
# Depot file: #depot/bc/gemini-3/dev/patches/6817/pb-90.pb
# File revision: 1
# 
# Input file: common.pb
# Depot file: #depot/bc/gemini-3/dev/patches/6817/common.pb
# File revision: 2
# 
# Input file: bt_pskeys.pb
# Depot file: #depot/bc/gemini-3/dev/patches/6817/bt_pskeys.pb
# File revision: 8
# 
# Input file: patch_lna_bias_c.c
# Depot file: #depot/bc/gemini-3/dev/patches/6817/patch_lna_bias_c.c
# File revision: 2
# 
# Input file: patch_lna_bias_asm.xap
# Depot file: #depot/bc/gemini-3/dev/patches/6817/patch_lna_bias_asm.xap
# File revision: 4
# 
# Input file: ../patch_utils.inc
# Depot file: #depot/bc/gemini-3/dev/patches/patch_utils.inc
# File revision: 1
# 
# Input file: patch_lo_injection.xap
# Depot file: #depot/bc/gemini-3/dev/patches/6817/patch_lo_injection.xap
# File revision: 2
# 
# Input file: patch_radio_trim.xap
# Depot file: #depot/bc/gemini-3/dev/patches/6817/patch_radio_trim.xap
# File revision: 3
# 
# Input file: patch_pcm_reset.xap
# Depot file: #depot/bc/gemini-3/dev/patches/6817/patch_pcm_reset.xap
# File revision: 3

# explicit, PSKEY_RX_MR_SAMP_CONFIG (0x003c, 60), 1 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_0                                  ((CsrUint16)0x003c)
static const CsrUint16 buildId_6817_patch_0[] = {
	0x1426 };

/* 
# explicit, PSKEY_TX_OFFSET_HALF_MHZ (0x0217, 535), 1 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_1                                  ((CsrUint16)0x0217)
static const CsrUint16 buildId_6817_patch_1[] = {
	0xffff };

/* 
# explicit, PSKEY_TEMPERATURE_VS_DELTA_TX_BB_MR_PAYLOAD (0x03aa, 938), 4 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_2                                  ((CsrUint16)0x03aa)
static const CsrUint16 buildId_6817_patch_2[] = {
	0xffd8, 0x0010, 0x0014, 0x0000 };

/* 
# explicit, PSKEY_TEMPERATURE_VS_DELTA_TX_BB_MR_HEADER (0x03ab, 939), 4 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_3                                  ((CsrUint16)0x03ab)
static const CsrUint16 buildId_6817_patch_3[] = {
	0xffd8, 0x000b, 0x0014, 0x0000 };

/* 
# explicit, PSKEY_TEMPERATURE_VS_DELTA_TX_BB (0x03d8, 984), 4 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_4                                  ((CsrUint16)0x03d8)
static const CsrUint16 buildId_6817_patch_4[] = {
	0xffd8, 0x0017, 0x0014, 0x0000 };

/* 
# explicit, PSKEY_TEMPERATURE_VS_DELTA_INTERNAL_PA (0x03da, 986), 4 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_5                                  ((CsrUint16)0x03da)
static const CsrUint16 buildId_6817_patch_5[] = {
	0xffd8, 0xfffc, 0x0014, 0x0000 };

/* 
# patch_hardware_0, PSKEY_PATCH50 (0x212c, 8492), 39 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_6                                  ((CsrUint16)0x212c)
static const CsrUint16 buildId_6817_patch_6[] = {
	0x0000, 0x57dc, 0x0118, 0xff2b, 0xff0e, 0x2400, 0xb418, 0x009e,
	0x011b, 0x2c16, 0x0900, 0x6234, 0xec00, 0x9e85, 0x0df4, 0xec00,
	0x9f85, 0x0af4, 0xec00, 0xa085, 0x07f4, 0xec00, 0xa185, 0x04f4,
	0xec00, 0xa285, 0x06f0, 0xf000, 0x1c15, 0x07b4, 0xf000, 0x1c25,
	0x0018, 0xff2b, 0xff0e, 0x5800, 0xdf18, 0x00e2, 0x10f9 };

/* 
# patch_hardware_1, PSKEY_PATCH51 (0x212d, 8493), 13 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_7                                  ((CsrUint16)0x212d)
static const CsrUint16 buildId_6817_patch_7[] = {
	0x0000, 0xf95f, 0xe099, 0x03f4, 0x0214, 0x8f0f, 0x0018, 0xff2b,
	0xff0e, 0xf900, 0x6518, 0x00e2, 0x4e7a };

/* 
# patch_hardware_15, PSKEY_PATCH65 (0x213b, 8507), 59 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_8                                  ((CsrUint16)0x213b)
static const CsrUint16 buildId_6817_patch_8[] = {
	0x0001, 0x3f38, 0x0218, 0xf200, 0x1929, 0x0100, 0x0114, 0xf200,
	0x1a25, 0xfa00, 0x9019, 0x01b8, 0xfa00, 0x9029, 0xfa00, 0x0211,
	0x03b0, 0xfa00, 0x0221, 0x1414, 0xff34, 0xfff0, 0x0800, 0x0014,
	0xf200, 0x1725, 0x1414, 0xff34, 0xfff0, 0xf200, 0x1725, 0xfcc0,
	0xfa00, 0x0221, 0xf200, 0x1925, 0xf200, 0x1a25, 0xfec8, 0xfa00,
	0x9029, 0xf200, 0xf315, 0x09f0, 0x0110, 0xf200, 0xf321, 0xf200,
	0x1725, 0xf200, 0x1825, 0xcfe0, 0x0118, 0xff2b, 0xff0e, 0x3f00,
	0x6518, 0x00e2, 0x77be };

/* 
# explicit, PSKEY_BT_TX_MIXER_CTRIM_OFFSET (0x2175, 8565), 5 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_9                                  ((CsrUint16)0x2175)
static const CsrUint16 buildId_6817_patch_9[] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/* 
# explicit, PSKEY_BT_ANA_TX_MIXER_CONFIG (0x218c, 8588), 1 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_10                                  ((CsrUint16)0x218c)
static const CsrUint16 buildId_6817_patch_10[] = {
	0x0072 };

/* 
# patch_bc_boot, PSKEY_PATCH101 (0x21f5, 8693), 58 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_11                                  ((CsrUint16)0x21f5)
static const CsrUint16 buildId_6817_patch_11[] = {
	0xfe0b, 0xe300, 0x8215, 0x0010, 0x0009, 0x6494, 0xf925, 0x5e00,
	0xd414, 0x0010, 0x0009, 0xf995, 0xf825, 0x6100, 0xe014, 0x0010,
	0x0009, 0xf995, 0xfb25, 0xf815, 0xf991, 0x0534, 0xfa25, 0xf819,
	0xff2b, 0x0010, 0xf821, 0x1ae0, 0xfa15, 0x0010, 0x0009, 0x0a94,
	0xe111, 0xf700, 0x9e34, 0x4e84, 0x022c, 0x0ae0, 0xf815, 0x0484,
	0x022c, 0x06e0, 0xf819, 0xec00, 0x9e22, 0x0138, 0xf829, 0xff13,
	0x0130, 0xff23, 0xfa15, 0xf935, 0xfa25, 0xff1b, 0xfb89, 0xe52c,
	0xfe0f, 0xe266 };

/* 
# patch_hal_lo_settings_shared_patchpoint, PSKEY_PATCH146 (0x2222, 8738), 9 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_12                                  ((CsrUint16)0x2222)
static const CsrUint16 buildId_6817_patch_12[] = {
	0x0300, 0x5388, 0x00f2, 0xef00, 0x1d14, 0xe400, 0x7825, 0x00e2,
	0x5a9e };

/* 
# explicit, PSKEY_RSSI_PREFERRED_VALUES (0x22ec, 8940), 5 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_13                                  ((CsrUint16)0x22ec)
static const CsrUint16 buildId_6817_patch_13[] = {
	0x0000, 0x0003, 0x0003, 0x03d0, 0x0000 };

/* 
# explicit, PSKEY_BT_POWER_TABLE_V0 (0x241a, 9242), 40 words
 */
#define CSR_PSKEY_BUILDID_6817_PATCH_14                                  ((CsrUint16)0x241a)
static const CsrUint16 buildId_6817_patch_14[] = {
	0x2718, 0x0050, 0x2c28, 0x0050, 0xec00, 0x2918, 0x0040, 0x2e28,
	0x0040, 0xf000, 0x2818, 0x0030, 0x2c28, 0x0030, 0xf400, 0x2818,
	0x0020, 0x2c28, 0x0020, 0xf800, 0x2918, 0x0010, 0x2d28, 0x0010,
	0xfc00, 0x2d1a, 0x0000, 0x302a, 0x0000, 0x0000, 0x365c, 0x0000,
	0x386c, 0x0000, 0x0400, 0x3fff, 0x0000, 0x3fff, 0x0000, 0x0800 };


/*************************************************************************************/
/*                              END OF PATCHES                                       */
/*************************************************************************************/
/* Specific bootstrap helper function: Set the host transport
 * interface. Use the framework TRANSPORT_TYPE defines */
void CsrBtBootstrapSetInterface(void *bootContext,
                                CsrUint16 hostInterface)
{
}

/* Specific bootstrap helper function: Set the host interface bitrate
 * (for use with UART host transports like BCSP, H4, H4DS etc) */
void CsrBtBootstrapSetBitRate(void *bootContext,
                              CsrUint32 bitRate)
{
}

/* Specific bootstrap helper function: The the Bluetooth friendly
 * name */
void CsrBtBootstrapSetFriendlyName(void *bootContext,
                                   CsrUtf8String *name)
{
}
                                    
/* Specific bootstrap helper function: Set the BlueCore Bluetooth
 * address */   
void CsrBtBootstrapSetLocalBtAddress(void *bootContext,
                                     CsrBtDeviceAddr *address)
{
}

/* Specific bootstrap helper function: Set the crystal frequency */
void CsrBtBootstrapSetFrequency(void *bootContext,
                                CsrUint16 frequency)
{
}

/* Specific bootstrap helper function: Set the crystal frequency
 * trim */
void CsrBtBootstrapSetFreqTrim(void *bootContext,
                               CsrUint16 freqTrim)
{
}


CsrUint8 **CsrTmBlueCoreGetBootstrap(CsrUint16 buildId, CsrUint16 *entries)
{
	CsrUint16 numOfEntries = 14;
    CsrUint16 i            = 0;
    CsrUint8  **a          = NULL;
    CsrUint16 bitRateArray[2]; 
    bitRateArray[0] = (CsrUint16)(bitRate >>16);
    bitRateArray[1] = (CsrUint16)(bitRate & 0xffff);


    switch (buildId)
    {
		case CSR_BUILD_ID_6817:
		{
			numOfEntries += 15;
			break;
		}
        default:
        {
            break;
        }
    }

    a = (CsrUint8 **) CsrPmemAlloc(sizeof(CsrUint8 **) * numOfEntries);


    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_ANA_FREQ, CSR_BCCMD_STORES_PSRAM, (sizeof(mhz)/2), &mhz);
    if (host_interface == CSR_PSKEY_HOST_INTERFACE_BCSP ||
        host_interface == CSR_PSKEY_HOST_INTERFACE_H4DS)
    {
        a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_UART_BITRATE, CSR_BCCMD_STORES_PSRAM, (sizeof(bitRateArray)/2), bitRateArray);
    }
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_HOSTIO_MAP_SCO_PCM, CSR_BCCMD_STORES_PSRAM, (sizeof(hostio_map_sco_pcm)/2), &hostio_map_sco_pcm);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_HOSTIO_MAP_CODEC, CSR_BCCMD_STORES_PSRAM, (sizeof(hostio_map_sco_codec)/2), &hostio_map_sco_codec);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_CODEC_PIO, CSR_BCCMD_STORES_PSRAM, (sizeof(codec_pio)/2), &codec_pio);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_ANA_FTRIM, CSR_BCCMD_STORES_PSRAM, (sizeof(ana_ftrim)/2), &ana_ftrim);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_H_HC_FC_MAX_ACL_PKT_LEN, CSR_BCCMD_STORES_PSRAM, (sizeof(h_hc_fc_max_acl_pkt_len)/2), &h_hc_fc_max_acl_pkt_len);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_H_HC_FC_MAX_ACL_PKTS, CSR_BCCMD_STORES_PSRAM, (sizeof(h_hc_fc_max_acl_pkts)/2), &h_hc_fc_max_acl_pkts);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_PCM_MIN_CPU_CLOCK, CSR_BCCMD_STORES_PSRAM, (sizeof(pcm_min_cpu_clock)/2), &pcm_min_cpu_clock);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_MAX_SCOS, CSR_BCCMD_STORES_PSRAM, (sizeof(max_scos)/2), &max_scos);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_HOST_INTERFACE, CSR_BCCMD_STORES_PSRAM, (sizeof(host_interface)/2), &host_interface);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_VM_DISABLE, CSR_BCCMD_STORES_PSRAM, (sizeof(vm_disable)/2), &vm_disable);
    a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BDADDR, CSR_BCCMD_STORES_PSRAM, (sizeof(bdaddr_data)/2), bdaddr_data);
            
	/* a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_GENERIC_PATCH_0,CSR_BCCMD_STORES_PSRAM, (sizeof(generic_patch_0)/2),generic_patch_0); */

	switch (buildId)
	{
		case CSR_BUILD_ID_6817:
		{
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_0,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_0)/2),buildId_6817_patch_0);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_1,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_1)/2),buildId_6817_patch_1);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_2,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_2)/2),buildId_6817_patch_2);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_3,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_3)/2),buildId_6817_patch_3);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_4,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_4)/2),buildId_6817_patch_4);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_5,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_5)/2),buildId_6817_patch_5);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_6,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_6)/2),buildId_6817_patch_6);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_7,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_7)/2),buildId_6817_patch_7);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_8,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_8)/2),buildId_6817_patch_8);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_9,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_9)/2),buildId_6817_patch_9);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_10,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_10)/2),buildId_6817_patch_10);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_11,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_11)/2),buildId_6817_patch_11);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_12,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_12)/2),buildId_6817_patch_12);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_13,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_13)/2),buildId_6817_patch_13);
			a[i++] = CsrTmBlueCoreBuildBccmdPsSetMsg(CSR_PSKEY_BUILDID_6817_PATCH_14,CSR_BCCMD_STORES_PSRAM, (sizeof(buildId_6817_patch_14)/2),buildId_6817_patch_14);
			break;
		}
        default:
        {
            break;
        }
    }

    a[i++] = CsrTmBlueCoreBuildBccmdSetMsg(CSR_VARID_WARM_RESET, 0, NULL);
    *entries = i;
    return a;
}

