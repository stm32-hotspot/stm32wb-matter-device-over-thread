/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include <platform/KeyValueStoreManager.h>
#include <lib/support/CodeUtils.h>
#include "flash_wb.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MATTER_KEY_NAME_MAX_LENGTH (15 * 2) //AR ADD Max key name string size is 30 "keyType...;KeyName..."
namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {
/** Singleton instance of the KeyValueStoreManager implementation object.
 */
KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char *key, void *value, size_t value_size, size_t *read_bytes_size, size_t offset) {

	CHIP_ERROR err = CHIP_NO_ERROR;
#ifdef DEBUG_KMS

	ChipLogDetail( DataManagement, "ST => GET: key =%s",key);
#endif

	if ((key != NULL) && (value != NULL) && (read_bytes_size != NULL)) {
		if (!NM_GetKeyValue(value, key, (uint32_t) value_size, read_bytes_size)) {
#ifdef DEBUG_KMS
			ChipLogDetail( DataManagement, "ST => Size to read : size =%i",value_size);
			ChipLogDetail( DataManagement, "ST => RETRUN BUFFER  : size =%i",*read_bytes_size);
			ChipLogDetail( DataManagement, "ST =>OFFSET  = %i",offset);
		#endif

		} else {
#ifdef DEBUG_KMS
			ChipLogDetail( DataManagement, "ST => KEY NOT FOUND");
		#endif
			err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
		}
	}
	return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char *key) {
	CHIP_ERROR err = CHIP_NO_ERROR;

	if (key != NULL) {
		if (NM_DeleteKey(key) != 0) {
			err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
		}
	}
	return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char *key, const void *value, size_t value_size) {
#ifdef DEBUG_KMS
	ChipLogDetail( DataManagement, "ST => PUT: key =%s",key);
	ChipLogDetail( DataManagement, "ST => PUT: value =%d \r\n",value_size);
#endif
	CHIP_ERROR err = CHIP_NO_ERROR;

	if ((value_size != 0) && (key != NULL) && (value != NULL)) {
		if (NM_SetKeyValue((char*) value, (char*) key, (uint32_t) value_size) != 0) {
			err = CHIP_ERROR_PERSISTED_STORAGE_FAILED;
			ChipLogDetail( DataManagement, "CHIP_ERROR_PERSISTED_STORAGE_FAILED");
		}
	}
	return err;
}

} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip
