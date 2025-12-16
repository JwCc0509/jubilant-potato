#pragma once

#include "GenTL.h"

#ifdef __cplusplus
extern "C" {
    namespace GenTL {
#endif

            GC_API GCReadRemotePortPri(IF_HANDLE hIface, const char* sDeviceID, uint64_t iAddress, void* pBuffer, size_t* piSize);

            GC_API_P (PGCReadRemotePortPri)(IF_HANDLE hIface, const char* sDeviceID, uint64_t iAddress, void* pBuffer, size_t* piSize);

#ifdef __cplusplus
    }
}
#endif