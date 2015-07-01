/*
 *  Copyright (c) 2003 ARM Limited
 *  Copyright (c) u-boot contributors
 *  Copyright (c) 2012 Pavel Machek <pavel@denx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/memory.h>
#include <asm/assembler.h>

	.arch	armv7-a

ENTRY(s3ma_secondary_trampoline)
ARM_BE8(setend	be)				@ ensure we are in BE8 mode
	ldr	r0, s3ma_secondary_trampoline_jump
ARM_BE8(rev	r0, r0)
	bx	r0
.globl s3ma_secondary_trampoline_jump
s3ma_secondary_trampoline_jump:
	/* Space for jumping address */
	.word	/* cpu 1 */
.globl s3ma_secondary_trampoline_end
s3ma_secondary_trampoline_end:
ENDPROC(s3ma_secondary_trampoline)

ENTRY(s3ma_secondary_startup)
       bl      v7_invalidate_l1
       b       secondary_startup
ENDPROC(s3ma_secondary_startup)
