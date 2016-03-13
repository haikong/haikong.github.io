
#ifndef _DEBUG_MANAGER_H
#define _DEBUG_MANAGER_H

#define APP_EMERG       "<0>"
#define APP_ALERT       "<1>"
#define APP_CRIT        "<2>"
#define APP_ERR         "<3>"
#define APP_WARNING     "<4>"
#define APP_NOTICE      "<5>"
#define APP_INFO        "<6>"
#define APP_DEBUG       "<7>"

#define DEFAULT_DBG_LEVEL 4

typedef struct _DebugOpr{
    char    *name;
    int     isCanUse;
    int     (*DebugInit)(void);
    int     (*DebugExit)(void);
    int     (*DebugPrint)(char* strData);
    struct _DebugOpr *ptNext;
}T_DebugOpr,*PT_DebugOpr;

int RegisterDebugOpr(PT_DebugOpr ptDebugOpr);
void ShowDebugOpr(void);
PT_DebugOpr GetDebugOpr(char *pcName);
int SetDbgLevel(char* pcStrBuf);
int SetDbgChanel(char* pcStrBuf);
int Debugprint(const char* pcFormat,...);
int DebugInit(void);
int InitDebugChanel(void);
int StdoutInit(void);
int NetPrintInit(void);

#endif /* _DEBUG_MANAGER_H */

