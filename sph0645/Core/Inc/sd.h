/*
 * sd.h
 *
 *  Created on: Dec 8, 2024
 *      Author: babeler
 */

#ifndef INC_SD_H_
#define INC_SD_H_

#include <stdint.h>
#include <stdio.h>
#define WAV_WRITE_SAMPLE_COUNT 2048

void sd_card_init();
void start_recording(uint32_t frequency);
void write2wave_file(uint8_t *data, uint16_t data_size);
void stop_recording();

#endif /* INC_SD_H_ */
