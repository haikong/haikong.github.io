#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <types.h>
#include <bsp.h>
#include <io.h>
/*platform device struct*/
struct platform_device {
	const char*			name;
	int					id;
	UINT32				num_resources;
	struct resource*	resource;
	struct platform_device* next;
	void* 				private_data;/*private data*/
};
/*function declaration*/
int platform_add_devices(struct platform_device **devs, int num);
#endif

