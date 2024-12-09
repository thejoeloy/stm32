/*
 * l298n.c
 *
 *  Created on: Dec 6, 2024
 *      Author: babeler
 */
#include <main.h>
#include <stdio.h>
#include "l298n.h"

extern TIM_HandleTypeDef htim2;

void l298n_init() {
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}
