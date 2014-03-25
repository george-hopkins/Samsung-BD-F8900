/* header file sdp_spi master driver */

#ifndef __ARM_MACH_SDP_SPI_H
#define __ARM_MACH_SDP_SPI_H

struct spi_device;

#ifndef CONFIG_SDP_SPI_NUM_OF_CS
#define CONFIG_SDP_SPI_NUM_OF_CS	(1)// default 1, is defined at Kconfig
#endif

/**
 * struct sdp_spi_info - SDP specific SPI descriptor
 * @num_chipselect: number of chip selects on this board, must be
 *                  at least one
 * @init: board specific init code.
 * @default_chip_ops: default chip ops pointer.
 */
struct sdp_spi_info {
	int	num_chipselect;
	int	max_clk_limit;
	int (*init)(void);
	struct sdp_spi_chip_ops *default_chip_ops;
	unsigned long ext_clkrate;
};

/**
 * struct sdp_spi_chip_ops - operation callbacks for SPI slave device
 * @setup: setup the chip select mechanism
 * @cleanup: cleanup the chip select mechanism
 * @cs_control: control the device chip select
 */
struct sdp_spi_chip_ops {
	int	(*setup)(struct spi_device *spi);
	void	(*cleanup)(struct spi_device *spi);
	void	(*cs_control)(struct spi_device *spi, int value);
};

#endif /* __ARM_MACH_SDP_SPI_H */
