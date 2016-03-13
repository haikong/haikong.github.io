#include <config.h>
#include <page_manager.h>
#include <disp_manager.h>
#include <stdlib.h>
#include <render.h>
#include <string.h>

static void SettingPageRun(PT_PageParams ptParentPageParams);
static int SettingPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);
static int SettingPagePrepare(void);

static T_Layout g_atSettingPageLayout[] = {
    {0,0,0,0,"select_fold.bmp"},
    {0,0,0,0,"interval.bmp"},
    {0,0,0,0,"return.bmp"},
    {0,0,0,0,NULL},
};

static T_PageLayout g_tSettingPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atSettingPageLayout,
};


static T_PageAction g_tSettingPageAction = {
    .name           = "setting",
    .Run            = SettingPageRun,
    .GetInputEvent  = SettingPageGetInputEvent,
    .Prepare        = SettingPagePrepare,
};

/* 计算各图标坐标值 */
static void  CalcSettingPageLayout(PT_PageLayout ptPageLayout)
{
	int iStartY;
	int iWidth;
	int iHeight;
	int iXres, iYres, iBpp;
	int iTmpTotalBytes;
	PT_Layout atLayout;

	atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;

	/*
	 *    ----------------------
	 *                           1/2 * iHeight
	 *          select_fold.bmp  iHeight
	 *                           1/2 * iHeight
	 *          interval.bmp     iHeight
	 *                           1/2 * iHeight
	 *          return.bmp       iHeight
	 *                           1/2 * iHeight
	 *    ----------------------
	 */

	iHeight = iYres * 2 / 10;
	iWidth  = iHeight;
	iStartY = iHeight / 2;

	/* select_fold图标 */
	atLayout[0].iTopLeftY  = iStartY;
	atLayout[0].iBotRightY = atLayout[0].iTopLeftY + iHeight - 1;
	atLayout[0].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[0].iBotRightX = atLayout[0].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1) * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}


	/* interval图标 */
	atLayout[1].iTopLeftY  = atLayout[0].iBotRightY + iHeight / 2 + 1;
	atLayout[1].iBotRightY = atLayout[1].iTopLeftY + iHeight - 1;
	atLayout[1].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[1].iBotRightX = atLayout[1].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[1].iBotRightX - atLayout[1].iTopLeftX + 1) * (atLayout[1].iBotRightY - atLayout[1].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

	/* return图标 */
	atLayout[2].iTopLeftY  = atLayout[1].iBotRightY + iHeight / 2 + 1;
	atLayout[2].iBotRightY = atLayout[2].iTopLeftY + iHeight - 1;
	atLayout[2].iTopLeftX  = (iXres - iWidth) / 2;
	atLayout[2].iBotRightX = atLayout[2].iTopLeftX + iWidth - 1;

	iTmpTotalBytes = (atLayout[2].iBotRightX - atLayout[2].iTopLeftX + 1) * (atLayout[2].iBotRightY - atLayout[2].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

}


static void ShowSettingPage(PT_PageLayout ptPageLayout)
{
    PT_VideoMem ptVideoMem;
    int iError;

    PT_Layout atLayout = ptPageLayout->atLayout;

    /* 1. 获得显存 */
    ptVideoMem = GetVedioMem(ID("setting"), 1);
    if (ptVideoMem == NULL)
    {
        DBG_PRINTF("can't get video mem for setting page!\n");
        return;
    }

    /* 2. 描画数据 */

    /* 如果还没有计算过各图标的坐标 */
    if (atLayout[0].iTopLeftX == 0)
    {
        CalcSettingPageLayout(ptPageLayout);
    }

    iError = GeneratePage(ptPageLayout, ptVideoMem);

    /* 3. 刷到设备上去 */
    FlushVideoMemToDev(ptVideoMem);

    /* 4. 解放显存 */
    PutVideoMem(ptVideoMem);
}



static int SettingPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}


static void SettingPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
    T_PageParams tPageParams;

    tPageParams.iPageID = ID("setting");

	DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 1. 显示页面 */
	ShowSettingPage(&g_tSettingPageLayout);

	/* 2. 创建Prepare线程 */

	/* 3. 调用GetInputEvent获得输入事件，进而处理 */
	while (1)
	{
		iIndex = SettingPageGetInputEvent(&g_tSettingPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			/* 如果是松开 */
			if (bPressed)
			{
				/* 曾经有按钮被按下 */
				ReleaseButton(&g_atSettingPageLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) /* 按下和松开都是同一个按钮 */
				{
					switch (iIndexPressed)
					{
                        case 0: /*browse*/
                        {
							Page("browse")->Run(&tPageParams);
                            ShowSettingPage(&g_tSettingPageLayout);
                        }
                            break;
                        case 1:{/*interval button*/
                            Page("interval")->Run(&tPageParams);
                            ShowSettingPage(&g_tSettingPageLayout);
                        }
                            break;
						case 2: /* 返回按钮 */
						{
						    return;
						}
                            break;
						default:
						{
							break;
						}
					}
				}

				iIndexPressed = -1;
			}
		}
		else
		{
			/* 按下状态 */
			if (iIndex != -1)
			{
				if (!bPressed)
				{
					/* 未曾按下按钮 */
					bPressed = 1;
					iIndexPressed = iIndex;
					PressButton(&g_atSettingPageLayout[iIndexPressed]);
				}
			}
		}
	}
}

static int SettingPagePrepare(void)
{
    return 0;
}

int SettingPageInit(void)
{
	return RegisterPageAction(&g_tSettingPageAction);
}

