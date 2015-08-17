/*
 * Copyright 2015 Silver Bullet Technology.
 * Copyright 2010-2011 Calxeda, Inc.
 * Copyright 2012 Pavel Machek <pavel@denx.de>
 * Based on platsmp.c, Copyright (C) 2002 ARM Ltd.
 * Copyright (C) 2015 Silver Bullet Technology
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/irqchip/arm-gic.h>

#include <asm/cacheflush.h>
#include <asm/smp_scu.h>
#include <asm/smp_plat.h>

#include "core.h"

/*
 * Store number of cores in the system
 * Because of scu_get_core_count() must be in __init section and can't
 * be called from zynq_cpun_start() because it is not in __init section.
 */
static int ncores;

int s3ma_cpun_start(u32 address, int cpu)
{
	u32  size = (&s3ma_secondary_trampoline_end  - &s3ma_secondary_trampoline) / 4;
	u32  jump = (&s3ma_secondary_trampoline_jump - &s3ma_secondary_trampoline) / 4;
	u32  buff[size + 1] __attribute__((aligned(__alignof__(u64))));
	u32  word;

	/* MS: Expectation that SLCR are directly map and accessible */
	/* Not possible to jump to non aligned address */
	if ( !(address & 3) ) {
		/* Store pointer to ioremap area which points to address 0x0 */
		static void  __iomem *zero;
		volatile u64 __iomem *write;

		// TODO: halt cpu n in reset, assume for now its in WFI
		if (address) {
			if (__pa(PAGE_OFFSET)) {
				zero = ioremap(0, size * 4);
				if (!zero) {
					pr_warn("BOOTUP jump vectors not accessible\n");
					return -1;
				}
			} else {
				zero = (__force u8 __iomem *)PAGE_OFFSET;
			}
			write = (u64 __iomem *)zero;

			/*
			* This is elegant way how to jump to any address
			* 0x0: Load address at 0x8 to r0
			* 0x4: Jump by mov instruction
			* 0x8: Jumping address
			*/
			/* On S3MA ASIC the reset vector is in 64-bit OCM so setup the trampoline code
			 * and jump address in a stack buffer and use 64-bit writes to copy */
			memcpy(buff, &s3ma_secondary_trampoline, size * 4);
			buff[jump] = address;
			for ( word = 0; word < size; word += 2 ) {
				u64  tmp = *((u64 *)&buff[word]);
				*write++ = tmp;
			}

			flush_cache_all();
			outer_flush_range(0, sizeof(buff));
			smp_wmb();

			if (__pa(PAGE_OFFSET))
				iounmap(zero);
		}

		/* Since arch_send_wakeup_ipi_mask() takes logical CPU numbers and early in the
		 * boot the GIC map isn't setup, using it for this spams wakeup IRQs to all the
		 * other CPU cores.  Instead use the GIC directly to wakeup each core in order */
		writel_relaxed(0x10000 << cpu, s3ma_gic_dist_base + GIC_DIST_SOFTINT);
		return 0;
	}

	pr_warn("Can't start CPU%d: Wrong starting address %x\n", cpu, address);

	return -1;
}
EXPORT_SYMBOL(s3ma_cpun_start);

static int s3ma_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	return s3ma_cpun_start(virt_to_phys(s3ma_secondary_startup), cpu);
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
static void __init s3ma_smp_init_cpus(void)
{
	unsigned int i;

	ncores = scu_get_core_count(s3ma_scu_base_addr);

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);

	/* sanity check */
	if (ncores > num_possible_cpus()) {
		pr_warn("s3ma: no. of cores (%d) greater than configured"
			"maximum of %d - clipping\n", ncores, num_possible_cpus());
		ncores = num_possible_cpus();
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
}

static void __init s3ma_smp_prepare_cpus(unsigned int max_cpus)
{
	scu_enable(s3ma_scu_base_addr);
}

/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
static void s3ma_cpu_die(unsigned int cpu)
{
	cpu_do_idle();

	/* We should have never returned from idle */
	panic("cpu %d unexpectedly exit from shutdown\n", cpu);
}

struct smp_operations s3ma_smp_ops __initdata = {
	.smp_init_cpus		= s3ma_smp_init_cpus,
	.smp_prepare_cpus	= s3ma_smp_prepare_cpus,
	.smp_boot_secondary	= s3ma_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= s3ma_cpu_die,
#endif
};
