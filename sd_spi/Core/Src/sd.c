/*
 * sd.c
 *
 *  Created on: Dec 14, 2024
 *      Author: babeler
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "fatfs.h"
#include "sd.h"

static FRESULT sd_result;
static FATFS sdCard;
static FIL sdFile;

void sd_card_mount()
{
	sd_result = f_mount(&sdCard, "", 1);
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

void sd_card_unmount()
{
	f_mount(NULL, "", 0);
}

void sd_card_read(const char* filename)
{
    BYTE readBuf[128]; // Adjust buffer size as needed

    // Open the file for reading
    sd_result = f_open(&sdFile, filename, FA_READ);
    if (sd_result != FR_OK)
    {
        printf("Error opening file '%s' for reading: %d\n", filename, sd_result);
        return;
    }

    printf("Successfully opened file '%s' for reading\n", filename);

    // Read data from the file
    TCHAR* rres = f_gets((TCHAR*)readBuf, sizeof(readBuf), &sdFile);
    if (rres != NULL)
    {
        printf("Read from '%s': %s\n", filename, readBuf);
    }
    else
    {
        printf("Error reading from '%s': %d\n", filename, sd_result);
    }

    // Close the file
    f_close(&sdFile);
    printf("Closed file '%s' after reading\n", filename);
}

void sd_card_write(const char* filename, const char* data)
{
    UINT bytesWritten;

    // Open the file for writing (create if not exists, overwrite if exists)
    sd_result = f_open(&sdFile, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (sd_result != FR_OK)
    {
        printf("Error opening file '%s' for writing: %d\n", filename, sd_result);
        return;
    }

    printf("Successfully opened file '%s' for writing\n", filename);

    // Write data to the file
    sd_result = f_write(&sdFile, data, strlen(data), &bytesWritten);
    if (sd_result == FR_OK)
    {
        printf("Wrote %u bytes to '%s'\n", bytesWritten, filename);
    }
    else
    {
        printf("Error writing to '%s': %d\n", filename, sd_result);
    }

    // Close the file
    f_close(&sdFile);
    printf("Closed file '%s' after writing\n", filename);
}

