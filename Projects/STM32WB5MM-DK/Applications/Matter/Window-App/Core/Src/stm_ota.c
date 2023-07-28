/**
 ******************************************************************************
 * @file    stm_ota.c
 * @author  MCD Application Team
 * @brief   Write new image in external flash
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm_ota.h"
#include "s25fl128s.h"
#include "cmsis_os.h"

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern QSPI_HandleTypeDef hqspi;

uint8_t aRxBuffer[256];
uint8_t aTxBuffer[] = "Hello";
uint32_t addressDelete = 0;
uint32_t addressWrite = 0; // TODO CHANGE ADDRESS
uint32_t addressRead = 0; // TODO CHANGE ADDRESS
#define CHUNK_SIZE 256
#define BLOCK_SIZE 65536
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

STM_OTA_StatusTypeDef STM_OTA_Delete_Image(void) {
	STM_OTA_StatusTypeDef error = STM_OTA_OK;
	int32_t internal_error = 0;
	uint32_t addressDelete = 0;

	for (int i = 0; i < 16; i++) { // erase 1MB
		S25FL128S_WriteEnable(&hqspi, S25FL128S_SPI_MODE);
		internal_error = S25FL128S_BlockErase(&hqspi, S25FL128S_SPI_MODE,
				addressDelete, S25FL128S_ERASE_64K);
		S25FL128S_AutoPollingMemReady(&hqspi, S25FL128S_SPI_MODE);
		addressDelete += BLOCK_SIZE;
		if (internal_error != 0) {
			return STM_OTA_DELETE_FAILED;
		}
	}
	return error;
}

STM_OTA_StatusTypeDef STM_OTA_WriteChunk(uint64_t DownloadedBytes,
		size_t DataSize, uint8_t* Data) {
	STM_OTA_StatusTypeDef error = STM_OTA_OK;
	int32_t internal_error = 0;

	if ((Data == NULL) && (DataSize == 0)) {
		return STM_OTA_INVALID_PARAM;
	}
	S25FL128S_WriteEnable(&hqspi, S25FL128S_SPI_MODE);
	error = S25FL128S_PageProgram(&hqspi, S25FL128S_SPI_1I4O_MODE, aTxBuffer,
			addressWrite, sizeof(aTxBuffer));
	S25FL128S_AutoPollingMemReady(&hqspi, S25FL128S_SPI_MODE);
	addressWrite += CHUNK_SIZE;
	if (internal_error != 0) {
		return STM_OTA_WRITE_FAILED;
	}
	return error;
}

STM_OTA_StatusTypeDef STM_OTA_Read_Chunk(void) {
	STM_OTA_StatusTypeDef error = 0;

	error = S25FL128S_ReadSTR(&hqspi, S25FL128S_SPI_4IO_MODE, aRxBuffer,
			addressRead, sizeof(aRxBuffer));
	addressRead += CHUNK_SIZE;

	if (addressRead > 1040000) {
		printf("data %s at addr %d\n", aRxBuffer, addressRead);
	}
	return error;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

