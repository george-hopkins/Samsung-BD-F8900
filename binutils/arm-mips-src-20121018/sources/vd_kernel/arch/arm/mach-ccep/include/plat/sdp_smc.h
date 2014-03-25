#ifndef _SDP_SMC_H_
#define _SDP_SMC_H_

struct sdp_smc_bank;
struct sdp_smc_operations
{
	void	(*write_sl) (struct sdp_smc_bank *bank, u16 val, u16 __iomem *addr);
	u16	(*read_sl) (struct sdp_smc_bank *bank, u16 __iomem *addr);
	void	(*clear_sl) (struct sdp_smc_bank *bank, u16 mask, u16 __iomem *addr);
	void	(*set_sl) (struct sdp_smc_bank *bank, u16 val, u16 __iomem *addr);
	
	void	(*write_sb) (struct sdp_smc_bank *bank, u8 val, u8 __iomem *addr);
	u8	(*read_sb) (struct sdp_smc_bank *bank, u8 val, u8 __iomem *addr);
	void	(*clear_sb) (struct sdp_smc_bank *bank, u8 mask, u8 __iomem *addr);
	void	(*set_sb) (struct sdp_smc_bank *bank, u8 val, u8 __iomem *addr);
	
	void	(*read_stream) (struct sdp_smc_bank *bank, void *dst, void *src, size_t len);
	void	(*write_stream) (struct sdp_smc_bank *bank, void *dst, void *src, size_t len);
};

struct sdp_smc_bank
{
	int		id;		/* bank number 0~3 */
	u32		va_base;
	u32		pa_base;
	u32		size;
	int		state;	
	struct sdp_smc_operations	*ops;
};

struct sdp_smc_bank* sdp_smc_getbank(int id, u32 base, u32 size);
void sdp_smc_putbank(void *ptr);

#endif
