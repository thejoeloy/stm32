/*
 * l298n.c
 *
 *  Created on: Feb 18, 2024
 *      Author: thejoey
 */
#include <main.h>
#include <stdio.h>
#include "l298n.h"

extern TIM_HandleTypeDef htim2;
# define SPEED_0 0
# define SPEED_1 25
# define SPEED_2 50
# define SPEED_3 75
# define SPEED_4 100
# define SPEED_5 125
# define SPEED_6 150
# define SPEED_7 175
# define SPEED_8 200
# define SPEED_9 255


void l298n_init() {
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void l298n_setMotor(char motor, char direction, char speed) {
	if (motor == '1' && direction == 'f') {
		// Spin Motor 1 clockwise
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);   // Set IN1 to HIGH
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Set IN2 to LOW
	} else if (motor == '1' && direction == 'b') {
		// Spin Motor 1 counterclockwise
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Set IN1 to LOW
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   // Set IN2 to HIGH
	} else if (motor == '2' && direction == 'f') {
		// Spin Motor 2 clockwise
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // Set IN3 to HIGH
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Set IN4 to LOW
	} else { // assumes motor == '2' && direction == 'b'
		// Spin Motor 2 counterclockwise
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // Set IN3 to LOW
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   // Set IN4 to HIGH
	}

	switch (speed) {
		case '0':
			printf("Zero\n");
			if (motor == '1') {
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_0);
			} else { // assume motor == '2'
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_0);
			}
	        break;
	    case '1':
	    	printf("One\n");
	    	if (motor == '1') {
	    					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_1);
	    				} else { // assume motor == '2'
	    					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_1);
	    				}
	        break;
	    case '2':
	        printf("Two\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_2);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_2);
	        			}
	        break;
	    case '3':
	        printf("Three\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_3);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_3);
	        			}
	        break;
	    case '4':
	        printf("Four\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_4);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_4);
	        			}
	        break;
	    case '5':
	        printf("Five\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_5);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_5);
	        			}
	        break;
	    case '6':
	        printf("Six\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_6);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_6);
	        			}
	        break;
	    case '7':
	        printf("Seven\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_7);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_7);
	        			}
	        break;
	    case '8':
	        printf("Eight\n");
	        if (motor == '1') {
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_8);
	        			} else { // assume motor == '2'
	        				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_8);
	        			}
	        break;
	    case '9':
	        printf("Nine\n");
	        if (motor == '1') {
	        	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SPEED_9);
	        } else { // assume motor == '2'
	        	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, SPEED_9);
	        }
	        break;
	    default:
	        printf("Invalid input: Not a digit\n");
	        if (motor == '1') {
	        	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	        } else { // assume motor == '2'
	        	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	        }
	}
}

