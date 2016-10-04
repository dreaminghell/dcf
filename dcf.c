/*
 *
 * Copyright (c) 2016, Rockchip Technology Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MOCK_DCF
#define MOCK_DCF 0
#endif
#ifndef TEST
#define TEST 0
#endif
#ifndef DEBUG
#define DEBUG 0
#endif

#if TEST
#undef MOCK_DCF
#define MOCK_DCF 1
#endif


#define DCF_PAMET_BASE	(SRAM_BASE_ADDR + 0x1000)
#define DCF_PAMET_SIZE	0x200
#define DCF_SP_BASE		(DCF_PAMET_BASE + DCF_PAMET_SIZE)
#define DCF_SP_SIZE		0x200

#define SRAM_DATA_BASE DCF_SP_BASE
#define SRAM_DATA_SIZE DCF_SP_SIZE

#include "hw_memmap.h"
#include "dcf.h"
#include "ddr_rk3399.h"

void ifcond0_vop_get_st(void)
{
	/* vop power is enable */
	/* if((g_pmu_reg->PMU_PWRDN_ST & (PD_VOP_PWR_STAT))!=0) */
	PCODE("if(g_pmu_reg->PMU_PWRDN_ST & (PD_VOP_PWR_STAT) == 0)\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_PWRDN_ST/4), PD_VOP_PWR_STAT);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void ifcond1_1_vop_get_st(void)
{
	/* vop_big DCLK is enable */
	/*if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP0_SRC_EN |
			ACLK_VOP0_PRE_SRC_EN | HCLK_VOP0_PRE_EN)) == 0)*/
	PCODE("if((g_cru_reg->CRU_CLKGATE10_CON & DCLK_VOP0_SRC_EN) == 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_cru_reg, CRU_CLKGATE10_CON/4),
			DCLK_VOP0_SRC_EN | ACLK_VOP0_PRE_SRC_EN | HCLK_VOP0_PRE_EN);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void ifcond1_2_vop_get_st(void)
{
	/* vop_big DCLK is enable */
	/* if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP0_EN |
			ACLK_VOP0_EN)) == 0)*/
	PCODE("if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP0_EN |\
				ACLK_VOP0_EN)) == 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_cru_reg, CRU_CLKGATE28_CON/4),
			HCLK_VOP0_EN | ACLK_VOP0_EN);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void ifcond1_3_vop_get_st(void)
{
	/* vop_big unstandby */
	/* if ((g_vop_big_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0) */
	PCODE("if((g_vop_big_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL/4), VOP_STANDBY_EN);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void ifcall1_4_vop_get_st(void)
{
	/* vop_big_enable = 1 */
	PCODE("vop_big_enable = 1;\n");
	set_val_sync(DATA_ADDR(vop_big_enable), 1);
}

void ifcall1_3_vop_get_st(void)
{
	/* if ((g_vop_big_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0) */
	if_flag(ifcond1_3_vop_get_st, ifcall1_4_vop_get_st);
}


void ifcall1_2_vop_get_st(void)
{
	/* if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP0_EN |
			ACLK_VOP0_EN)) == 0)*/
	if_flag(ifcond1_2_vop_get_st, ifcall1_3_vop_get_st);
}

void ifcall1_1_vop_get_st(void)
{
	/*if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP0_SRC_EN |
			ACLK_VOP0_PRE_SRC_EN | HCLK_VOP0_PRE_EN)) == 0)*/
	if_flag(ifcond1_1_vop_get_st, ifcall1_2_vop_get_st);
}

void ifcond2_1_vop_get_st(void)
{
	/* vop_lit DCLK is enable */
	/* if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP1_SRC_EN |
			ACLK_VOP1_PRE_SRC_EN | HCLK_VOP1_PRE_EN)) == 0)*/
	PCODE("if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP1_SRC_EN |\
		ACLK_VOP1_PRE_SRC_EN | HCLK_VOP1_PRE_EN)) == 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_cru_reg, CRU_CLKGATE10_CON/4),
			DCLK_VOP1_SRC_EN | ACLK_VOP1_PRE_SRC_EN | HCLK_VOP1_PRE_EN);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}


void ifcond2_2_vop_get_st(void)
{
	/* vop_lit DCLK is enable */
	/* if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP1_EN |
				ACLK_VOP1_EN)) == 0)*/
	PCODE("if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP1_EN |\
			ACLK_VOP1_EN)) == 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_cru_reg, CRU_CLKGATE28_CON/4),
			HCLK_VOP1_EN | ACLK_VOP1_EN);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void ifcond2_3_vop_get_st(void)
{
	/* vop_lit unstandby */
	/*if((g_vop_lit_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0);*/
	PCODE("if((g_vop_lit_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL/4), VOP_STANDBY_EN);
	ifcond_eq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void ifcall2_4_vop_get_st(void)
{
	/* vop_big_enable = 1 */
	PCODE("vop_lit_enable = 1;\n");
	set_val_sync(DATA_ADDR(vop_lit_enable), 1);
}

void ifcall2_3_vop_get_st(void)
{
	/*if((g_vop_lit_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0);*/
	if_flag(ifcond2_3_vop_get_st, ifcall2_4_vop_get_st);
}


void ifcall2_2_vop_get_st(void)
{
	/* if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP1_EN |
				ACLK_VOP1_EN)) == 0)*/
	if_flag(ifcond2_2_vop_get_st, ifcall2_3_vop_get_st);
}

void ifcall2_1_vop_get_st(void)
{
	/* if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP1_SRC_EN |
			ACLK_VOP1_PRE_SRC_EN | HCLK_VOP1_PRE_EN)) == 0)*/
	if_flag(ifcond2_1_vop_get_st, ifcall2_2_vop_get_st);
}

/* get vop_bit and vop_lit status*/
void vop_get_status(void)
{
	/*
	if((g_pmu_reg->PMU_PWRDN_ST & (PD_VOP_PWR_STAT))!=0)
		if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP0_SRC_EN |
			ACLK_VOP0_PRE_SRC_EN | HCLK_VOP0_PRE_EN)) == 0)
			if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP0_EN |
				ACLK_VOP0_EN)) == 0)
				if ((g_vop_big_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0)
					vop_big_enable = 1;
	if((g_pmu_reg->PMU_PWRDN_ST & (PD_VOP_PWR_STAT))!=0)
		if((g_cru_reg->CRU_CLKGATE10_CON & (DCLK_VOP1_SRC_EN |
			ACLK_VOP1_PRE_SRC_EN | HCLK_VOP1_PRE_EN)) == 0)
			if((g_cru_reg->CRU_CLKGATE28_CON & (HCLK_VOP1_EN |
				ACLK_VOP1_EN)) == 0)
				if ((g_vop_lit_reg->VOP_SYS_CTRL & VOP_STANDBY_EN) == 0)
					vop_lit_enable = 1;
	*/

	PCODE("vop_big_enable = 0;\n");
	set_val_sync(DATA_ADDR(vop_big_enable), 0);
	PCODE("vop_lit_enable = 0;\n");
	set_val_sync(DATA_ADDR(vop_lit_enable), 0);

	if_flag(ifcond0_vop_get_st, ifcall1_1_vop_get_st);
	if_flag(ifcond0_vop_get_st, ifcall2_1_vop_get_st);
}


void ifcond_vop_big_en(void)
{
	PCODE("if(vop_big_enable != 0)\n");
	ifcond_neq_val(DATA_ADDR(vop_big_enable), 0);
}

void wait_vop_big_finish(void)
{
	/* clear dma finish irq flag */
	PCODE("g_vop_big_reg->VOP_INTR_CLEAR0 = INT_CLR_DMA_FINISH | (INT_CLR_DMA_FINISH << 16);;\n");
	set_val_sync(REG_ADDR(g_vop_big_reg, VOP_INTR_CLEAR0/4),
				 INT_CLR_DMA_FINISH | (INT_CLR_DMA_FINISH << 16));
	/* waitting for dma finish */
	PCODE("while(g_vop_bit_reg->VOP_INTR_RAW_STATUS0 & INT_RAW_STATUS_DMA_FINISH == 0);\n");
	while_zero(REG_ADDR(g_vop_big_reg, VOP_INTR_RAW_STATUS0/4), INT_RAW_STATUS_DMA_FINISH);
}

void ifcond_vop_lit_en(void)
{
	PCODE("if(vop_lit_enable != 0)\n");
	ifcond_neq_val(DATA_ADDR(vop_lit_enable), 0);
}

void wait_vop_lit_finish(void)
{
	PCODE("g_vop_lit_reg->VOP_INTR_CLEAR0 = INT_CLR_DMA_FINISH | (INT_CLR_DMA_FINISH << 16);;\n");
	set_val_sync(REG_ADDR(g_vop_lit_reg, VOP_INTR_CLEAR0/4),
				 INT_CLR_DMA_FINISH | (INT_CLR_DMA_FINISH << 16));
	PCODE("while(g_vop_lit_reg->VOP_INTR_RAW_STATUS0 & INT_RAW_STATUS_DMA_FINISH == 0);\n");
	while_zero(REG_ADDR(g_vop_lit_reg, VOP_INTR_RAW_STATUS0/4), INT_RAW_STATUS_DMA_FINISH);
}

void wait_vop_lit_finish_when_en(void)
{
	if_flag(ifcond_vop_lit_en, wait_vop_lit_finish);
}

void wait_vop_dma_finish(void)
{
	/*
	if(vop_big_enable) {
		g_vop_big_reg->VOP_INTR_CLEAR0 = INT_CLR_DMA_FINISH | (INT_CLR_DMA_FINISH << 16);
		dsb;
		while(g_vop_bit_reg->VOP_INTR_RAW_STATUS0 & INT_RAW_STATUS_DMA_FINISH == 0);
	} else if(vop_lit_enable) {
	g_vop_lit_reg->VOP_INTR_CLEAR0 = INT_CLR_DMA_FINISH | (INT_CLR_DMA_FINISH << 16);
	dsb;
	while(g_vop_lit_reg->VOP_INTR_RAW_STATUS0 & INT_RAW_STATUS_DMA_FINISH == 0);
	}
	*/
	if_else_flag(ifcond_vop_big_en, wait_vop_big_finish,
				 wait_vop_lit_finish_when_en);
}

/*********************************
 * get lcdc type from vop register
*********************************/
/*because if_flag's parameter cond ,if_func can't pass parameter*/
unsigned int _vop_type, _disp_type;
void ifcond_lcdc_type_from_vop(void)
{
	if (_disp_type == SCREEN_MIPI) {
		if (_vop_type == 0) {
			PCODE("if(g_vop_big_reg->VOP_SYS_CTRL & MIPI_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL/4), MIPI_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		} else {
			PCODE("if(g_vop_lit_reg->VOP_SYS_CTRL & MIPI_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL/4), MIPI_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		}
	} else  if (_disp_type == SCREEN_HDMI) {
		if (_vop_type == 0) {
			PCODE("if(g_vop_big_reg->VOP_SYS_CTRL & HDMI_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL/4), HDMI_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		} else {
			PCODE("if(g_vop_lit_reg->VOP_SYS_CTRL & HDMI_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL/4), HDMI_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		}
	} else if (_disp_type == SCREEN_RGB) {
		if (_vop_type == 0) {
			PCODE("if(g_vop_big_reg->VOP_SYS_CTRL & RGB_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL/4), RGB_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		} else {
			PCODE("if(g_vop_lit_reg->VOP_SYS_CTRL & RGB_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL/4), RGB_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		}
	} else if (_disp_type == SCREEN_EDP) {
		if (_vop_type == 0) {
			PCODE("if(g_vop_big_reg->VOP_SYS_CTRL & EDP_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL/4), EDP_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		} else {
			PCODE("if(g_vop_lit_reg->VOP_SYS_CTRL & EDP_OUT_EN != 0)\n");
			lock_reg(REG_R0);
			and_val(REG_R0, REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL/4), EDP_OUT_EN);
			ifcond_neq_val(REG_R0, 0);
			unlock_reg(REG_R0);
		}
	}
}
void set_lcdc_type(void)
{
	if(_disp_type == SCREEN_MIPI) {
		PCODE("vop_need_standby = 1;\n");
		set_val_sync(DATA_ADDR(vop_need_standby), 1);
		PCODE("dclk_need_div = 0;\n");
		set_val_sync(DATA_ADDR(dclk_need_div), 0);
	} else if (_disp_type == SCREEN_HDMI) {
		PCODE("vop_need_standby = 0;\n");
		set_val_sync(DATA_ADDR(vop_need_standby), 0);
		PCODE("dclk_need_div = 0;\n");
		set_val_sync(DATA_ADDR(dclk_need_div), 0);
	} else if (_disp_type == SCREEN_RGB) {
		PCODE("vop_need_standby = 1;\n");
		set_val_sync(DATA_ADDR(vop_need_standby), 1);
		PCODE("dclk_need_div = 0;\n");
		set_val_sync(DATA_ADDR(dclk_need_div), 0);
	} else if (_disp_type == SCREEN_EDP) {
		PCODE("vop_need_standby = 0;\n");
		set_val_sync(DATA_ADDR(vop_need_standby), 0);
		PCODE("dclk_need_div = 1;\n");
		set_val_sync(DATA_ADDR(dclk_need_div), 1);
	}
}
void detect_lcdc_rgb(void)
{
	int disp_type = _disp_type;
	_disp_type = SCREEN_RGB;
	if_flag(ifcond_lcdc_type_from_vop, set_lcdc_type);
	_disp_type = disp_type;
}

void detect_lcdc_edp(void)
{
	int disp_type = _disp_type;
	_disp_type = SCREEN_EDP;
	if_else_flag(ifcond_lcdc_type_from_vop, set_lcdc_type, detect_lcdc_rgb);
	_disp_type = disp_type;
}

void detect_lcdc_mipi(void)
{
	int disp_type = _disp_type;
	_disp_type = SCREEN_MIPI;
	if_else_flag(ifcond_lcdc_type_from_vop, set_lcdc_type, detect_lcdc_edp);
	_disp_type = disp_type;
}
void get_vop_lcdc_type(void)
{
	/*_disp_type = SCREEN_RGB;
	if_flag(ifcond_lcdc_type_from_vop, set_lcdc_type);

	_disp_type = SCREEN_EDP;
	if_flag(ifcond_lcdc_type_from_vop, set_lcdc_type);
	_disp_type = SCREEN_HDMI;
	if_flag(ifcond_lcdc_type_from_vop, set_lcdc_type);
	_disp_type = SCREEN_MIPI;
	if_flag(ifcond_lcdc_type_from_vop, set_lcdc_type);
	*/
	int disp_type = _disp_type;
	_disp_type = SCREEN_HDMI;
	if_else_flag(ifcond_lcdc_type_from_vop, set_lcdc_type, detect_lcdc_mipi);
	//if_else_flag(ifcond_lcdc_type_from_vop, detect_lcdc_mipi, set_lcdc_type);
	_disp_type = disp_type;
}

void get_vop_big_lcdc_type(void)
{
	int vop_type = _vop_type;
	_vop_type = 0;
	get_vop_lcdc_type();
	_vop_type = vop_type;
}

void get_vop_lit_lcdc_type(void)
{
	int vop_type = _vop_type;
	_vop_type = 1;
	if_flag(ifcond_vop_lit_en, get_vop_lcdc_type);
	_vop_type = vop_type;
}

void vop_get_lcdc_type(void)
{
	PCODE("vop_need_standby = 0;\n");
	set_val_sync(DATA_ADDR(vop_need_standby), 0);
	PCODE("dclk_need_div = 0;\n");
	set_val_sync(DATA_ADDR(dclk_need_div), 0);
	if_else_flag(ifcond_vop_big_en, get_vop_big_lcdc_type,
				 get_vop_lit_lcdc_type);
}

void cond_deidle_cpu_stat(void)
{
	PCODE("if(g_pmu_reg->PMU_BUS_IDLE_ST & IDLE_CCIM0 != 0);\n");
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_ST/4), IDLE_CCIM0);
	neq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}
void idle_cpu(unsigned int idle_en)
{
	if (idle_en) {
		/* enable correlation clk for idle for idle port */
		PCODE("IN enable idle port\n");
		PCODE("g_pmu_reg->PMU_NOC_AUTO_ENA |= CCIM0_GATING_DISB;\n");
		lock_reg(REG_R0);
		or_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), CCIM0_GATING_DISB);
		set_sync(REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), REG_R0);
		/* idle req */
		PCODE("g_pmu_reg->PMU_BUS_IDLE_REQ |= IDLE_REQ_CCIM0;\n");
		or_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4), IDLE_REQ_CCIM0);
		set_sync(REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4), REG_R0);
		unlock_reg(REG_R0);
		/* waitting for idle */
		PCODE("while(g_pmu_reg->PMU_BUS_IDLE_ST & IDLE_CCIM0 == 0);\n"); //error
		while_zero(REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_ST/4), IDLE_CCIM0);
		/* disable correlation clk for idle for idle port */
		PCODE("g_pmu_reg->PMU_NOC_AUTO_ENA &= ~CCIM0_GATING_DISB;\n");
		lock_reg(REG_R0);
		and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), ~CCIM0_GATING_DISB);
		set_sync(REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), REG_R0);
		unlock_reg(REG_R0);
	} else {
		/* enable correlation clk for idle for idle port */
		PCODE("IN disable idle port");
		PCODE("g_pmu_reg->PMU_NOC_AUTO_ENA |= CCIM0_GATING_DISB;\n");
		lock_reg(REG_R0);
		or_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), CCIM0_GATING_DISB);
		set_sync(REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), REG_R0);
		/* deidle req */
		PCODE("g_pmu_reg->PMU_BUS_IDLE_REQ &= ~IDLE_REQ_CCIM0;\n");
		and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4), ~IDLE_REQ_CCIM0);
		set_sync(REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4), REG_R0);
		unlock_reg(REG_R0);
		/* waitting for idle */
		PCODE("while(g_pmu_reg->PMU_BUS_IDLE_ST & IDLE_CCIM0 != 0);\n");
		while_flag(cond_deidle_cpu_stat);
		/* disable correlation clk for idle for idle port */
		PCODE("g_pmu_reg->PMU_NOC_AUTO_ENA &= ~CCIM0_GATING_DISB;\n");
		lock_reg(REG_R0);
		and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), ~CCIM0_GATING_DISB);
		set_sync(REG_ADDR(g_pmu_reg, PMU_NOC_AUTO_ENA/4), REG_R0);
		unlock_reg(REG_R0);
	}
}


