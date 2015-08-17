/*
 *  Copyright (C) 2015 Silver Bullet Technology LLC
 *  Derived from Altera Socfpga
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/irqchip.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/clk-provider.h>
#include <linux/clocksource.h>
#include <linux/reboot.h>
#include <linux/clk-provider.h>
#include <linux/clocksource.h>

#include <asm/hardware/cache-l2x0.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/smp_scu.h>
#include <mach/s3ma-regs.h>

#include "core.h"

void __iomem *s3ma_scu_base_addr = ((void __iomem *)(S3MA_SCU_VIRT_BASE));
void __iomem *s3ma_mru_base_addr = ((void __iomem *)(S3MA_MRU_VIRT_BASE));
void __iomem *s3ma_gic_dist_base = ((void __iomem *)(S3MA_GIC_DIST_BASE));

static struct map_desc v2p_io_desc[] __initdata = {
	{
		.virtual	= S3MA_SCU_VIRT_BASE,
		.pfn		= 0, /* run-time */
		.length		= SZ_256,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= S3MA_MRU_VIRT_BASE,
		.pfn		= __phys_to_pfn(MRU_APB_ABSOLUTE_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
#if 1
	{
		.virtual	= S3MA_UART0_VIRT_BASE,
		.pfn		= __phys_to_pfn(UART0_APB_ABSOLUTE_BASE),
		.length		= SZ_8K,
		.type		= MT_DEVICE,
	},
#endif
	{
		.virtual	= S3MA_GIC_DIST_BASE,
		.pfn		= __phys_to_pfn(IDIST_ABSOLUTE_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
};


static void __init s3ma_scu_map_io(void)
{
	unsigned long base;

	/* Get SCU base */
	base = scu_a9_get_base();
	BUG_ON(!base);
	v2p_io_desc[0].pfn = __phys_to_pfn(base);
}

static void __init s3ma_map_io(void)
{
	s3ma_scu_map_io();
	iotable_init(v2p_io_desc, ARRAY_SIZE(v2p_io_desc));
	debug_ll_io_init();
	early_printk("Early printk initialized\n");
}

static void __init s3ma_init_irq(void)
{
	//gic_arch_extn.flags = IRQCHIP_SKIP_SET_WAKE | IRQCHIP_MASK_ON_SUSPEND;
	irqchip_init();
}

// May use to override default kernel time_init
void __init s3ma_time_init(void)
{
	of_clk_init(NULL);
	clocksource_of_init();
}

static void s3ma_restart(enum reboot_mode mode, const char *cmd)
{
	u32 temp = 0x5B31F086; /* magic reset word */

	writel(temp, s3ma_mru_base_addr + RF_CONTROL_RESET_OFFSET);
}

static const char *s3ma_dt_match[] = {
	"exelis,s3ma-sp3dev",
	"exelis,s3ma-sp3dts",
	NULL
};

DT_MACHINE_START(S3MA, "Exelis S3MA SoC")
//	.l2c_aux_val	= 0,
//	.l2c_aux_mask	= ~0,
	.smp		= smp_ops(s3ma_smp_ops),
	.map_io		= s3ma_map_io,
	.init_irq	= s3ma_init_irq,
	.restart	= s3ma_restart,
	.dt_compat	= s3ma_dt_match,
MACHINE_END
