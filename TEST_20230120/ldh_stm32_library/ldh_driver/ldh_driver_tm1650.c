/*********************************************************************************************************************
*
* @file             ldh_driver_tm1650.c
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-17
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   SDA					PD14
*					SCL					PD15
*                   电源引脚
*                   VCC                 5V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#include "ldh_driver_tm1650.h"

/*******数码管显示0~9对应的值*******/
const uint8_t NUM[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void TM1650_Wr_RAM(uint8_t Address, uint8_t Data)
{
    IIC_Start();
    IIC_Send_Byte(Address);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

// 打开数码管显示
void open_display()
{
	// IIC GPIO初始化
	IIC_Init();
	TM1650_Wr_RAM(0x48, 0x71);
}

// 写入数字
void set_number(int num)
{
	TM1650_Wr_RAM(0x68, NUM[num / 1000]);
	// 加上小数点
	TM1650_Wr_RAM(0x6A, NUM[num % 1000 / 100] | 0X80);
	TM1650_Wr_RAM(0x6C, NUM[num % 100 / 10]);
	TM1650_Wr_RAM(0x6E, NUM[num % 10]);
}

//写入数字（char）
void set_number_ch(int num,char ch1,char ch2)
{
	TM1650_Wr_RAM(0x6E, NUM[ch2-48]);
	TM1650_Wr_RAM(0x6C, NUM[ch1-48]);
	TM1650_Wr_RAM(0x6A, 0x40);
	TM1650_Wr_RAM(0x68, NUM[num]);
}
