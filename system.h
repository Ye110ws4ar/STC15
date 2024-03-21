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

/* 用typedef定义一个叫bits的结构体“数据类型”， 从而可以用 bits b; 定义b为结构体的“变量”*/
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

/* 用typedef定义一个叫HexToBin的共用体“数据类型”， 从而可以用 HexToBin led_ctrl; 定义led_ctrl为共用体的“变量”*/
typedef union
{
	unsigned char hex;		
	bits b;
}HexToBin;


#endif