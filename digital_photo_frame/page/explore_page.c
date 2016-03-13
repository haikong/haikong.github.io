#include <config.h>
#include <page_manager.h>

static void ExplorePageRun(void);
static int  ExplorePageGetInputEvent(PT_InputEvent inputevent);
static int  ExplorePagePrepare(void);


static T_PageAction g_tExplorePageAction = {
    .name           = "explore",
    .Run            = ExplorePageRun,
    .GetInputEvent  = ExplorePageGetInputEvent,
    .Prepare        = ExplorePagePrepare,
};

static void ExplorePageRun(void)
{
    /*��ʾҳ��*/

    /*����prepare�߳�*/

    /*����GetinputEvent��������¼�����������*/
    while(1){
        InputEvent = ExplorePageGetInputEvent();
        switch(InputEvent){
            case "����":{
                /*�ж��Ƿ��Ƕ���*/
                if(isTopLevel)
                    return 0;
                else{
                    /*��ʾ��һ��Ŀ¼*/
                }
                break;
            }
            case "ѡ��":{
               if(isSelectDir)
               {
                    /*��ʾ��һ��Ŀ¼*/
               }
               else
               {
                    /*���浱ǰҳ��*/
                    StorePage();
                    Page("browse")->Run();
                    /*�ָ�֮ǰҳ��*/
                    RestorePage();
               }
                break;
            }
            case "��ҳ":{
                /*��ʾ��һҳ*/
                break;
            }
            case "��ҳ":{
                /*��ʾ��һҳ*/
                break;
            }
            default:
                break;
        }

    }
}

int ExplorePageInit(void)
{
    return RegisterPageAction(&g_tExplorePageAction);
}

