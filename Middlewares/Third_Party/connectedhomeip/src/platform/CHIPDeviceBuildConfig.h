
/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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
 *    @file
 *          Platform-specific configuration overrides for the chip Device Layer
 *          on stm32 platforms.
 */
#pragma once 

#define CHIP_DEVICE_PROJECT_CONFIG_INCLUDE CHIP_PROJECT_CONFIG_INCLUDE
#define CHIP_DEVICE_PLATFORM_CONFIG_INCLUDE CHIP_PLATFORM_CONFIG_INCLUDE

// ==================== Platform Adaptations ====================
#define CHIP_DEVICE_LAYER_TARGET stm32
#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_STATION 0
#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP 0

#define CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE 1
#define CHIP_ENABLE_OPENTHREAD 1

#if CHIP_ENABLE_OPENTHREAD
#define CHIP_DEVICE_CONFIG_THREAD_FTD 0 // 0 = MTD
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD 1
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_SRP_CLIENT 1
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_DNS_CLIENT 1
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_COMMISSIONABLE_DISCOVERY 1
#endif

// ========== Platform-specific Configuration =========

// These are configuration options that are unique to the platform.
// These can be overridden by the application as needed.

// ...

// ========== Platform-specific Configuration Overrides =========

#define CHIP_DEVICE_CONFIG_CHIP_TASK_NAME  "STM32 TASK"

/**
 * CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE
 *
 * The stack size of the chip task.
 */
#define CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE (8 * 1024)

/**
 * CHIP_DEVICE_CONFIG_CHIP_TASK_PRIORITY
 *
 * The priority of the chip task.
 */
#define CHIP_DEVICE_CONFIG_CHIP_TASK_PRIORITY 24

/**
 * CHIP_DEVICE_CONFIG_CHIP_TASK_PRIORITY
 *
 * The stack size of thread task.
 */
#define CHIP_DEVICE_CONFIG_THREAD_TASK_STACK_SIZE (4 * 1024)

/**
 * CHIP_DEVICE_CONFIG_THREAD_TASK_PRIORITY
 *
 * The priority of the OpenThread task.
 */
#define CHIP_DEVICE_CONFIG_THREAD_TASK_PRIORITY 24

#define CHIP_DEVICE_CONFIG_MAX_EVENT_QUEUE_SIZE 25

/**
 * CHIP_SYSTEM_CONFIG_PACKETBUFFER_POOL_SIZE
 *
 * Reduce packet buffer pool size to 8 (default 15) to reduce ram consumption
 */
#define CHIP_SYSTEM_CONFIG_PACKETBUFFER_POOL_SIZE 8

#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_TELEMETRY 0
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_TELEMETRY 0
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_TELEMETRY_FULL 0

// Per 5.2.5.2. Commencement Section of CHIP spec, BLE advertisement is
// disabled for Locks and Barrier Access Devices.
#define CHIP_DEVICE_CONFIG_CHIPOBLE_ENABLE_ADVERTISING_AUTOSTART 0
#define CHIP_DEVICE_CONFIG_ENABLE_PAIRING_AUTOSTART 0

// ========== Platform-specific Cluster Overrides =========
#define CHIP_CLUSTER_NETWORK_COMMISSIONING_MAX_NETWORKS 1


/**
 * CHIP_DEVICE_CONFIG_ENABLE_CHIP_TIME_SERVICE_TIME_SYNC
 *
 * Enables synchronizing the device's real time clock with a remote Chip Time service
 * using the Chip Time Sync protocol.
 */
#define CHIP_DEVICE_CONFIG_ENABLE_CHIP_TIME_SERVICE_TIME_SYNC 0

#define CHIP_SYSTEM_CONFIG_PLATFORM_PROVIDES_TIME 1

/**
 *  @def CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL
 *
 *  @brief
 *    Active retransmit interval, or time to wait before retransmission after
 *    subsequent failures in milliseconds.
 *
 *  This is the default value, that might be adjusted by end device depending on its
 *  needs (e.g. sleeping period) using Service Discovery TXT record CRA key.
 *
 */
#define CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL (2000_ms32)

#define OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE 1

/**
 * CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
 *
 * Enable support for Chip-over-BLE (CHIPoBLE).
 */
#define CHIP_CONFIG_ENABLE_SESSION_RESUMPTION 1

#define CHIP_CONFIG_MEMORY_MGMT_PLATFORM 1
#define CHIP_CONFIG_MEMORY_MGMT_MALLOC 0
