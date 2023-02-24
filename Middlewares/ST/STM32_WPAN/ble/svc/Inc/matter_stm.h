
/**
  ******************************************************************************
  *  @file    matter_stm.h
  * @author  MCD Application Team
  * @brief   Header for matter_stm.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __P2PS_STM_H
#define __P2PS_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

 
  
/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MATTER_STM_Init( void );
tBleStatus MATTER_STM_App_Update_Char(uint16_t UUID,  uint8_t *pPayload, uint16_t Length);



#ifdef __cplusplus
}
#endif

#endif /*__P2PS_STM_H */


