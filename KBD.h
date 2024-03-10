#ifndef __KBD_H_
#define __KBD_H_
#include "system.h"

extern unsigned char Trg_KBD; 
extern unsigned char Cont_KBD; 

u8 ucKBD_Read_State();
void vKBD_Read_ThreeLine();

#endif