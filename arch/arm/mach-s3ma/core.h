/*
 * Copyright 2015 Silver Bullet Technology
 * Derived from Socfpga, Copyright (C) 2012 Altera Corporation
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __MACH_CORE_H
#define __MACH_CORE_H

#include <mach/s3ma-regs.h>

extern void __iomem *s3ma_scu_base_addr;
extern void __iomem *s3ma_mru_base_addr;

extern void s3ma_init_clocks(void);
extern void s3ma_mcu_init(void);

#ifdef CONFIG_SMP
extern void s3ma_secondary_startup(void);
extern char s3ma_secondary_trampoline;
extern char s3ma_secondary_trampoline_jump;
extern char s3ma_secondary_trampoline_end;
extern int s3ma_cpun_start(u32 address, int cpu);
extern struct smp_operations s3ma_smp_ops;
#endif

#define S3MA_SCU_VIRT_BASE   0xf8000000
#define S3MA_MRU_VIRT_BASE   0xf8002000
#define S3MA_UART0_VIRT_BASE 0xf8004000

#endif
