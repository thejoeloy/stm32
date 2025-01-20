/*
 * audio_sd.c
 *
 *  Created on: Dec 12, 2024
 *      Author: babeler
 */
// WAV file format for integer data
// 0 - 3   -> "RIFF"                     	 {0x52, 0x49, 0x46, 0x46}
// 4 - 7   -> size of the file in bytes  	 {data_section size + 36}
// 8 - 11  -> File type header, "WAVE"   	 {0x57 ,0x41, 0x56, 0x45}
// 12 - 15 -> "fmt "				     	 {0x66, 0x6d, 0x74, 0x20}
// 16 - 19 -> Length of format data          16	{0x10, 0x00, 0x00, 0x00}
// 20 - 21 -> type of format, pcm is         1 {0x01 0x00}
// 22 - 23 -> number of channels             2 {0x02 0x00}
// 24 - 27 -> sample rate,                   32 kHz	{0x80, 0x7d, 0x00, 0x00}
// 28 - 31 -> sample rate x bps x channels   19200 {0x00, 0xf4, 0x01, 0x00 }
// 32 - 33 -> bps * channels                 4 {0x04, 0x00}
// 34 - 35 -> bits per sample				 16 {0x10, 0x00}
// 36 - 39 -> "data" 						 {0x64, 0x61, 0x74, 0x61}
// 40 - 43 -> size of the data section		 {data section size}
/*
// new code mono mode

#include "audio_sd.h"
#include "stdio.h"
#include "fatfs.h"
#include "stdint.h"
#include "string.h"

#define NUM_CHANNELS 1
#define NUM_DATA_BYTES_PER_SAMPLE 2

static uint8_t wav_file_header[44]={
	0x52, 0x49, 0x46, 0x46, // RIFF
	0xa4, 0xa9, 0x03, 0x00, // file size
	0x57 ,0x41, 0x56, 0x45, // File Type Header
	0x66, 0x6d, 0x74, 0x20, // fmt
	0x10, 0x00, 0x00, 0x00, // Subchunk1Size
	0x01, 0x00,				// type of format (16 bit PCM is 1)
	0x01, 0x00, 			// channels (Only 1 channel for 1 mic)
	0x00, 0x7d, 0x00, 0x00, // Sample Rate (32kHz)
	0x00, 0xfa, 0x00, 0x00, // Byte Rate (Sample rate * Channels * num_bytes)
	0x02, 0x00,				// Block Align (num_channels * num_bytes)
	0x10, 0x00,				// Bits Per Sample (NUM_BYTES * 8)
	0x64, 0x61, 0x74, 0x61, // data chunk id
	0x80, 0xa9, 0x03, 0x00  // Data Size
};


static FRESULT sd_result;
static FATFS sdCard;
static FIL wavFile;
static uint32_t wav_file_size;
static uint8_t first_time = 0;

void sd_card_init()
{
	//	mounting an sd card
	sd_result = f_mount(&sdCard,SDPath, 1);
	if(sd_result != 0)
	{
		printf("error in mounting an sd card: %d \n", sd_result);
		while(1);
	}
	else
	{
		printf("succeeded in mounting an sd card \n");
	}
}

void start_recording(uint32_t frequency)
{
	static char file_name[] = "w_000.wav";
	static uint8_t file_counter = 1;
	int file_number_digits = file_counter;
	uint32_t byte_rate = frequency * NUM_CHANNELS * NUM_DATA_BYTES_PER_SAMPLE;
	wav_file_header[24] = (uint8_t)frequency;
	wav_file_header[25] = (uint8_t)(frequency >> 8);
	wav_file_header[26] = (uint8_t)(frequency >> 16);
	wav_file_header[27] = (uint8_t)(frequency >> 24);
	wav_file_header[28] = (uint8_t)byte_rate;
	wav_file_header[29] = (uint8_t)(byte_rate >> 8);
	wav_file_header[30] = (uint8_t)(byte_rate >> 16);
	wav_file_header[31] = (uint8_t)(byte_rate >> 24);

	// defining a wave file name
	file_name[4] = file_number_digits%10 + 48;
	file_number_digits /= 10;
	file_name[3] = file_number_digits%10 + 48;
	file_number_digits /= 10;
	file_name[2] = file_number_digits%10 + 48;
	printf("file name %s \n", file_name);
	file_counter++;

	// creating a file
	sd_result = f_open(&wavFile ,file_name, FA_WRITE|FA_CREATE_ALWAYS);
	if(sd_result != 0)
	{
		printf("error in creating a file: %d \n", sd_result);
		while(1);
	}
	else
	{
		printf("succeeded in opening a file \n");
	}
	wav_file_size = 0;
}

void stop_recording()
{
	uint16_t temp_number;
	// updating data size sector
	wav_file_size -= 8;
	wav_file_header[4] = (uint8_t)wav_file_size;
	wav_file_header[5] = (uint8_t)(wav_file_size >> 8);
	wav_file_header[6] = (uint8_t)(wav_file_size >> 16);
	wav_file_header[7] = (uint8_t)(wav_file_size >> 24);
	wav_file_size -= 36;
	wav_file_header[40] = (uint8_t)wav_file_size;
	wav_file_header[41] = (uint8_t)(wav_file_size >> 8);
	wav_file_header[42] = (uint8_t)(wav_file_size >> 16);
	wav_file_header[43] = (uint8_t)(wav_file_size >> 24);

	// moving to the beginning of the file to update the file format
	f_lseek(&wavFile, 0);
	f_write(&wavFile,(void *)wav_file_header, sizeof(wav_file_header),(UINT*)&temp_number);
	if(sd_result != 0)
	{
		printf("error in updating the first sector: %d \n", sd_result);
		while(1);
	}
	f_close(&wavFile);
	first_time = 0;
	printf("closed the file \n");
}

void write2wav_file(uint8_t *data, uint16_t data_size)
{
    uint32_t temp_number;

    // Temporary buffer for mono data
    uint16_t mono_data_size = data_size / 2; // Mono will have half the size of stereo
    uint8_t mono_data[mono_data_size];

    // Convert stereo to mono by copying every odd index (left channel)
    for (uint16_t i = 0, j = 0; i < data_size; i += 4, j += 2)
    {
        // i: index of stereo data (16-bit PCM = 2 bytes per channel, 4 bytes per sample pair)
        // j: index of mono data
        mono_data[j] = data[i];       // Copy left channel - first byte
        mono_data[j + 1] = data[i + 1]; // Copy left channel - second byte
    }

    printf("Writing mono audio data\n");

    // Write mono data to the file
    sd_result = f_write(&wavFile, (void *)mono_data, mono_data_size, (UINT*)&temp_number);

    if (sd_result != 0)
    {
        printf("Error in writing to the file: %d \n", sd_result);
        while (1);
    }

    wav_file_size += mono_data_size; // Increment file size by mono data size
}
*/
// old code - stereo mode