/******************************************************************************
* contorl dclk div and mipi reset(for recover mipi display when dclk div change)
*******************************************************************************/
void ifcond_vop_need_standby(void)
{
	PCODE("if(vop_need_standby != 0)\n");
	ifcond_neq_val(DATA_ADDR(vop_need_standby), 0);
}
void ifcond_dclk_need_div(void)
{
	PCODE("if(dclk_need_div) != 0\n");
	ifcond_neq_val(DATA_ADDR(dclk_need_div), 0);
}
#if 0
void mipi_power_of(void)
{

}
void mipi_power_on(void)
{

}
void vop_dclk_div(void)
{
	if (_vop_type == 0) {
		lock_reg(REG_R0);
		and_val(REG_R0, REG_ADDR(g_cru_reg, CRU_CLKSEL49_CON/4), 0xFF);
		set_val(REG_ADDR(g_cru_reg, CRU_CLKSEL49_CON/4), 0xff00ff);
		set_val(DATA_ADDR(dclk_div_val), REG_R0);
		unlock_reg(REG_R0);
	} else {
		lock_reg(REG_R0);
		and_val(REG_R0, REG_ADDR(g_cru_reg, CRU_CLKSEL50_CON/4), 0xFF);
		set_val(REG_ADDR(g_cru_reg, CRU_CLKSEL50_CON/4), 0xff00ff);
		set_val(DATA_ADDR(dclk_div_val), REG_R0);
		unlock_reg(REG_R0);
	}
}
void vop_dclk_restore(void)
{
	if (_vop_type == 0) {
		lock_reg(REG_R0);
		set_val(REG_R0, DATA_ADDR(dclk_div_val));
		or_val(REG_ADDR(g_cru_reg, CRU_CLKSEL49_CON/4), REG_R0, 0xff0000);
		unlock_reg(REG_R0);
	} else {
		lock_reg(REG_R0);
		set_val(REG_R0, DATA_ADDR(dclk_div_val));
		or_val(REG_ADDR(g_cru_reg, CRU_CLKSEL50_CON/4), REG_R0, 0xff0000);
		unlock_reg(REG_R0);
	}
}

