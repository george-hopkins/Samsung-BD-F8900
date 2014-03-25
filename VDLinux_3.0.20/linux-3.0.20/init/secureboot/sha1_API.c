/*
 *  sha1test.c
 *
 *  Description:
 *      This file will exercise the SHA-1 code performing the three
 *      tests documented in FIPS PUB 180-1 plus one which calls
 *      SHA1Input with an exact multiple of 512 bits, plus a few
 *      error test checks.
 *
 *  Portability Issues:
 *      None.
 *
 */

#include <linux/types.h>	/* for size_t */
#include <linux/syscalls.h>
#include "include/sha1.h"
#include "include/sha256.h"
#include "include/hmac_sha1.h"
#include "include/Secureboot.h"

extern void * memcpy(void *, const void *, unsigned int);
//extern void * memset(void *,int,__kernel_size_t);

int read_from_file_or_ff(int fd, unsigned char *buf, int size) 
{
	int read_cnt;
	int i;

	read_cnt = sys_read(fd, buf, size);

	if( read_cnt <= 0 )
	{
		read_cnt = 0;
	}

	for(i=read_cnt; i<size; i++)
	{
		buf[i] = 0xff;
	}

	return size;
}

/*
 *  Define patterns for testing
 */
int HMAC_Sha1_buf(unsigned char *key, unsigned char *buf, int size, unsigned char *mac) {
	SHA1Context sha;
	unsigned char opad[SHA1_BLOCK_SIZE];
	unsigned char ipad[SHA1_BLOCK_SIZE];
	unsigned char intermediate[SHA1_BLOCK_SIZE];	
	uint8_t Message_Digest[20];
	int i;

	memset(opad, 0x5c, SHA1_BLOCK_SIZE);
	memset(ipad, 0x36, SHA1_BLOCK_SIZE);
	memset(intermediate,0x00, SHA1_BLOCK_SIZE);

	for(i=0;i<16;i++) {
		ipad[i] = ipad[i] ^ key[i];
		opad[i] = opad[i] ^ key[i];
	}

	SHA1Reset(&sha);
	SHA1Input(&sha, ipad, SHA1_BLOCK_SIZE);
	SHA1Input(&sha, buf, size);
	SHA1Result(&sha, intermediate);

	SHA1Reset(&sha);
	SHA1Input(&sha, opad, SHA1_BLOCK_SIZE);
	SHA1Input(&sha, intermediate, SHA1HashSize);
	SHA1Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SIZE);
	return 1;

}

int HMAC_Sha256_buf(unsigned char *key, unsigned char *buf, int size, unsigned char *mac) {
	SHA256Context sha;
	unsigned char opad[SHA1_BLOCK_SIZE];
	unsigned char ipad[SHA1_BLOCK_SIZE];
	unsigned char intermediate[SHA1_BLOCK_SIZE];	
	uint8_t Message_Digest[32];
	int i;

	memset(opad, 0x5c, SHA1_BLOCK_SIZE);
	memset(ipad, 0x36, SHA1_BLOCK_SIZE);
	memset(intermediate, 0x00, SHA1_BLOCK_SIZE);

	for(i=0;i<16;i++) {
		ipad[i] = ipad[i] ^ key[i];
		opad[i] = opad[i] ^ key[i];
	}

	SHA256Reset(&sha);
	SHA256Input(&sha, ipad, SHA1_BLOCK_SIZE);
	SHA256Input(&sha, buf, size);
	SHA256Result(&sha, intermediate);

	SHA256Reset(&sha);
	SHA256Input(&sha, opad, SHA1_BLOCK_SIZE);
	SHA256Input(&sha, intermediate, SHA256HashSize);
	SHA256Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SHA256_SIZE);
	return 1;

}


int HMAC_Sha1(unsigned char *key, int fd, int real_size, unsigned char *mac) {
	SHA1Context sha;
	unsigned char opad[SHA1_BLOCK_SIZE];
	unsigned char ipad[SHA1_BLOCK_SIZE];
	int i, err;
	int read_len, total_len;
	unsigned char msg[SIZE_1K];
	uint8_t Message_Digest[20];
	unsigned char intermediate[SHA1_BLOCK_SIZE];

	memset(opad, 0x5c, SHA1_BLOCK_SIZE);
	memset(ipad, 0x36, SHA1_BLOCK_SIZE);
	for(i=0;i<16;i++) {
		ipad[i] = ipad[i] ^ key[i];
		opad[i] = opad[i] ^ key[i];
	}

	SHA1Reset(&sha);
	SHA1Input(&sha, ipad, SHA1_BLOCK_SIZE);

	total_len = 0;

	while(1) {

		read_len = read_from_file_or_ff(fd, msg, SIZE_1K);  // file에서 읽거나 file 범위 밖이면 ff로 읽어들임.


		if( total_len+read_len >= real_size ) {
			read_len = real_size - total_len;
			total_len += read_len;
			SHA1Input(&sha, msg, read_len);
			break;
		}
		total_len += read_len;
		SHA1Input(&sha, msg, read_len);
	}
	err = SHA1Result(&sha, intermediate);
	SHA1Reset(&sha);
	SHA1Input(&sha, opad, SHA1_BLOCK_SIZE);
	SHA1Input(&sha, intermediate, SHA1HashSize);
	SHA1Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SIZE);

	return 1;
}