#include "audio_sd.h"
#include "stdio.h"
#include "fatfs.h"
#include "stdint.h"
#include "string.h"
// WAV file format for integer data
// 0 - 3   -> "RIFF"                     	 {0x52, 0x49, 0x46, 0x46}
// 4 - 7   -> size of the file in bytes  	 {data_section size + 36}
// 8 - 11  -> File type header, "WAVE"   	 {0x57 ,0x41, 0x56, 0x45}
// 12 - 15 -> "fmt "				     	 {0x66, 0x6d, 0x74, 0x20}
// 16 - 19 -> Length of format data          16	{0x10, 0x00, 0x00, 0x00}
// 20 - 21 -> type of format, pcm is         1 {0x01 0x00}
// 22 - 23 -> number of channels             2 {0x02 0x00}
// 24 - 27 -> sample rate,                   32 kHz	{0x80, 0x7d, 0x00, 0x00}
// 28 - 31 -> sample rate x bps x channels   19200 {0x00, 0xf4, 0x01, 0x00 }
// 32 - 33 -> bps * channels                 4 {0x04, 0x00}
// 34 - 35 -> bits per sample				 16 {0x10, 0x00}
// 36 - 39 -> "data" 						 {0x64, 0x61, 0x74, 0x61}
// 40 - 43 -> size of the data section		 {data section size}