void vop_big_dclk_mipi_handle_in(void)
{
	_vop_type = 0;
	if_flag(ifcond_vop_need_standby, mipi_power_of);
	if_flag(ifcond_dclk_need_div, vop_dclk_div);
}

void iffunc_vop_lit_dclk_mipi_handle_in(void)
{
	_vop_type = 1;
	if_flag(ifcond_vop_need_standby, mipi_power_of);
	if_flag(ifcond_dclk_need_div, vop_dclk_div);
}

void vop_lit_dclk_mipi_handle_in(void)
{
	if_flag(ifcond_vop_lit_en, iffunc_vop_lit_dclk_mipi_handle_in);
}

void vop_big_dclk_mipi_handle_out(void)
{
	_vop_type = 0;
	if_flag(ifcond_vop_need_standby, mipi_power_on);
	if_flag(ifcond_dclk_need_div, vop_dclk_restore);
}

void iffunc_vop_lit_dclk_mipi_handle_out(void)
{
	_vop_type = 1;
	if_flag(ifcond_dclk_need_div, vop_dclk_restore);
	if_flag(ifcond_vop_need_standby, mipi_power_on);
}

void vop_lit_dclk_mipi_handle_out(void)
{
	if_flag(ifcond_vop_lit_en, iffunc_vop_lit_dclk_mipi_handle_out);
}
void vop_vbank_prolong_in(void)
{
	/*
	if (vop_big_enbale) {
		if(vop_need_standby)
			power_on_mipi();
		if(dclk_need_div)
			div_dclk(vop0);
	} else if(vop_lit_enable) {
		if(vop_need_standby)
			power_on_mipi();
		if(dclk_need_div)
			div_dclk(vop1);
	}*/
	if_else_flag(ifcond_vop_big_en, vop_big_dclk_mipi_handle_in,
				vop_lit_dclk_mipi_handle_in);
}

