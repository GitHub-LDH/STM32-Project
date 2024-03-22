/*********************************************************************************************************************
*
* @file             ldh_driver_dht11
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

#include "ldh_driver_dht11.h"
#include "ldh_delay.h"

float ldh_temp = 0;
float ldh_hum = 0;

static int init_key = 1;	//初始化变量，让初始化函数只运行一次

/* 私有宏定义 ----------------------------------------------------------------*/
#define Delay_ms(x)   HAL_Delay(x)

/* 私有函数原形 --------------------------------------------------------------*/
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t DHT11_ReadByte(void);

//-------------------------------------------------------------------------------------------------------------------
// @brief       DHT11从初始化函数
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void DHT11_Init ( void )
{
	DHT11_Dout_GPIO_CLK_ENABLE();
	DHT11_Mode_Out_PP();
	DHT11_Dout_HIGH();  // 拉高GPIO
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       使DHT11-DATA引脚变为上拉输入模式 内部调用
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void DHT11_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* 串口外设功能GPIO配置 */
  GPIO_InitStruct.Pin   = DHT11_Dout_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_Dout_PORT, &GPIO_InitStruct);
	
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       使DHT11-DATA引脚变为推挽输出模式 内部调用
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void DHT11_Mode_Out_PP(void) //拉升，上升沿有效
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* 串口外设功能GPIO配置 */
	GPIO_InitStruct.Pin = DHT11_Dout_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11_Dout_PORT, &GPIO_InitStruct); 	 
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       从DHT11读取一个字节，MSB先行 内部调用
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static uint8_t DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	
	for(i=0;i<8;i++)    
	{	 
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(DHT11_Data_IN()==GPIO_PIN_RESET);

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		ldh_delay_us(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  

		if(DHT11_Data_IN()==GPIO_PIN_SET)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(DHT11_Data_IN()==GPIO_PIN_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;
}

//-------------------------------------------------------------------------------------------------------------------
//	函数功能: 一次完整的数据传输为40bit，高位先出
//  输入参数: DHT11_Data:DHT11数据类型
//	返 回 值: ERROR：  读取出错
//            SUCCESS：读取成功
//	说    明：8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
//-------------------------------------------------------------------------------------------------------------------
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
	uint8_t temp;
	uint16_t humi_temp;
  
	if(init_key)
	{
		DHT11_Init();
		init_key = 0;
	}
	
	/*输出模式*/
	DHT11_Mode_Out_PP();
	/*主机拉低*/
	DHT11_Dout_LOW();
	/*延时18ms*/
	ldh_delay_ms(18);

	/*总线拉高 主机延时30us*/
	DHT11_Dout_HIGH(); 

	ldh_delay_us(30);   //延时30us

	/*主机设为输入 判断从机响应信号*/ 
	DHT11_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(DHT11_Data_IN()==GPIO_PIN_RESET)     
	{
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(DHT11_Data_IN()==GPIO_PIN_RESET);
		
		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(DHT11_Data_IN()==GPIO_PIN_SET);

		/*开始接收数据*/   
		DHT11_Data->humi_high8bit= DHT11_ReadByte();
		DHT11_Data->humi_low8bit = DHT11_ReadByte();
		DHT11_Data->temp_high8bit= DHT11_ReadByte();
		DHT11_Data->temp_low8bit = DHT11_ReadByte();
		DHT11_Data->check_sum    = DHT11_ReadByte();
		printf("humi_high8bit  = %d\n",DHT11_Data->humi_high8bit);
		printf("humi_low8bit  = %d\n",DHT11_Data->humi_low8bit);
		printf("temp_high8bit  = %d\n",DHT11_Data->temp_high8bit);
		printf("temp_low8bit  = %d\n",DHT11_Data->temp_low8bit);
		
		/*读取结束，引脚改为输出模式*/
		DHT11_Mode_Out_PP();
		/*主机拉高*/
		DHT11_Dout_HIGH();
		
		/* 对数据进行处理 */
		humi_temp=DHT11_Data->humi_high8bit*100+DHT11_Data->humi_low8bit;
		DHT11_Data->humidity =(float)humi_temp/100;

		humi_temp=DHT11_Data->temp_high8bit*100+DHT11_Data->temp_low8bit;
		DHT11_Data->temperature=(float)humi_temp/100;    

		/*检查读取的数据是否正确*/
		temp = DHT11_Data->humi_high8bit + DHT11_Data->humi_low8bit + 
			   DHT11_Data->temp_high8bit+ DHT11_Data->temp_low8bit;
		printf("sum  = %d\n",DHT11_Data->check_sum);
		printf("temp = %d\n",temp);
		if(DHT11_Data->check_sum==temp)
		{
			return SUCCESS;
		}
		else 
		{
			printf("line: 184 数据检查错误！\n");
			return ERROR;
		}
	}	
	else
	{
		printf("line:191 从机无响应！\n");
		return ERROR;
	}	
}

//-------------------------------------------------------------------------------------------------------------------
//	函数功能: 一次完整的数据传输为40bit，高位先出
//  输入参数: DHT11_Data:DHT11数据类型
//	返 回 值: ERROR：  读取出错
//            SUCCESS：读取成功
//	说    明：8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
//-------------------------------------------------------------------------------------------------------------------
uint8_t ldh_DHT11_Read(float *tem, float *hum)
{  
	uint8_t temp;
	uint16_t humi_temp;
	DHT11_Data_TypeDef DHT11_Data;
	*tem = 0;	//初始化为0
	*hum = 0;	//初始化为0
	
	if(init_key)
	{
		DHT11_Init();
		init_key = 0;
	}
	/*输出模式*/
	DHT11_Mode_Out_PP();
	/*主机拉低*/
	DHT11_Dout_LOW();
	/*延时18ms*/
	ldh_delay_ms(18);

	/*总线拉高 主机延时30us*/
	DHT11_Dout_HIGH(); 

	ldh_delay_us(30);   //延时30us

	/*主机设为输入 判断从机响应信号*/ 
	DHT11_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(DHT11_Data_IN()==GPIO_PIN_RESET)     
	{
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(DHT11_Data_IN()==GPIO_PIN_RESET);
		
		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(DHT11_Data_IN()==GPIO_PIN_SET);

		/*开始接收数据*/   
		DHT11_Data.humi_high8bit= DHT11_ReadByte();
		DHT11_Data.humi_low8bit = DHT11_ReadByte();
		DHT11_Data.temp_high8bit= DHT11_ReadByte();
		DHT11_Data.temp_low8bit = DHT11_ReadByte();
		DHT11_Data.check_sum    = DHT11_ReadByte();
		
		/*读取结束，引脚改为输出模式*/
		DHT11_Mode_Out_PP();
		/*主机拉高*/
		DHT11_Dout_HIGH();
		
		/* 对数据进行处理 */
		humi_temp=DHT11_Data.humi_high8bit*100+DHT11_Data.humi_low8bit;
		DHT11_Data.humidity =(float)humi_temp/100;

		humi_temp=DHT11_Data.temp_high8bit*100+DHT11_Data.temp_low8bit;
		DHT11_Data.temperature=(float)humi_temp/100;    

		/*检查读取的数据是否正确*/
		temp = DHT11_Data.humi_high8bit + DHT11_Data.humi_low8bit + 
			   DHT11_Data.temp_high8bit+ DHT11_Data.temp_low8bit;
		if(DHT11_Data.check_sum==temp)
		{
			*tem = DHT11_Data.temperature;
			*hum = DHT11_Data.humidity;
			return SUCCESS;
		}
		else 
		{
			printf("line: 184 数据检查错误！\n");
			return ERROR;
		}
	}	
	else
	{
		printf("line:191 从机无响应！\n");
		return ERROR;
	}	
}
