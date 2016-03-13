#include <config.h>
#include <page_manager.h>
#include <disp_manager.h>
#include <stdlib.h>
#include <render.h>

static void MainPageRun(PT_PageParams ptParentPageParams);
static int MainPageGetInputEvent(PT_PageLayout ptPageLayout,PT_InputEvent ptInputEvent);
static int MainPagePrepare(void);

static T_Layout g_atMainPageLayout[] = {
    {0,0,0,0,"browse_mode.bmp"},
    {0,0,0,0,"continue_mod.bmp"},
    {0,0,0,0,"setting.bmp"},
    {0,0,0,0,NULL},
};

static T_PageLayout g_tMainPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atMainPageLayout,
};

static T_PageAction g_tMainPageAction = {
    .name           = "main",
    .Run            = MainPageRun,
    .GetInputEvent  = MainPageGetInputEvent,
    .Prepare        = MainPagePrepare,
};



/* 计算各图标坐标值 */
static void  CalcMainPageLayout(PT_PageLayout ptPageLayout)
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
	 *          browse_mode.bmp  iHeight
	 *                           1/2 * iHeight
	 *         continue_mod.bmp     iHeight
	 *                           1/2 * iHeight
	 *          setting.bmp       iHeight
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
	atLayout[2].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[2].iBotRightX = atLayout[2].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[2].iBotRightX - atLayout[2].iTopLeftX + 1) * (atLayout[2].iBotRightY - atLayout[2].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

}



static void ShowMainPage(PT_PageLayout ptPageLayout)
{
    PT_VideoMem ptVideoMem;
    int iError;

    PT_Layout atLayout = ptPageLayout->atLayout;

    /* 1. 获得显存 */
    ptVideoMem = GetVedioMem(ID("main"), 1);
    if (ptVideoMem == NULL)
    {
        DBG_PRINTF("can't get video mem for main page!\n");
        return;
    }

    /* 2. 描画数据 */

    /* 如果还没有计算过各图标的坐标 */
    if (atLayout[0].iTopLeftX == 0)
    {
        CalcMainPageLayout(ptPageLayout);
    }

    iError = GeneratePage(ptPageLayout, ptVideoMem);

    /* 3. 刷到设备上去 */
    FlushVideoMemToDev(ptVideoMem);

    /* 4. 解放显存 */
    PutVideoMem(ptVideoMem);
}


static void MainPageRun(PT_PageParams ptParentPageParams)
{
    int iIndex;
    T_InputEvent tInputEvent;
    int iBepressed = 0;
    int iIndexPressed = -1;


    T_PageParams tPageParams;

    tPageParams.iPageID = ID("main");
    /*显示页面*/
    ShowMainPage(&g_tMainPageLayout);
    /*创建prepare线程*/

    /*调用GetinputEvent获得输入事件，进而处理*/
    while(1){
        iIndex = MainPageGetInputEvent(&g_tMainPageLayout,&tInputEvent);
        if(tInputEvent.iPressure == 0)
        {
            /*如果松开*/
            if(iBepressed)
            {
                /*曾经有按钮被按下*/
                ReleaseButton(&g_atMainPageLayout[iIndexPressed]);
                iBepressed = 0;
                if(iIndexPressed == iIndex) /*按下和松开都是同一个按钮*/
                {
                    switch (iIndexPressed){

					case 0: /* 浏览按钮 */
					{
                        tPageParams.strCurPictureFile[0] = '\0';
                        Page("manual")->Run(&tPageParams);
						/* 从设置页面返回后显示当首的主页面 */
						ShowMainPage(&g_tMainPageLayout);

						break;
					}
                    case 1: /* 连播按钮 */
					{
                        Page("auto")->Run(&tPageParams);

						/* 从设置页面返回后显示当首的主页面 */
						ShowMainPage(&g_tMainPageLayout);

						break;
					}
                    case 2:/*设置按钮 */{

                        Page("setting")->Run(&tPageParams);
                        /* 从设置页面返回后显示当首的主页面 */
                        ShowMainPage(&g_tMainPageLayout);
                    }
                        break;
                    default:
                        break;
                    }
                    iIndexPressed = -1;
                }
            }
            else{
                /*按下状态*/
                if(iIndex != -1){
                   if(!iBepressed){
                        /*第一次按下*/
                        iBepressed = 1;
                        iIndexPressed = iIndex;
                        PressButton(&g_atMainPageLayout[iIndexPressed]);
                   }
                }
            }
        }
    }
}

static int MainPagePrepare(void)
{
    return 0;
}

static int MainPageGetInputEvent(PT_PageLayout ptPageLayout,PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);

}

int MainPageInit(void)
{
    return RegisterPageAction(&g_tMainPageAction);
}
