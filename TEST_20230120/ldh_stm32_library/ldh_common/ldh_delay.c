/*********************************************************************************************************************
*
* @file             ldh_delay.c
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-17
********************************************************************************************************************/

#include "ldh_delay.h"
#include "time.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		基于time6的微秒延时函数
// @param		us				需要延时的微秒次数
// @return		void
// Sample usage:				ldh_delay_us(10); //延时10us
//-------------------------------------------------------------------------------------------------------------------
void ldh_delay_us(uint16_t us)
{
	uint16_t counter = 0xffff - (us/10) - 5;

	HAL_TIM_Base_Start(&DELAY_TIME);
	__HAL_TIM_SetCounter(&DELAY_TIME, counter);

	while (counter < 0xffff - 5)
	{
		counter = __HAL_TIM_GetCounter(&DELAY_TIME);
	}

	HAL_TIM_Base_Stop(&DELAY_TIME);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		基于time6的毫秒延时函数
// @param		us				需要延时的微秒次数
// @return		void
// Sample usage:				ldh_delay_ms(10); //延时10ms
//-------------------------------------------------------------------------------------------------------------------
void ldh_delay_ms(uint16_t ms)
{
	while (ms--)
		ldh_delay_us(1000);
}
