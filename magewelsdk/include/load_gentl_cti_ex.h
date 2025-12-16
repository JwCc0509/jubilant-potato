#pragma once
#include "GenTL.h"//GenTL接口
#include "windows.h"
#include "mw_genapi_producer.h"//GenApi接口
#include "mw_gentl_log.h"
#include "cxp_standard.h"

using namespace GenTL;
using namespace mw_genapi;
using namespace mw_gentl_log;

//动态加载gentl cti库
class CStdLoadCtiEx
{
public:
	CStdLoadCtiEx();
	virtual ~CStdLoadCtiEx();

	bool8_t load_sdk(const char* dll_name);
	void	set_sdk_mode(scap_sdk_t sdk_mode);
	scap_sdk_t get_sdk_mode();
public:
	HINSTANCE							m_dll_handle;
	scap_sdk_t							m_sdk_mode;//cxp, cl ....

	//动态库函数指针对象成员
	PGCGetInfo					GCGetInfo;
	PGCGetLastError				GCGetLastError;
	PGCInitLib					GCInitLib;
	PGCCloseLib					GCCloseLib;
	PGCReadPort					GCReadPort;
	PGCWritePort				GCWritePort;
	PGCGetPortURL				GCGetPortURL;
	PGCGetPortInfo				GCGetPortInfo;
	PGCRegisterEvent			GCRegisterEvent;
	PGCUnregisterEvent			GCUnregisterEvent;
	PEventGetData				EventGetData;
	PEventGetDataInfo			EventGetDataInfo;
	PEventGetInfo				EventGetInfo;
	PEventFlush					EventFlush;
	PEventKill					EventKill;
	PTLOpen						TLOpen;
	PTLClose					TLClose;
	PTLGetInfo					TLGetInfo;
	PTLGetNumInterfaces			TLGetNumInterfaces;
	PTLGetInterfaceID			TLGetInterfaceID;
	PTLGetInterfaceInfo			TLGetInterfaceInfo;
	PTLOpenInterface			TLOpenInterface;
	PTLUpdateInterfaceList		TLUpdateInterfaceList;
	PIFClose					IFClose;
	PIFGetInfo					IFGetInfo;
	PIFGetNumDevices			IFGetNumDevices;
	PIFGetDeviceID				IFGetDeviceID;
	PIFUpdateDeviceList			IFUpdateDeviceList;
	PIFGetDeviceInfo			IFGetDeviceInfo;
	PIFOpenDevice				IFOpenDevice;
	PDevGetPort					DevGetPort;
	PDevGetNumDataStreams		DevGetNumDataStreams;
	PDevGetDataStreamID			DevGetDataStreamID;
	PDevOpenDataStream			DevOpenDataStream;
	PDevGetInfo					DevGetInfo;
	PDevClose					DevClose;
	PDSAnnounceBuffer			DSAnnounceBuffer;
	PDSAllocAndAnnounceBuffer	DSAllocAndAnnounceBuffer;
	PDSFlushQueue				DSFlushQueue;
	PDSStartAcquisition			DSStartAcquisition;
	PDSStopAcquisition			DSStopAcquisition;
	PDSGetInfo					DSGetInfo;
	PDSGetBufferID				DSGetBufferID;
	PDSClose					DSClose;
	PDSRevokeBuffer				DSRevokeBuffer;
	PDSQueueBuffer				DSQueueBuffer;
	PDSGetBufferInfo			DSGetBufferInfo;
	PGCGetNumPortURLs			GCGetNumPortURLs;
	PGCGetPortURLInfo			GCGetPortURLInfo;

	//genapi
	p_mw_genapi_call_command     mw_genapi_call_command;
	p_mw_genapi_set_string       mw_genapi_set_string;
	p_mw_genapi_get_string       mw_genapi_get_string;
	p_mw_genapi_set_integer      mw_genapi_set_integer;
	p_mw_genapi_get_integer      mw_genapi_get_integer;
	p_mw_genapi_set_float        mw_genapi_set_float;
	p_mw_genapi_get_float        mw_genapi_get_float;
	p_mw_genapi_load_ini		 mw_genapi_load_ini;

	p_mw_get_xml				 mw_get_xml;
	p_mw_get_node_info			 mw_get_node_info;
	p_mw_get_enum_array			 mw_get_enum_array;
	p_mw_get_node_state			 mw_get_node_state;
};