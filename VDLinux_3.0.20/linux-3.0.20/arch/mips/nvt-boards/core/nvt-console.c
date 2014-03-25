/*
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
#include <linux/console.h>
#include <linux/init.h>
#include <linux/serial_reg.h>
#include <asm/io.h>

#include <nvt-sys.h>

#define PORT(offset) (NVT_SYS_UART_BASE + ((offset)<<2))

static inline unsigned int serial_in(int offset)
{
	return *(volatile unsigned int*)(PORT(offset));
}

static inline void serial_out(int offset, int value)
{
	*(volatile unsigned int*)(PORT(offset)) = value;

}

int prom_putchar(char c)
{
	while ((serial_in(UART_LSR) & UART_LSR_THRE) == 0)
		;

	serial_out(UART_TX, c);

	return 1;
}
