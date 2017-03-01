#include <types.h>
#include <bsp.h>
#include <net.h>
#include <s3c2440_addr.h>
#include <platform.h>
#include <io.h>
#include <dm9000.h>
#include <intirq.h>
#include <stdio.h>
/* DM9000AEP 10/100 ethernet controller */
static struct resource s3c2440_dm9000_resource[] _init_data = {
	[0]={
		.start = DM9000_CMD_BASE,
		.end   = DM9000_CMD_BASE + 3,
		.flags = IORESOURCE_MEM

	},
	[1]={
		.start = DM9000_DAT_BASE,
		.end   = DM9000_DAT_BASE + 3,
		.flags = IORESOURCE_MEM
	},
	[2]={
		.start = EXTERNIRQ7,
		.end   = EXTERNIRQ7,
		.flags = IORESOURCE_IRQ | HLEVL,
	}
};

/*
 * The DM9000 has no eeprom, and it's MAC address is set by
 * the bootloader before starting the kernel.
 */
static struct dm9000_plat_data s3c2440_dm9000_pdata _init_data = {
	.flags		= (DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM),
	.dev_addr[0] = 0x0a,
	.dev_addr[1] = 0x1b,	
	.dev_addr[2] = 0x2c,	
	.dev_addr[3] = 0x3d,	
	.dev_addr[4] = 0x4e,	
	.dev_addr[5] = 0x5f,	
};

/*The DM9000 resource*/
static t_platform_device s3c2440_device_eth _init_data= {
	.name			= "dm9000",
	.id				= -1,
	.num_resources	= ARRAY_SIZE(s3c2440_dm9000_resource),
	.resource		= s3c2440_dm9000_resource,
	.private_data	= &s3c2440_dm9000_pdata,
};

/*the borad platform devices*/
static t_platform_device *smdk2440_devices[] _init_data = {
	&s3c2440_device_eth,
	NULL,
};

/*****************************************************************************
 �� �� ��  : smdk2440_machine_init
 ��������  : Registers all the platform devices.
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��28��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int  smdk2440_machine_init(void)
{	
	int ret;
	ret = platform_add_devices(smdk2440_devices, ARRAY_SIZE(smdk2440_devices));
	if(ret)
		printf("\n\tSome devices regsited error.\n\r");
	else
		printf("\n\tRegisters all the platform devices.\n\r");
	return ret;
}

