/*
 * Copyright (C) 2015 Silver Bullet Technology
 *
 * Based on "Generic on-chip SRAM allocation driver"
 *
 * Copyright (C) 2012 Philipp Zabel, Pengutronix
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/genalloc.h>

#include "common.h"

#define S3MA_OCM_ADDR	0x04000000
#define S3MA_OCM_BLOCK_SIZE	0x10000
#define S3MA_OCM_BLOCKS		8
#define S3MA_OCM_GRANULARITY	32


struct s3ma_ocm_dev {
	void __iomem *base;
	int irq;
	struct gen_pool *pool;
	struct resource res[S3MA_OCM_BLOCKS];
};


/**
 * s3ma_ocm_probe - Probe method for the OCM driver
 * @pdev:	Pointer to the platform_device structure
 *
 * This function initializes the driver data structures and the hardware.
 *
 * Return:	0 on success and error value on failure
 */
static int s3ma_ocm_probe(struct platform_device *pdev)
{
	int ret;
	struct s3ma_ocm_dev *s3ma_ocm;
	u32 i, curr;
	struct resource *res;

	s3ma_ocm = devm_kzalloc(&pdev->dev, sizeof(*s3ma_ocm), GFP_KERNEL);
	if (!s3ma_ocm)
		return -ENOMEM;

	s3ma_ocm->pool = devm_gen_pool_create(&pdev->dev,
					      ilog2(S3MA_OCM_GRANULARITY), -1);
	if (!s3ma_ocm->pool)
		return -ENOMEM;

	curr = 0; /* For storing current struct resource for OCM */
	for (i = 0; i < S3MA_OCM_BLOCKS; i++) {
		u32 base, start, end;

		base = S3MA_OCM_ADDR;

		/* Calculate start and end block addresses */
		start = i * S3MA_OCM_BLOCK_SIZE + base;
		end = start + (S3MA_OCM_BLOCK_SIZE - 1);

		/* Concatenate OCM blocks together to get bigger pool */
		if (i > 0 && start == (s3ma_ocm->res[curr - 1].end + 1)) {
			s3ma_ocm->res[curr - 1].end = end;
		} else {
			/* First resource is always initialized */
			s3ma_ocm->res[curr].start = start;
			s3ma_ocm->res[curr].end = end;
			s3ma_ocm->res[curr].flags = IORESOURCE_MEM;
			curr++; /* Increment curr value */
		}
		dev_dbg(&pdev->dev, "OCM block %d, start %x, end %x\n",
			i, start, end);
	}

	/*
	 * Separate pool allocation from OCM block detection to ensure
	 * the biggest possible pool.
	 */
	for (i = 0; i < S3MA_OCM_BLOCKS; i++) {
		unsigned long size;
		void __iomem *virt_base;

		/* Skip all zero size resources */
		if (s3ma_ocm->res[i].end == 0)
			break;
		dev_dbg(&pdev->dev, "OCM resources %d, start %x, end %x\n",
			i, s3ma_ocm->res[i].start, s3ma_ocm->res[i].end);
		size = resource_size(&s3ma_ocm->res[i]);
		virt_base = devm_ioremap_resource(&pdev->dev,
						  &s3ma_ocm->res[i]);
		if (IS_ERR(virt_base))
			return PTR_ERR(virt_base);

		ret = gen_pool_add_virt(s3ma_ocm->pool,
					(unsigned long)virt_base,
					s3ma_ocm->res[i].start, size, -1);
		if (ret < 0) {
			dev_err(&pdev->dev, "Gen pool failed\n");
			return ret;
		}
		dev_info(&pdev->dev, "S3MA OCM pool: %ld KiB @ 0x%p\n",
			 size / 1024, virt_base);
	}

	platform_set_drvdata(pdev, s3ma_ocm);

	return 0;
}

/**
 * s3ma_ocm_remove - Remove method for the OCM driver
 * @pdev:	Pointer to the platform_device structure
 *
 * This function is called if a device is physically removed from the system or
 * if the driver module is being unloaded. It frees all resources allocated to
 * the device.
 *
 * Return:	0 on success and error value on failure
 */
static int s3ma_ocm_remove(struct platform_device *pdev)
{
	struct s3ma_ocm_dev *s3ma_ocm = platform_get_drvdata(pdev);

	if (gen_pool_avail(s3ma_ocm->pool) < gen_pool_size(s3ma_ocm->pool))
		dev_dbg(&pdev->dev, "removed while SRAM allocated\n");

	return 0;
}

static struct of_device_id s3ma_ocm_dt_ids[] = {
	{ .compatible = "exelis,s3ma-ocmc-1.0" },
	{ /* end of table */ }
};

static struct platform_driver s3ma_ocm_driver = {
	.driver = {
		.name = "s3ma-ocm",
		.of_match_table = s3ma_ocm_dt_ids,
	},
	.probe = s3ma_ocm_probe,
	.remove = s3ma_ocm_remove,
};

static int __init s3ma_ocm_init(void)
{
	return platform_driver_register(&s3ma_ocm_driver);
}

arch_initcall(s3ma_ocm_init);
