#ifndef __HMAC_SHA1_H__
#define __HMAC_SHA1_H__

#define SHA1_BLOCK_SIZE         (64)

int HMAC_Sha1_buf(unsigned char *key, unsigned char *buf, int size, unsigned char *mac) ;
int HMAC_Sha1(unsigned char *key, int fd, int real_size, unsigned char *mac);
int HMAC_Sha1_buf_nokey(unsigned char *buf, int size, unsigned char *mac);
int HMAC_Sha1_nokey(int fd, int real_size, unsigned char *mac);

int HMAC_Sha256_buf(unsigned char *key, unsigned char *buf, int size, unsigned char *mac) ;
int HMAC_Sha256(unsigned char *key, int fd, int real_size, unsigned char *mac);
int HMAC_Sha256_buf_nokey(unsigned char *buf, int size, unsigned char *mac);
int HMAC_Sha256_nokey(int fd, int real_size, unsigned char *mac);


#endif // __HMAC_SHA1_H__

