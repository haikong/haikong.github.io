#include <config.h>
#include <debug_manager.h>
#include <string.h>
#include <stdarg.h>

static PT_DebugOpr g_ptDebugOprHead;
static int g_iDbgLevelLimit = 8;

int RegisterDebugOpr(PT_DebugOpr ptDebugOpr)
{
	PT_DebugOpr ptTmp;

	if (!g_ptDebugOprHead)
	{
		g_ptDebugOprHead   = ptDebugOpr;
		ptDebugOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptDebugOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptDebugOpr;
		ptDebugOpr->ptNext = NULL;
	}

	return 0;
}


void ShowDebugOpr(void)
{
	int i = 0;
	PT_DebugOpr ptTmp = g_ptDebugOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

PT_DebugOpr GetDebugOpr(char *pcName)
{
	PT_DebugOpr ptTmp = g_ptDebugOprHead;

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


/*strBuf = "dbglevel = <0-7>"*/
int SetDbgLevel(char* pcStrBuf)
{
    g_iDbgLevelLimit = pcStrBuf[9] - '0';
    return 0;
}

/*
*stdout = 0 ....关闭stdout打印
*stdout = 1 ....打开stdout打印
*netprint = 0 ....关闭netprint打印
*netprint = 1 ....打开netprint打印
*/

int SetDbgChanel(char* pcStrBuf)
{
    char *pStrTmp;
    char strName[100];
    PT_DebugOpr ptDbgOprTmp;
    pStrTmp = strchr(pcStrBuf,'=');
    if(!pStrTmp)
        return -1;
    else
    {
        strncpy(strName,pcStrBuf,pStrTmp - pcStrBuf);
        strName[pStrTmp - pcStrBuf] = '\0';
        ptDbgOprTmp = GetDebugOpr(strName);
        if(!ptDbgOprTmp)
            return -1;
        if(pStrTmp[1] == '0')
            ptDbgOprTmp->isCanUse  = 0;
        else if(pStrTmp[1] == '1')
            ptDbgOprTmp->isCanUse  = 1;
    }
    return 0;
}

int Debugprint(const char* pcFormat,...)
{
    char strTempBuf[1000];
    char* pcTemp = strTempBuf;
    PT_DebugOpr ptDbgOprTmp = g_ptDebugOprHead;
    va_list tArgs;
    int iNum,iDbgLevel= DEFAULT_DBG_LEVEL;
    va_start(tArgs,pcFormat);
    iNum = vsprintf(strTempBuf,pcFormat,tArgs);
    va_end(tArgs);
    strTempBuf[iNum] = '\0';
    /*根据打印级别决定是否打印*/
    if((strTempBuf[0]== '<') && (strTempBuf[2]== '>'))
    {
        iDbgLevel = strTempBuf[1] - '0';
        if(iDbgLevel >= 0 && iDbgLevel<= 9)
        {
            pcTemp += 3;
        }
        else
        {
            iDbgLevel = DEFAULT_DBG_LEVEL;
        }
    }

    if(iDbgLevel > g_iDbgLevelLimit)
    {
        return -1;
    }
    /*调用链表中的所有isUseCan为1的结构体dbgprint*/
	while (ptDbgOprTmp)
	{
		if(1 == ptDbgOprTmp->isCanUse)
            ptDbgOprTmp->DebugPrint(pcTemp);
		ptDbgOprTmp = ptDbgOprTmp->ptNext;
	}

    return 0;
}

int DebugInit(void)
{
	int iError;
    iError  = StdoutInit();
    iError |= NetPrintInit();
	return iError;
}


int InitDebugChanel(void)
{
	PT_DebugOpr ptTmp = g_ptDebugOprHead;

	while (ptTmp)
	{
 		if (ptTmp->isCanUse && ptTmp->DebugInit)
 		{
 			ptTmp->DebugInit();
 		}
		ptTmp = ptTmp->ptNext;
	}
    return 0;

}
