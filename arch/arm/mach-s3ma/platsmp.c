/*
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
		pr_warn("socfpga: no. of cores (%d) greater than configured"
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
