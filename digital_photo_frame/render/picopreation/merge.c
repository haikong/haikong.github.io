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
 * ����ͼƬ������ͼƬ
 * iStartXofNewPic, iStartYofNewPic : ����ͼƬ��(iStartXofNewPic, iStartYofNewPic)��ʼ�����������ںϲ�
 * iStartXofOldPic, iStartYofOldPic : �ϲ�����ͼƬ��(iStartXofOldPic, iStartYofOldPic)ȥ
 * iWidth, iHeight : �ϲ�����Ĵ�С
 * ptNewPic        : ��ͼƬ
 * ptOldPic        : ��ͼƬ
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


