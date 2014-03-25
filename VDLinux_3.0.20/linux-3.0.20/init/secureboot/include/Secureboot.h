#include <linux/kernel.h>

#define CONFIG_HW_SHA1 0

//#define SECURE_DEBUG

#define CIP_CRIT_PRINT(fmt, ...)		printk(KERN_CRIT "[CIP_KERNEL] " fmt,##__VA_ARGS__)
#define CIP_WARN_PRINT(fmt, ...)		printk(KERN_WARNING "[CIP_KERNEL] " fmt,##__VA_ARGS__)
#define CIP_DEBUG_PRINT(fmt, ...)		printk(KERN_DEBUG "[CIP_KERNEL] " fmt,##__VA_ARGS__)

#ifndef	SIZE_1K
#define	SIZE_1K					(1024)
#endif

#ifndef	SZ_1M	
#define	SZ_1M                   (1048576)           // 1024*1024
#endif

#ifndef SZ_4K
#define SZ_4K                   (4096)              // 4*1024
#endif

#ifndef	SZ_128K
#define SZ_128K                 (131072)            // 128*1024
#endif

#ifndef	SZ_256K
#define SZ_256K                 (262144)            // 256*1024
#endif

#ifndef	SZ_4M
#define SZ_4M                   (4194304)           // 4*1024*1024
#endif

#define HMAC_SIZE			(20)
#define HMAC_SHA256_SIZE            (32)

#define RSA_1024_SIGN_SIZE  (128)
#define RSA_1024_KEY_SIZE            (128)
#define RSA_2048_SIGN_SIZE      (256)
#define RSA_2048_KEY_SIZE       (256)

#define SLEEP_WAITING       (30)

#ifdef CONFIG_NVT_NT72568 			//NT13
#define ROOTFS_PARTITION    ("/dev/bml0/5")
#define MICOM_DEV			/dev/ttyS1  /* NOTE : Do not quote this string */
#endif

#if defined(CONFIG_MSTAR_EMERALD) 	//X13
#define ROOTFS_PARTITION	("/dev/bml0/5")
#define MICOM_DEV			/dev/ttyS1  /* NOTE : Do not quote this string */
#endif

#if defined(CONFIG_MSTAR_EDISON) 	//X12
#define MICOM_DEV			/dev/ttyS1  /* NOTE : Do not quote this string */
#endif

#if defined(CONFIG_ARCH_SDP1202)
#define MICOM_DEV			/dev/ttyS2  /* NOTE : Do not quote this string */
#endif

#define SIGNAL_FLAG_TEST    "/dtv/.data_test"

//#define CONFIG_RSA1024
typedef struct 
{
#ifdef CONFIG_RSA1024
	unsigned char au8PreCmacResult[RSA_1024_SIGN_SIZE];	// MAC
#elif CONFIG_RSA2048
	unsigned char au8PreCmacResult[RSA_2048_SIGN_SIZE];	// MAC
#else
	#error "Please set CONFIG_RSA1024 or CONFIG_RSA2048!!!!"
#endif
	unsigned int msgLen;	// the length of a message to be authenticated
} MacInfo_t;

typedef struct 
{
	MacInfo_t macAuthULD;
} macAuthULd_t;

int getAuthUld(macAuthULd_t *authuld);
int read_from_file_or_ff (int fd, unsigned char *buf, int size);
#ifdef CONFIG_RSA1024
int verify_rsa_signature(unsigned char *pText, unsigned long pTextLen, unsigned char *signature, unsigned long signLen);
#else
int verify_rsa_signature_2048(unsigned char *pText, unsigned long pTextLen, unsigned char *signature, unsigned long signLen);
#endif

#ifdef	SECURE_DEBUG
void print_20byte(unsigned char *bytes);
void print_128byte(unsigned char *bytes);
void print_32byte(unsigned char *bytes);
void print_256byte(unsigned char *bytes);
#endif
