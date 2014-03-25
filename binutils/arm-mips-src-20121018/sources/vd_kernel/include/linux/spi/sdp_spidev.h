/*
 * include/linux/spi/sdp_spidev.h
 *
 * Copyright (C) 2011 Samsung elctronics SoC
 *	dongseok lee <drain.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  */

#ifndef SDP_SPIDEV_H
#define SDP_SPIDEV_H

#include <linux/types.h>
#include <linux/spi/spidev.h>

//add frame format in <driver/spi/sdp_spi.c>
enum sdp_spi_frame_format {
	MOTOROLA_SPI = 0x0,
	TI_SYNC_SERIAL = 0x1,
	NATIONAL_MICRO_WIRE = 0x2,
};

struct sdp_spi_chip {
	const struct spi_device		*spi;
	unsigned long			rate;
	__u8				div_cpsr;
	__u8				div_scr;
	__u8				dss;
	enum sdp_spi_frame_format	frame_format;
	struct sdp_spi_chip_ops	*ops;
};
/* Read / Write SPI device frame format
   (MOTOROLA_SPI, TI_SYNC_SERIAL, NATIONAL_MICRO_WIRE)  */
#define SPI_IOC_RD_FRAME_FORMAT		_IOR(SPI_IOC_MAGIC, 5, __u32)
#define SPI_IOC_WR_FRAME_FORMAT		_IOW(SPI_IOC_MAGIC, 5, __u32)

struct sdp_spi_whole_config {
	__u32			max_speed_hz;
	__u8			bits_per_word;//4~16bits
	__u8			spi_mode;//SPI_MODE_0~3
	enum sdp_spi_frame_format	frame_format;//0~2 value
};
#define SPI_IOC_WR_WHOLE_CONFIG		_IOW(SPI_IOC_MAGIC, 6, __u32)


#endif /* SDP_SPIDEV_H */
