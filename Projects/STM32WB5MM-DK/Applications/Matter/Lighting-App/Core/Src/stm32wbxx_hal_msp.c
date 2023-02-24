/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : stm32wbxx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
#include "app_conf.h"
/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_lpuart1_tx;

extern DMA_HandleTypeDef hdma_usart1_tx;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
		HAL_Delay(100);
	}
	/* USER CODE END Error_Handler_Debug */
}
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void) {
	/* USER CODE BEGIN MspInit 0 */

	/* USER CODE END MspInit 0 */

	__HAL_RCC_HSEM_CLK_ENABLE();

	/* System interrupt init*/
	/* PendSV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

	/* Peripheral interrupt init */
	/* PVD_PVM_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PVD_PVM_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(PVD_PVM_IRQn);
	/* FLASH_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(FLASH_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(FLASH_IRQn);
	/* RCC_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(RCC_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(RCC_IRQn);
	/* C2SEV_PWR_C2H_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(C2SEV_PWR_C2H_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(C2SEV_PWR_C2H_IRQn);
	/* PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQn);
	/* HSEM_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(HSEM_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(HSEM_IRQn);
	/* FPU_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(FPU_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(FPU_IRQn);

	/* USER CODE BEGIN MspInit 1 */
	/* USER CODE END MspInit 1 */
}

/**
 * @brief IPCC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hipcc: IPCC handle pointer
 * @retval None
 */
void HAL_IPCC_MspInit(IPCC_HandleTypeDef *hipcc) {
	if (hipcc->Instance == IPCC) {
		/* USER CODE BEGIN IPCC_MspInit 0 */

		/* USER CODE END IPCC_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_IPCC_CLK_ENABLE();
		/* IPCC interrupt Init */
		HAL_NVIC_SetPriority(IPCC_C1_RX_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(IPCC_C1_RX_IRQn);
		HAL_NVIC_SetPriority(IPCC_C1_TX_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(IPCC_C1_TX_IRQn);
		/* USER CODE BEGIN IPCC_MspInit 1 */

		/* USER CODE END IPCC_MspInit 1 */
	}

}

/**
 * @brief IPCC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hipcc: IPCC handle pointer
 * @retval None
 */
void HAL_IPCC_MspDeInit(IPCC_HandleTypeDef *hipcc) {
	if (hipcc->Instance == IPCC) {
		/* USER CODE BEGIN IPCC_MspDeInit 0 */

		/* USER CODE END IPCC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_IPCC_CLK_DISABLE();

		/* IPCC interrupt DeInit */
		HAL_NVIC_DisableIRQ(IPCC_C1_RX_IRQn);
		HAL_NVIC_DisableIRQ(IPCC_C1_TX_IRQn);
		/* USER CODE BEGIN IPCC_MspDeInit 1 */

		/* USER CODE END IPCC_MspDeInit 1 */
	}

}

/**
 * @brief RTC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc) {
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	if (hrtc->Instance == RTC) {
		/* USER CODE BEGIN RTC_MspInit 0 */
		HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

		/**
		 *  Write twice the value to flush the APB-AHB bridge
		 *  This bit shall be written in the register before writing the next one
		 */
		HAL_PWR_EnableBkUpAccess();

		__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE); /**< Select LSE as RTC Input */

		/* USER CODE END RTC_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;

		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			//  Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_RTC_ENABLE();
		__HAL_RCC_RTCAPB_CLK_ENABLE();
		/* RTC interrupt Init */
		HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
		/* USER CODE BEGIN RTC_MspInit 1 */

		MODIFY_REG(RTC->CR, RTC_CR_WUCKSEL, CFG_RTC_WUCKSEL_DIVIDER);
		/* USER CODE END RTC_MspInit 1 */
	}

}

/**
 * @brief RTC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc) {
	if (hrtc->Instance == RTC) {
		/* USER CODE BEGIN RTC_MspDeInit 0 */

		/* USER CODE END RTC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();
		__HAL_RCC_RTCAPB_CLK_DISABLE();

		/* RTC interrupt DeInit */
		HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
		/* USER CODE BEGIN RTC_MspDeInit 1 */

		/* USER CODE END RTC_MspDeInit 1 */
	}

}

/* USER CODE BEGIN 1 */
/**
 * @brief RNG MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hrng: RNG handle pointer
 * @retval None
 */
void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng) {
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	if (hrng->Instance == RNG) {
		/* USER CODE BEGIN RNG_MspInit 0 */

		/* USER CODE END RNG_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
		PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		}

		/* Peripheral clock enable */
		__HAL_RCC_RNG_CLK_ENABLE();
		/* USER CODE BEGIN RNG_MspInit 1 */

		/* USER CODE END RNG_MspInit 1 */
	}

}

/**
 * @brief RNG MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hrng: RNG handle pointer
 * @retval None
 */
void HAL_RNG_MspDeInit(RNG_HandleTypeDef *hrng) {
	if (hrng->Instance == RNG) {
		/* USER CODE BEGIN RNG_MspDeInit 0 */

		/* USER CODE END RNG_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_RNG_CLK_DISABLE();
		/* USER CODE BEGIN RNG_MspDeInit 1 */

		/* USER CODE END RNG_MspDeInit 1 */
	}

}

/* USER CODE END 1 */
