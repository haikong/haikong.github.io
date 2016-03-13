#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include <pic_operation.h>

typedef struct _Layout{
    int iTopLeftX;
    int iTopLeftY;
    int iBotRightX;
    int iBotRightY;
    char *strIconName;
}T_Layout,*PT_Layout;

typedef enum {
    VMS_FREE = 0,
    VMS_USED_FOR_PREPARE,
    VMS_USED_FOR_CUR,
}E_VideoMemState;

typedef enum {
    PS_BLANK = 0,
    PS_GENERATING,
    PS_GENERATED,
}E_PicState;

typedef struct _VideoMem{
    int                 iID;
    int                 bCevFrameBuffer;
    E_VideoMemState     eVideoMemState;
    E_PicState          ePicState;
    T_PixelDatas        tPixelDatas;
    struct _VideoMem    *pNext;
}T_VideoMem,*PT_VideoMem;


typedef struct DispOpr {
	char *name;
	int iXres;
	int iYres;
	int iBpp;
    int iLineWidth;
    unsigned char* pucDispMem;
	int (*DeviceInit)(void);
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int dwColor);
	int (*CleanScreen)(unsigned int dwBackColor);
    int (*ShowPage)(PT_VideoMem ptVideoMem);
	struct DispOpr *ptNext;
}T_DispOpr, *PT_DispOpr;


int RegisterDispOpr(PT_DispOpr ptDispOpr);
void ShowDispOpr(void);
int DisplayInit(void);
int FBInit(void);
int GetDispResolution(int* piXres,int* piYres,int* piBpp);
int AllocVideoMem(int size);
PT_DispOpr GetDefaultDispDev(void);
PT_VideoMem GetVedioMem(int iID,int bCur);
void PutVideoMem(PT_VideoMem ptVideoMem);
int  SelectAndInitDefaultDisplayDev(char* name);
void ClearVideoMem(PT_VideoMem ptVideoMem, unsigned int dwColor);
PT_VideoMem GetDevVideoMem(void);
void ClearVideoMemRegion(PT_VideoMem ptVideoMem, PT_Layout ptLayout, unsigned int dwColor);

#endif /* _DISP_MANAGER_H */

