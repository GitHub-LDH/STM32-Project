/*********************************************************************************************************************
*
* @file             ldh_led_beep.h
* @author           Αυ¶¬»ª(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-02-04
********************************************************************************************************************/
#ifndef __ldh_led_beep_H_
#define __ldh_led_beep_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "ldh_delay.h"

#define LED0_ON() 	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_OFF() 	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#define LED1_ON() 	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF() 	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
#define LED2_ON() 	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2_OFF() 	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define BEEP_ON() 	HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)
#define BEEP_OFF() 	HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)

void turn_led(int n, int ms);
#endif
