/*********************************************************************************************************************
*
* @file             ldh_driver_esp01s.c
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

#include "ldh_driver_esp01s.h"

/********���ͷ�����Ҫ���������ã�������������Ҫ����********/
/********     ע��ESP8266ֻ������2.4GHz���ƶ��ȵ�	   ********/
unsigned char WIFI_NAME[] = "LDH";
unsigned char WIFI_MI[] = "LDH12345678";
unsigned char IP[] = "192.168.137.1";		// ��Ҫ���ݵ���IPV4��IP�����޸�
int PORT = 5000;

/* �жϽ������ñ��� */
short wifi_count = 0;
char wifirx;
uint8_t rxBuffer;
uint8_t wifi_rxbf[rx_rxBUFF_SIZE];
unsigned char rxCount = 0;  		//������
unsigned char rxwifiCount = 0;  	//������
char rxBuff[rx_rxBUFF_SIZE] = {0};  //������

char serve[150] = {0};
char client[50] = {0};
int step=0,en=0;

char ldh_message[256];

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S ������Ϣ����
// @param       *cmd   ׼�����͵���Ϣ
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void wifi_sendMessage(char *cmd)
{
	HAL_UART_Transmit(&wifiUsart,(unsigned char*)cmd,strlen(cmd),1000);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01Sָ��д�뺯��	�ڲ�����
// @param       cmd   		ָ��
// @param       timeout		100ms�ı���
// @return      uint8_t		0:��ȷ    1:����
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
				if(strstr(rxBuff,"OK"))  //���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle ��λ��
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
// @brief       ESP01Sָ��д�뺯��	�ڲ�����
// @param       cmd   		ָ��
// @param       timeout		100ms�ı���
// @return      uint8_t		0:��ȷ    1:����
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
				if(strstr(rxBuff,"CONNECT"))  //���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle ��λ��
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
// @brief       ESP01S ��λ����  �ڲ�����
// @param       timeout      ��λѭ������
// @return      uint8_t      1��0 1Ϊʧ�ܣ�0Ϊ�ɹ� ��while����
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
// @brief       ESP01S��Ϊ�ͻ�����ʼ������
// @return      uint8_t      1��0 1Ϊʧ�ܣ�0Ϊ�ɹ� ��while����
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_config_client(void)
{
	memset(serve,0,150);
	memset(client,0,50);
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);

	if(wifi_reset(50))
	{
		printf("wifi ��λʧ��\r\n");
		return 1;	
	}
	else
	{
		printf("wifi ��λ�ɹ�!!\r\n");
	}

	printf("(1) ģʽ������\r\n");
	if(wifi_sendCmd("AT+CWMODE=3\r\n",50))
	{
		printf("ģʽ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("ģʽ���óɹ�!!\r\n");
	}
	
	printf("(2)��λ������\r\n");
	if(wifi_sendCmd("AT+RST\r\n",50))
	{
		printf("��λ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("��λ���óɹ�!!\r\n");
	}
	
	HAL_Delay(3000);
	printf("(3) ����wifi��\r\n");
	printf("�������ӵ�wifi��=%s\r\n",WIFI_NAME);
	printf("�������ӵ�wifi����= %s\r\n",WIFI_MI);
	
	sprintf(client,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_NAME,WIFI_MI);
	if(wifi_sendCmd(client,500))
	{
		printf("wifi����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("wifi���ӳɹ�!!\r\n");
	}
	
	printf("(4) ����s_connection����\r\n");
	if(wifi_sendCmd("AT+CIPMUX=0\r\n",50))
	{
		printf("s_connection����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("s_connection���óɹ�!!\r\n");
	}
	
	printf("(5) TCP������\r\n");
	sprintf(serve, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", IP, PORT);
								 

	printf("send=%s\r\n",serve);
	if(wifi_sendCmd(serve,50))
	{
		printf("TCP����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("TCP�����ɹ�!!\r\n");
	}
	
	printf("(6) ����OPEN_TI_MODE ����\r\n");
	if(wifi_sendCmd("AT+CIPMODE=1\r\n",50))
	{
		printf("OPEN_TI_MODE ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("OPEN_TI_MODE ���óɹ�!!\r\n");
	}
	
	printf("(7) �������ݴ�����\r\n");
	if(wifi_sendCmd("AT+CIPSEND\r\n",50))
	{
		printf("�������ݴ���ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("�������ݴ���ɹ� ���Է���������!!\r\n");
		wifi_sendMessage("hello STM32!");
	}
	step = 7;			// �ý������ݴ���ɹ�ʱ�жϲŻ���Ч
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S��Ϊ��������ʼ������
// @return      uint8_t      1��0 1Ϊʧ�ܣ�0Ϊ�ɹ� ��while����
//-------------------------------------------------------------------------------------------------------------------
uint8_t wifi_config_server(void)
{
	memset(serve,0,150);
	memset(client,0,50);
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);
	
	if(wifi_reset(50))
	{
		printf("wifi ��λʧ��\r\n");
		printf("%s\r\n",&rxBuffer);
		return 1;	
	}
	else
	{
		printf("wifi ��λ�ɹ�!!\r\n");
	}

	printf("(1) ģʽ������\r\n");
	if(wifi_sendCmd("AT+CWMODE=2",50))
	{
		printf("ģʽ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("ģʽ���óɹ�!!\r\n");
	}
	
	printf("(2)��λ������\r\n");
	if(wifi_sendCmd("AT+RST",50))
	{
		printf("��λ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("��λ���óɹ�!!\r\n");
	}
	
	HAL_Delay(3000);
	printf("(3) ����wifi��\r\n");
	printf("��������wifi�ȵ�\r\n");
	
	sprintf(client,"AT+CWSAP=\"LDH_ESP8266\",\"LDH12345678\",6,4");
	if(wifi_sendCmd(client,500))
	{
		printf("wifi����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("wifi���óɹ�!!\r\n");
	}
	HAL_Delay(300);
	printf("(4) ����s_connection����\r\n");
	if(wifi_sendCmd("AT+CIPMUX=1",50))
	{
		printf("s_connection����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("s_connection���óɹ�!!\r\n");
	}
	HAL_Delay(1500);
	printf("(6) TCP������\r\n");
	sprintf(serve,"AT+CIPSERVER=1,5000");
	printf("send=%s\r\n",serve);
	
	if(wifi_sendCmd(serve,50))
	{
		printf("TCP����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("TCP�����ɹ�!!\r\n");
	}
	step=7;
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ESP01S���ڽ����жϺ���	 ESP01S���յ���Ϣʱ����
// @param       {UART_HandleTypeDef}   *huart
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);

	if(huart->Instance==wifiInterrupt)  //����wifi�ж�
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
//						  printf("�ҵ�#\r\n");
//				memset(wifi_rxbf, 0, sizeof(wifi_rxbf));
//			}
//			else if (rxBuffer == '$')
//			{
//						printf("�ҵ�$");
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
