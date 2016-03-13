/******************************************************************************

                  版权所有 (C), 2001-2011, 海空

 ******************************************************************************
  文 件 名   : fonts_manager.c
  版 本 号   : 初稿
  作    者   : haikong
  生成日期   : 2014年11月3日
  最近修改   :
  功能描述   : 文件编码获取
  函数列表   :
              FontsInit
              GetFontBitmap
              GetFontOpr
              GetFontSize
              RegisterFontOpr
              SetFontsDetail
              SetFontSize
              ShowFontOpr
  修改历史   :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <config.h>
#include <fonts_manager.h>
#include <string.h>

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
 static PT_FontOpr g_ptFontOprHead = NULL;  //编码链表头
 static int g_dwFontSize;                   //字体宽度

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
 函 数 名  : RegisterFontOpr
 功能描述  : 注册"字体模块", 所谓字体模块就是取出字符位图的方法
 输入参数  : PT_FontOpr ptFontOpr  - 一个结构体,内含"取出字符位图"的操作函数
 输出参数  : 无
 返 回 值  : 0 - 成功, 其他值 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : ShowFontOpr
 功能描述  : 显示本程序能支持的"字体模块"
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : GetFontOpr
 功能描述  : 根据名字取出指定的"字体模块"
 输入参数  : char *pcName - 名字
 输出参数  : 无
 返 回 值  :NULL   - 失败,没有指定的模块, 非NULL - 字体模块的PT_FontOpr结构体指针
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : SetFontSize
 功能描述  : 设置字符的尺寸(单位:色素)
 输入参数  : unsigned int dwFontSize - 字符的尺寸(单位:色素)
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : GetFontSize
 功能描述  : 获得字符的尺寸(单位:色素)
 输入参数  : void
 输出参数  : 无
 返 回 值  : 字符的尺寸(单位:色素)
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
unsigned int GetFontSize(void)
{
	return g_dwFontSize;
}

/*****************************************************************************
 函 数 名  : GetFontBitmap
 功能描述  : 获得字符的位图
 输入参数  : unsigned int dwCode            - 字符的编码值(可能是ASCII/GBK/UNICODE)
 输出参数  : PT_FontBitMap ptFontBitMap     - 内含位图信息
 返 回 值  : 0  - 成功,-1 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : SetFontsDetail
 功能描述  : 设置字体模块的信息,比如指定字库文件,指定字符尺寸
 输入参数  : char *pcFontsName          - 要设置的字体模块的名字
             char *pcFontsFile          - 字库文件
             unsigned int dwFontSize    - 字符的尺寸(单位:色素), 程序运行时可用SetFontSize修改
 输出参数  : 无
 返 回 值  :0      - 成功,其他值 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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
 函 数 名  : FontsInit
 功能描述  : 调用各个字体模块的初始化函数
 输入参数  : void
 输出参数  : 无
 返 回 值  : 0 - 成功, 其他值 - 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年11月3日
    作    者   : haikong
    修改内容   : 新生成函数

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

