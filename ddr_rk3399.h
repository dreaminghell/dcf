#ifndef _DDR_RK3399_H_
#define _DDR_RK3399_H_

/* LCDC type */
#define SCREEN_NULL        0
#define SCREEN_RGB         1
#define SCREEN_LVDS        2
#define SCREEN_DUAL_LVDS   3
#define SCREEN_MCU         4
#define SCREEN_TVOUT       5
#define SCREEN_HDMI        6
#define SCREEN_MIPI        7
#define SCREEN_DUAL_MIPI   8
#define SCREEN_EDP         9
#define SCREEN_TVOUT_TEST  10
#define SCREEN_LVDS_10BIT        11
#define SCREEN_DUAL_LVDS_10BIT   12


/* PMU */
#define PMU_PWRDN_ST		0x18
#define PMU_BUS_IDLE_REQ	0x60
#define PMU_BUS_IDLE_ST		0x64
#define PMU_NOC_AUTO_ENA	0xD8

/* PMU_BUS_IDLE_REQ */
#define IDLE_REQ_SDIOAUDIO	(1<<29)
#define IDLE_REQ_SD			(1<<28)
#define IDLE_REQ_GIC		(1<<27)
#define IDLE_REQ_PMUM0		(1<<26)
#define IDLE_REQ_CENTER1	(1<<25)
#define IDLE_REQ_EMMC		(1<<24)
#define IDLE_REQ_GMAC		(1<<23)
#define IDLE_REQ_EDP		(1<<22)
#define IDLE_REQ_PMU		(1<<21)
#define IDLE_REQ_ALIVE		(1<<20)
#define IDLE_REQ_MSCH1		(1<<19)
#define IDLE_REQ_MSCH0		(1<<18)
#define IDLE_REQ_VIO		(1<<17)
#define IDLE_REQ_CCIM1		(1<<16)
#define IDLE_REQ_CCIM0		(1<<15)
#define IDLE_REQ_CENTER		(1<<14)
#define IDLE_REQ_PERILPM0	(1<<13)
#define IDLE_REQ_USB3		(1<<12)
#define IDLE_REQ_HDCP		(1<<11)
#define IDLE_REQ_ISP1		(1<<10)
#define IDLE_REQ_ISP0		(1<<9)
#define IDLE_REQ_VOPL		(1<<8)
#define IDLE_REQ_VOPB		(1<<7)
#define IDLE_REQ_IEP		(1<<6)
#define IDLE_REQ_RGA		(1<<5)
#define IDLE_REQ_VDU		(1<<4)
#define IDLE_REQ_VCODE		(1<<3)
#define IDLE_REQ_PERIHP		(1<<2)
#define IDLE_REQ_PERIP		(1<<1)
#define IDLE_REQ_GPU		(1<<0)

/* #define PMU_BUS_IDLE_ST */
#define IDLE_SDIOAUDIO	(1<<29)
#define IDLE_SD			(1<<28)
#define IDLE_GIC		(1<<27)
#define IDLE_PMUM0		(1<<26)
#define IDLE_CENTER1	(1<<25)
#define IDLE_EMMC		(1<<24)
#define IDLE_GMAC		(1<<23)
#define IDLE_EDP		(1<<22)
#define IDLE_PMU		(1<<21)
#define IDLE_ALIVE		(1<<20)
#define IDLE_MSCH1		(1<<19)
#define IDLE_MSCH0		(1<<18)
#define IDLE_VIO		(1<<17)
#define IDLE_CCIM1		(1<<16)
#define IDLE_CCIM0		(1<<15)
#define IDLE_CENTER		(1<<14)
#define IDLE_PERILPM0	(1<<13)
#define IDLE_USB3		(1<<12)
#define IDLE_HDCP		(1<<11)
#define IDLE_ISP1		(1<<10)
#define IDLE_ISP0		(1<<9)
#define IDLE_VOPL		(1<<8)
#define IDLE_VOPB		(1<<7)
#define IDLE_IEP		(1<<6)
#define IDLE_RGA		(1<<5)
#define IDLE_VDU		(1<<4)
#define IDLE_VCODE		(1<<3)
#define IDLE_PERIHP		(1<<2)
#define IDLE_PERIP		(1<<1)
#define IDLE_GPU		(1<<0)

