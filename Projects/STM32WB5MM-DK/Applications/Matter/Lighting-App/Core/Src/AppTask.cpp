/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    All rights reserved.
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

/*STM32 includes*/
#include "app_common.h"
#include "app_thread.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "cmsis_os.h"
#include "AppEvent.h"
#include "AppTask.h"
#include "flash_wb.h"
#include "stm32wb5mm_dk_lcd.h"
#include "stm32_lcd.h"
#include "ssd1315.h"

/*Matter includes*/
#include <app/server/OnboardingCodesUtil.h>
#include <app-common/zap-generated/attribute-id.h>
#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-id.h>
#include <app/server/Dnssd.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>
#include <platform/TestOnlyCommissionableDataProvider.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <inet/EndPointStateOpenThread.h>
#include <DeviceInfoProviderImpl.h>
#include <DeviceNetworkProvisioningDelegateImpl.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>
#include <platform/CHIPDeviceLayer.h>
#if CHIP_ENABLE_OPENTHREAD
#include <platform/OpenThread/OpenThreadUtils.h>
#include <platform/ThreadStackManager.h>
#include <platform/stm32wb/ThreadStackManagerImpl.h>
#endif

using namespace ::chip;
using namespace ::chip::app;
using namespace chip::TLV;
using namespace chip::Credentials;
using namespace chip::DeviceLayer;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;
using chip::DeviceLayer::PersistedStorage::KeyValueStoreMgr;

AppTask AppTask::sAppTask;
chip::DeviceLayer::TestOnlyCommissionableDataProvider gTestOnlyCommissionableDataProvider;
chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;

//#define APP_ON_OFF_BUTTON BUTTON_USER1_PIN
#define APP_FUNCTION_BUTTON BUTTON_USER1
//#define APP_LEVEL_BUTTON BUTTON_USER3_PIN

#define STM32ThreadDataSet "STM32DataSet"
#define APP_EVENT_QUEUE_SIZE 10
#define NVM_TIMEOUT 1000  // timer to handle PB to save data in nvm or do a factory reset
#define DELAY_NVM 5000 // save data in nvm after commissioning with a delay of 5 sec
#define STM32_LIGHT_ENDPOINT_ID 1

static QueueHandle_t sAppEventQueue;
TimerHandle_t sPushButtonTimeoutTimer;
TimerHandle_t DelayNvmTimer;
const osThreadAttr_t AppTask_attr = { .name = APPTASK_NAME, .attr_bits =
APP_ATTR_BITS, .cb_mem = APP_CB_MEM, .cb_size = APP_CB_SIZE, .stack_mem =
APP_STACK_MEM, .stack_size = APP_STACK_SIZE, .priority =
APP_PRIORITY };

static bool sIsThreadProvisioned = false;
static bool sIsThreadEnabled = false;
static bool sHaveBLEConnections = false;
static bool sHaveFabric = false;
static bool sStartCommissioning = false;
static uint8_t NvmTimerCpt = 0;
static uint8_t NvmButtonStateCpt = 0;

CHIP_ERROR AppTask::StartAppTask() {
	sAppEventQueue = xQueueCreate(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent));
	if (sAppEventQueue == NULL) {
		APP_DBG("Failed to allocate app event queue");
		return CHIP_ERROR_NO_MEMORY;
	}

	// Start App task.
	osThreadNew(AppTaskMain, NULL, &AppTask_attr);

	return CHIP_NO_ERROR;
}

void LockOpenThreadTask(void) {
	chip::DeviceLayer::ThreadStackMgr().LockThreadStack();
}

void UnlockOpenThreadTask(void) {
	chip::DeviceLayer::ThreadStackMgr().UnlockThreadStack();
}

