#include <config.h>
#include <disp_manager.h>
#include <stdlib.h>
#include <string.h>

static PT_DispOpr   g_ptDispOprHead;
static PT_DispOpr   g_ptDefaultDispOpr;
static PT_VideoMem  g_ptVedioMemHead;

int RegisterDispOpr(PT_DispOpr ptDispOpr)
{
	PT_DispOpr ptTmp;

	if (!g_ptDispOprHead)
	{
		g_ptDispOprHead   = ptDispOpr;
		ptDispOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptDispOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptDispOpr;
		ptDispOpr->ptNext = NULL;
	}

	return 0;
}


void ShowDispOpr(void)
{
	int i = 0;
	PT_DispOpr ptTmp = g_ptDispOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

PT_DispOpr GetDispOpr(char *pcName)
{
	PT_DispOpr ptTmp = g_ptDispOprHead;

	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

int  SelectAndInitDefaultDisplayDev(char* name)
{
    g_ptDefaultDispOpr = GetDispOpr(name);
    if(g_ptDefaultDispOpr){
        g_ptDefaultDispOpr->DeviceInit();
        g_ptDefaultDispOpr->CleanScreen(0);
        return 0;
    }
    return -1;
}


PT_DispOpr GetDefaultDispDev(void)
{
    return g_ptDefaultDispOpr;
}

int GetDispResolution(int* piXres,int* piYres,int* piBpp)
{
    if(g_ptDefaultDispOpr)
    {
        *piXres = g_ptDefaultDispOpr->iXres;

        *piYres = g_ptDefaultDispOpr->iYres;
        *piBpp  = g_ptDefaultDispOpr->iBpp;
        return 0;
    }
    return -1;
}

/*inum =0,��Դ����ʱ������ֻ��frambuff*/
int AllocVideoMem(int iNum)
{
    int i,iRet;
    int iXres;
    int iYres;
    int iBpp;
    int iVedioMemSize,iLineBytes;
    PT_VideoMem ptNew;

    iRet = GetDispResolution(&iXres,&iYres,&iBpp);
    if(iRet){
        DBG_PRINTF("%s %s: %d\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    iVedioMemSize = iXres * iYres * iBpp /8;
    iLineBytes    = iXres * iBpp /8;

    /*�Ȱ��豸�����frambuff��������*/
    ptNew = malloc(sizeof(T_VideoMem ) );
    if(!ptNew){

        DBG_PRINTF("%s %s: %d\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    ptNew->tPixelDatas.aucPixelDatas = g_ptDefaultDispOpr->pucDispMem;
    ptNew->iID = 0;
    ptNew->bCevFrameBuffer =1;
    ptNew->eVideoMemState       = VMS_FREE;
    ptNew->ePicState            = PS_BLANK;
    ptNew->tPixelDatas.iWith    = iXres;
    ptNew->tPixelDatas.iHeight  = iYres;
    ptNew->tPixelDatas.iBpp     = iBpp;
    ptNew->tPixelDatas.iLineByteCnt = iLineBytes;
    ptNew->tPixelDatas.iTotalBytes = iVedioMemSize;

    if(iNum != 0)
    {
        ptNew->eVideoMemState       = VMS_USED_FOR_CUR;

    }
	ptNew->pNext = g_ptVedioMemHead;
	g_ptVedioMemHead = ptNew;
    for(i = 0;i  < iNum;i++){
        ptNew = malloc(sizeof(T_VideoMem ) + iVedioMemSize);
        if(!ptNew){

            DBG_PRINTF("%s %s: %d\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
        }
        ptNew->tPixelDatas.aucPixelDatas = (unsigned char*)(ptNew + 1);
        ptNew->iID = 0;
        ptNew->bCevFrameBuffer      = 0;
        ptNew->eVideoMemState       = VMS_FREE;
        ptNew->ePicState            = PS_BLANK;
        ptNew->tPixelDatas.iWith    = iXres;
        ptNew->tPixelDatas.iHeight  = iYres;
        ptNew->tPixelDatas.iBpp     = iBpp;
        ptNew->tPixelDatas.iLineByteCnt = iLineBytes;
        ptNew->tPixelDatas.iTotalBytes = iVedioMemSize;
        ptNew->pNext = g_ptVedioMemHead;
        g_ptVedioMemHead = ptNew;
    }
    return 0;
}

/* ���һ��ɲ�����videomem, �������PutVideoMem���ͷ� */
PT_VideoMem GetVedioMem(int iID,int bCur)
{
    PT_VideoMem ptTemp = g_ptVedioMemHead;

    DBG_PRINTF("ptTemp->iID = %d iID = %d\n",ptTemp->iID,iID);
    /*1.����:ȡ�����е�ID��ͬ*/
    while(ptTemp){
        if((ptTemp->eVideoMemState == VMS_FREE) && (ptTemp->iID == iID)){
            ptTemp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
            DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

            return ptTemp;
        }
        else
            ptTemp = ptTemp->pNext;
    }
    /* 2. ���ǰ�治�ɹ�, ȡ��һ�����еĲ���ptVideoMem->ePicState = PS_BLANK��videomem */
	ptTemp = g_ptVedioMemHead;
	while (ptTemp)
	{
		if ((ptTemp->eVideoMemState == VMS_FREE) && (ptTemp->ePicState == PS_BLANK))
		{
			ptTemp->iID = iID;
			ptTemp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
			return ptTemp;
		}
		ptTemp = ptTemp->pNext;
	}
    /*3.���ǰ�治�ɹ�: ȡ������һ�����е�videomem*/
    ptTemp = g_ptVedioMemHead;
    while(ptTemp){
        if(ptTemp->eVideoMemState == VMS_FREE){
            ptTemp->iID = iID;
            ptTemp->ePicState = PS_BLANK;
            ptTemp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
            DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

            return ptTemp;
        }
        else
            ptTemp = ptTemp->pNext;
    }

    /* 4. ���û�п��е�videomem����bCurΪ1, ��ȡ������һ��videomem(�������Ƿ����) */
    if (bCur)
    {
    	ptTemp = g_ptVedioMemHead;
    	ptTemp->iID = iID;
    	ptTemp->ePicState = PS_BLANK;
    	ptTemp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
    	return ptTemp;
    }
    return NULL;
}

/* ��õ�Ӳ�����Դ�, �ڸ��Դ��ϲ����Ϳ���ֱ����LCD����ʾ���� */
PT_VideoMem GetDevVideoMem(void)
{
	PT_VideoMem ptTmp = g_ptVedioMemHead;

	while (ptTmp)
	{
		if (ptTmp->bCevFrameBuffer)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->pNext;
	}
	return NULL;
}

/* ���Դ�ȫ����Ϊĳ����ɫ */
void ClearVideoMem(PT_VideoMem ptVideoMem, unsigned int dwColor)
{
	unsigned char *pucVM;
	unsigned short *pwVM16bpp;
	unsigned int *pdwVM32bpp;
	unsigned short wColor16bpp; /* 565 */
	int iRed;
	int iGreen;
	int iBlue;
	int i = 0;

	pucVM	   = ptVideoMem->tPixelDatas.aucPixelDatas;
	pwVM16bpp  = (unsigned short *)pucVM;
	pdwVM32bpp = (unsigned int *)pucVM;

    DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	switch (ptVideoMem->tPixelDatas.iBpp)
	{
		case 8:
		{
			memset(pucVM, dwColor, ptVideoMem->tPixelDatas.iTotalBytes);
			break;
		}
		case 16:
		{
			iRed   = (dwColor >> (16+3)) & 0x1f;
			iGreen = (dwColor >> (8+2)) & 0x3f;
			iBlue  = (dwColor >> 3) & 0x1f;
			wColor16bpp = (iRed << 11) | (iGreen << 5) | iBlue;
			while (i < ptVideoMem->tPixelDatas.iTotalBytes)
			{
				*pwVM16bpp	= wColor16bpp;
				pwVM16bpp++;
				i += 2;
			}
			break;
		}
		case 32:
		{
			while (i < ptVideoMem->tPixelDatas.iTotalBytes)
			{
				*pdwVM32bpp = dwColor;
				pdwVM32bpp++;
				i += 4;
			}
			break;
		}
		default :
		{
			DBG_PRINTF("can't support %d bpp\n", ptVideoMem->tPixelDatas.iBpp);
			return;
		}
	}

    DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

void PutVideoMem(PT_VideoMem ptVideoMem)
{
    ptVideoMem->eVideoMemState = VMS_FREE;
    if (ptVideoMem->iID == -1)
    {
        ptVideoMem->ePicState = PS_BLANK;
    }

}
/* ���Դ���ĳ����ȫ����Ϊĳ����ɫ */
void ClearVideoMemRegion(PT_VideoMem ptVideoMem, PT_Layout ptLayout, unsigned int dwColor)
{
	unsigned char *pucVM;
	unsigned short *pwVM16bpp;
	unsigned int *pdwVM32bpp;
	unsigned short wColor16bpp; /* 565 */
	int iRed;
	int iGreen;
	int iBlue;
	int iX;
	int iY;
    int iLineBytesClear;
    int i;

	pucVM	   = ptVideoMem->tPixelDatas.aucPixelDatas + ptLayout->iTopLeftY * ptVideoMem->tPixelDatas.iLineByteCnt+ ptLayout->iTopLeftX * ptVideoMem->tPixelDatas.iBpp / 8;
	pwVM16bpp  = (unsigned short *)pucVM;
	pdwVM32bpp = (unsigned int *)pucVM;

    iLineBytesClear = (ptLayout->iBotRightX - ptLayout->iTopLeftX + 1) * ptVideoMem->tPixelDatas.iBpp / 8;

	switch (ptVideoMem->tPixelDatas.iBpp)
	{
		case 8:
		{
            for (iY = ptLayout->iTopLeftY; iY <= ptLayout->iBotRightY; iY++)
            {
    			memset(pucVM, dwColor, iLineBytesClear);
                pucVM += ptVideoMem->tPixelDatas.iLineByteCnt;
            }
			break;
		}
		case 16:
		{
			iRed   = (dwColor >> (16+3)) & 0x1f;
			iGreen = (dwColor >> (8+2)) & 0x3f;
			iBlue  = (dwColor >> 3) & 0x1f;
			wColor16bpp = (iRed << 11) | (iGreen << 5) | iBlue;
            for (iY = ptLayout->iTopLeftY; iY <= ptLayout->iBotRightY; iY++)
            {
                i = 0;
                for (iX = ptLayout->iTopLeftX; iX <= ptLayout->iBotRightX; iX++)
    			{
    				pwVM16bpp[i++]	= wColor16bpp;
    			}
                pwVM16bpp = (unsigned short *)((unsigned int)pwVM16bpp + ptVideoMem->tPixelDatas.iLineByteCnt);
            }
			break;
		}
		case 32:
		{
            for (iY = ptLayout->iTopLeftY; iY <= ptLayout->iBotRightY; iY++)
            {
                i = 0;
                for (iX = ptLayout->iTopLeftX; iX <= ptLayout->iBotRightX; iX++)
    			{
    				pdwVM32bpp[i++]	= dwColor;
    			}
                pdwVM32bpp = (unsigned int *)((unsigned int)pdwVM32bpp + ptVideoMem->tPixelDatas.iLineByteCnt);
            }
			break;
		}
		default :
		{
			DBG_PRINTF("can't support %d bpp\n", ptVideoMem->tPixelDatas.iBpp);
			return;
		}
	}

}

int DisplayInit(void)
{
	int iError;

	iError = FBInit();

	return iError;
}

