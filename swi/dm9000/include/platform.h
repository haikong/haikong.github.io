#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <types.h>
#include <bsp.h>
#include <io.h>
/*platform device struct*/
typedef struct platform_device{
	const char*			name;
	int					id;
	UINT32				num_resources;
	struct resource*	resource;
	struct platform_device *next;
	void* 				private_data;/*private data*/
}t_platform_device;
/*function declaration*/
int platform_add_devices(t_platform_device **devs, int num);
#endif