CHIP_ERROR AppTask::Init() {

	CHIP_ERROR err = CHIP_NO_ERROR;
	ChipLogProgress(NotSpecified, "Current Software Version: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);

	// Setup button handler
	APP_ENTRY_PBSetReceiveCallback(ButtonEventHandler);

	// Create FreeRTOS sw timer for Push button timeouts.
	sPushButtonTimeoutTimer = xTimerCreate("PushButtonTimer",       // Just a text name, not used by the RTOS kernel
			pdMS_TO_TICKS(NVM_TIMEOUT),                   // == default timer period (mS)
			true,               // no timer reload (==one-shot)
			(void*) this,       // init timer id
			TimerEventHandler // timer callback handler
			);

	DelayNvmTimer = xTimerCreate("Delay_NVM", // Just a text name, not used by the RTOS kernel
			DELAY_NVM,        // == default timer period (mS)
			pdFALSE,               //  timer reload
			0,       // init timer
			DelayNvmHandler // timer callback handler
			);

	ThreadStackMgr().InitThreadStack();

	ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_Router);

	PlatformMgr().AddEventHandler(MatterEventHandler, 0);

	err = LightingMgr().Init();
	if (err != CHIP_NO_ERROR) {
		APP_DBG("LightingMgr().Init() failed");
		return err;
	}
	LightingMgr().SetCallbacks(ActionInitiated, ActionCompleted);

#if CHIP_DEVICE_CONFIG_ENABLE_EXTENDED_DISCOVERY
	chip::app::DnssdServer::Instance().SetExtendedDiscoveryTimeoutSecs(extDiscTimeoutSecs);
#endif

	SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
	// Init ZCL Data Model
	static chip::CommonCaseDeviceServerInitParams initParams;
	(void) initParams.InitializeStaticResourcesBeforeServerInit();
	chip::DeviceLayer::SetCommissionableDataProvider(&gTestOnlyCommissionableDataProvider);
	gExampleDeviceInfoProvider.SetStorageDelegate(initParams.persistentStorageDelegate);
	chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

	chip::Inet::EndPointStateOpenThread::OpenThreadEndpointInitParam nativeParams;
	nativeParams.lockCb = LockOpenThreadTask;
	nativeParams.unlockCb = UnlockOpenThreadTask;
	nativeParams.openThreadInstancePtr = chip::DeviceLayer::ThreadStackMgrImpl().OTInstance();
	initParams.endpointNativeParams = static_cast<void*>(&nativeParams);
	chip::Server::GetInstance().Init(initParams);

	ConfigurationMgr().LogDeviceConfig();

	// Open commissioning after boot if no fabric was available
	if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
		PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
		// Enable BLE advertisements
		chip::Server::GetInstance().GetCommissioningWindowManager().OpenBasicCommissioningWindow();
		APP_DBG("BLE advertising started. Waiting for Pairing.");
	} else {  // try to attach to the thread network
		uint8_t datasetBytes[Thread::kSizeOperationalDataset];
		size_t datasetLength = 0;
		APP_BLE_Init_Dyn_3();
		UTIL_LCD_DisplayStringAt(0, LINE(1), (uint8_t*) "Fabric Found", LEFT_MODE);
		BSP_LCD_Refresh(0);
		CHIP_ERROR error = KeyValueStoreMgr().Get(STM32ThreadDataSet, datasetBytes, sizeof(datasetBytes), &datasetLength);
		if (error == CHIP_NO_ERROR) {
			DeviceLayer::DeviceNetworkProvisioningDelegateImpl deviceDelegate;
			deviceDelegate.ProvisionThread(ByteSpan(datasetBytes, datasetLength));
		} else {
			APP_DBG("Thread network Data set was not found");
		}
	}

	err = PlatformMgr().StartEventLoopTask();
	if (err != CHIP_NO_ERROR) {
		APP_DBG("PlatformMgr().StartEventLoopTask() failed");
	}

	return err;
}

void AppTask::InitMatter() {
	GetAppTask().StartAppTask();
}

void AppTask::AppTaskMain(void *pvParameter) {
	AppEvent event;

	CHIP_ERROR err = sAppTask.Init();

	if (err != CHIP_NO_ERROR) {
		APP_DBG("App task init failled ");
	}

	APP_DBG("App Task started");
	while (true) {

		BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, pdMS_TO_TICKS(10));
		while (eventReceived == pdTRUE) {
			sAppTask.DispatchEvent(&event);
			eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
		}
	}

}

void AppTask::LightingActionEventHandler(AppEvent *aEvent) {
	LightingManager::Action_t action;

	if (aEvent->Type == AppEvent::kEventType_Button) {
		// Toggle light
		if (LightingMgr().IsTurnedOn()) {
			action = LightingManager::OFF_ACTION;
		} else {
			action = LightingManager::ON_ACTION;
		}

		sAppTask.mSyncClusterToButtonAction = true;
		LightingMgr().InitiateAction(action, 0, 0, 0);
	}
	if (aEvent->Type == AppEvent::kEventType_Level && aEvent->ButtonEvent.Action != 0) {
		// Toggle Dimming of light between 2 fixed levels
		uint8_t val = 0x0;
		val = LightingMgr().GetLevel() == 0x7f ? 0x1 : 0x7f;
		action = LightingManager::LEVEL_ACTION;

		sAppTask.mSyncClusterToButtonAction = true;
		LightingMgr().InitiateAction(action, 0, 1, &val);
	}
}