int HMAC_Sha256(unsigned char *key, int fd, int real_size, unsigned char *mac) {

	SHA256Context sha;
	unsigned char opad[SHA1_BLOCK_SIZE];
	unsigned char ipad[SHA1_BLOCK_SIZE];
	int i, err;
	int read_len, total_len;
	unsigned char msg[SIZE_1K];
	uint8_t Message_Digest[32];
	unsigned char intermediate[SHA1_BLOCK_SIZE];

	memset(opad, 0x5c, SHA1_BLOCK_SIZE);
	memset(ipad, 0x36, SHA1_BLOCK_SIZE);
	for(i=0;i<16;i++) {
		ipad[i] = ipad[i] ^ key[i];
		opad[i] = opad[i] ^ key[i];
	}

	SHA256Reset(&sha);
	SHA256Input(&sha, ipad, SHA1_BLOCK_SIZE);
	
	total_len = 0;

	while(1) {

		read_len = read_from_file_or_ff(fd, msg, SIZE_1K);  // file에서 읽거나 file 범위 밖이면 ff로 읽어들임.


		if( total_len+read_len >= real_size ) {
			read_len = real_size - total_len;
			total_len += read_len;
			SHA256Input(&sha, msg, read_len);
			break;
		}
		total_len += read_len;
		SHA256Input(&sha, msg, read_len);
	}

	err = SHA256Result(&sha, intermediate);
	SHA256Reset(&sha);
	SHA256Input(&sha, opad, SHA1_BLOCK_SIZE);
	SHA256Input(&sha, intermediate, SHA256HashSize);
	SHA256Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SHA256_SIZE);

	return 1;

}


/* H/W use this way.. */
int HMAC_Sha1_nokey(int fd, int real_size, unsigned char *mac)
{
	SHA1Context sha;
	int err;
	int read_len, total_len;
	unsigned char msg[SIZE_1K];
	uint8_t Message_Digest[20];

	SHA1Reset(&sha);
	total_len = 0;

	while(1) {

		read_len = read_from_file_or_ff(fd, msg, SIZE_1K);

		if( total_len+read_len >= real_size ) {
			read_len = real_size - total_len;
			total_len += read_len;
			SHA1Input(&sha, msg, read_len);
			break;
		}
		total_len += read_len;
		SHA1Input(&sha, msg, read_len);
	}
	err = SHA1Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SIZE);

	return 1;
}

int HMAC_Sha256_nokey(int fd, int real_size, unsigned char *mac)
{
	SHA256Context sha;
	int err;
	int read_len, total_len;
	unsigned char msg[SIZE_1K];
	uint8_t Message_Digest[32];

	SHA256Reset(&sha);
	total_len = 0;

	while(1) {

		/* read from file and fill 0xff */
		read_len = read_from_file_or_ff(fd, msg, SIZE_1K);

		/* already read as much as real_size */
		if( total_len+read_len >= real_size ) {
			read_len = real_size - total_len;
			total_len += read_len;	/* FIXME : It should be removed */
			SHA256Input(&sha, msg, read_len);
			break;
		}
		total_len += read_len;
		SHA256Input(&sha, msg, read_len);
	}
	err = SHA256Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SHA256_SIZE);

	return 1;
}


/*
 *  Define patterns for testing
 */
int HMAC_Sha1_buf_nokey(unsigned char *buf, int size, unsigned char *mac)
{
	SHA1Context sha;
	uint8_t Message_Digest[20];


	SHA1Reset(&sha);
	SHA1Input(&sha, buf, size);

	SHA1Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SIZE);
	return 1;

}

int HMAC_Sha256_buf_nokey(unsigned char *buf, int size, unsigned char *mac)
{
	SHA256Context sha;
	uint8_t Message_Digest[32];


	SHA256Reset(&sha);
	SHA256Input(&sha, buf, size);

	SHA256Result(&sha, Message_Digest);
	memcpy(mac, Message_Digest, HMAC_SHA256_SIZE);
	return 1;

}

