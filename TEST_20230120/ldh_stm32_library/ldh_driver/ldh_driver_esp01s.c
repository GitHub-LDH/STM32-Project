/*********************************************************************************************************************
*
* @file             ldh_driver_esp01s.c
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

#include "ldh_driver_esp01s.h"

/********做客服端需要做以下设置，做服务器则不需要设置********/
/********     注意ESP8266只能连接2.4GHz的移动热点	   ********/
unsigned char WIFI_NAME[] = "LDH";
unsigned char WIFI_MI[] = "LDH12345678";
unsigned char IP[] = "192.168.137.1";		// 需要根据电脑IPV4的IP进行修改
int PORT = 5000;

/* 中断接收所用变量 */
short wifi_count = 0;
char wifirx;
uint8_t rxBuffer;
uint8_t wifi_rxbf[rx_rxBUFF_SIZE];
unsigned char rxCount = 0;  		//计数器
unsigned char rxwifiCount = 0;  	//计数器
char rxBuff[rx_rxBUFF_SIZE] = {0};  //缓冲区

char serve[150] = {0};
char client[50] = {0};
int step=0,en=0;

char ldh_message[256];

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S 发送信息函数
// @param       *cmd   准备发送的信息
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void wifi_sendMessage(char *cmd)
{
	HAL_UART_Transmit(&wifiUsart,(unsigned char*)cmd,strlen(cmd),1000);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S指令写入函数	内部调用
// @param       cmd   		指令
// @param       timeout		100ms的倍数
// @return      uint8_t		0:正确    1:错误
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_sendCmd(char *cmd,int timeout)
{
	unsigned char send_cmd[rx_rxBUFF_SIZE] = {0};
	rxCount = 0;
	memset(send_cmd,0,rx_rxBUFF_SIZE);
	sprintf((char*)send_cmd,"%s\r\n",cmd);
	while(1) 
	{
		if(HAL_UART_Transmit(&wifiUsart,send_cmd,strlen((char*)send_cmd),1000)==HAL_OK)
		{
			while(timeout--)
			{
				HAL_Delay(100);
				if(strstr(rxBuff,"OK"))  //在字符串 haystack 中查找第一次出现字符串 needle 的位置
					break;
				printf("%d ",timeout);
			}
			printf("\r\n");
			if(timeout<=0) return 1;
			else return 0;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S指令写入函数	内部调用
// @param       cmd   		指令
// @param       timeout		100ms的倍数
// @return      uint8_t		0:正确    1:错误
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_send(char *cmd,int timeout)
{
	unsigned char send_cmd[rx_rxBUFF_SIZE] = {0};
	rxCount = 0;
	memset(send_cmd,0,rx_rxBUFF_SIZE);
	sprintf((char*)send_cmd,"%s\r\n",cmd);
	while(1) 
	{
		if(HAL_UART_Transmit(&wifiUsart,send_cmd,strlen((char*)send_cmd),1000)==HAL_OK)
		{
			while(timeout--)
			{
				HAL_Delay(100);
				if(strstr(rxBuff,"CONNECT"))  //在字符串 haystack 中查找第一次出现字符串 needle 的位置
					break;
				printf("%d ",timeout);
			}
			printf("\r\n");
			if(timeout<=0) return 1;
			else return 0;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S 复位函数  内部调用
// @param       timeout      复位循环次数
// @return      uint8_t      1或0 1为失败，0为成功 与while连用
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_reset(int timeout)
{
	HAL_GPIO_WritePin(ESP01S_GPIO_Port,ESP01S_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(ESP01S_GPIO_Port,ESP01S_Pin,GPIO_PIN_SET);

	while(timeout--)
	{
		HAL_Delay(100);
		if(strstr(rxBuff,"invalid")||strstr(rxBuff,"ready"))
			break;
		printf("%d ",timeout);
	}
	printf("\r\n");
	if(timeout<=0) return 1;
	else return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S作为客户机初始化函数
// @return      uint8_t      1或0 1为失败，0为成功 与while连用
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_config_client(void)
{
	memset(serve,0,150);
	memset(client,0,50);
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);

	if(wifi_reset(50))
	{
		printf("wifi 复位失败\r\n");
		return 1;	
	}
	else
	{
		printf("wifi 复位成功!!\r\n");
	}

	printf("(1) 模式配置中\r\n");
	if(wifi_sendCmd("AT+CWMODE=3\r\n",50))
	{
		printf("模式配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("模式配置成功!!\r\n");
	}
	
	printf("(2)复位配置中\r\n");
	if(wifi_sendCmd("AT+RST\r\n",50))
	{
		printf("复位配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("复位配置成功!!\r\n");
	}
	
	HAL_Delay(3000);
	printf("(3) 连接wifi中\r\n");
	printf("正在连接的wifi名=%s\r\n",WIFI_NAME);
	printf("正在连接的wifi密码= %s\r\n",WIFI_MI);
	
	sprintf(client,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_NAME,WIFI_MI);
	if(wifi_sendCmd(client,500))
	{
		printf("wifi连接失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("wifi连接成功!!\r\n");
	}
	
	printf("(4) 正在s_connection配置\r\n");
	if(wifi_sendCmd("AT+CIPMUX=0\r\n",50))
	{
		printf("s_connection配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("s_connection配置成功!!\r\n");
	}
	
	printf("(5) TCP建立中\r\n");
	sprintf(serve, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", IP, PORT);
								 

	printf("send=%s\r\n",serve);
	if(wifi_sendCmd(serve,50))
	{
		printf("TCP建立失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("TCP建立成功!!\r\n");
	}
	
	printf("(6) 正在OPEN_TI_MODE 配置\r\n");
	if(wifi_sendCmd("AT+CIPMODE=1\r\n",50))
	{
		printf("OPEN_TI_MODE 配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("OPEN_TI_MODE 配置成功!!\r\n");
	}
	
	printf("(7) 建立数据传输中\r\n");
	if(wifi_sendCmd("AT+CIPSEND\r\n",50))
	{
		printf("建立数据传输失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("建立数据传输成功 可以发送数据了!!\r\n");
		wifi_sendMessage("hello STM32!");
	}
	step = 7;			// 让建立数据传输成功时中断才会生效
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S作为服务器初始化函数
// @return      uint8_t      1或0 1为失败，0为成功 与while连用
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_config_server(void)
{
	memset(serve,0,150);
	memset(client,0,50);
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);
	
	if(wifi_reset(50))
	{
		printf("wifi 复位失败\r\n");
		printf("%s\r\n",&rxBuffer);
		return 1;	
	}
	else
	{
		printf("wifi 复位成功!!\r\n");
	}

	printf("(1) 模式配置中\r\n");
	if(wifi_sendCmd("AT+CWMODE=2",50))
	{
		printf("模式配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("模式配置成功!!\r\n");
	}
	
	printf("(2)复位配置中\r\n");
	if(wifi_sendCmd("AT+RST",50))
	{
		printf("复位配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("复位配置成功!!\r\n");
	}
	
	HAL_Delay(3000);
	printf("(3) 设置wifi中\r\n");
	printf("正在设置wifi热点\r\n");
	
	sprintf(client,"AT+CWSAP=\"LDH_ESP8266\",\"LDH12345678\",6,4");
	if(wifi_sendCmd(client,500))
	{
		printf("wifi设置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("wifi设置成功!!\r\n");
	}
	HAL_Delay(300);
	printf("(4) 正在s_connection配置\r\n");
	if(wifi_sendCmd("AT+CIPMUX=1",50))
	{
		printf("s_connection配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("s_connection配置成功!!\r\n");
	}
	HAL_Delay(1500);
	printf("(6) TCP建立中\r\n");
	sprintf(serve,"AT+CIPSERVER=1,5000");
	printf("send=%s\r\n",serve);
	
	if(wifi_sendCmd(serve,50))
	{
		printf("TCP建立失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("TCP建立成功!!\r\n");
	}
	step=7;
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S串口接收中断函数	 ESP01S接收到信息时触发
// @param       {UART_HandleTypeDef}   *huart
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);

	if(huart->Instance==wifiInterrupt)  //若是wifi中断
	{
		if(rxBuffer!=0) 
		{
			rxBuff[rxCount] = rxBuffer;
			rxCount++;
			if(step==7 && huart->Instance==wifiInterrupt && rxBuffer == '#')
				{
				// printf("1");
				memset(wifi_rxbf, 0, sizeof(wifi_rxbf));
				rxwifiCount=0;
				}
				else if(step==7 && huart->Instance==wifiInterrupt && rxBuffer=='$')
				{
				printf("%s",wifi_rxbf);
				rxwifiCount=0;
				}
				else if(step==7 && huart->Instance==wifiInterrupt )
				{
            wifi_rxbf[rxwifiCount] = rxBuffer;
            rxwifiCount++;
				}		
		}
	}	
//	else{
//			if (rxBuffer == '#')
//			{
//						  wifi_count =1;
//						  printf("找到#\r\n");
//				memset(wifi_rxbf, 0, sizeof(wifi_rxbf));
//			}
//			else if (rxBuffer == '$')
//			{
//						printf("找到$");
//					printf("%s\r\n", wifi_rxbf);
//						wifi_count = -1;
//			}
//			else if ((wifi_count > 0) && (wifi_count < rx_rxBUFF_SIZE))
//			{
//						  printf("%c",rxBuffer);
//				wifi_rxbf[wifi_count - 1] = rxBuffer;
//				wifi_count++;
//			}
//				}
		HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);
}
