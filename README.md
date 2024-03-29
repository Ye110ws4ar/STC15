


# 蓝桥杯STC 15代码注解
**本仓库中所有代码皆来自淘宝的电子设计工坊，非营利目的，况且如果你喜欢这些代码的话我强烈建议去淘宝购买视频！**

## 系统代码

### 1.系统的头文件system.h
system.h一共由四个主要部分构成，分别是**声明**、**设置数据类型**、**包含头函数**以及**创建共用体**。我们下面依次进行讲解：<br>

**声明部分**：

    #ifdefine __SYSTEM_H_
    #define __SYSTEM_H_
	......
    #endif
这些都是声明头文件的套话，可以在STC15.h里习得，没有特别记忆的必要，以后的头文件讲解里会**默认不写**这部分。<br>

**设置数据类型**：

    typedef char s8;
    typedef unsigned char u8;
    typedef int s16;
    typedef unsigned int u16;
    typedef unsigned long u32;
这里我们通过**typedef**这个函数，将我们平常用到的数据类型改变了名字，其中s与u代表有没有符号，而8与16更是直观的告诉了我们这个数的范围，比如u8的范围就是0~255，而255就是2^8-1。<br>

**包含头函数**：

    #include <STC15F2K60S2.H>
    #include <intrins.h>
    ......
    #include "string.h"
这部分没有难度，但在真正coding的过程中不要疏忽大意忘记添加。其中包含的头文件第一个是系统的头文件，需要在STC-ISP的软件中添加，第二个头文件在书写Delay延迟函数时会用到，中间的头函数用到什么就写什么，没有什么难的，最后一个string.h在书写串口函数时会用到，如果没有用到串口函数时没有必要写。<br>

**创建共用体**:

    typedef struct
    {
	    u8 b0:1;
	    u8 b1:1;
	    u8 b2:1;
	    u8 b3:1;
	    u8 b4:1;
	    u8 b5:1;
	    u8 b6:1;
	    u8 b7:1;
    }bits;
    
    typedef union
    {
	    unsigned char hex;
	    bits b;
    }HexToBin;
在这里我们利用了typedef函数创建了一个叫HexToBin的共用体，字面意思就是16进制转2进制，共用体的好处在于我们只要控制其中的struct b中b0到b7的量就可以对HexToBin中的hex控制，在赋值时可以整体赋值hex即可。这个数据类型在以后控制led与蜂鸣器继电器时使用较多。

### 2.延迟函数Delay
废话不多说我们先看代码：<br>

  **Delay.h**:
  

    #ifndef __DELAY_H_
    #define __DELAY_H_（最后一次写套话部分）
    #include "system.h"
    void vDelay_Ms(u16 ms);
    void vDelay_Us(u16 us);
    void vDelay_10us(u16 us);
    void vDelay_100us(u16 us);
    #endif

就代码而言没什么难度，除了声明**需要外部调用**的函数外，要注意由于在调用该函数时参数值一般较大，所以用u16用作输入的数据类型！<br>

**Delay.c**:（以1ms延时为例）

    #include "Delay.h"
    void vDelay_Ms(u16 ms)
    {
	    unsigned char i,j;
	    unsigned int k;
	    for(k=0;k<ms;k++)
	    {
		    i=12;
		    j=169;
		    do
		    {
			    while(--j);
		    }while(--i);
	    }
    }
   书写延迟函数我们不会也没有能力全部自己写，我们会用提供的STC-ISP其中的软件延时计算器，并在此基础上加上一个次数为k的循环。需要注意的是，k的数据类型也是u16，就是unsigned int，同时在用STC-ISP输入参数时系统频率为**12.0000MHz**。由于较长的延时函数会导致程序的堵塞，所以在以后几乎只有向EEPROM中写入数据时会用到。

### 3.定时器2的初始化函数：
**Timer.h**:

    #include "system.h"
    void vTimer2_Init();
    void vTimer2_Init_100us();
在头文件里我们通常用到两种规格的定时器函数，正常的都是1ms进行一次中断，而在下面的函数里是100us一次中断，是用在PWM调制里面控制输入保持高频的一个中断函数。我们重点需要关注定时器是如何配置初始化的。

