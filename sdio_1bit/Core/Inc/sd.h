/*
 * sd.h
 *
 *  Created on: Dec 13, 2024
 *      Author: babeler
 */

#ifndef INC_SD_H_
#define INC_SD_H_

void sd_card_mount();
void sd_card_unmount();
void sd_card_read(const char* filename);
void sd_card_write(const char* filename, const char* data);

#endif /* INC_SD_H_ */
