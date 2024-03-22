/*********************************************************************************************************************
*
* @file             ldh_driver_esp01s.h
* @author           ������(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   RX					U4_TX
					TX					U4_RX
					RST					PF14
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ldh_driver_esp01s_H_
#define _ldh_driver_esp01s_H_

#include "usart.h"
#include "string.h"
#include "stdio.h"

#define wifiUsart 			huart4          // ESP01S��Ӧ�Ĵ���
#define wifiInterrupt       UART4		   	// ESP01S��Ӧ�Ĵ����ж�
#define ESP01S_GPIO_Port 	GPIOF			// ESP01S��λ����
#define ESP01S_Pin          GPIO_PIN_14
#define rx_rxBUFF_SIZE      512				//�����жϻ�������С

extern char ldh_message[];

uint8_t wifi_config_client(void);			// ESP01s���ͻ���
uint8_t wifi_config_server(void);			// ESP01s��������
void 	wifi_sendMessage(char *cmd);		// ESP01s������Ϣ
	
#endif
