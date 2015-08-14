/** \file      sbt-test.c
 *  \brief     /proc/ test stub
 *
 *  \copyright Copyright 2013-2015 Silver Bullet Technologies
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
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>


static struct proc_dir_entry *st_proc_root;
static struct proc_dir_entry *st_proc_dma;

int st_dma_chan = -1;


static struct proc_dir_entry *st_dma_chan_proc;
static ssize_t st_dma_chan_read (struct file *f, char __user *u, size_t s, loff_t *o)
{
	char     b[128];
	ssize_t  l = 0;

	if ( *o )
		return 0;

	l = snprintf(b, sizeof(b), "%d\n", st_dma_chan);

	if ( copy_to_user(u, b, l) )
		return -EFAULT;

	*o += l;
	return l;
}
static ssize_t st_dma_chan_write (struct file *f, const char __user *u, size_t s,
                                      loff_t *o)
{
	long  dest;
	char  b[128];
	char *p, *q;

	if ( *o )
		return 0;

	if ( s >= sizeof(b) )
		s = sizeof(b) - 1;
	b[s] = '\0';

	if ( copy_from_user(b, u, s) )
		return -EFAULT;

	for ( p = b; *p && !isdigit(*p); p++ ) ;
	for ( q = p; *q &&  isdigit(*q); q++ ) ;
	*q++ = '\0';
	if ( kstrtol(p, 10, &dest) )
		goto einval;

	printk("chan set: '%s' -> '%s' -> %lu\n", b, p, dest);
	st_dma_chan = dest;

	*o += s;
	return s;

einval:
	return -EINVAL;
}
struct file_operations  st_dma_chan_fops =
{
	read:   st_dma_chan_read,
	write:  st_dma_chan_write,
};


size_t st_dma_size = 1 << 20;
static struct proc_dir_entry *st_dma_size_proc;
static ssize_t st_dma_size_read (struct file *f, char __user *u, size_t s, loff_t *o)
{
	char     b[128];
	ssize_t  l = 0;

	if ( *o )
		return 0;

	l = snprintf(b, sizeof(b), "%zu\n", st_dma_size);

	if ( copy_to_user(u, b, l) )
		return -EFAULT;

	*o += l;
	return l;
}
static ssize_t st_dma_size_write (struct file *f, const char __user *u, size_t s,
                                      loff_t *o)
{
	long  dest;
	char  b[128];
	char *p, *q;

	if ( *o )
		return 0;

	if ( s >= sizeof(b) )
		s = sizeof(b) - 1;
	b[s] = '\0';

	if ( copy_from_user(b, u, s) )
		return -EFAULT;

	for ( p = b; *p && !isdigit(*p); p++ ) ;
	for ( q = p; *q &&  isdigit(*q); q++ ) ;
	*q++ = '\0';
	if ( kstrtol(p, 10, &dest) )
		goto einval;

	printk("size set: '%s' -> '%s' -> %lu\n", b, p, dest);
	st_dma_size = dest;

	*o += s;
	return s;

einval:
	return -EINVAL;
}
struct file_operations  st_dma_size_fops =
{
	read:   st_dma_size_read,
	write:  st_dma_size_write,
};


extern int sbt_test_dma_test (int chan, size_t size);
static struct proc_dir_entry *st_dma_trig_proc;
static void st_dma_trig (void)
{
	int ret;

	printk("Trigger DMA test, chan %d, size %zu...\n", st_dma_chan, st_dma_size);
	ret = sbt_test_dma_test(st_dma_chan, st_dma_size);
	printk("DMA test returned %d\n", ret);
}
static ssize_t st_dma_trig_read (struct file *f, char __user *u, size_t s, loff_t *o)
{
	if ( *o )
		return 0;

	st_dma_trig();

	return 0;
}
static ssize_t st_dma_trig_write (struct file *f, const char __user *u, size_t s,
                                      loff_t *o)
{
	char  b[128];

	if ( *o )
		return 0;

	if ( s >= sizeof(b) )
		s = sizeof(b) - 1;

	if ( copy_from_user(b, u, s) )
		return -EFAULT;

	st_dma_trig();

	*o += s;
	return s;
}
struct file_operations  st_dma_trig_fops =
{
	read:   st_dma_trig_read,
	write:  st_dma_trig_write,
};


void sd_test_exit (void)
{
	if ( st_dma_chan_proc             ) proc_remove(st_dma_chan_proc);
	if ( st_dma_size_proc             ) proc_remove(st_dma_size_proc);
	if ( st_dma_trig_proc             ) proc_remove(st_dma_trig_proc);
	if ( st_proc_dma                  ) proc_remove(st_proc_dma);
	if ( st_proc_root                 ) proc_remove(st_proc_root);
}
module_exit(sd_test_exit);

int sd_test_init (void)
{
	if ( !(st_proc_root = proc_mkdir("sbt", NULL)) )
		return -ENOMEM;


	if ( !(st_proc_dma = proc_mkdir("dma", st_proc_root)) )
		return -ENOMEM;

	st_dma_chan_proc = proc_create("chan", 0666, st_proc_dma, &st_dma_chan_fops);
	if ( !st_dma_chan_proc )
		goto fail;

	st_dma_size_proc = proc_create("size", 0666, st_proc_dma, &st_dma_size_fops);
	if ( !st_dma_size_proc )
		goto fail;

	st_dma_trig_proc = proc_create("trig", 0666, st_proc_dma, &st_dma_trig_fops);
	if ( !st_dma_trig_proc )
		goto fail;


	return 0;

fail:
	sd_test_exit();
	return -1;
}
module_init(sd_test_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Morgan Hughes <morgan.hughes@silver-bullet-tech.com>");
MODULE_DESCRIPTION("SBT test module for S3MA");

