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
	u32 trampoline_code_size = &s3ma_secondary_trampoline_end -
						&s3ma_secondary_trampoline;

	/* MS: Expectation that SLCR are directly map and accessible */
	/* Not possible to jump to non aligned address */
	if (!(address & 3) && (!address || (address >= trampoline_code_size))) {
		/* Store pointer to ioremap area which points to address 0x0 */
		static u8 __iomem *zero;
		u32 trampoline_size = &s3ma_secondary_trampoline_jump -
						&s3ma_secondary_trampoline;

		// TODO: halt cpu n in reset, assume for now its in WFI
		if (address) {
			if (__pa(PAGE_OFFSET)) {
				zero = ioremap(0, trampoline_code_size);
				if (!zero) {
					pr_warn("BOOTUP jump vectors not accessible\n");
					return -1;
				}
			} else {
				zero = (__force u8 __iomem *)PAGE_OFFSET;
			}

			/*
			* This is elegant way how to jump to any address
			* 0x0: Load address at 0x8 to r0
			* 0x4: Jump by mov instruction
			* 0x8: Jumping address
			*/
			memcpy((__force void *)zero, &s3ma_secondary_trampoline,
							trampoline_size);
			writel(address, zero + trampoline_size);

			flush_cache_all();
			outer_flush_range(0, trampoline_code_size);
			smp_wmb();

			if (__pa(PAGE_OFFSET))
				iounmap(zero);
		}
		// TODO: restart cpu n in reset
		// Currently assumes that after wfi instruction the core will reset to 0
		arch_send_wakeup_ipi_mask(cpumask_of(cpu));

		return 0;
	}

	pr_warn("Can't start CPU%d: Wrong starting address %x\n", cpu, address);

	return -1;
}
EXPORT_SYMBOL(s3ma_cpun_start);

static int s3ma_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	/* Send an interrupt to the core to wake it.  Assumes its sitting in WFI */
	arch_send_wakeup_ipi_mask(cpumask_of(cpu));

	return 0;
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
static void __init s3ma_smp_init_cpus(void)
{
	unsigned int i, ncores;

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
