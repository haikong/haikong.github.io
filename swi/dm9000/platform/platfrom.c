#include <platform.h>
/*platform devices global variable*/
struct platform_device *gt_platform_device;

/**
 * platform_add_devices - add a numbers of platform devices
 * @devs: array of platform devices to add
 * @num: number of platform devices in array
 */
int platform_add_devices(struct platform_device **devs, int num)
{
	struct platform_device *pdev;
	int i, ret = 0;
	pdev = gt_platform_device;
	while(pdev)
		pdev = pdev->next;
	/*add platform devices into gt_platform_device*/
	for (i = 0; i < num; i++) {
		pdev = devs[i];
		pdev = pdev->next;
	}
	pdev->next = NULL;
	return ret;
}