void AppTask::ButtonEventHandler(Push_Button_st *Button) {

	AppEvent button_event = { };
	button_event.Type = AppEvent::kEventType_Button;
	button_event.ButtonEvent.ButtonIdx = Button->Pushed_Button;
	button_event.ButtonEvent.Action = Button->State;

	if (Button->Pushed_Button == APP_FUNCTION_BUTTON) {
		// Hand off to Functionality handler - depends on duration of press
		button_event.Handler = FunctionHandler;
	} else {
		return;
	}

	sAppTask.PostEvent(&button_event);
}

void AppTask::TimerEventHandler(TimerHandle_t xTimer) {

	NvmTimerCpt++;
	if (BSP_PB_GetState(BUTTON_USER1) == 0) {
		NvmButtonStateCpt++;
	}
	if (NvmTimerCpt >= 10) {
		xTimerStop(sPushButtonTimeoutTimer, 0);
		if (NvmButtonStateCpt >= 9) {
			AppEvent event;
			event.Type = AppEvent::kEventType_Timer;
			event.Handler = UpdateNvmEventHandler;
			sAppTask.mFunction = kFunction_FactoryReset;
			sAppTask.PostEvent(&event);
		}
	} else if ((NvmTimerCpt > NvmButtonStateCpt) && (NvmTimerCpt <= 2)) {
		AppEvent event;
		event.Type = AppEvent::kEventType_Timer;
		event.Handler = UpdateNvmEventHandler;
		xTimerStop(sPushButtonTimeoutTimer, 0);
		sAppTask.mFunction = kFunction_SaveNvm;
		sAppTask.PostEvent(&event);
	}
}

void AppTask::FunctionHandler(AppEvent *aEvent) {
	if (xTimerIsTimerActive(sPushButtonTimeoutTimer) == 0) {
		xTimerStart(sPushButtonTimeoutTimer, 0);
		NvmTimerCpt = 0;
		NvmButtonStateCpt = 0;
	}
}

void AppTask::ActionInitiated(LightingManager::Action_t aAction) {
	// Placeholder for light action
	UTIL_LCD_ClearStringLine(2);
	if (aAction == LightingManager::ON_ACTION) {
		APP_DBG("Light goes on");
		char Message[11];
		snprintf(Message, sizeof(Message), "LED ON %d", LightingMgr().GetLevel());
		UTIL_LCD_DisplayStringAt(0, LINE(2), (uint8_t*) Message, CENTER_MODE);

	} else if (aAction == LightingManager::OFF_ACTION) {
		APP_DBG("Light goes off ");
		UTIL_LCD_ClearStringLine(2);
	} else if (aAction == LightingManager::LEVEL_ACTION) {
		if (LightingMgr().IsTurnedOn()) {
			char Message[11];
			snprintf(Message, sizeof(Message), "LED ON %d", LightingMgr().GetLevel());
			UTIL_LCD_DisplayStringAt(0, LINE(2), (uint8_t*) Message, CENTER_MODE);
			APP_DBG("Update level control %d", LightingMgr().GetLevel());
		}
	}
	BSP_LCD_Refresh(0);
}

void AppTask::ActionCompleted(LightingManager::Action_t aAction) {
	// Placeholder for light action completed
	if (aAction == LightingManager::ON_ACTION) {
		APP_DBG("Light action on completed");
	} else if (aAction == LightingManager::OFF_ACTION) {
		APP_DBG("Light action off completed");
	}
	if (sAppTask.mSyncClusterToButtonAction) {
		sAppTask.UpdateClusterState();
		sAppTask.mSyncClusterToButtonAction = false;
	}
}

void AppTask::PostEvent(const AppEvent *aEvent) {
	if (sAppEventQueue != NULL) {
		if (!xQueueSend(sAppEventQueue, aEvent, 1)) {
			ChipLogError(NotSpecified, "Failed to post event to app task event queue");
		}
	} else {
		ChipLogError(NotSpecified, "Event Queue is NULL should never happen");
	}
}

void AppTask::DispatchEvent(AppEvent *aEvent) {
	if (aEvent->Handler) {
		aEvent->Handler(aEvent);
	} else {
		ChipLogError(NotSpecified, "Event received with no handler. Dropping event.");
	}
}

/**
 * Update cluster status after application level changes
 */
