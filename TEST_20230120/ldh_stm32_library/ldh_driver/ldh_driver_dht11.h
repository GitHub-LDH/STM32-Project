/*********************************************************************************************************************
*
* @file             ldh_driver_dht11.h
* @author           ������(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   DAT					PD3
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/
#ifndef __ONEWIRE_ldh_driver_dht11_H__
#define	__ONEWIRE_ldh_driver_dht11_H__

#include "stm32f4xx_hal.h"
#include "stdio.h"

/************************ DHT11 �������Ͷ���******************************/
typedef struct
{
	uint8_t  humi_high8bit;		//ԭʼ���ݣ�ʪ�ȸ�8λ
	uint8_t  humi_low8bit;	 	//ԭʼ���ݣ�ʪ�ȵ�8λ
	uint8_t  temp_high8bit;	 	//ԭʼ���ݣ��¶ȸ�8λ
	uint8_t  temp_low8bit;	 	//ԭʼ���ݣ��¶ȸ�8λ
	uint8_t  check_sum;	 	    //У���
	float    humidity;        	//ʵ��ʪ��
	float    temperature;     	//ʵ���¶�  
} DHT11_Data_TypeDef;

/***********************   DHT11 �������Ŷ���  **************************/
#define DHT11_Dout_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()
#define DHT11_Dout_PORT                           GPIOD
#define DHT11_Dout_PIN                            GPIO_PIN_3

/***********************   DHT11 �����궨��  ****************************/
#define DHT11_Dout_LOW()                          HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_RESET) 
#define DHT11_Dout_HIGH()                         HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_SET)
#define DHT11_Data_IN()	                          HAL_GPIO_ReadPin(DHT11_Dout_PORT,DHT11_Dout_PIN)

extern float ldh_temp;
extern float ldh_hum;

/* �������� ------------------------------------------------------------------*/
void DHT11_Init( void );
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
uint8_t ldh_DHT11_Read(float *tem, float *hum);

#endif
