#include <hkstdio.h>
#include <hkstring.h>

void hkmemset(void* dest,char c,unsigned long size)
{
    int i;
    char *ptemp = (char*)dest;
    if((dest != NULL) || (size != 0)){
        for(i = 0;i < size;i++)
            *(ptemp + i) = c;
    }
}

/*可考虑字长问题*/
void hkmemcpy(void* dest,const void* src,unsigned long size)
{
    int i;
    char* dtemp = (char*)dest;
    char* stemp = (char*)src;
    if((dest == NULL) || (src == NULL))
        return;
    for(i = 0;i < size;i++)
        dtemp[i] = stemp[i];
}


