#ifndef _ICON_MANAGER_H
#define _ICON_MANAGER_H

#include <disp_manager.h>
#include <page_manager.h>
#include <pic_operation.h>

int  GetPixelDatasForIcon(char* strFileName,PT_PixelDatas ptPixelDatas);
void FreePixelDatasForIcon(PT_PixelDatas ptPixelDatas);
void FlushVideoMemToDev(PT_VideoMem ptVideoMem);
void PressButton(PT_Layout ptLayout);
void ReleaseButton(PT_Layout ptLayout);
int GetFontPixel(void);
int GetPicPixel(void);
int DrawPixel(void);
int MergerStringToCenterOfRectangleInVideoMem(int iTopLeftX, int iTopLeftY, int iBotRightX, int iBotRightY, unsigned char *pucTextString, PT_VideoMem ptVideoMem);
void ClearRectangleInVideoMem(int iTopLeftX, int iTopLeftY, int iBotRightX, int iBotRightY, PT_VideoMem ptVideoMem, unsigned int dwColor);
int GetPixelDatasFrmFile(char *strFileName, PT_PixelDatas ptPixelDatas);
void FreePixelDatasFrmFile(PT_PixelDatas ptPixelDatas);
int isPictureFileSupported(char *strFileName);
int PicMergeRegion(int iStartXofNewPic, int iStartYofNewPic, int iStartXofOldPic, int iStartYofOldPic, int iWidth, int iHeight, PT_PixelDatas ptNewPic, PT_PixelDatas ptOldPic);



#endif


