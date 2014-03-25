/*
 * (C) Copyright 2011
 * Dongseok Lee, Samsung Erectronics, drain.lee@samsung.com.
 *      - only support for Firenze
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/************************************************
 * NAME	    : sdp_nand.h
 ************************************************/


#ifndef __SDP_NAND_H__
#define __SDP_NAND_H__

/* for platform data */
struct sdp_nand_platform {
	/*timing data. NF_TACLS, NF_TWRPH0, NF_TWRPH1 */
	int tacls;
	int twrph0;
	int twrph1;
};

#endif//__SDP_NAND_H__