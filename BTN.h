#ifndef __BTN_H_
#define __BTN_H_
#include "system.h"

extern unsigned char Trg_BTN;
extern unsigned char Cont_BTN;

u8 ucBTN_Read_State(void);
void vBTN_Read_ThreeLine(void);
#endif