void vop_vbank_prolong_out(void)
{
	if_else_flag(ifcond_vop_big_en, vop_big_dclk_mipi_handle_out,
				vop_lit_dclk_mipi_handle_out);
}
#endif
void mipi_power_off(void)
{
	set_val_sync(REG_ADDR(g_mipi0_reg, 1), 0);
	set_val_sync(REG_ADDR(g_mipi1_reg, 1), 0);
}
void mipi_power_on(void)
{
	set_val_sync(REG_ADDR(g_mipi0_reg,1), 0);
	set_val_sync(REG_ADDR(g_mipi1_reg,1), 0);
	set_val_sync(REG_ADDR(g_mipi0_reg, 0x34/4), 0);
	set_val_sync(REG_ADDR(g_mipi1_reg, 0x34/4), 0);
	set_val_sync(REG_ADDR(g_mipi0_reg,1), 1);
	set_val_sync(REG_ADDR(g_mipi1_reg,1), 1);
}

void vop_fp_standby(unsigned int en, unsigned int vop)
{
	if (en) {
		if (vop) {
			or_val(REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL1 / 4),
				REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL1 / 4), 1<<31);
		} else {
			or_val(REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL1 / 4),
				REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL1 / 4), 1<<31);
		}
	} else {
		if (vop) {
			and_val(REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL1 / 4),
				REG_ADDR(g_vop_lit_reg, VOP_SYS_CTRL1 / 4), ~(1<<31));
		} else {
			and_val(REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL1 / 4),
				REG_ADDR(g_vop_big_reg, VOP_SYS_CTRL1 / 4), ~(1<<31));
		}
	}
}

