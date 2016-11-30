#include <platform.h>
#include <types.h>
#include <stdio.h>
/*platform devices global variable*/
t_platform_device *gt_platform_device = NULL;


/*****************************************************************************
 �� �� ��  : list_add
 ��������  : insert a list_node into the rootp list
 �������  : t_platform_device **rootp  
             t_platform_device *pdev    
 �������  : ��
 �� �� ֵ  : 0:succeed -1:failed
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

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

