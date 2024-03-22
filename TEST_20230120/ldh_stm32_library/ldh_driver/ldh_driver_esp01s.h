/*********************************************************************************************************************
*
* @file             ldh_driver_esp01s.h
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   RX					U4_TX
					TX					U4_RX
					RST					PF14
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ldh_driver_esp01s_H_
#define _ldh_driver_esp01s_H_

#include "usart.h"
#include "string.h"
#include "stdio.h"

#define wifiUsart 			huart4          // ESP01S对应的串口
#define wifiInterrupt       UART4		   	// ESP01S对应的串口中断
#define ESP01S_GPIO_Port 	GPIOF			// ESP01S复位引脚
#define ESP01S_Pin          GPIO_PIN_14
#define rx_rxBUFF_SIZE      512				//接收中断缓冲区大小

extern char ldh_message[];

uint8_t wifi_config_client(void);			// ESP01s做客户机
uint8_t wifi_config_server(void);			// ESP01s做服务器
void 	wifi_sendMessage(char *cmd);		// ESP01s发送信息
	
#endif
