/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ����

 ******************************************************************************
  �� �� ��   : encoding_manager.c
  �� �� ��   : ����
  ��    ��   : haikong
  ��������   : 2014��10��29��
  ����޸�   :
  ��������   : �����ļ���֧�ֱ�����ļ���ʽ���ļ������ϲ���ýӿڡ�
  �����б�   :
              AddFontOprForEncoding
              DelFontOprFrmEncoding
              Encode
              EncodingInit
              GetCodeFrmBuf
              RegisterEncodingOpr
              SelectEncodingOprForFile
              ShowEncodingOpr
  �޸���ʷ   :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <config.h>
#include <encoding_manager.h>
#include <string.h>
#include <stdlib.h>

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
    static PT_EncodingOpr g_ptEncodingOprHead;  //֧�ֱ����ʽ������ͷ

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
 �� �� ��  : RegisterEncodingOpr
 ��������  : ע��"����ģ��", һ�����ֿ��ܲ���ASCII����,Ҳ������UTF8�ȱ���.
             ��ô��һ��������ȡ��һ����,��ͬ�ı���ȡ����һ��,ÿһ�ֱ��뷽ʽ��
             ����RegisterEncodingOpr����ؽṹ�����g_ptEncodingOprHead����
 �������  : PT_EncodingOpr ptEncodingOpr
 �������  : ��
 �� �� ֵ  :0 - �ɹ�, ����ֵ - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	PT_EncodingOpr ptTmp;

	if (!g_ptEncodingOprHead)
	{
		g_ptEncodingOprHead   = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptEncodingOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	      = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	}

	return 0;
}

