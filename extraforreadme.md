


> Written with [StackEdit](https://stackedit.io/).
# 函数外的补充
**尽管我们（至少我）已经学会了编写所有的上层代码，包括头文件和函数声明，但是今天用的了EEPROM差点要存一个大于255的数还是给我吓出了冷汗，所以我们今天开一个新的专题来看一看一些不用代码实现的部分，比如PWM调制以及用EEPROM读取不同的数据类型。**
## 1.PWM调制
我们从一条题目开始切入：
**在P34管脚上输出PWM信号，要求频率是1kHz，占空比10%~90%可调**
占空比的改变可以通过按键操作，这里不加赘述，重要的给定一定的占空比，用PWM来实现频率是1kHz的调控，由于是1kHz，所以我们在书写定时器的头文件时，就要把原本1ms一次的中断改为100us一次，用STC-ISP生成后我们有：

    void vTimer2_Init_100us()
    {
	    AUXR &= 0xFB;
	    T2L=0x9C;
	    T2H=0xFF;
	    AUXR |= 0x10;
	    IE2 |= 0x04;
	    EA = 1;
    }
而后在主函数里要把如下结构的代码写入中断：

    u8 cnt_pwm=1;
    u8 pwm_duty=90;
    void vPWM_Process()
    {
	    cnt_pwm=cnt_pwm%10+1;//让cnt_pwm在1~10之间变化
	    if(cnt_pwm<=(pwm_duty/10))
		    P34=1;
		else
			P34=0;
    }
倘若我们把P34=1改成LED灯亮起或者数码管正常输出，P34=0改成LED熄灭或者数码管全部为0x00，则实现了对于亮度的把控，这也是PWM最多的用处。
## 2.用EEPROM读写各种类型的数据
**之前我们用EEPROM都是占用一个字节写一个u8类型的数，但是我们也会有特殊情况，有几种比较典型的如：u16，float，s8，s16（特指负数），字符串，面对这些类型的数据我们该如何用EEPROM读写呢？我们分门别类来看。**
### u16：
写入u16的数据思路就是拆成高八位和低八位，在读取时再将高位回到高位并加上低位即可，范例代码如下：

    u16 u16_write=1234;
    u16 u16_read=0;
    void vEEPROM_Process()
    {
	    u8 i=0;
	    vWrite_EEPROM(0x00,u16_write>>8);
	    vWrite_EEPROM(0x01,u16_write);
	    u16_read=(ucRead_EEPROM(0x00)<<8)+ucRead_EEPROM(0x01);
    }
### float:
写入float真是一件麻烦事，需要先把他*1000再/256并且由于数字较大用u16的数据类型短暂存储这个中间过程。在读取时稍有不慎又会写错，不多说我们直接看代码：

    float float_write=3.1415;
    float float_read=0;
    void vEEPROM_Process()
    {
	    vWrite_EEPROM(0x03,(u16)(float_write*1000)/256);
	    vWrite_EEPROM(0x04,(u16)(float_write*1000)%256);
	    float_read=(ucRead_EEPROM(0x03)*256+ucRead_EEPROM(0x04))/1000.0f;
    }
### s8、s16:
在处理这两个负数时，我们要知道计算机在储存负数时是用他的补码进行储存的，所以我们可以直接读取，但是我在这里插一嘴什么是补码，毕竟我自己也忘了，原码的反码+1就是补码，比如5是0000 0101，那么反码+1就是1111 1011，即-5就如此表示。知道这些后我们看代码：

    s8 minus_write=-23;
    s8 minus_read=0;
    s16 minus_s16_write=-1234;
    s16 minus_s16_read=0;
    void vEEPROM_Process()
    {
	    vWrite_EEPROM(0x06,minus_write);
	    minus_read=ucRead_EEPROM(0x06);
	    vWrite_EEPROM(0x08,minus_s16_write>>8);
	    vWrite_EEPROM(0x09,minus_s16_write);
	    minus_s16_read=(ucRead_EEPROM(0x08)<<*)+ucRead_EEPROM(0x09);
    }
### 字符串：
利用好循环就可以写出字符串在EEPROM中的读写：

    u8 str_write[11]={"hello world"};
    u8 str_read[11];
    void vEEPROM_Process()
    {
	    for(i=0;i<sizeof(str_write);i++)
	    {
		    vWrite_EEPROM(0x10+i,str_write[i]);
	    }
	    for(i=0;i<sizeof(str_write);i++)
	    {
		    str_read[i]=ucRead_EEPROM(0x10+i);
	    }
    }