**Timer.c**:

    #include "Timer.h"
    
    void vTimer2_Init(void)//12.000MHz
    {
	    AUXR &= 0xFB;//定时器时钟设为12T模式
	    T2L = 0x18;//设置定时初值
	    T2H = 0xFC;//设置定时初值
	    AUXR |= 0x10;//定时器2开始计时
	    IE2 |= 0x04;//开定时器2中断
	    EA = 1;//开启总中断
    }
    
    void vTimer2_Init_100us(void)
    {
	    AUXR |= 0x04;//定时器时钟设为1T模式
	    T2L = 0x50;
	    T2H = 0xFB;
	    AUXR |= 0x10;
	    IE2 |= 0x04;
	    EA = 1;
    }
这种底层层面初始化的配置重要的就是查阅知道各种特殊的寄存器的功能，下面我从STC15的手册中找来了相关控制位的图片，辅助理解：<br>
![AUXR辅助寄存器](https://github.com/Ye110ws4ar/STC15/blob/main/AUXR.jpg)<br>![IE](https://github.com/Ye110ws4ar/STC15/blob/main/IE.png)<br>![IE2](https://github.com/Ye110ws4ar/STC15/blob/main/IE2.jpg)
<br>那我们可以一行一行地看一看大体配置的思路是什么样的，首先AUXR &= 0xFB，那么我们就可以知道T2R=1，T2_C/非T=0，T2x12=1，那么就是说允许了定时器2的运行，且设置为定时器模式，而且不分频，就是12T模式。到这里相当于对定时器的最初工作模式进行了限定，再对T2L与T2H进行赋值后限定中断的时间，并且开启中断，实现定时中断的功能。<br>
当然我们在实际书写的时候并不会真的自己配置，我们会在STC-ISP中的定时器计算器输入参数就有对应的代码生成，需要注意的是在软件中生成的没有最后两行的开启定时器中断与开启总中断，需要在范例程序的**定时器配置：定时器2的16位自动重载**中找到对应代码复制粘贴加上去。同时注意选择12MHz的模式。

### 4.用串口进行通讯\*
串口通讯是省赛几乎不考，只有国赛才会考到的一个功能，所以我把它放在最后进行讲解。我们先介绍串口的基础知识：
<br>P30管脚是单片机的串口接收管脚，即电脑向单片机发送信息；P31管脚是单片机的串口发送管脚，即单片机向电脑发送信息。由此我们不难发现按键会影响串口的发送（尤其是最上面两行）。
<br>而串口的控制字中有两个很重要的标志位，分别是**TI**与**RI**。TI是**发送**中断请求标志位，在停止位开始发送时由内部硬件置位，即TI=1，响应中断后TI必须用软件清零。RI是**接收**中断请求标志位，串行接收到停止位的中间时刻由内部硬件置位，即RI=1，向CPU发中断申请，响应中断后RI必须由软件清零。这在书写串口中断函数有大用。
<br>于是我们有如下串口接收固定长度数据的思路：

 1. 在串口中断服务函数里测试串口接收是否成功：SBUF=SBUF
 2. 定义rx_buf为串口接收缓存数组，rx_cnt为接收计数变量，rx_flag为接收完成标志位
 3. 在串口标志位判断里，将SBUF的一个字节数据赋值给rx_buf数组，rx_buf[rx_cnt++]=SBUF
 4. 判断rx_cnt的值，如果接受到固定长度的数据则置位rx_flag
 5. 在while(1)里处理rx_buf数组
 <br>但是由于有可能用户没有使用正确长度的数据导致程序错误，所以我们考虑到电脑端发送的连续两个字节之间的间隔小于50ms，可以通过如下方式实现对以上bug的修复：<br>
 1. 在定时器2中断里对串口接收的空闲时间开始计时，如果到50ms，则清空rx_buf和rx_cnt
 2. 在串口接收服务函数里，重置串口接收空闲计时
 <br>在书写串口相关的函数时我们两次利用了STC-ISP，分别是波特率计算器（**12MHz 4800波特率 1T模式**）以及STC-ISP范例程序中**使用定时器1的16位重装载**
 而上诉范例程序我引用至此：[范例程序](%E5%AE%9A%E6%97%B6%E5%99%A81%E7%94%A8%E4%BD%9C%E4%B8%B2%E5%8F%A31%E7%9A%84%E6%B3%A2%E7%89%B9%E7%8E%87.c)
 接下来我们来看串口函数的头文件以及c文件怎么写：
 **Uart.h**：
 ```
 #include "system.h"
#define RXBUF_NUM 3
extern u8 rx_buf[RXBUF_NUM];
extern bit rx_flag;
void Uart_Init(void);
void SendData(u8 dat);
void SendString(char *s);
void vRxIdle_Process(void); 
 ```
 而主函数部分就可以细分为很多部分，首先是串口的初始化：
 ```
 #include "Uart.h"
 void Uart_Init()
 {
	 SCON = 0x50;
	 AUXR |= 0x40;
	 AUXR &= 0xFE;
	 TMOD &= 0x0F;
	 TL1 = 0x8F;
	 TH1 = 0xFD;
	 ET1 = 0;
	 TR1 = 1;
	 ES = 1;
	 EA = 1;//最后两行要自己加上
 }
 ```
然后是串口的中断函数，相较于范例程序有很多改动，但还是往简单改：
```
bit busy;
u8 rx_buf[RXBUF_NUM];
u8 rx_cnt=0;
bit rx_flag;
u8 rx_idletime=0;
void Uart() interrupt 4
{
	if(RI)
	{
	RI=0;
	rx_idletime=0;
	rx_buf[rx_cnt++]=SBUF;
	if(rx_cnt==3)
	{
		rx_cnt=0;
		rx_flag=1;
	}
	}
	if(TI)
	{
		TI=0;
		busy=0;
	}
} 
 ```
 接下来是放在Timer2的ISR里的用来消除bug的函数，由于其中用到了memset函数所以要在system.h中包含string.h，代码如下：
 ```
void vRxIdle_Process()
{
	rx_idletime++;
	if(rx_idletime>=50)
	{
		rx_idletime=0;
		rx_cnt=0;
		memset(rx_buf,'\0',sizeof(rx_buf));
	}
}
 ```
 然后分别是发送数据与字符串的函数：
 ```
 void SendData(u8 dat)
 {
	 while(busy);
	 busy=1;
	 SBUF=dat;
 }
 void SendString(char *s)
 {
	 while(*s)
	 {
		 SendData(*s++);
	 }
 }
 ```
 而在实际应用中，我们假设我们需要把串口接收到的数据第一个用于控制LED，第二个用于控制继电器，我们需要把如下函数放在while(1)里：
 ```
void vUartRx_Process()
{
	if(rx_flag)
	{
		rx_flag=0;
		vDevice_Ctrl(0x80,rx_buf[0]);
		vDevice_Ctrl(0x80,rx_buf[1]);
	}
} 
 ```
 且在初始化时不仅把串口初始化了，初始化后还要加上Delay50毫秒，并且不要忘了在中断服务函数加上`vRxIdle_Process()`实现消除bug。
 

## 外设代码
### 1. 对P27~P25以及P0赋值
**Device.h**:
		 

    #include "system.h"
    void vDevice_Ctrl(u8 p2data,u8 p0data);
**Device.c**:

    #include "Device.h"
    
    void vDevice_Ctrl(u8 p2data,u8 p0data)
    {
	    P0 = p0data;
	    P2 = P2 & 0x1f | p2data;
	    P2 &= 0x1f;
    }
整个Device中函数的书写和应用是要考虑到竞赛版中的译码器电路与驱动芯片，其中P2口中的P27~P25对译码器选通，译码器选通后控制P0使不同的驱动芯片输出不同的信号从而对外设进行操作，在这里我罗列一些常用的P2输入以及其控制的外设部分：
* 0x80打开LED控制
* 0xa0打开对于继电器和蜂鸣器的控制
* 0xc0打开数码管位选的控制
* 0xe0打开数码管段选的控制
<br>此外我也说一下不同的外设部分对于高低电平的要求以确定P0的输入：

* LED在低电平时亮，P0对应也为低电平
* 继电器与蜂鸣器也在低电平时工作，但由于U10芯片是取反操作，所以在P0中的P04与P06实则是高电平，且要注意P04控制继电器，P06控制蜂鸣器
* 在操作数码管时如果想要对应的部分亮，段选要为低电平，位选要为高电平，我们竞赛版是共阳的数码管

### 2.数码管的正确显示
**SMG.h**:
		 
    #include "system.h"
    
    extern u8 smg_buf[8];
    extern u8 code smg_code[];
    
    void vSMG_Display();
**SMG.c**:

    #include "SMG.h"
    
    u8 smg_buf[8];
    u8 code smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    void vSMG_Display()
    {
	    static u8 i=0;
	    vDevice_Ctrl(0xc0,0);//段选前关闭位选
	    vDevice_Ctrl(0xe0,~smg_buf[i]);//由于是码表是共阴的数码管所以要取反
	    vDevice_Ctrl(0xc0,0x01<<i);//开启位选
	    i=(i+1)%8;
    }
在这里我们要注意到头文件的两个数组在主函数里的数码管处理中都要引用到，所以要写为**全局变量**。在书写数码管显示的函数中，我们选择在段选前关闭位选的方式来消除所谓“鬼影”，而在段选时由于我们的码表的性质不同所以要取反，关于码表的获取我们可以在**实验箱4的驱动8个数码管**中找到。而开启位选的操作使用到了移位的手法，在平时书写时可以学习借鉴。
### 3.按键操作函数的书写
按键操作主要有两种写法，所以我在此分类：
#### 状态机法：
**BTN.h**:

    #include "system.h"
    extern u8 key_io;
    u8 ucBTN_Read_State(void);
**BTN.c**:

    #include "BTN.h"
    #define KEY_NO 0
    #define KEY_DOWN 1
    #define KEY_UP 2
    
    u8 key_io;
    
    u8 ucBTN_Read_State()
    {
	    static u8 key_state=0;
	    u8 key_return=0;
	    key_io=P3&0x0f;
	    
	    switch(key_state)
	    {
		    case KEY_NO:
			    if(key_io!=0x0f) key_state=KEY_DOWN;
			break;
			
			case KEY_DOWN:
				if(key_io!=0x0f)
				{
					key_state=KEY_UP;
					
					if(key_io==0x0e) key_return=7;
					if(key_io==0x0d) key_return=6;
					if(key_io==0x0b) key_return=5;
					if(key_io==0x07) key_return=4;
				}
				else
					key_state=KEY_NO;
			break;
			
			case KEY_UP:
				if(key_io==0x0f) key_state=KEY_NO;
			break;
	    }
	    return key_return;
    }
![状态机法思路图](https://github.com/Ye110ws4ar/STC15/blob/main/state%20machine.png)
<br>相较于原本的状态机法我做出了一些改进，通过定义key_io为全局变量让状态机的长按检测得到实现。而在编写整体的函数时只要遵守上面的思路图就能写出这么一个可以**消抖**，且存在**松手检测**的按键读取函数。
#### 三行按键法：
**BTN.h**:

    #include "system.h"
    extern u8 Trg_BTN;
    extern u8 Cont_BTN;
    void vBTN_Read_ThreeLine(void);
**BTN.c**:

    #include "BTN.h"
    u8 Trg_BTN;
    u8 Cont_BTN;
    void vBTN_Read_ThreeLine()
    {
	    u8 ReadData = P3^0xff;
	    Trg_BTN = ReadData&(ReadData^Cont_BTN);//^代表异或操作，相同为0，不同为1
	    Cont_BTN = ReadData;
    }
三行按键法的优势在于行数更少而且可以进行长按检测，并且我们知道按键在键盘中对应的控制的字节如下：<br>![单片机中矩阵键盘](https://github.com/Ye110ws4ar/STC15/blob/main/kbd.png)
<br>我们以按下S7的过程为例，理解这其中的实现过程：
* 无按下：ReadData=0，Trg=0，Cont=0
* P30=0：P3=0xfe，ReadData=0x01，Trg=0x01，Cont=0x01
* P30总是0：Trg=0，Cont=0x01
* P30变为1：Trg=0x00&(0x00^0x01)=0，Cont=0

如此我们给出一个按下不同按键对应的不同P3（三行按键法，状态机法则后四位取反）：
* S7→0x01
* S6→0x02
* S5→0x04
* S4→0x08

此外需要注意的是无论是状态机法还是三行按键法都要每**10ms**操作一次！

### 4.矩阵键盘的按键操作
大体的思路实际上与前文的四个按键操作相似，我在这里继续重复书写一下状态机法与三行按键法，并且给出矩阵键盘下不同按键对应的触发值：
#### 状态机法：
**KBD.h**:

    #include "system.h"
    extern u8 key_cont;
    u8 ucKBD_Read_State();
**KBD.c**:

    #include "KBD.h"
    u8 ucKBD_KeyIO_State()
    {
	    u8 key_io=0;
	    P3=0xf0;p4=0xff;
	    if(P44==0) key_io=4;
	    if(P42==0) key_io=8;
	    if(P35==0) key_io=12;
	    if(P34==0) key_io=16;
	    P3=0x0f;P40x00;
	    if(P33==0) key_io=key_io+0;
	    if(P32==0) key_io=key_io+1;
	    if(P31==0) key_io=key_io+2;
	    if(P30==0) key_io=key_io+3;
	    return key_io;
    }
    u8 key_cont;
    #define KEY_NO 0
    #define KEY_DOWN 1
    #define KEY_UP 2
    u8 ucKBD_Read_State()
    {
	    static u8 key_state=0;
	    u8 key_io=0,key_return=0;
	    key_cont=ucKBD_KeyIO_State();
	    key_io=ucKBD_KeyIO_State();
	    switch(key_state)
	    {
		    case KEY_NO:
			    if(key_io) key_state=KEY_DOWN;
			break;
			case KEY_DOWN:
				if(key_io)
				{
					key_state=KEY_UP;
					key_return=key_io;
				}
				else
					key_state=KEY_NO;
			break;
			case KEY_UP:
				if(key_io==0) key_state=KEY_NO;
			break;
	    }
	    return key_return;
    }
    
#### 三行按键法：
**KBD.h**:

    #include "system.h"
    extern u8 Trg_KBD;
    extern u8 Cont_KBD;
    void vKBD_Read_ThreeLine();
**KBD.c**:

    #include "KBD.h"
    u8 ucKBD_KeyIO_ThreeLine()
    {
	    u8 key_io=0xff;
	    P3=0xf0;P4=0xff;
	    if(P44==0) key_io=0x70;
	    if(P42==0) key_io=0xb0;
	    if(P35==0) key_io=0xd0;
	    if(P34==0) key_io=0xe0;
	    P3=0x0f;P4=0x00;
	    if(P33==0) key_io=key_io|0x07;
	    if(P32==0) key_io=key_io|0x0b;
	    if(P31==0) key_io=key_io|0x0d;
	    if(P30==0) key_io=key_io|0x0e;
	    return key_io;
    }
    u8 Trg_KBD;
    u8 Cont_KBD;
    void vKBD_Read_ThreeLine()
    {
	    u8 ReadData=ucKBD_KeyIO_ThreeLine()^0xff;
	    Trg_KBD=ReadData&(ReadData^Cont_KBD);
	    Cont_KBD=ReadData;
    }
 三行按键对应输出表：
 ![矩阵键盘三行按键法](https://github.com/Ye110ws4ar/STC15/blob/main/threeline.png)   
   
### 5.利用onewire协议里的DS18B20进行温度读取
从这一章节开始很多函数的书写需要借助比赛时官方提供的**底层驱动代码参考**进行改写，篇幅较长所以完整代码我会使用链接的方式提供。<br>
**onewire.h**:

    #include "system.h"
    float fRead_Temperature(void);
**onewire.c**:
[onewire.c完整](https://github.com/Ye110ws4ar/STC15/pull/1/files#diff-817bd94e15c99c552d6f8c3f48a02a113546a3eefff1dd7b8e2867fcfca70fb6)<br>重要的说在前面，因为官方提供的是1T模式下的代码，我们需要通过在`Delay_OneWire`函数里写上`t=t*12;`!!!有且只有这个芯片需要如此操作。
<br>借助提供的官方代码，在比赛时我们自己要书写的部分是DS18B20温度的最后读取，在书写之前我们要知道几个芯片的重要控制字：
* 0xcc使芯片跳过读取ROM
* 0x44使芯片启动温度转换
* 0xbe使芯片获取暂存器数据

<br>在此基础上我们读取温度的思路是：

 1. 初始化
 2. 跳过读取ROM
 3. 启动温度转换
 4. 初始化
 5. 跳过读取ROM
 6. 获取暂存器数据
 7. 表示温度（温度的分辨率是0.0625）

<br>于是我们有如下的代码：

    float fRead_Temperature()
    {
	    float temp_return;
	    u8 low,high;
	    init_ds18b20();
	    Write_DS18B20(0xcc);
	    Write_DS18B20(0x44);
	    init_ds18b20();
	    Write_DS18B20(0xcc);
	    Write_DS18B20(0xbe);
	    low=Read_DS18B20();
	    high=Read_DS18B20();
	    temp_return=(high<<8|low)*0.0625;
	    return temp_return;
    }

	
	
### 6.利用ds1302芯片进行时间读取与设置
[ds1302.c完整代码](https://github.com/Ye110ws4ar/STC15/pull/1/files#diff-5789f50b69f26a6e1cb12172824d308b0ea87c54e375291c61af183b10f76e52)
<br> ds1302是一个很厉害的芯片，可以在芯片资料里的table3找到读写各类参数的八进制数，我在此摘抄如下：
|类型  | 读 |写|
|--|--|--|
| 秒 | 81 |80|
| 分 | 83 |82|
| 时 | 85 |84|
| 日期 | 87 |86|
| 月 | 89 |88|
| 星期 | 8B |8A|
| 年 | 8D |8C|


我们不难看出读数据都是奇数，写数据都是偶数，此外比较有趣的是`时`的bit7位在低电平时是24h，高电平是12h。
<br>此外我们要知道的一点是ds1302用**BCD码**来存储数据，什么是BCD码呢，就是用**四位二进制数**表示**一位十进制数**，比如十进制的25转为BCD就是0010,0101，而二进制的0010,0101也就是十进制里的37，所以我们记住一个案例，即十进制（Dec）的25转化为BCD就变为了37，通过这个案例我们写下BCD与Dec相互转换的方程并写入头文件中：<br>
**ds1302.h**:

    #include "system.h"
    #define DecToBCD(dec) (dec/10*16)+(dec%10)
    #define BCDToDec(bcd) (bcd/16*10)+(bcd%16)
    unsigned char Read_Ds1302_Byte(unsigned char address);
    void vClock_Set(u8 hour,u8 minute,u8 second);
在书写头文件中的自己写的部分时我们还要知道一个知识点就是ds1302的写保护，默认情况下写保护是开启的，但是在设置时间时要暂时关闭，我们在头文件中如下书写：<br>
**ds1302.c**:

    void vClock_Set(u8 hour,u8 minute,u8 second)
    {
	    Write_Ds1302_Byte(0x8e,0x00);//关闭写保护
	    Write_Ds1302_Byte(0x80,DecToBCD(second));
	    Write_Ds1302_Byte(0x82,DecToBCD(minute));
	    Write_Ds1302_Byte(0x84,DecToBCD(hour));
	    Write_Ds1302_Byte(0x8e,0x80);//打开写保护
    }
而在进行时间读取时，我们进行如下书写：

    u8 hour,minute,second;
    void vDS1302_Process()
    {
	    second=BCDToDec(Read_Ds1302_Byte(0x81));
	    minute=BCDToDec(Read_Ds1302_Byte(0x83));
	    hour=BCDToDec(Read_Ds1302_Byte(0x85));
    }
### 7.利用IIC协议进行读写EEPROM与ADC、DAC
[iic.c完整代码](https://github.com/Ye110ws4ar/STC15/pull/1/files#diff-5d6dc31d1dc775ea5539ff7cf3b80adb7579a97d6d4b639043e0ceb53b3708aa)
#### EEPROM：
实现EEPROM读写的是芯片AT24C02，而iic中该器件的控制字的顺序如下：<br>1 0 1 0 A2 A1A0 R/W，其中A2到A0用于确定器件的地址，最后一位确定对器件执行的是读还是写，由于对于AT24C02芯片来说A2到A0都是0，所以0xa0关键字是对EEPROM进行写操作，0xa1关键字是对EEPROM进行读操作，再结合EEPROM的图示，即下图：
![写入EEPROM](https://github.com/Ye110ws4ar/STC15/blob/main/eepromwrite.drawio.png)
![读取EEPROM](https://github.com/Ye110ws4ar/STC15/blob/main/eepromread.drawio.png)
对于ack来说，0为有，1为无
<br>所以我们能结合上面的图片写出如下的EEPROM操作函数：<br>
**iic.h**:

    #include "system.h"
    void vWrite_EEPROM(u8 add,u8 dat)；
    u8 ucRead_EEPROM(u8 add);
**iic.c**:

    #include "iic.h"
    void vWrite_EEPROM(u8 add,u8 dat)
    {
	    IIC_Start();
	    IIC_SendByte(0xa0);
	    IIC_WaitAck();
	    IIC_SendByte(add);
	    IIC_WaitAck();
	    IIC_SendByte(dat);
	    IIC_WaitAck();
	    IIC_Stop();
	    vDelay_Ms(5);
    }
    u8 ucRead_EEPROM(u8 add)
    {
	    u8 dat;
	    IIC_Start();
	    IIC_SendByte(0xa1);
	    IIC_WaitAck();
	    dat=IIC_RecByte();
	    IIC_SendAck(1);
	    IIC_Stop();
	    return dat;
    }
关于写入EEPROM的地址范围则是0x00~0xff都可以，此外要注意在写入EEPROM时有独有的**延时5ms**。
#### ADC与DAC：
在单片机中进行AD与DA的是芯片PCF8591，与上文类似，该器件也有独特的地址，其为1001 000 R/非W，所以**写入0x90时表示向8591写入数据，写入0x91的时候表示从8591读数据**，而在进行ADC读取数字信号的时候又分为三个不同的端口，分别是**AIN0、AIN1、AIN3**，其中AIN1由一个**光敏电阻**控制的**分压电路**控制，在光线暗时AIN1口的电压较低，在光线亮的时候AIN1口的电压较高；AIN0在J3的18引脚上，可以输出信号；AIN3由一个**滑动变压器**控制，通过旋转控制不同的电压大小。
<br> 同时的，与EEPROM一样，在书写ADC与DAC的函数时我们也需要借助芯片手册里的图片，我在此简化如下：
![读取数据，即ADC](https://github.com/Ye110ws4ar/STC15/blob/main/ADC.png)
![写入数据，即DAC](https://github.com/Ye110ws4ar/STC15/blob/main/DAC.png)
<br>由此我们下面开始书写头文件与函数：<br>
**iic.h**:

    #include "system.h"
    u8 ucRead_ADC(u8 ctrl_byte);
    void vWrite_DAC(u8 dat);
**iic.c**:

    #include "iic.h"
    u8 ucRead_ADC(u8 ctrl_byte)
    {
	    u8 adc_val;
	    IIC_Start();
	    IIC_SendByte(0x90);
	    IIC_WaitAck();
	    IIC_SendByte(ctrl_byte);
	    IIC_WaitAck();
	    
	    IIC_Start();
	    IIC_SendByte(0x91);
	    IIC_WaitAck();
	    adc_val=IIC_RecByte();
	    IIC_SendAck(1);
	    IIC_Stop();
	    return adc_val;
    }
    void vWrite_DAC(u8 dat)
    {
	    IIC_Start();
	    IIC_SendByte(0x90);
	    IIC_WaitAck();
	    IIC_SendByte(0x40);
	    IIC_WaitAck();
	    IIC_SendByte(dat);
	    IIC_WaitAck();
	    IIC_Stop();
    }
在实际编程时，ADC常用且需要注意的点很多，首先是ADC时输入的ctrl_byte，在为0x40时通AIN0，在0x41时通AIN1，在0x43时通AIN3，此外由于进行一次ADC要耗时0.75ms，所以在运行时不能太频繁，我们需要利用定时器中断**10ms读取一次**。
<br>而在处理实际问题时，我们经常要对于不同的情况对读到的数据做调整，变换不同的范围，比如我们假设ch3为读到的ADC结果，则有：

    u16 ch3_volt;u8 ch3_0_99,ch3_1_5;
    ch3_volt=ch3*100/51;//加小数点后范围变为0~5.00
    ch3_0_99=ch3/2.57f;//变为0~99
    ch3_1_5=ch3/51.1f+1;//变为1~5
一些话说在最后，是关于如何记忆IIC相关的这四个函数的，比如写EEPROM，实际上就三件事，告诉芯片我要写入了，写个地址，写个数据，结束（别忘了延时5ms）；而读EEPROM就是告诉芯片我要读了，然后读走数据回复个no ack，除此之外每一小步完成都要等待ack；写8591，实际上就是DAC，也很简单，告诉芯片我要DAC，也就是写数据了，然后选择AIN0口输出，输入数据大小结束；最难的是ADC，明明是读数据却要先从写入数据开始，所以是先开始，然后告诉芯片我要写入了，然后写入一个想ADC的地址，之后重新开始，告诉芯片我要读数据了，读走数据后再回复个no ack，再结束。
### 8.NE555芯片进行频率测量
频率测量实际上很简单，没有听起来那么的难，实际上就是一个计时器的事情，计时器在一秒内读到了多少次电平变化就是多少频率了，而在NE555中我们用的是计时器0对P34进行计数，具体配置方法在手册中有如下图片：
![TMOD的位构成](https://github.com/Ye110ws4ar/STC15/blob/main/TMOD-1.jpg)
![关键位1](https://github.com/Ye110ws4ar/STC15/blob/main/TMOD-2.jpg)
![关键位2](https://github.com/Ye110ws4ar/STC15/blob/main/TMOD-3.jpg)
<br>根据需求，我们选择16位不可自动重装载模式的计数器0，所以TMOD应该初始化为0x05;同时我们已经分析过频率测量只要读取1s后计数器数值即可，所以我们写出如下代码：<br>
**NE555.h**:

    #include "system.h"
    extern u16 freq_ne555;
    void vCounter0_Init();
    void vNE555_Process();
**NE555.c**:

    #include "NE555.h"
    void vCounter0_Init()
    {
	    TMOD  |= 0x05;
	    TL0=0x00;
	    TH0=0x00;
	    TR0=1;
    }
    u16 cnt_ne555;
    u16 freq_ne555;
    void vNE555_Process()
    {
	    cnt_ne555++;
	    if(cnt_ne555>=1000)
	    {
		    cnt_ne555=0;
		    freq_ne555=(TH0<<8)|TL0;
		    TH0=0;
		    TL0=0;
	    }
    }
或者在处理实际问题时跟据不同的更新时长要求对上述函数进行修改。
### 9.利用超声波测量距离
我们先介绍一下用超声波测距的原理，即超声波在P10管脚产生一个40kHz的方波，在超声波模块接收到40kHz信号时，会在P11口产生一个低电平。
<br>由此我们可以得到书写函数的大体步骤：

 1. 跳线帽放在Sonic处
 2. P10产生40kHz方波，并开始计时
 3. P11读到低电平，结束计时，得到时间T（单位是s）
 4. 距离=时间乘以速度除以二
 
 <br>由此我们先书写出头函数：<br>
 **Sonic.h**:
 

    #ifndef __SONIC_H_
    #define __SONIC_H_
    #include "system.h"
    u16 uiDistance_Get();
    #endif
而后我们开始书写Sonic.c，它又分为如下部分：**发射方波**、**接收方波并计算**，而发射方波前还需要借助一个延时函数，由于是40kHz的方波我们可以算出我们需要一个12us的延时函数，所以我们书写如下：<br>
**Sonic.c的头文件声明**：

    #include "system.h"
**Sonic.c的延时函数**:

    void vDelay_12us()
    {
	    unsigned char i;
	    _nop_();
	    _nop_();
	    i=33;
	    while(--i);
    }
**Sonic.c的发射方波部分**：

    void vSend_Wave()
    {
	    u8 i;
	    for(i=0;i<8;i++)
	    {
		    P10=1;
		    vDelay_12us();
		    P10=0;
		    vDelay_12us();
	    }
    }
**Sonic.c的获取时间以及距离计算部分**：

    u16 uiDistance_Get()
    {
	    u16 dist;
	    vSend_Wave();
	    TH1=0;TL1=0;//清零定时器1
	    TR1=1;//开启定时器1
	    while(P11==1);//等待P11变成低电平
	    TR1=0;//关闭定时器1
	    dist=(TH1<<8|Tl1)*0.017;//计算距离，单位是cm
	    return dist;
    }
这样我们就收获了超声波函数的写法，最后可能要注意的就是在实际操作中不能太频繁，可以利用定时器每100ms操作一次。
