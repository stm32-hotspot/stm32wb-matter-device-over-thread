/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm_ota.h
 * @author  MCD Application Team
 * @brief   Header file for stm_ota.c
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM_OTA_H
#define STM_OTA_H

/* Includes ------------------------------------------------------------------*/
#include "utilities_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	STM_OTA_OK,
	STM_OTA_WRITE_FAILED,
	STM_OTA_READ_FAILED,
	STM_OTA_DELETE_FAILED,
	STM_OTA_INVALID_PARAM,
	STM_OTA_SIZE_FULL
} STM_OTA_StatusTypeDef;

/* Exported variables ------------------------------------------------------- */
//extern QSPI_HandleTypeDef hqspi;

/* Exported functions ------------------------------------------------------- */


/**
 * @brief  Delete old image in external flash
 */
STM_OTA_StatusTypeDef STM_OTA_Delete_Image(void);

/**
 * @brief  Write chunk of data in external flash
 */
STM_OTA_StatusTypeDef STM_OTA_WriteChunk(uint64_t DownloadedBytes,
		size_t DataSize, uint8_t* Data);

STM_OTA_StatusTypeDef STM_OTA_Read_Chunk(void);
#ifdef __cplusplus
}
#endif

#endif /*STM_OTA_H */
