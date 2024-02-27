/*
 * l298n.h
 *
 *  Created on: Feb 18, 2024
 *      Author: thejoey
 */

#ifndef INC_L298N_H_
#define INC_L298N_H_

void l298n_init();
void l298n_setMotor(char motor, char direction, char speed);

#endif /* INC_L298N_H_ */