/*****************************************************************************
 �� �� ��  : ShowEncodingOpr
 ��������  : ��ʾ��������֧�ֵ�"���뷽ʽ"
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
void ShowEncodingOpr(void)
{
	int i = 0;
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

/*****************************************************************************
 �� �� ��  : SelectEncodingOprForFile
 ��������  : ���ļ�ѡ��һ������ģ��,��ע�����������һ����
 �������  : unsigned char *pucFileBufHead - �ļ�ͷ��, ������mmapӳ���ļ����õ��ڴ�ָ��
 �������  : ��
 �� �� ֵ  : NULL   - ���еı��뷽ʽ����֧�ִ��ļ�
             ��NULL - ֧�ִ��ļ��ı��뷽ʽ��Ӧ�Ľṹ��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead)
{
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

	while (ptTmp)
	{
		if (ptTmp->isSupport(pucFileBufHead))
			return ptTmp;
		else
			ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

/*****************************************************************************
 �� �� ��  : AddFontOprForEncoding
 ��������  : Ҫ��ʾһ���ַ�, �漰��������:
             1. �ַ��ı���ֵ, ������ļ���ʲô��ֵ����ʾ, ����'A'��0x41����ʾ,
                '��'�ı��뷽ʽ�ж���, GBK���뷽ʽ��������ֵΪ0xD0D6, ����UNICODE
                ������������ֵΪ0x4E2D
             2. ����ֵ�ı�ʾ��ʽ, ����UTF8/UTF16LE/UTF16BE��UNICODE��������ֱ�ʾ��ʽ,����:
                'A'��ASCII��ʾΪ0x41;UTF16-LE��ʾΪ0x41,0x00;UTF16-BE��ʾΪ0x00,0x41;UTF8��ʾΪ0x41
                '��'��GBK��ʾΪ0xD6,0xD0;UTF16-LE��ʾΪ0x2D,0x4E;UTF16-BE��ʾΪ0x4E,0x2D;UTF8��ʾΪ0xE4,0xB8,0xAD
                ע��:UTF8/UTF16LE/UTF16BEֻ��UNICODE����Ĳ�ͬ��ʾ��ʽ,����ͬһ���ַ�,���Ǳ��ı���ֵ��һ����,ֻ�Ǳ�ʾ������ͬ

 �������  : PT_EncodingOpr ptEncodingOpr
             PT_FontOpr ptFontOpr
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptFontOprCpy;

	if (!ptEncodingOpr || !ptFontOpr)
	{
		return -1;
	}
	else
	{
		ptFontOprCpy = malloc(sizeof(T_FontOpr));
		if (!ptFontOprCpy)
		{
			return -1;
		}
		else
		{
			memcpy(ptFontOprCpy, ptFontOpr, sizeof(T_FontOpr));
			ptFontOprCpy->ptNext = ptEncodingOpr->ptFontOprSupportedHead;
			ptEncodingOpr->ptFontOprSupportedHead = ptFontOprCpy;
			return 0;
		}
	}
}


/*****************************************************************************
 �� �� ��  : Encode
 ��������  : ��������ȡ��ָ����"����ģ��"
 �������  : char *pcName - ����
 �������  : ��
 �� �� ֵ  : NULL   - ʧ��,û��ָ����ģ��,
             ��NULL - "ҳ��ģ��"��PT_EncodingOpr�ṹ��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
PT_EncodingOpr Encode(char *pcName)
{
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

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

/*****************************************************************************
 �� �� ��  : DelFontOprFrmEncoding
 ��������  : AddFontOprForEncoding�ķ�����,��PT_EncodingOpr->ptFontOprSupportedHead�����а�ĳ��PT_FontOprȥ��
 �������  : PT_EncodingOpr ptEncodingOpr   - ���뷽ʽ
             PT_FontOpr ptFontOpr           - �ַ�����ṹ��,�ں�"GetFontBitmap"����(�����ݱ���ֵ���λͼ)
 �������  : ��
 �� �� ֵ  :0  - �ɹ�;-1 - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptTmp;
	PT_FontOpr ptPre;

	if (!ptEncodingOpr || !ptFontOpr)
	{
		return -1;
	}
	else
	{
		ptTmp = ptEncodingOpr->ptFontOprSupportedHead;
		if (strcmp(ptTmp->name, ptFontOpr->name) == 0)
		{
			/* ɾ��ͷ�ڵ� */
			ptEncodingOpr->ptFontOprSupportedHead = ptTmp->ptNext;
			free(ptTmp);
			return 0;
		}

		ptPre = ptEncodingOpr->ptFontOprSupportedHead;
		ptTmp = ptPre->ptNext;
		while (ptTmp)
		{
			if (strcmp(ptTmp->name, ptFontOpr->name) == 0)
			{
				/* ��������ȡ�����ͷ� */
				ptPre->ptNext = ptTmp->ptNext;
				free(ptTmp);
				return 0;
			}
			else
			{
				ptPre = ptTmp;
				ptTmp = ptTmp->ptNext;
			}
		}

		return -1;
	}
}

/*****************************************************************************
 �� �� ��  : GetCodeFrmBuf
 ��������  : ���ַ�����ȡ��UTF8��ʽ���ַ�����UNICODE����ֵ
 �������  : unsigned char *pucBufStart  - �ַ�����ʼλ��
             unsigned char *pucBufEnd    - �ַ�������λ��(���λ�õ��ַ�������)

 �������  : unsigned int *pdwCode       - ��ȡ���ı���ֵ��������
 �� �� ֵ  : ʹ���˶��ٸ��ַ��ŵõ��������ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��29��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode)
{
   return  Encode("utf-8")->GetCodeFrmBuf(pucBufStart, pucBufEnd, pdwCode);
}

/*****************************************************************************
 �� �� ��  : EncodingInit
 ��������  : ���ø��ֱ��뷽ʽ�ĳ�ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�, ����ֵ - ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��11��3��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
int EncodingInit(void)
{
	int iError;

	iError = AsciiEncodingInit();
	if (iError)
	{
		DBG_PRINTF("AsciiEncodingInit error!\n");
		return -1;
	}

	iError = Utf16leEncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf16leEncodingInit error!\n");
		return -1;
	}

	iError = Utf16beEncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf16beEncodingInit error!\n");
		return -1;
	}

	iError = Utf8EncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf8EncodingInit error!\n");
		return -1;
	}

	return 0;
}

