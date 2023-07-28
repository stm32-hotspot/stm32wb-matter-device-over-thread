/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * NOTE: This file is generated by src/tools/chip-cert/dacs.py.
 */
#pragma once
#include <cstdint>

#include <lib/support/Span.h>
#include <platform/CHIPDeviceConfig.h>

namespace chip {
namespace DevelopmentCerts {

extern ByteSpan kPaiCert;
extern ByteSpan kPaiPublicKey;
extern ByteSpan kPaiPrivateKey;

#if CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID == 0xFFF1
extern const uint8_t kDevelopmentPAI_Cert_FFF1[463];
extern const uint8_t kDevelopmentPAI_PublicKey_FFF1[65];
extern const uint8_t kDevelopmentPAI_PrivateKey_FFF1[32];
#endif
#if CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID == 0xFFF2
extern const uint8_t kDevelopmentPAI_Cert_FFF2[441];
extern const uint8_t kDevelopmentPAI_PublicKey_FFF2[65];
extern const uint8_t kDevelopmentPAI_PrivateKey_FFF2[32];
#endif
#if CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID == 0xFFF3
extern const uint8_t kDevelopmentPAI_Cert_FFF3[440];
extern const uint8_t kDevelopmentPAI_PublicKey_FFF3[65];
extern const uint8_t kDevelopmentPAI_PrivateKey_FFF3[32];
#endif
} // namespace DevelopmentCerts
} // namespace chip
