/** \file       sbt_test_dma_test.c
 *  \brief      memory-memory DMA speed test for PL330 on Xilinx Zynq
 *
 *  \copyright Copyright 2014-2015 Silver Bullet Technologies
 *
 *             This program is free software; you can redistribute it and/or modify it
 *             under the terms of the GNU General Public License as published by the Free
 *             Software Foundation; either version 2 of the License, or (at your option)
 *             any later version.
 *
 *             This program is distributed in the hope that it will be useful, but WITHOUT
 *             ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *             FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *             more details.
 *
 * vim:ts=4:noexpandtab
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/of_device.h>
#include <linux/pagemap.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>


static size_t sbt_test_dma_size = 0x00100000; // 1MByte

static uint32_t   *sbt_test_dma_src_virt = NULL;
static dma_addr_t  sbt_test_dma_src_phys = 0;
static uint32_t   *sbt_test_dma_dst_virt = NULL;
static dma_addr_t  sbt_test_dma_dst_phys = 0;


static void sbt_test_dma_cb (void *cmp)
{
	printk("completion\n");
    complete(cmp);
}

static void sbt_test_dma_paint (uint32_t *buff, size_t size)
{
	while ( size )
	{
		*buff = (uint32_t)buff;
		buff++;
		size -= sizeof(uint32_t);
	}
}

static void sbt_test_dma_compare (const uint32_t *src, const uint32_t *dst, size_t size)
{
	unsigned long err = 0;

	while ( size )
	{
		if ( *src != *dst )
		{
			err++;
			if ( err < 10 )
				printk("src %p / dst %p mismatch\n", src, dst);
		}
		src++;
		dst++;
		size -= sizeof(uint32_t);
	}

	if ( err > 9 )
		printk("%lu more mismatches\n", err - 9);
	else if ( !err )
		printk("buffers match after DMA\n");
}

static bool sbt_test_dma_dma_filter (struct dma_chan *chan, void *filter_param)
{
	if ( chan->chan_id != (int)filter_param )
		return false;

	printk("picked chan %p with id %d\n", chan, chan->chan_id);
	return true;
}

int sbt_test_dma_test (int chan, size_t size)
{
	struct dma_chan                *dma_chan;
	struct dma_device              *dev;
	struct dma_async_tx_descriptor *dsc;
	struct completion               cmp;
	dma_cookie_t                    cookie;
	struct timespec                 beg, end;
	unsigned long                   timeout;
	enum dma_status                 status;
	struct timespec                 delta;
	uint32_t                        usec;
	uint32_t                        rate;
	int                             ret = 0;

	dma_cap_mask_t mask;
	dma_cap_zero(mask);
	dma_cap_set(DMA_MEMCPY, mask);

	if ( chan < 0 )
    	dma_chan = dma_request_channel(mask, NULL, NULL);
	else
    	dma_chan = dma_request_channel(mask, sbt_test_dma_dma_filter, (void *)chan);

    if ( !dma_chan )
	{
		pr_err("dma_request_channel(..., %d) failed, stop\n", chan);
		return -ENODEV;
	}
	printk("dma_chan %p (%d) -> ", dma_chan, dma_chan->chan_id);

	if ( !(dev = dma_chan->device) )
	{
		pr_err("no dev? stop.\n");
		ret = -ENODEV;
		goto release;
	}
	printk("dev %p (%s)\n", dev, dev_name(dev->dev));

	if ( size >= 0 )
	{
		printk("Size %d from cmdline\n", size);
		sbt_test_dma_size = size;
	}

//	struct dma_slave_config cfg;
//	dev->device_control(dma_chan, DMA_SLAVE_CONFIG, (unsigned long)&cfg);

	sbt_test_dma_dst_virt = dma_alloc_coherent(NULL, sbt_test_dma_size, &sbt_test_dma_dst_phys, GFP_KERNEL);
	if ( !sbt_test_dma_dst_virt )
	{
		pr_err("dma_alloc_coherent(..., %08zu, dst, ...) failed, stop\n", sbt_test_dma_size);
		ret = -ENOMEM;
		goto release;
	}
	printk("dst buffer: %p virt, %08lx phys\n", sbt_test_dma_dst_virt, (unsigned long)sbt_test_dma_dst_phys);
	sbt_test_dma_paint(sbt_test_dma_dst_virt, sbt_test_dma_size);

	sbt_test_dma_src_virt = dma_alloc_coherent(NULL, sbt_test_dma_size, &sbt_test_dma_src_phys, GFP_KERNEL);
	if ( !sbt_test_dma_src_virt )
	{
		pr_err("dma_alloc_coherent(..., %08zu, src, ...) failed, stop\n", sbt_test_dma_size);
		ret = -ENOMEM;
		goto free_dst;
	}
	printk("src buffer: %p virt, %08lx phys\n", sbt_test_dma_src_virt, (unsigned long)sbt_test_dma_src_phys);
	sbt_test_dma_paint(sbt_test_dma_src_virt, sbt_test_dma_size);

	dsc = dev->device_prep_dma_memcpy(dma_chan, sbt_test_dma_dst_phys, sbt_test_dma_src_phys,
	                                  sbt_test_dma_size, DMA_CTRL_ACK);
	if ( !dsc )
	{
		ret = -ENOSYS;
		goto free_src;
	}

	// setup completion callback
	init_completion(&cmp);
	dsc->callback       = sbt_test_dma_cb;
	dsc->callback_param = &cmp;

	// submit transaction
	cookie = dsc->tx_submit(dsc);
	if ( dma_submit_error(cookie) )
	{
		pr_err("tx_submit() failed, stop\n");
		ret = -ENOSYS;
		goto free_src;
	}
	printk("tx_submit() ok, cookie %lx\n", (unsigned long)cookie);

	getrawmonotonic(&beg);
	dma_async_issue_pending(dma_chan);

	timeout = wait_for_completion_timeout(&cmp, 10 * HZ);
	getrawmonotonic(&end);

	printk("Size: %zu bytes\n", sbt_test_dma_size);

	// check status
	status = dma_async_is_tx_complete(dma_chan, cookie, NULL, NULL);
	if ( timeout == 0 )
		pr_warn("DMA timeout, stop\n");
	else if ( status != DMA_COMPLETE )
		pr_warn("tx got completion callback, but status is \'%s\'\n",
		        status == DMA_ERROR ? "error" : "in progress");
	else
	{
		printk("no timeout, status DMA_COMPLETE\n");

		delta = timespec_sub(end, beg);
		printk("Time: %ld.%09lu sec\n", delta.tv_sec, delta.tv_nsec);

		// calculate speed
		usec = delta.tv_sec;
	    usec *= 1000000000;
	    usec += delta.tv_nsec;
		usec /= 1000;

    	rate = 0;
		if ( usec > 0 )
		{
	    	rate = sbt_test_dma_size / usec;
			printk("transfer rate : %u MB/s\n", rate);
		}
	}

	// check buffers
	sbt_test_dma_compare(sbt_test_dma_src_virt, sbt_test_dma_dst_virt, sbt_test_dma_size);

free_src:
	dma_free_coherent(NULL, sbt_test_dma_size, sbt_test_dma_src_virt, sbt_test_dma_src_phys);

free_dst:
	dma_free_coherent(NULL, sbt_test_dma_size, sbt_test_dma_dst_virt, sbt_test_dma_dst_phys);
	
release:
	dma_release_channel(dma_chan);

	return ret;
}


