/*********************************************************************************************************************
*
* @file             ldh_led_beep.c
* @author           Αυ¶¬»ª(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-02-04
********************************************************************************************************************/
#include "ldh_led_beep.h"

void turn_led(int n, int ms)
{
	int i;
	for(i=0;i<n;i++)
	{
		LED0_ON();
		ldh_delay_ms(ms);
		LED0_OFF();
		LED1_ON();
		ldh_delay_ms(ms);
		LED1_OFF();
		LED2_ON();
		ldh_delay_ms(ms);
		LED2_OFF();
	}
}
