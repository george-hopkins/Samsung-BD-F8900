/*
 * linux/arch/arm/mach-ccep/sdp_unzip.h
 *
 * Copyright (C) 2012 Samsung Electronics.co
 * Author : seunjgun.heo@samsung.com
 *
 */
#ifndef __SDP_UNZIP_H
#define __SDP_UNZIP_H

#ifdef CONFIG_SDP_UNZIP_TEST_DRIVER

/*
#define GZIP_MODE_NORMAL		0
#define GZIP_MODE_IN_PAR		1
#define GZIP_MODE_OUT_PAR	2
#define GZIP_MODE_INOUT_PAR	3
#define GZIP_MODE_ADV		4
*/

#define GZIP_IOC_DECOMPRESS	'D'
#define GZIP_IOC_DECOMPRESS_AGING	'E'

struct sdp_unzip_arg_t {
	int npages;
	int size_ibuff;
	char *ibuff;
	char *opages[32];
};

struct sdp_unzip_aging_arg_t {
	int nbufs;
	char *ibuffes[20];
	int ibuffsize[20];
	char *obuff;
	char *tbuffes[20];
};


#endif

/**
 * sdp_unzip_init - decompress gzip file
 * @ibuff : input buffer pointer. must be aligned QWORD(8bytes)
 * @ilength : input buffer pointer. must be multiple of 8.
 * @opages: array of output buffer pages. page size = 4K(fixed)
 * @npages: number of output buffer pages. maximum number is 32.
 */
int sdp_unzip_init(void);

/**
 * sdp_unzip_decompress - decompress gzip file
 * @ibuff : input buffer pointer. must be aligned 64bytes
 * @ilength : input buffer pointer. must be multiple of 64.
 * @opages: array of output buffer pages. page size = 4K(fixed)
 * @npages: number of output buffer pages. maximum number is 32.
 */

int sdp_unzip_decompress(char *ibuff, int ilength, char *opages[], int npages);

#endif
