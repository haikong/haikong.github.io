#ifndef HKSTRING_H_
#define HKSTRING_H_

#ifdef	__cplusplus
extern "C"{
#endif

void hkmemset(void* dest,char c,unsigned long size);
void hkmemcpy(void* dest,const void* src,unsigned long size);

#ifdef	__cplusplus
}
#endif

#endif