void vop_big_standby_en(void)
{
	vop_fp_standby(1, 0);
}

void vop_big_standby_dis(void)
{
	vop_fp_standby(0, 0);
}

void vop_lit_standby_en(void)
{
	vop_fp_standby(1, 1);
}

void vop_lit_standby_dis(void)
{
	vop_fp_standby(0, 1);
}

void vop_dclk_div(void)
{
	PCODE("dclk0_div_val = g_cru_reg->CRU_CLKSEL49_CON & 0xff;\n");
	and_val(DATA_ADDR(dclk0_div_val), REG_ADDR(g_cru_reg, CRU_CLKSEL49_CON/4), 0xFF);
	PCODE("g_cru_reg->CRU_CLKSEL49_CON = 0xff00ff;\n");
	set_val_sync(REG_ADDR(g_cru_reg, CRU_CLKSEL49_CON/4), 0xff00ff);
	PCODE("dclk1_div_val = g_cru_reg->CRU_CLKSEL50_CON & 0xff;\n");
	and_val(DATA_ADDR(dclk1_div_val), REG_ADDR(g_cru_reg, CRU_CLKSEL50_CON/4), 0xFF);
	PCODE("g_cru_reg->CRU_CLKSEL50_CON = 0xff00ff;\n");
	set_val_sync(REG_ADDR(g_cru_reg, CRU_CLKSEL50_CON/4), 0xff00ff);
}
void vop_standby_en(void)
{
	if_flag(ifcond_vop_big_en, vop_big_standby_en);
	if_flag(ifcond_vop_lit_en, vop_lit_standby_en);
}

