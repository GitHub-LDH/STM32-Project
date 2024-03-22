/*********************************************************************************************************************
*
* @file             ldh_driver_sr04.c
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   Echo				PD0
					Trig				PD1
*                   电源引脚
*                   VCC                 5V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#include "ldh_driver_sr04.h"
#include "stdio.h"
#include "tim.h"

float ldh_distance = 0;
static int init_key = 1;	//初始化变量，让初始化函数只运行一次

//-------------------------------------------------------------------------------------------------------------------
// @brief       超声波引脚初始化 内部调用
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void SR04_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_PIN, GPIO_PIN_RESET);
	
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = Trig_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Trig_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PDPin PDPin PDPin */
	GPIO_InitStruct.Pin = Echo_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       启动超声波 内部调用
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void UltrasonicWave_StartMeasure(void)
{
	HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_PIN,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_PIN,GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       超声波采集数据函数
// @return      距离
//-------------------------------------------------------------------------------------------------------------------
float ldh_UltrasonicWave_Measure(void) //
{
	int time_out = 0;	//判断运行是否超时
	uint16_t time_node;		//超声波运行时间
	float distance; 
	
	if(init_key)
	{
		SR04_Init();
		init_key = 0;
	}
	UltrasonicWave_StartMeasure(); //启动超声波    			
	while(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_PIN) == 0)
	{
		time_out++;
		if(time_out>50000)
		{
			printf("运行超时！ line:85\n");
			return 0;
		}
	}
	__HAL_TIM_SET_COUNTER(&htim6,0);
	__HAL_TIM_ENABLE(&htim6); 
	
	time_out = 0;
	while(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_PIN)  == 1)
	{
		time_out++;
		if(time_out>100000)
		{
			printf("运行超时！ line:100\n");
			return 0;
		}
	}
	time_node=TIM6->CNT;	//获取定时器6的时间
	__HAL_TIM_DISABLE(&htim6);
	
	distance = time_node*17/100;		//此处单位转换为cm
	if(distance>0 && distance<500)
		return distance;
	else 
	{
		printf("距离结算错误！line:112\n");  	
		return 0;
	}
		
}
