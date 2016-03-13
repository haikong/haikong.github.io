
#ifndef _PIC_OPERATION_H
#define _PIC_OPERATION_H

#include <sys/time.h>
#include <pthread.h>
#include <file.h>

#define INPUT_TYPE_STDIN        0
#define INPUT_TYPE_TOUCHSCREEN  1

#define INPUT_VALUE_UP              0
#define INPUT_VALUE_DOWN            1
#define INPUT_VALUE_EXIT            2
#define INPUT_VALUE_UNKOWN            -1

typedef struct _PixelDatas{
    int iWith;
    int iHeight;
    int iBpp;
    int iLineByteCnt;
    int iTotalBytes;
    unsigned char* aucPixelDatas;
}T_PixelDatas,*PT_PixelDatas;


typedef struct _PicFileParser{
    char* name;
    int   (*isSupport)(PT_FileMap ptFileMap);
    int   (*GetPixelDatas)(PT_FileMap ptFileMap,PT_PixelDatas ptPixelDatas);
    int   (*FreePixelDatas)(PT_PixelDatas ptPixelDatas);
    struct _PicFileParser *pnext;
}T_PicFileParser,*PT_PicFileParser;

int PicMerge(int iX,int iY,PT_PixelDatas ptSmallPic,PT_PixelDatas ptBigPic);
int PicZoom(const PT_PixelDatas ptOriginPic,const PT_PixelDatas ptZoomPic);
#endif /* _INPUT_MANAGER_H */

