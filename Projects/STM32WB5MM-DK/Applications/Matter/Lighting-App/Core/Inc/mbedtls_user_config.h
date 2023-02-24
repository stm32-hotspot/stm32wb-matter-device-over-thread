/**
******************************************************************************
* @file mbedtls_user_config.h
* @author MCD Application Team
* @brief mbedtls configuration user overlay
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */



#ifndef MBEDTLS_USER_CONFIG_H
#define MBEDTLS_USER_CONFIG_H


#ifdef __cplusplus
extern "C" {
#endif


// -------------------------- MATTER MBEDTLS CONF ---------------------------- //
/**
* Enable H Crypto and Entropy modules
*/

#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_X509_CSR_WRITE_C
#define MBEDTLS_X509_CREATE_C
#define MBEDTLS_PEM_WRITE_C
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_PKCS5_C
#define MBEDTLS_SHA1_C
#undef MBEDTLS_NET_C
#undef MBEDTLS_TIMING_C
#undef MBEDTLS_FS_IO 
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_MEMORY_BUFFER_ALLOC_C
#define MBEDTLS_THREADING_ALT
#define MBEDTLS_THREADING_C

#undef MBEDTLS_PSA_ITS_FILE_C
#define MBEDTLS_SHA256_SMALLER
#undef MBEDTLS_SHA512_C 

#ifdef __cplusplus
}
#endif
#endif /* MBEDTLS_USER_CONFIG_H */
