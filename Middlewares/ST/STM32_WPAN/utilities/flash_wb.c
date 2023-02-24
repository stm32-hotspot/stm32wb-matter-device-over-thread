/**
  ******************************************************************************
  * @file    flash_wb.c
  * @author  MCD Application Team
  * @brief   Middleware between keymanager and flahs_driver ,
  *          to manage key needed for Matter
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
#include "flash_wb.h"
#include "flash_driver.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Private defines -----------------------------------------------------------*/
#define MATTER_KEY_NAME_MAX_LENGTH (15 * 2) //AR ADD Max key name string size is 30 "keyType...;KeyName..."
// ^ STM32STORE_MAX_KEY_SIZE
#define NVM_OFFSET_KEY 512
#define NVM_MAX_KEY NVM_SIZE_FLASH/NVM_OFFSET_KEY
#define NVM_END_FLASH
#define NVM_BLOCK_SIZE NVM_OFFSET_KEY
#define FLASH_START 0x08000000
#define NVM_SIZE_FLASH 12288
#define NB_SECTOR 3
#define NVM_MATTER_ADDR_INIT  0x08070000
#define DEFAULT_VALUE  0xFF
#define SECTOR_SIZE 12288
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t ram_nvm[SECTOR_SIZE] = { 0 };
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static uint8_t flash_get(uint8_t *KeyValue, uint8_t *KeyAddr, size_t KeySize, size_t *read_by_size);
static uint8_t flash_update(uint8_t *Page, uint8_t *KeyName, uint8_t *KeyValue, size_t KeySize);
static NVM_StatusTypeDef flash_replace(uint8_t *PtKeyfind, uint8_t *KeyName, uint8_t *KeyValue, size_t KeySize);
static NVM_StatusTypeDef flash_write(uint8_t *PtKeyFree, uint8_t *key, uint8_t *value, size_t value_size);
static uint8_t* SearchKey(uint8_t *PtPage, uint8_t *KeyName);

/* Public functions ----------------------------------------------------------*/

void NM_Init(void) {
	memcpy(ram_nvm, (void*) NVM_MATTER_ADDR_INIT, (size_t) SECTOR_SIZE);

}

void NM_Dump(void) {
	FD_WriteData(NVM_MATTER_ADDR_INIT, (uint64_t*) ram_nvm, (uint32_t) (SECTOR_SIZE / sizeof(uint64_t)));
}

NVM_StatusTypeDef NM_GetKeyValue(void *KeyValue, const char *KeyName, uint32_t KeySize, size_t *read_by_size) {
	uint8_t *key_search = SearchKey(ram_nvm, (uint8_t*) KeyName);
	if (key_search != NULL) {
		flash_get(KeyValue, key_search, KeySize, read_by_size);
		return NVM_OK;
	}

	return NVM_KEY_NOT_FOUND;
}

NVM_StatusTypeDef NM_SetKeyValue(char *KeyValue, char *KeyName, uint32_t KeySize) {
	void *Ptkey = NULL;
	if (KeySize > NVM_BLOCK_SIZE) {

		return NVM_BLOCK_SIZE_OVERFLOW;
	}

	Ptkey = SearchKey(ram_nvm, (uint8_t*) KeyName);

	if (Ptkey == NULL) {
		if (!flash_update(ram_nvm, (uint8_t*) KeyName, (uint8_t*) KeyValue, KeySize)) {
			return NVM_OK;
		}
	} else {
		if (!flash_replace(Ptkey, (uint8_t*) KeyName, (uint8_t*) KeyValue, KeySize)) {
			return NVM_OK;
		}

	}

	return NVM_WRITE_FAILED;
}

uint8_t NM_DeleteKey(const char *Keyname) {
	uint8_t *Ptkey = SearchKey(ram_nvm, (uint8_t*) Keyname);

	if (Ptkey != NULL) {

		memset(Ptkey, DEFAULT_VALUE, (size_t) NVM_BLOCK_SIZE);
		return NVM_OK;
	}
	return NVM_KEY_NOT_FOUND;
}

void NM_ResetFactory(void) {
	while (1) {
		FD_EraseSectors((NVM_MATTER_ADDR_INIT - FLASH_START) / (NVM_SIZE_FLASH / NB_SECTOR), NB_SECTOR);
		NVIC_SystemReset();

	}
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

static uint8_t* SearchKey(uint8_t *PtPage, uint8_t *KeyName) {

	for (uint8_t *i = PtPage; i < (PtPage + NVM_SIZE_FLASH); i = i + NVM_BLOCK_SIZE) {
		if (*i != DEFAULT_VALUE) {
			if (strcmp((char*) KeyName, (char*) i) == 0) {
				return i;
			}
		}
	}
	return NULL;
}

static uint8_t flash_get(uint8_t *KeyValue, uint8_t *KeyAddr, size_t KeySize, size_t *read_by_size) {
	*read_by_size = *(size_t*) ((uint8_t*) KeyAddr + MATTER_KEY_NAME_MAX_LENGTH);
	if (KeySize > *read_by_size) {
		memcpy(KeyValue, KeyAddr + MATTER_KEY_NAME_MAX_LENGTH + sizeof(size_t), *read_by_size);
		return NVM_OK;
	} else {
		return NVM_BUFFER_TOO_SMALL;
	}

}

static NVM_StatusTypeDef flash_update(uint8_t *Page, uint8_t *KeyName, uint8_t *KeyValue, size_t KeySize) {

	for (uint8_t *i = Page; i < (Page + NVM_SIZE_FLASH); i += NVM_BLOCK_SIZE) {

		if (*i == DEFAULT_VALUE) {
			return flash_write(i, KeyName, KeyValue, KeySize);
		}
	}

	return NVM_SIZE_FULL;

}

static NVM_StatusTypeDef flash_replace(uint8_t *PtKeyfind, uint8_t *KeyName, uint8_t *KeyValue, size_t KeySize) {

	if ((PtKeyfind != NULL) && (KeyName != NULL) && (KeyValue != NULL)) {
		memset(PtKeyfind, DEFAULT_VALUE, NVM_OFFSET_KEY);
		memset(PtKeyfind, 0x00, MATTER_KEY_NAME_MAX_LENGTH);
		memcpy(PtKeyfind, KeyName, strlen((char*) KeyName));
		memcpy(PtKeyfind + MATTER_KEY_NAME_MAX_LENGTH, &KeySize, sizeof(size_t));
		memcpy(PtKeyfind + MATTER_KEY_NAME_MAX_LENGTH + sizeof(size_t), KeyValue, KeySize);
		return NVM_OK;
	}
	return NVM_WRITE_FAILED;

}

static NVM_StatusTypeDef flash_write(uint8_t *PtKeyFree, uint8_t *key, uint8_t *value, size_t value_size) {

	if ((PtKeyFree != NULL) && (key != NULL) && (value != NULL)) {

		memset(PtKeyFree, DEFAULT_VALUE, NVM_OFFSET_KEY);
		memset(PtKeyFree, 0x00, MATTER_KEY_NAME_MAX_LENGTH);
		memcpy(PtKeyFree, key, strlen((char*) key));
		memcpy(PtKeyFree + MATTER_KEY_NAME_MAX_LENGTH, &value_size, sizeof(size_t));
		memcpy(PtKeyFree + MATTER_KEY_NAME_MAX_LENGTH + sizeof(size_t), value, value_size);
		return NVM_OK;
	}

	return NVM_WRITE_FAILED;
}

