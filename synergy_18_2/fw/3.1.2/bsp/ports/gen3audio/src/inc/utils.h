/****************************************************************************/
/* MODULE:                                                                  								*/
/*   - Utils.h
		Definition for Utility Functions unsing TCC721 (208 pin ver.)					*/
/****************************************************************************/
/*
 *   TCC Version 0.0
 *   Copyright (c) telechips, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef __UTILS_H__
#define	__UTILS_H__
extern char upperChar(char ch);
extern char lowerChar(char ch);
extern int printNum(char *sNum, int value);
extern int HexToStr(char *sNum, int value);
extern int str_cmp(void *sA, void *sB);
extern int str_cmp_sfn_fd(const char *fdstr,const  char *str);
extern int str_cmp_path(void *sA, void *sB);
extern int str_cmp_path2(void *sA, void *sB);
extern int str_cmp16(void *sA, void *sB);
extern int str_cmp16a(void *sA, void *sB);
extern int str_cmp16_path(void *sA, void *sB);
extern int str_cmpu(void *sA, void *sB);
extern int str_ncmp(const char *cs, const char *ct, unsigned int count);
extern int str_len(void *pStr);
extern int str_len16(void *pString);
extern char *str_str(char *s1, char *s2);
extern int mem_cmp(void *sA, void *sB, int len);
extern void* mem_cpy(void *pDes, void *pSrc, long size);
extern void* mem_move(void *pDes, void *pSrc, long size);
extern void* mem_cpyw(void *pDes, void *pSrc, long size);
extern void* mem_set(void *pDes, unsigned char value, long size);
extern void itoa(int num, unsigned char * str);
extern const unsigned short crc16_table[256];
extern unsigned short UTIL_CALC_CRC16(unsigned short lInit_Accum, const unsigned char *plData, unsigned int lDataLen);
extern unsigned _random_number_seed[55];
extern int UTIL_Rand(void);

#endif