void vop_dclk_restore(void)
{
	PCODE("g_cru_reg->CRU_CLKSEL49_CON = dclk0_div_val | 0xff0000;\n");
	or_val(REG_ADDR(g_cru_reg, CRU_CLKSEL49_CON/4), DATA_ADDR(dclk0_div_val), 0xff0000);
	PCODE("g_cru_reg->CRU_CLKSEL50_CON = dclk1_div_val | 0xff0000;\n");
	or_val(REG_ADDR(g_cru_reg, CRU_CLKSEL50_CON/4), DATA_ADDR(dclk1_div_val), 0xff0000);
}

void vop_standby_disable(void)
{
	if_flag(ifcond_vop_big_en, vop_big_standby_dis);
	if_flag(ifcond_vop_lit_en, vop_lit_standby_dis);
}

void vop_vbank_prolong_in(void)
{
	if_flag(ifcond_dclk_need_div, vop_dclk_div);
	if_flag(ifcond_vop_need_standby, vop_standby_en);
}

void vop_vbank_prolong_out(void)
{
	if_flag(ifcond_dclk_need_div, vop_dclk_restore);
	if_flag(ifcond_vop_need_standby, vop_standby_disable);
}

void cond_idle_msch(void)
{
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_ST/4), IDLE_MSCH1 | IDLE_MSCH0);
	neq_val(REG_R0, IDLE_MSCH1 | IDLE_MSCH0);
	unlock_reg(REG_R0);
}

