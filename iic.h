#ifndef _IIC_H
#define _IIC_H
#include "system.h"

void vWrite_EEPROM(u8 add, u8 dat);
u8 ucRead_EEPROM(u8 add);
u8 ucRead_ADC(u8 ctrl_byte);
void vWrite_DAC(u8 dat);
#endif