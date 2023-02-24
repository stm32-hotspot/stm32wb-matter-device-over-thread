

#pragma once


#define INET_CONFIG_TEST 0
//#define INET_PROJECT_CONFIG_INCLUDE 0
//#define INET_PLATFORM_CONFIG_INCLUDE 0
#define INET_CONFIG_ENABLE_IPV4 0
#define INET_CONFIG_ENABLE_TCP_ENDPOINT 0  // JB 06/05/22 was set to 0 with Qorvo
#define INET_CONFIG_ENABLE_UDP_ENDPOINT 1
#define HAVE_LWIP_RAW_BIND_NETIF 0

//#include <inet/InetConfig.h>
//#include <inet/IANAConstants.h>
//#include <inet/IPAddress.h>

#define INET_UDP_END_POINT_IMPL_CONFIG_FILE <inet/UDPEndPointImplOpenThread.h>
#define INET_PLATFORM_CONFIG_INCLUDE <platform/stm32wb/InetPlatformConfig.h>
//#include INET_UDP_END_POINT_IMPL_CONFIG_FILE