void idle_msch(void)
{
	/* enable auto clk ungating */
	PCODE("g_pmu_reg->PMU_NOC_AUTO_ENA |= (MSCH0_GATING_DISB | MSCH1_GATING_DISB);\n");
	lock_reg(REG_R0);
	PCODE("g_pmu_cru_reg->PMU_CRU_GATEDIS_CON0 = 0x3fffffff;\n");
	set_val_sync(REG_ADDR(g_pmu_cru_reg, PMU_CRU_GATEDIS_CON0 / 4), 0x3fffffff);
	PCODE("g_pmu_reg->PMU_BUS_IDLE_REQ |= (IDLE_REQ_MSCH1 | IDLE_REQ_MSCH0);\n");
	or_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4),
			IDLE_REQ_MSCH0 | IDLE_REQ_MSCH1);
	set_sync(REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4), REG_R0);
	unlock_reg(REG_R0);

	PCODE("while(g_pmu_reg->PMU_BUS_IDLE_ST & (IDLE_MSCH1 | IDLE_MSCH1) != (IDLE_MSCH1 | IDLE_MSCH0));\n");
	while_flag(cond_idle_msch);
}

void cond_deidle_msch(void)
{
	lock_reg(REG_R0);
	and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_ST/4), IDLE_MSCH1 | IDLE_MSCH0);
	neq_val(REG_R0, 0);
	unlock_reg(REG_R0);
}

void deidle_msch(void)
{
	lock_reg(REG_R0);
	PCODE("g_pmu_reg->PMU_BUS_IDLE_REQ &= ~(IDLE_REQ_MSCH1 | IDLE_REQ_MSCH0);\n");
	and_val(REG_R0, REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4),
			~(IDLE_REQ_MSCH0 | IDLE_REQ_MSCH1));
	set_sync(REG_ADDR(g_pmu_reg, PMU_BUS_IDLE_REQ/4), REG_R0);
	unlock_reg(REG_R0);
	PCODE("while(g_pmu_reg->PMU_BUS_IDLE_ST & (IDLE_MSCH1 | IDLE_MSCH1) != 0);\n");
	while_flag(cond_deidle_msch);
	PCODE("g_pmu_cru_reg->PMU_CRU_GATEDIS_CON0 = 0;\n");
	set_val_sync(REG_ADDR(g_pmu_cru_reg, PMU_CRU_GATEDIS_CON0 / 4), 0x0);

}

void ddr_set_pll(void)
{
	set_val_sync(REG_ADDR(g_cru_reg, CRU_DPLL_CON3 /4), PLL_MODE(PLL_SLOW_MODE));
	idle(24);
	set_val_sync(REG_ADDR(g_cru_reg, CRU_DPLL_CON3 /4), PLL_POWER_DOWN(1));
	set_sync(REG_ADDR(g_cru_reg, CRU_DPLL_CON0 / 4), REG_ADDR(g_param_reg, PARAM_DPLL_CON0 / 4));
	set_sync(REG_ADDR(g_cru_reg, CRU_DPLL_CON1 / 4), REG_ADDR(g_param_reg, PARAM_DPLL_CON1 / 4));
	set_val_sync(REG_ADDR(g_cru_reg, CRU_DPLL_CON3 /4), PLL_POWER_DOWN(0));
	idle(24);
	while_zero(REG_ADDR(g_cru_reg, CRU_DPLL_CON2 / 4), 1u << 31);
	set_val_sync(REG_ADDR(g_cru_reg, CRU_DPLL_CON3 /4), PLL_MODE(PLL_NORMAL_MODE));
}

