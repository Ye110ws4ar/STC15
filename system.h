#ifndef __SYSTEM_H_
#define __SYSTEM_H_

typedef char s8;
typedef unsigned char u8;
typedef int s16;
typedef unsigned int  u16;
typedef unsigned long u32;

#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"
#include "Timer.h"
#include "SMG.h"
#include "BTN.h"
#include "KBD.h"
#include "onewire.h"
#include "iic.h"
#include "ds1302.h"
#include "NE555.h"
#include "Sonic.h"
#include "Uart.h" 
#include "string.h"

/* ��typedef����һ����bits�Ľṹ�塰�������͡��� �Ӷ������� bits b; ����bΪ�ṹ��ġ�������*/
typedef struct
{
	u8 b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
}bits;

/* ��typedef����һ����HexToBin�Ĺ����塰�������͡��� �Ӷ������� HexToBin led_ctrl; ����led_ctrlΪ������ġ�������*/
typedef union
{
	unsigned char hex;		
	bits b;
}HexToBin;


#endif