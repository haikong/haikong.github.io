#include <pic_operation.h>
#include <config.h>
#include <stdlib.h>
#include <string.h>

int PicZoom(const PT_PixelDatas ptOriginPic,const PT_PixelDatas ptZoomPic)
{
    int iX,iY;
    unsigned long dwsrcy;
    unsigned char* pucDest;
    unsigned char* pucSrc;
    unsigned long dwPicByte = ptOriginPic->iBpp /8;
    unsigned long dwdst_Width = ptZoomPic->iWith;
    unsigned long * pdwSrcXTable ;
    if((0 == ptOriginPic->iHeight) || (0 == ptOriginPic->iWith)){

        return -1;
    }
    if(ptZoomPic->iBpp != ptOriginPic->iBpp){
        DBG_PRINTF("ptZoomPic->iBpp != ptOriginPic->iBpp\n");
        return -1;
    }
    pdwSrcXTable = malloc(sizeof(unsigned long) * dwdst_Width);
    if (NULL == pdwSrcXTable)
    {
        DBG_PRINTF("malloc error!\n");
        return -1;
    }

    for(iX = 0;iX < dwdst_Width;iX++)
        pdwSrcXTable[iX] = (iX * ptOriginPic->iWith / ptZoomPic->iWith);

    for(iY = 0;iY < ptZoomPic->iHeight;iY++)
    {
        dwsrcy = (iY * ptOriginPic->iHeight / ptZoomPic->iHeight);
        pucDest = ptZoomPic->aucPixelDatas + iY * ptZoomPic->iLineByteCnt;
        pucSrc  = ptOriginPic->aucPixelDatas + dwsrcy * ptOriginPic->iLineByteCnt;
        for(iX = 0;iX < dwdst_Width;iX++)
        {
            /*原图坐标:pdwSrcX_Table[iX],dwsrcy
             *缩放坐标:x,y
             */
            memcpy(pucDest + iX * dwPicByte,pucSrc + pdwSrcXTable[iX] * dwPicByte,dwPicByte);
        }

    }
    free(pdwSrcXTable);
    return 0;
}
