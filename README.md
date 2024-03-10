# 蓝桥杯STC 15代码注解
**本仓库中所有代码皆来自淘宝的电子设计工坊，非营利目的，况且如果你喜欢这些代码的话我强烈建议去淘宝购买视频！**

## 1.系统的头文件system.h
system.h一共由四个主要部分构成，分别是**声明**、**设置数据类型**、**包含头函数**以及**创建共用体**。我们下面依次进行讲解：

**声明部分**：

    #ifdefine __SYSTEM_H_
    #define __SYSTEM_H_
	......
    #endif
这些都是声明头文件的套话，可以在STC15.h里习得，没有特别记忆的必要，以后的头文件讲解里会**默认不写**这部分。

**设置数据类型**：

    typedef char s8;
    typedef unsigned char u8;
    typedef int s16;
    typedef unsigned int u16;
    typedef unsigned long u32;
这里我们通过**typedef**这个函数，将我们平常用到的数据类型改变了名字，其中s与u代表有没有符号，而8与16更是直观的告诉了我们这个数的范围，比如u8的范围就是0~255，而255就是2^8-1。

**包含头函数**：

    #include <STC15F2K60S2.H>
    #include <intrins.h>
    ......
    #include "string.h"
这部分没有难度，但在真正coding的过程中不要疏忽大意忘记添加。其中包含的头文件第一个是系统的头文件，需要在STC-ISP的软件中添加，第二个头文件在书写Delay延迟函数时会用到，中间的头函数用到什么就写什么，没有什么难的，最后一个string.h在书写串口函数时会用到，如果没有用到串口函数时没有必要写。

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

## 延迟函数Delay
废话不多说我们先看代码：

  **Delay.h**:
  

    #ifndef __DELAY_H_
    #define __DELAY_H_（最后一次写套话部分）
    #include "system.h"
    void vDelay_Ms(u16 ms);
    void vDelay_Us(u16 us);
    void vDelay_10us(u16 us);
    void vDelay_100us(u16 us);
    #endif

就代码而言没什么难度，除了声明**需要外部调用**的函数外，要注意由于在调用该函数时参数值一般较大，所以用u16用作输入的数据类型！

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
