#include <platform.h>
#include <types.h>
#include <stdio.h>
/*platform devices global variable*/
t_platform_device *gt_platform_device = NULL;


/*****************************************************************************
 函 数 名  : list_add
 功能描述  : insert a list_node into the rootp list
 输入参数  : t_platform_device **rootp  
             t_platform_device *pdev    
 输出参数  : 无
 返 回 值  : 0:succeed -1:failed
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月29日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static int list_add(t_platform_device **rootp,t_platform_device *pdev)
{
	int ret;
	t_platform_device *current;
	current = *rootp;
	if(pdev == NULL)
	{
		ret = -1;
	}
	else
	{
		pdev->next = current;
		*rootp = pdev;
		ret = 0;
	}
	return ret;
}
/**
 * platform_add_devices - add a numbers of platform devices
 * @devs: array of platform devices to add
 * @num: number of platform devices in array
 */
int platform_add_devices(t_platform_device **devs, int num)
{
	int i, ret = 0;
	/*add platform devices into gt_platform_device*/
	for (i = 0; i < num; i++) {
		if(NULL != devs[i]){
			ret |= list_add(&gt_platform_device,devs[i]);
		}
	}
	return ret;
}

