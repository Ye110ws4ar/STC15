#ifndef __UART_H_
#define __UART_H_

#include "system.h"

#define RXBUF_NUM 3
extern u8 rx_buf[RXBUF_NUM];
extern bit rx_flag;

void Uart_Init(void);
void SendData(u8 dat);
void SendString(char *s);
void vRxIdle_Process(void);
#endif