#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H
#include <input_manager.h>
#include <disp_manager.h>

typedef struct _pageLayout{
	int         iTopLeftX;
	int         iTopLeftY;
	int         iBotRightX;
	int         iBotRightY;
	int         iBpp;
	int         iMaxTotalBytes;
	PT_Layout   atLayout;
}T_PageLayout,*PT_PageLayout;

typedef struct PageParams {
    int iPageID;                  /* 页面的ID */
    char strCurPictureFile[256];  /* 要处理的第1个图片文件 */
}T_PageParams, *PT_PageParams;


typedef struct _PageAction{
    char    *name;
	void (*Run)(PT_PageParams ptParentPageParams);
    int     (*GetInputEvent)(PT_PageLayout ptPageLayout,PT_InputEvent ptInputEvent);
    int     (*Prepare)(void);
    struct _PageAction *ptNext;
}T_PageAction,*PT_PageAction;

typedef struct PageCfg {
    int iIntervalSecond;
    char strSeletedDir[256];
}T_PageCfg, *PT_PageCfg;


int ID(char* pcName);
int RegisterPageAction(PT_PageAction ptPageAction);
int PagesInit(void);
PT_PageAction Page(char *pcName);
int RegisterPageAction(PT_PageAction ptPageAction);
int MainPageInit(void);
int SettingPageInit(void);
int IntervalPageInit(void);
int BrowsePageInit(void);
int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem);
int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);
int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd);
int AutoPageInit(void);
void GetPageCfg(PT_PageCfg ptPageCfg);
int ManualPageInit(void);

#endif /* _PAGE_MANAGER_H */