void AppTask::UpdateClusterState(void) {
	ChipLogProgress(NotSpecified, "UpdateClusterState");
	// Write the new on/off value
	EmberAfStatus status = Clusters::OnOff::Attributes::OnOff::Set(STM32_LIGHT_ENDPOINT_ID, LightingMgr().IsTurnedOn());
	if (status != EMBER_ZCL_STATUS_SUCCESS) {
		ChipLogError(NotSpecified, "ERR: updating on/off %x", status);
	}

	// Write new level value
	status = Clusters::LevelControl::Attributes::CurrentLevel::Set(STM32_LIGHT_ENDPOINT_ID, LightingMgr().GetLevel());
	if (status != EMBER_ZCL_STATUS_SUCCESS) {
		ChipLogError(NotSpecified, "ERR: updating level %x", status);
	}
}

void AppTask::DelayNvmHandler(TimerHandle_t xTimer) {
	AppEvent event;
	event.Type = AppEvent::kEventType_Timer;
	event.Handler = UpdateNvmEventHandler;
	sAppTask.mFunction = kFunction_SaveNvm;
	sAppTask.PostEvent(&event);
}

void AppTask::UpdateLCD(void) {
	if (sIsThreadProvisioned && sIsThreadEnabled) {
		UTIL_LCD_DisplayStringAt(0, LINE(4), (uint8_t*) "Network Joined", LEFT_MODE);
	} else if ((sIsThreadProvisioned == false) || (sIsThreadEnabled == false)) {
		UTIL_LCD_ClearStringLine(4);
	}
	if (sHaveBLEConnections) {
		UTIL_LCD_ClearStringLine(1);
		BSP_LCD_Refresh(0);
		UTIL_LCD_DisplayStringAt(0, LINE(1), (uint8_t*) "BLE Connected", LEFT_MODE);
	}
	if (sHaveFabric) {
		UTIL_LCD_ClearStringLine(1);
		BSP_LCD_Refresh(0);
		UTIL_LCD_DisplayStringAt(0, LINE(1), (uint8_t*) "Fabric Created", LEFT_MODE);
	}
	if ((sHaveFabric == false) && (sHaveBLEConnections == false) && (sStartCommissioning == true)) {
		UTIL_LCD_DisplayStringAt(0, LINE(1), (uint8_t*) "", LEFT_MODE);
		BSP_LCD_Refresh(0);
		UTIL_LCD_DisplayStringAt(0, LINE(1), (uint8_t*) "Fabric Failed", LEFT_MODE);
		sStartCommissioning = false;
	}
	BSP_LCD_Refresh(0);
}

void AppTask::UpdateNvmEventHandler(AppEvent *aEvent) {

	if (sAppTask.mFunction == kFunction_SaveNvm) {
		if (sIsThreadProvisioned && sIsThreadEnabled) {
			chip::Thread::OperationalDataset dataset { };
			DeviceLayer::ThreadStackMgrImpl().GetThreadProvision(dataset);
			ByteSpan datasetbyte = dataset.AsByteSpan();
			KeyValueStoreMgr().Put(STM32ThreadDataSet, datasetbyte.data(), datasetbyte.size());
		}
		APP_DBG("SAVE NVM");
		NM_Dump();

	} else if (sAppTask.mFunction == kFunction_FactoryReset) {
		APP_DBG("FACTORY RESET");
		NM_ResetFactory();
	}
}

void AppTask::MatterEventHandler(const ChipDeviceEvent *event, intptr_t) {
	switch (event->Type) {
	case DeviceEventType::kServiceProvisioningChange: {
		sIsThreadProvisioned = event->ServiceProvisioningChange.IsServiceProvisioned;
		UpdateLCD();
		break;
	}

	case DeviceEventType::kThreadConnectivityChange: {
		sIsThreadEnabled = (event->ThreadConnectivityChange.Result == kConnectivity_Established);
		UpdateLCD();
		break;
	}

	case DeviceEventType::kCHIPoBLEConnectionEstablished: {
		sHaveBLEConnections = true;
		sStartCommissioning = true;
		APP_DBG("kCHIPoBLEConnectionEstablished");
		UpdateLCD();
		break;
	}

	case DeviceEventType::kCHIPoBLEConnectionClosed: {
		sHaveBLEConnections = false;
		APP_DBG("kCHIPoBLEConnectionClosed");
		UpdateLCD();
		break;
	}

	case DeviceEventType::kCommissioningComplete: {
		xTimerStart(DelayNvmTimer, 0);
		sHaveFabric = true;
		UpdateLCD();
		break;
	}

	default:
		break;
	}
}