static uint8_t wav_file_header[44]={
	0x52, 0x49, 0x46, 0x46, // RIFF
	0xa4, 0xa9, 0x03, 0x00, // file size
	0x57 ,0x41, 0x56, 0x45, // File Type Header
	0x66, 0x6d, 0x74, 0x20, // fmt
	0x10, 0x00, 0x00, 0x00, // Subchunk1Size
	0x01, 0x00,				// type of format
	0x02, 0x00, 			// channels
	0x00, 0x7d, 0x00, 0x00, // Sample Rate
	0x00, 0xf4, 0x01, 0x00, // Byte Rate
	0x04, 0x00,				// Block Align
	0x10, 0x00,				// Bits Per Sample
	0x64, 0x61, 0x74, 0x61, // data chunk id
	0x80, 0xa9, 0x03, 0x00  // Data Size
};

static FRESULT sd_result;
static FATFS sdCard;
static FIL wavFile;
static uint32_t wav_file_size;
static uint8_t first_time = 0;

void sd_card_init()
{
	//	mounting an sd card
	sd_result = f_mount(&sdCard,SDPath, 1);
	if(sd_result != 0)
	{
		printf("error in mounting an sd card: %d \n", sd_result);
		while(1);
	}
	else
	{
		printf("succeded in mounting an sd card \n");
	}
}

void start_recording(uint32_t frequency)
{
	static char file_name[] = "w_000.wav";
	static uint8_t file_counter = 1;
	int file_number_digits = file_counter;
	uint32_t byte_rate = frequency * 2 * 2;
	wav_file_header[24] = (uint8_t)frequency;
	wav_file_header[25] = (uint8_t)(frequency >> 8);
	wav_file_header[26] = (uint8_t)(frequency >> 16);
	wav_file_header[27] = (uint8_t)(frequency >> 24);
	wav_file_header[28] = (uint8_t)byte_rate;
	wav_file_header[29] = (uint8_t)(byte_rate >> 8);
	wav_file_header[30] = (uint8_t)(byte_rate >> 16);
	wav_file_header[31] = (uint8_t)(byte_rate >> 24);

	// defining a wave file name
	file_name[4] = file_number_digits%10 + 48;
	file_number_digits /= 10;
	file_name[3] = file_number_digits%10 + 48;
	file_number_digits /= 10;
	file_name[2] = file_number_digits%10 + 48;
	printf("file name %s \n", file_name);
	file_counter++;

	// creating a file
	sd_result = f_open(&wavFile ,file_name, FA_WRITE|FA_CREATE_ALWAYS);
	if(sd_result != 0)
	{
		printf("error in creating a file: %d \n", sd_result);
		while(1);
	}
	else
	{
		printf("succeeded in opening a file \n");
	}
	wav_file_size = 0;
}

void stop_recording()
{
	uint16_t temp_number;
	// updating data size sector
	wav_file_size -= 8;
	wav_file_header[4] = (uint8_t)wav_file_size;
	wav_file_header[5] = (uint8_t)(wav_file_size >> 8);
	wav_file_header[6] = (uint8_t)(wav_file_size >> 16);
	wav_file_header[7] = (uint8_t)(wav_file_size >> 24);
	wav_file_size -= 36;
	wav_file_header[40] = (uint8_t)wav_file_size;
	wav_file_header[41] = (uint8_t)(wav_file_size >> 8);
	wav_file_header[42] = (uint8_t)(wav_file_size >> 16);
	wav_file_header[43] = (uint8_t)(wav_file_size >> 24);

	// moving to the beginning of the file to update the file format
	f_lseek(&wavFile, 0);
	f_write(&wavFile,(void *)wav_file_header, sizeof(wav_file_header),(UINT*)&temp_number);
	if(sd_result != 0)
	{
		printf("error in updating the first sector: %d \n", sd_result);
		while(1);
	}
	f_close(&wavFile);
	first_time = 0;
	printf("closed the file \n");
}

void write2wav_file(uint8_t *data, uint16_t data_size)
{
	uint32_t temp_number;
	printf("w\n");
	if(first_time == 0)
	{
		for(int i = 0; i < 44; i++)
		{
			*(data + i) = wav_file_header[i];
		}
		first_time = 1;
	}

	sd_result = f_write(&wavFile,(void *)data, data_size,(UINT*)&temp_number);

	if(sd_result != 0)
	{
		printf("error in writing to the file: %d \n", sd_result);
		while(1);
	}
	wav_file_size += data_size;
}


