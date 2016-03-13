/******************************************************************************

                  版权所有 (C), 2001-2011, 海空

 ******************************************************************************
  文 件 名   : encoding_manager.c
  版 本 号   : 初稿
  作    者   : haikong
  生成日期   : 2014年10月29日
  最近修改   :
  功能描述   : 编码文件，支持编码的文件格式，文件编码上层调用接口。
  函数列表   :
              AddFontOprForEncoding
              DelFontOprFrmEncoding
              Encode
              EncodingInit
              GetCodeFrmBuf
              RegisterEncodingOpr
              SelectEncodingOprForFile
              ShowEncodingOpr
  修改历史   :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <config.h>
#include <encoding_manager.h>
#include <string.h>
#include <stdlib.h>

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
    static PT_EncodingOpr g_ptEncodingOprHead;  //支持编码格式的链表头

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


/*****************************************************************************
 函 数 名  : RegisterEncodingOpr
 功能描述  : 注册"编码模块", 一行文字可能采用ASCII编码,也可能用UTF8等编码.
             怎么从一行文字中取出一个字,不同的编码取法不一样,每一种编码方式都
             调用RegisterEncodingOpr把相关结构体放入g_ptEncodingOprHead链表。
 输入参数  : PT_EncodingOpr ptEncodingOpr
 输出参数  : 无
 返 回 值  :0 - 成功, 其他值 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : ShowEncodingOpr
 功能描述  : 显示本程序能支持的"编码方式"
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : SelectEncodingOprForFile
 功能描述  : 给文件选择一个编码模块,从注册的链表中逐一查找
 输入参数  : unsigned char *pucFileBufHead - 文件头部, 即调用mmap映射文件所得到内存指针
 输出参数  : 无
 返 回 值  : NULL   - 现有的编码方式都不支持此文件
             非NULL - 支持此文件的编码方式对应的结构体指针
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : AddFontOprForEncoding
 功能描述  : 要显示一个字符, 涉及三个概念:
             1. 字符的编码值, 即这个文件用什么数值来表示, 比如'A'用0x41来表示,
                '中'的编码方式有多种, GBK编码方式里它的数值为0xD0D6, 而在UNICODE
                编码里它的数值为0x4E2D
             2. 编码值的表示方式, 比如UTF8/UTF16LE/UTF16BE是UNICODE编码的三种表示方式,例如:
                'A'的ASCII表示为0x41;UTF16-LE表示为0x41,0x00;UTF16-BE表示为0x00,0x41;UTF8表示为0x41
                '中'的GBK表示为0xD6,0xD0;UTF16-LE表示为0x2D,0x4E;UTF16-BE表示为0x4E,0x2D;UTF8表示为0xE4,0xB8,0xAD
                注意:UTF8/UTF16LE/UTF16BE只是UNICODE编码的不同表示方式,对于同一个字符,它们表达的编码值是一样的,只是表示方法不同

 输入参数  : PT_EncodingOpr ptEncodingOpr
             PT_FontOpr ptFontOpr
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : Encode
 功能描述  : 根据名字取出指定的"编码模块"
 输入参数  : char *pcName - 名字
 输出参数  : 无
 返 回 值  : NULL   - 失败,没有指定的模块,
             非NULL - "页面模块"的PT_EncodingOpr结构体指针
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : DelFontOprFrmEncoding
 功能描述  : AddFontOprForEncoding的反函数,从PT_EncodingOpr->ptFontOprSupportedHead链表中把某个PT_FontOpr去掉
 输入参数  : PT_EncodingOpr ptEncodingOpr   - 编码方式
             PT_FontOpr ptFontOpr           - 字符处理结构体,内含"GetFontBitmap"函数(即根据编码值获得位图)
 输出参数  : 无
 返 回 值  :0  - 成功;-1 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
			/* 删除头节点 */
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
				/* 从链表里取出、释放 */
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
 函 数 名  : GetCodeFrmBuf
 功能描述  : 从字符串中取出UTF8格式的字符串的UNICODE编码值
 输入参数  : unsigned char *pucBufStart  - 字符串起始位置
             unsigned char *pucBufEnd    - 字符串结束位置(这个位置的字符不处理)

 输出参数  : unsigned int *pdwCode       - 所取出的编码值存在这里
 返 回 值  : 使用了多少个字符才得到这个编码值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年10月29日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode)
{
   return  Encode("utf-8")->GetCodeFrmBuf(pucBufStart, pucBufEnd, pdwCode);
}

/*****************************************************************************
 函 数 名  : EncodingInit
 功能描述  : 调用各种编码方式的初始化函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 0 - 成功, 其他值 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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

