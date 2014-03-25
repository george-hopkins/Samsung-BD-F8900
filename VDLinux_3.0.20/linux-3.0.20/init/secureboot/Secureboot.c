
#include <linux/proc_fs.h>
#include <linux/syscalls.h>
#include "include/Secureboot.h"
#include "include/SBKN_CC_API.h"
#include "include/publicKey.h"

extern char rootfs_name[64];

#ifdef SECURE_DEBUG 
void print_20byte(unsigned char *bytes)
{
	int i;
	for(i=0;i<20;i++)
	{
		printk("0x%2x ",bytes[i]);
		if( i%8 == 7 )
			printk("\n");
	}
	printk("\n");
}
void print_32byte(unsigned char *bytes)
{
	int i;
	for(i=0;i<32;i++)
	{
		printk("0x%2x ",bytes[i]);
		if( i%8 == 7 )
			printk("\n");
	}
	printk("\n");
}
void print_128byte(unsigned char *bytes)
{
	int i;
	for(i=0;i<128;i++)
	{
		printk("0x%2x ",bytes[i]);
		if( i%8 == 7 )
			printk("\n");
	}
	printk("\n");
}
void print_256byte(unsigned char *bytes)
{
	int i;
	for(i=0;i<256;i++)
	{
		printk("0x%2x ",bytes[i]);
		if( i%8 == 7 )
			printk("\n");
	}
	printk("\n");
}
#endif

int getAuthUld(macAuthULd_t *mac_authuld)
{
	int fd;

#if defined(CONFIG_NVT_NT72568) || defined(CONFIG_MSTAR_EMERALD)	//NT13 and X13
	if(!strcmp(rootfs_name, "/dev/tfsr5"))
	{
		strncpy(rootfs_name, ROOTFS_PARTITION, 19);
	}
#endif

	fd = sys_open(rootfs_name, O_RDONLY, 0);
	if(fd >= 0) 
	{
		sys_lseek((unsigned int)fd, -4096, SEEK_END);
		sys_read((unsigned int)fd, (void *)mac_authuld, sizeof(MacInfo_t));
		sys_close((unsigned int)fd);
		if(mac_authuld->macAuthULD.msgLen > 0x2FFFF) // authuld size error checking.
		{
			CIP_CRIT_PRINT("%x authuld length is over 0xFFFF: check if authuld sign is added in rootfs \n", 
					mac_authuld->macAuthULD.msgLen);
			return 0;
		}
		else
		{        
			return 1;
		}
	} 
	CIP_CRIT_PRINT("%s file open error(rootfs)\n", rootfs_name);
	
	return 0;
}

#ifdef CONFIG_RSA1024
int verify_rsa_signature(unsigned char *pText, unsigned long pTextLen, unsigned char *signature, unsigned long signLen) 
{
	cc_u8 GE[256] = {0x01, 0x00, 0x01};
	unsigned long NLen = 128, ELen = 3;
	int result =0;

	CryptoCoreContainer *crt;	

	if( pTextLen > 128 )
	{
		pTextLen = 128;
	}
	crt = create_CryptoCoreContainer(ID_RSA1024);
	crt->RSA_setKeypair(crt, ID_NO_PADDING, GN, NLen, GE, ELen, NULL, 0);
	crt->DS_verify(crt, pText, pTextLen, signature, signLen, &result);	
	destroy_CryptoCoreContainer(crt);

#ifdef SECURE_DEBUG
	printk("pukey\n");
	print_128byte(GN);

	printk("sign\n");
	print_128byte(signature);

	printk("rsa result : %d\n",result);
#endif

	if (result == CRYPTO_VALID_SIGN) 
	{
		return CRYPTO_SUCCESS;
	}
	else 
	{
		return CRYPTO_ERROR;
	}
}
#elif CONFIG_RSA2048
int verify_rsa_signature_2048(unsigned char *pText, unsigned long pTextLen, unsigned char *signature, unsigned long signLen) 
{
	cc_u8 GE[256] = {0x01, 0x00, 0x01};
	unsigned long NLen = 256, ELen = 3;
	int result =0;

	CryptoCoreContainer *crt;	

	if( pTextLen > 256 )
	{
		pTextLen = 256;
	}
	crt = create_CryptoCoreContainer(ID_RSA2048);
	crt->RSA_setKeypair(crt, ID_NO_PADDING, GN2048_REL, NLen, GE, ELen, NULL, 0);
	crt->DS_verify(crt, pText, pTextLen, signature, signLen, &result);	
	destroy_CryptoCoreContainer(crt);

#ifdef SECURE_DEBUG
	printk("pukey\n");
	print_256byte(GN2048_REL);

	printk("sign\n");
	print_256byte(signature);

	printk("rsa result : %d\n",result);
#endif

	if (result == CRYPTO_VALID_SIGN) 
	{
		return CRYPTO_SUCCESS;
	}
	else 
	{
		return CRYPTO_ERROR;
	}
}
#endif