/* PMU_NOC_AUTO_ENA */
#define SDIOAUDIO_GATING_DISB	(1<<29)
#define SD_GATING_DISB	(1<<28)
#define GIC_GATING_DISB	(1<<27)
#define GPU_GATING_DISB	(1<<26)
#define PERILP_GATING_DISB	(1<<25)
#define PERIHP_GATING_DISB	(1<<24)
#define VCODEC_GATING_DISB	(1<<23)
#define VDU_GATING_DISB	(1<<22)
#define RGA_GATING_DISB	(1<<21)
#define IEP_GATING_DISB	(1<<20)
#define VOPB_GATING_DISB	(1<<19)
#define VOPL_GATING_DISB	(1<<18)
#define ISP0_GATING_DISB	(1<<17)
#define ISP1_GATING_DISB	(1<<16)
#define HDCP_GATING_DISB	(1<<15)
#define USB3_GATING_DISB	(1<<14)
#define PERILPM0_GATING_DISB	(1<<13)
#define CENTER_GATING_DISB	(1<<12)
#define CCIM0_GATING_DISB	(1<<11)
#define CCIM1_GATING_DISB	(1<<10)
#define VIO_GATING_DISB	(1<<9)
#define MSCH0_GATING_DISB	(1<<8)
#define MSCH1_GATING_DISB	(1<<7)
#define ALIVE_GATING_DISB	(1<<6)
#define PMU_GATING_DISB	(1<<5)
#define EDP_GATING_DISB	(1<<4)
#define GMAC_GATING_DISB	(1<<3)
#define EMMC_GATING_DISB	(1<<2)
#define CENTER1_GATING_DISB	(1<<1)
#define PMUM0_GATING_DISB	(1<<0)

#define PD_VOP_PWR_STAT		(1<< 20)


/* CRU */
#define CRU_DPLL_CON0		0x40
#define CRU_DPLL_CON1		0x44
#define CRU_DPLL_CON2		0x48
#define CRU_DPLL_CON3		0x4c
#define CRU_DPLL_CON4		0x50
#define CRU_DPLL_CON5		0x54

#define CRU_DPLL_CON2		0x48
#define CRU_DPLL_CON3		0x4c
#define CRU_CLKGATE10_CON	0x328
#define CRU_CLKGATE28_CON	0x370

#define CRU_CLKSEL49_CON	0x1c4
#define CRU_CLKSEL50_CON	0x1c8

/* CRU_CLKGATE10_CON */
#define ACLK_VOP0_PRE_SRC_EN	(1 << 8)
#define HCLK_VOP0_PRE_EN		(1 << 9)
#define ACLK_VOP1_PRE_SRC_EN	(1 << 10)
#define HCLK_VOP1_PRE_EN		(1 << 11)
#define DCLK_VOP0_SRC_EN		(1 << 12)
#define DCLK_VOP1_SRC_EN		(1 << 13)

/* CRU_CLKGATE28_CON */
#define HCLK_VOP0_EN		(1 << 2)
#define ACLK_VOP0_EN		(1 << 3)
#define HCLK_VOP1_EN		(1 << 6)
#define ACLK_VOP1_EN		(1 << 7)

typedef enum PLL_ID_Tag
{
		APLL_L=0,
		APLL_B,
		DPLL,
		CPLL,
		GPLL,
		NPLL,
		VPLL,
		PLL_MAX
}PLL_ID;

/* CRU_PLL_CON0 */
#define FBDIV(n)      ((0xFFF << 16) | (n))

