#
# Makefile for the linux kernel.
#

ccflags-y += -I$(srctree)/arch/arm/mach-s3ma/include

obj-y					:= s3ma.o ocm.o
obj-$(CONFIG_SMP)	+= headsmp.o platsmp.o