void main() {
	unsigned int i;
	setup();
	/* reg declare and variable define */
	DECLARE_REG(g_ddrc0_reg, DDRC0_BASE_ADDR, DDRC0_SIZE);
	DECLARE_REG(g_ddrc1_reg, DDRC1_BASE_ADDR, DDRC1_SIZE);
	DECLARE_REG(g_vop_big_reg, VOP_BIG_BASE_ADDR, VOP_BIG_SIZE);
	DECLARE_REG(g_vop_lit_reg, VOP_LITE_BASE_ADDR, VOP_LITE_SIZE);
	DECLARE_REG(g_cru_reg, CRU_BASE_ADDR, CRU_SIZE);
	DECLARE_REG(g_pmu_cru_reg, PMU_CRU_BASE_ADDR, PMU_CRU_SIZE);
	DECLARE_REG(g_grf_reg, GRF_BASE_ADDR, GRF_SIZE);
	DECLARE_REG(g_pmu_reg, PMU_BASE_ADDR, PMU_SIZE);
	DECLARE_REG(g_pmu_grf_reg, PMU_GRF_BASE_ADDR, PMU_GRF_SIZE);
	DECLARE_REG(g_sgrf_reg, SGRF_BASE_ADDR, SGRF_SIZE);
	DECLARE_REG(g_server_msch0_reg, SERVER_MSCH0_BASE_ADDR, SERVER_MSCH0_SIZE);
	DECLARE_REG(g_server_msch1_reg, SERVER_MSCH1_BASE_ADDR, SERVER_MSCH1_SIZE);
	DECLARE_REG(g_mipi0_reg, MIPI_DIS_HOST0_BASE_ADDR, MIPI_DIS_HOST0_SIZE);
	DECLARE_REG(g_mipi1_reg, MIPI_DIS_HOST1_BASE_ADDR, MIPI_DIS_HOST1_SIZE);
	DECLARE_REG(g_sram_reg, SRAM_BASE_ADDR, SRAM_SIZE);
	DECLARE_REG(g_param_reg, DCF_PAMET_BASE, DCF_PAMET_SIZE);
	DECLARE_REG(g_cic_reg, CIC_BASE_ADDR, CIC_SIZE);

	/* record vop0 enable status */
	PCODE("volatile u32 vop_big_enable;\n");
	DECLARE_U32(vop_big_enable);
	/* record vop1 enable status */
	PCODE("volatile u32 vop_lit_enable;\n");
	DECLARE_U32(vop_lit_enable);
	/* record vop0 dclk default div value */
	PCODE("volatile u32 dclk0_div_val;\n");
	DECLARE_U32(dclk0_div_val);
	/* record vop1 dclk default div value */
	PCODE("volatile u32 dclk1_div_val;\n");
	DECLARE_U32(dclk1_div_val);
	/* record if dclk need reduce freq (except hdmi) */
	PCODE("volatile u32 dclk_need_div;\n");
	DECLARE_U32(dclk_need_div);
	/* record if mipi need reset when dclk freq
	   change(if lcdc type is mipi)*/
	PCODE("volatile u32 vop_need_standby;\n");
	DECLARE_U32(vop_need_standby);

	/* get vop enable status */
	PCODE("get vop enable status\n");
	vop_get_status();
	/* get lcdc type for dclk div and mipi reset behavior */
	PCODE("get lcdc type for dclk div and mipi reset beahvior\n");
	vop_get_lcdc_type();
	/* waitting for vop vbank */
	PCODE("waitting for vop vbank\n");
	wait_vop_dma_finish();

	/* cci idle req stall */
	PCODE("cci idle req stall\n");
	set_val_sync(REG_ADDR(g_grf_reg, GRF_SOC_CON0 / 4), 0xffffffff);
	set_val_sync(REG_ADDR(g_grf_reg, GRF_SOC_CON1 / 4), 0xffffffff);
	set_val_sync(REG_ADDR(g_grf_reg, GRF_SOC_CON2 / 4), 0xffffffff);
	set_val_sync(REG_ADDR(g_grf_reg, GRF_SOC_CON3 / 4), 0xffffffff);
	set_val_sync(REG_ADDR(g_grf_reg, GRF_SOC_CON4 / 4), 0x70007000);

	/* IDLE MSCH */
	PCODE("IDLE_MSCH\n");
	idle_msch();
	vop_vbank_prolong_in();

	lock_reg(REG_R0);
	or_val(REG_R0, REG_ADDR(g_param_reg, PARAM_FREQ_SELECT / 4),
		(((0x3 << 4) | (1 << 2) | 1) << 16) | (1 << 2) | 1);
	set_sync(REG_ADDR(g_cic_reg, CIC_CTRL0 / 4), REG_R0);
	unlock_reg(REG_R0);
	while_zero(REG_ADDR(g_cic_reg, CIC_STATUS0 / 4), 1 << 2);

	ddr_set_pll();

	set_val_sync(REG_ADDR(g_cic_reg,CIC_CTRL0 / 4 ), 0x20002);

	while_zero(REG_ADDR(g_cic_reg, CIC_STATUS0 / 4), 1 << 0);

	/* restore DCLK div and mipi power on */
	PCODE("restore DCLK div and mipi power on\n");
	vop_vbank_prolong_out();
	/* DEIDLE MSCH */
	PCODE("DEIDLE_MSCH\n");
	deidle_msch();

	end();
}
