/*********************************************************************************************************************
*
* @file             ldh_driver_dht11.h
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   DAT					PD3
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/
#ifndef __ONEWIRE_ldh_driver_dht11_H__
#define	__ONEWIRE_ldh_driver_dht11_H__

#include "stm32f4xx_hal.h"
#include "stdio.h"

/************************ DHT11 数据类型定义******************************/
typedef struct
{
	uint8_t  humi_high8bit;		//原始数据：湿度高8位
	uint8_t  humi_low8bit;	 	//原始数据：湿度低8位
	uint8_t  temp_high8bit;	 	//原始数据：温度高8位
	uint8_t  temp_low8bit;	 	//原始数据：温度高8位
	uint8_t  check_sum;	 	    //校验和
	float    humidity;        	//实际湿度
	float    temperature;     	//实际温度  
} DHT11_Data_TypeDef;

/***********************   DHT11 连接引脚定义  **************************/
#define DHT11_Dout_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()
#define DHT11_Dout_PORT                           GPIOD
#define DHT11_Dout_PIN                            GPIO_PIN_3

/***********************   DHT11 函数宏定义  ****************************/
#define DHT11_Dout_LOW()                          HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_RESET) 
#define DHT11_Dout_HIGH()                         HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_SET)
#define DHT11_Data_IN()	                          HAL_GPIO_ReadPin(DHT11_Dout_PORT,DHT11_Dout_PIN)

extern float ldh_temp;
extern float ldh_hum;

/* 函数声明 ------------------------------------------------------------------*/
void DHT11_Init( void );
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
uint8_t ldh_DHT11_Read(float *tem, float *hum);

#endif