/* CRU_PLL_CON1 */
#define POSTDIV2(n)   ((0x7 << (12 + 16)) | ((n) << 12))
#define POSTDIV1(n)   ((0x7 << (8 + 16)) | ((n) << 8))
#define REFDIV(n)     ((0x3F << 16) | (n))

/* CRU_PLL_CON2 */
#define PLL_LOCK(n)       (((n) >> 31) & 0x1)

/* CRU_PLL_CON3 */
#define PLL_SLOW_MODE		(0)
#define PLL_NORMAL_MODE		(1)
#define PLL_DP_SLOW_MODE	(2)
#define PLL_MODE(n)			((0x3 << (8 + 16)) | ((n) << 8))
#define FOUT4PHASEPD(n)		((0x1 << (6 + 16)) | ((n) << 6))
#define FOUTVCOPD(n)		((0x1 << (5 + 16)) | ((n) << 5))
#define FOUTPOSTDIVPD(n)	((0x1 << (4 + 16)) | ((n) << 4))
#define DSMPD(n)			((0x1 << (3 + 16)) | ((n) << 3))
#define DACPD(n)			((0x1 << (2 + 16)) | ((n) << 2))
#define PLL_BYPASS(n)		((0x1 << (1 + 16)) | ((n) << 1))
#define PLL_POWER_DOWN(n)	((0x1 << (0 + 16)) | ((n) << 0))

/* PMU CRU */
#define PMU_CRU_GATEDIS_CON0	0x130
#define PMU_CRU_GATEDIS_CON1	0x134

/* VOP */
#define VOP_SYS_CTRL			0x8
#define VOP_SYS_CTRL1			0xc
#define	VOP_INTR_CLEAR0			0x284
#define VOP_INTR_RAW_STATUS0	0x28c

/* VOP_SYS_CTRL */
#define VOP_STANDBY_EN			(1<<22)
#define MIPI_OUT_EN				(1<<15)
#define EDP_OUT_EN				(1<<14)
#define HDMI_OUT_EN				(1<<13)
#define RGB_OUT_EN				(1<<12)
/* VOP_SYS_CTRL1 */
#define DSP_FP_STANDBY			(1<<31)
/* VOP_INTR_CLEAR0 */
#define	INT_CLR_DMA_FINISH		(1<<15)
#define INT_CLR_LINE_FLAG1		(1<<4)
#define INT_CLR_LINE_FLAG0		(1<<3)

/* VOP_INTR_RAW_STATUS0 */
#define	INT_RAW_STATUS_DMA_FINISH	(1<<15)
#define INT_RAW_STATUS_LINE_FLAG1	(1<<4)
#define INT_RAW_STATUS_LINE_FLAG0	(1<<3)

/* GRF */
#define GRF_SOC_CON0	(0xe200)
#define GRF_SOC_CON1	(0xe204)
#define GRF_SOC_CON2	(0xe208)
#define GRF_SOC_CON3	(0xe20c)
#define GRF_SOC_CON4	(0xe210)

/* CIC */
#define CIC_CTRL0		(0)
#define CIC_CTRL1		(0x4)
#define CIC_IDLE_TH		(0x8)
#define CIC_CG_WAIT_TH	(0xC)
#define CIC_STATUS0		(0x10)
#define CIC_STATUS1		(0x14)
#define CIC_CTRL2		(0x18)
#define CIC_CTRL3		(0x1C)
#define CIC_CTRL4		(0x20)


/* DCF_PAMET */
#define PARAM_DRAM_FREQ		(0)
#define PARAM_DPLL_CON0		(4)
#define PARAM_DPLL_CON1		(8)
#define PARAM_DPLL_CON2		(0xc)
#define PARAM_DPLL_CON3		(0x10)
#define PARAM_DPLL_CON4		(0x14)
#define PARAM_DPLL_CON5		(0x18)
#define PARAM_FREQ_SELECT	(0x1c)	//equal to fn<<4


#endif /* _DDR_RK3399_H_ */
