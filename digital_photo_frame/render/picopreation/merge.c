#include <pic_operation.h>
#include <stdlib.h>
#include <string.h>

int PicMerge(int iX,int iY,PT_PixelDatas ptSmallPic,PT_PixelDatas ptBigPic)
{
    int i;
    unsigned char* pucSrc;
    unsigned char* pucDest;

    if((ptSmallPic->iWith > ptBigPic->iWith) || (ptSmallPic->iHeight > ptBigPic->iHeight)
        || (ptSmallPic->iBpp != ptBigPic->iBpp))
        return -1;

    pucSrc  = ptSmallPic->aucPixelDatas;
    pucDest = ptBigPic->aucPixelDatas + iY * ptBigPic->iLineByteCnt + iX * ptBigPic->iBpp /8;

    for(i = 0;i < ptSmallPic->iHeight;i++)
    {
        memcpy(pucDest,pucSrc,ptSmallPic->iLineByteCnt);
        pucDest  += ptBigPic->iLineByteCnt ;
        pucSrc   += ptSmallPic->iLineByteCnt;
    }
    return 0;
}

/*
 * 把新图片并入老图片
 * iStartXofNewPic, iStartYofNewPic : 从新图片的(iStartXofNewPic, iStartYofNewPic)开始读出数据用于合并
 * iStartXofOldPic, iStartYofOldPic : 合并到老图片的(iStartXofOldPic, iStartYofOldPic)去
 * iWidth, iHeight : 合并区域的大小
 * ptNewPic        : 新图片
 * ptOldPic        : 老图片
 */
int PicMergeRegion(int iStartXofNewPic, int iStartYofNewPic, int iStartXofOldPic, int iStartYofOldPic, int iWidth, int iHeight, PT_PixelDatas ptNewPic, PT_PixelDatas ptOldPic)
{
	int i;
	unsigned char *pucSrc;
	unsigned char *pucDst;
    int iLineBytesCpy = iWidth * ptNewPic->iBpp / 8;

    if ((iStartXofNewPic < 0 || iStartXofNewPic >= ptNewPic->iWith) || \
        (iStartYofNewPic < 0 || iStartYofNewPic >= ptNewPic->iHeight) || \
        (iStartXofOldPic < 0 || iStartXofOldPic >= ptOldPic->iWith) || \
        (iStartYofOldPic < 0 || iStartYofOldPic >= ptOldPic->iHeight))
    {
        return -1;
    }


	pucSrc = ptNewPic->aucPixelDatas + iStartYofNewPic * ptNewPic->iLineByteCnt+ iStartXofNewPic * ptNewPic->iBpp / 8;
	pucDst = ptOldPic->aucPixelDatas + iStartYofOldPic * ptOldPic->iLineByteCnt + iStartXofOldPic * ptOldPic->iBpp / 8;
	for (i = 0; i < iHeight; i++)
	{
		memcpy(pucDst, pucSrc, iLineBytesCpy);
		pucSrc += ptNewPic->iLineByteCnt;
		pucDst += ptOldPic->iLineByteCnt;
	}
	return 0;
}


