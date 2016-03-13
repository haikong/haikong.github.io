/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ����

 ******************************************************************************
  �� �� ��   : fonts_manager.c
  �� �� ��   : ����
  ��    ��   : haikong
  ��������   : 2014��11��3��
  ����޸�   :
  ��������   : �ļ������ȡ
  �����б�   :
              FontsInit
              GetFontBitmap
              GetFontOpr
              GetFontSize
              RegisterFontOpr
              SetFontsDetail
              SetFontSize
              ShowFontOpr
  �޸���ʷ   :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <config.h>
#include <fonts_manager.h>
#include <string.h>

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
 static PT_FontOpr g_ptFontOprHead = NULL;  //��������ͷ
 static int g_dwFontSize;                   //������

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/



/*****************************************************************************
 �� �� ��  : RegisterFontOpr
 ��������  : ע��"����ģ��", ��ν����ģ�����ȡ���ַ�λͼ�ķ���
 �������  : PT_FontOpr ptFontOpr  - һ���ṹ��,�ں�"ȡ���ַ�λͼ"�Ĳ�������
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�, ����ֵ - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int RegisterFontOpr(PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptTmp;

    if(!ptFontOpr)
        return -1;

	if (!g_ptFontOprHead)
	{
		g_ptFontOprHead   = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptFontOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext     = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	}

	return 0;
}

/*****************************************************************************
 �� �� ��  : ShowFontOpr
 ��������  : ��ʾ��������֧�ֵ�"����ģ��"
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
void ShowFontOpr(void)
{
	int i = 0;
	PT_FontOpr ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

/*****************************************************************************
 �� �� ��  : GetFontOpr
 ��������  : ��������ȡ��ָ����"����ģ��"
 �������  : char *pcName - ����
 �������  : ��
 �� �� ֵ  :NULL   - ʧ��,û��ָ����ģ��, ��NULL - ����ģ���PT_FontOpr�ṹ��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
PT_FontOpr GetFontOpr(char *pcName)
{
	PT_FontOpr ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
        DBG_PRINTF("ptTmp->name = %s,pcName = %s\n",ptTmp->name,pcName);
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

/*****************************************************************************
 �� �� ��  : SetFontSize
 ��������  : �����ַ��ĳߴ�(��λ:ɫ��)
 �������  : unsigned int dwFontSize - �ַ��ĳߴ�(��λ:ɫ��)
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
void SetFontSize(unsigned int dwFontSize)
{
	PT_FontOpr ptTmp = g_ptFontOprHead;

	g_dwFontSize = dwFontSize;

	while (ptTmp)
	{
		if (ptTmp->SetFontSize)
		{
			ptTmp->SetFontSize(dwFontSize);
		}
		ptTmp = ptTmp->ptNext;
	}
}

/*****************************************************************************
 �� �� ��  : GetFontSize
 ��������  : ����ַ��ĳߴ�(��λ:ɫ��)
 �������  : void
 �������  : ��
 �� �� ֵ  : �ַ��ĳߴ�(��λ:ɫ��)
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned int GetFontSize(void)
{
	return g_dwFontSize;
}

/*****************************************************************************
 �� �� ��  : GetFontBitmap
 ��������  : ����ַ���λͼ
 �������  : unsigned int dwCode            - �ַ��ı���ֵ(������ASCII/GBK/UNICODE)
 �������  : PT_FontBitMap ptFontBitMap     - �ں�λͼ��Ϣ
 �� �� ֵ  : 0  - �ɹ�,-1 - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int GetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int iError = 0;
	PT_FontOpr ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
		iError = ptTmp->GetFontBitmap(dwCode, ptFontBitMap);
		if (0 == iError)
		{
			return 0;
		}
		ptTmp = ptTmp->ptNext;
	}
	return -1;
}

/*****************************************************************************
 �� �� ��  : SetFontsDetail
 ��������  : ��������ģ�����Ϣ,����ָ���ֿ��ļ�,ָ���ַ��ߴ�
 �������  : char *pcFontsName          - Ҫ���õ�����ģ�������
             char *pcFontsFile          - �ֿ��ļ�
             unsigned int dwFontSize    - �ַ��ĳߴ�(��λ:ɫ��), ��������ʱ����SetFontSize�޸�
 �������  : ��
 �� �� ֵ  :0      - �ɹ�,����ֵ - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int SetFontsDetail(char *pcFontsName, char *pcFontsFile, unsigned int dwFontSize)
{
    int iError = 0;
    PT_FontOpr ptFontOpr;

    ptFontOpr = GetFontOpr(pcFontsName);
    if (NULL == ptFontOpr)
    {
        DBG_PRINTF("GetFontOpr failed.\n");
        return -1;
    }
    g_dwFontSize = dwFontSize;

    iError = ptFontOpr->FontInit(pcFontsFile, dwFontSize);

    return iError;

}

/*****************************************************************************
 �� �� ��  : FontsInit
 ��������  : ���ø�������ģ��ĳ�ʼ������
 �������  : void
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�, ����ֵ - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int FontsInit(void)
{
	int iError;

	iError = ASCIIInit();
	if (iError)
	{
		DBG_PRINTF("ASCIIInit error!\n");
		return -1;
	}

	iError = GBKInit();
	if (iError)
	{
		DBG_PRINTF("GBKInit error!\n");
		return -1;
	}

	iError = FreeTypeInit();
	if (iError)
	{
		DBG_PRINTF("FreeTypeInit error!\n");
		return -1;
	}

	return 0;
}

