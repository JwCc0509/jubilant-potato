#pragma once
#include "GenTL.h"

#   pragma pack (push, 1)
//GenTL标准接口层信息,在连接接口卡前即可获取
typedef struct _gentl_if_info_
{
	//GenTL标准参数，在链接接口前即可获取
	char if_id[128];				/* INTERFACE_INFO_CMD::INTERFACE_INFO_ID = 0,    STRING     Unique ID of the interface. */
	char if_display_name[128];		/* INTERFACE_INFO_CMD::INTERFACE_INFO_DISPLAYNAME = 1,STRING     User readable name of the interface. */
	char if_tltype[32];				/* INTERFACE_INFO_CMD::INTERFACE_INFO_TLTYPE = 2，STRING     Transport layer technology that is supported. */
	//自定义
	uint32_t IfIndex;				//Pcie识别index号
	int64_t	 MaxIfNum;				//最大卡数
	uint32_t BoardIndex;			//卡硬件拨号号码(预留)
	bool8_t	 IsAvailable;			//是否可用
	//...
	char reserve[256];				//Reserve
}gentl_if_info_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
typedef struct _gentl_dev_info_
{
	//GenTL标准参数，在链接设备前即可获取
	char	 DeviceID[16];				//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_ID
	char	 DeviceVendorName[32];    	//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_VENDOR
	char	 DeviceModelName[32];     	//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_MODEL
	char	 TlType[16];				//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_TLTYPE
	char	 DeviceDisplayName[128];	//				     DEVICE_INFO_CMD::DEVICE_INFO_DISPLAYNAME
	uint32_t AccessStatus;				//					 DEVICE_INFO_CMD::DEVICE_INFO_ACCESS_STATUS
	char	 DeviceUserID[16];			//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_USER_DEFINED_NAME
	char	 DeviceSerialNumber[16];  	//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_SERIAL_NUMBER
	char	 DeviceVersion[32];			//CXP Protocol Size, DEVICE_INFO_CMD::DEVICE_INFO_VERSION
	uint64_t TsFrequency;				//					 DEVICE_INFO_CMD::DEVICE_INFO_TIMESTAMP_FREQUENCY
	
	//自定义基础
	uint32_t DevIndex;			//当前设备index
	int64_t  MaxDevNum;			//最大设备数	
	bool8_t	 IsAvailable;		//是否可用

	//CXP独有, 待实现
	uint32_t CxpLinkConfiguration;			//0："Default"  1:"Auto"    0x10048:"CXP6_X1"  0x20048:"CXP6_X2"      0x40048:"CXP6_X4"  0x40058 : "CXP12_X4"
	//CL独有, 待实现
	uint32_t CLLinkConfiguration;			//0:"Unknown Protocol" 1:"GenCP"
	//...
	char reserve[256];					//Reserve
}gentl_dev_info_t;
#   pragma pack (pop)

//流信息扩充版信息（GenTL标准+自定义信息）
//GenTL标准数据流层信息，开启流通道后即可获取(DSGetInfo)
#   pragma pack (push, 1)
typedef struct _gentl_ds_info_
{
	//GenTL标准参数
	// STREAM_INFO_ID = 0,							/* STRING 	(M)  Unique ID of the data stream. */
	// STREAM_INFO_NUM_DELIVERED = 1,				/* UINT64 	(O)  Number of delivered buffers since last acquisition start. */
	// STREAM_INFO_NUM_UNDERRUN = 2,				/* UINT64 	(O)  Number of lost buffers due to queue underrun. */
	// STREAM_INFO_NUM_ANNOUNCED = 3,				/* SIZET	(O)  Number of announced buffers. */
	// STREAM_INFO_NUM_QUEUED = 4,					/* SIZET    (O)  Number of buffers in the input pool. */
	// STREAM_INFO_NUM_AWAIT_DELIVERY = 5,			/* SIZET    (O)  Number of buffers in the output queue. */
	// STREAM_INFO_NUM_STARTED = 6,					/* UINT64   (O)  Number of buffers started in the acquisition engine. */
	// STREAM_INFO_PAYLOAD_SIZE = 7,				/* SIZET    (CM)  Size of the expected data in bytes. */
	// STREAM_INFO_IS_GRABBING = 8,					/* BOOL8    (M)  Flag indicating whether the acquisition engine is started or not. */
	// STREAM_INFO_DEFINES_PAYLOADSIZE = 9,			/* BOOL8    (M)  Flag that indicated that this data stream defines a payload size independent from the remote device. */
	// STREAM_INFO_TLTYPE = 10,						/* STRING   (M)  Transport layer technology that is supported. */
	// STREAM_INFO_NUM_CHUNKS_MAX = 11,				/* SIZET    (CM)  Max number of chunks in a buffer, if known. GenTL v1.3 */
	// STREAM_INFO_BUF_ANNOUNCE_MIN = 12,			/* SIZET    (M)  Min number of buffers to announce before acq can start, if known. GenTL v1.3 */
	// STREAM_INFO_BUF_ALIGNMENT = 13,				/* SIZET    (O)  Buffer alignment in bytes. GenTL v1.3 */
	// STREAM_INFO_FLOW_TABLE = 14,					/* BUFFER   (CM)  Flow mapping table in GenDC format. GenTL v1.6 */
	// STREAM_INFO_GENDC_PREFETCH_DESCRIPTOR = 15,  /* BUFFER   (O)  Prefetch version of GenDC descriptor. GenTL v1.6 */
	char		StreamInfoID[16];				//(M)  StreamInfoID
	uint64_t	NumDelevered;					//(O)  未实现
	uint64_t	NumUnderrun;					//(O)  丢失的帧数
	size_t		NumAnnounced;					//(O)  申明绑定的缓存数量（输入队列数+输出队列数+未归还缓存数量）
	size_t		NumQueued;						//(O)  输入队列，当前引擎可用未填充缓存数
	size_t		NumAwaitDelivery;				//(O)  输出队列，当前可用的已完成填充缓存
	uint64_t	NumStarted;						//(O)  未实现
	size_t		PayloadSize;					//(CM) 1帧缓存的大小
	bool8_t		IsGrabbing;						//(M)  当前流抓取是否开始运行
	bool8_t		DefinesPayloadSize;				//(M)  SDK是否提供了PayloadSize
	char		TlType[16];						//(M)  SDK的GenTL类型
	size_t		NumChunksMax;					//(CM) 未实现
	size_t		BufAnnounceMin;					//(M)  流需要绑定的最小缓存数量
	size_t		BufAlignment;					//(O)  未实现
	char*		FlowTable;						//(CM) 未实现
	char*		GenDCPrefetchDescriptor;		//(O)  未实现

	//自定义(预留)
	uint64_t	NumSDKGet;			//未实现
	uint64_t	NumUserGet;			//未实现
	uint64_t	NumReServe;			//未实现
	//...
	char reserve[256];				//Reserve
}gentl_ds_info_t;
#   pragma pack (pop)


//远程设备（相机）的功能寄存器
//成员不能有std::string等标准库类型，因构造函数中使用memset初始化为0,会破坏std库
#   pragma pack (push, 1)
typedef struct _cxp_std_features_
{
	//相机只读信息，从genapi接口读取
	char device_vendor_name[128];		//相机公司,如"Adimec" 
	char device_model_name[128];		//相机型号，如"Q-12A180-Fm/CXP-6"
	char device_serial_number[128];		//相机序列号，如"100001"
	//相机可写配置项，从genapi接口读取
	int64_t width;		//宽	
	int64_t height;		//高
	int64_t format;		//图像格式数值
	char format_str[128];	//图像格式，字符串版（兼容性强），如"Mono8" 对应的值，有的相机参照cxp标准，有的相机参照PFNC标准。直接使用名称兼容性强
	int64_t offset_x;	//ROI OffsetX 偏移
	int64_t offset_y;	//ROI OffsetY 偏移

	//...
}cxp_std_features_t;
#   pragma pack (pop)

//回调控制信息
#   pragma pack (push, 1)
typedef struct _cxp_callback_param_
{
	uint64_t i_timeout;
	//....
}cxp_callback_param_t;
#   pragma pack (pop)

//流信息
#   pragma pack (push, 1)
typedef struct _cxp_ds_info_
{
	size_t	num_announced;			//申明缓存数量
	size_t	num_queued;				//当前排队数量
	size_t	payload_size;			//实际出流一帧大小
	bool8_t	is_grabbing;			//当前是否开启抓流
	bool8_t	defines_payloadsize;	//厂家是否定义了payload_size, true则payload_size由生产者提供
	size_t	announce_min;			//用户需要申明内存的最小值
}cxp_ds_info_t;
#   pragma pack (pop)


//buf信息
#   pragma pack (push, 1)
typedef struct _cxp_buf_info_
{
	//_cxp_buf_info_& operator= (const _cxp_buf_info_& item) {
	//	memcpy(this, &item, sizeof(_cxp_buf_info_));
	//	return *this;
	//}
	//相机层实时可改参数，fast mode模式为true下不访问
	int64_t	offset_x;
	int64_t	offset_y;
	//buf中
	int64_t width;			//宽	
	int64_t height;			//高
	int64_t format;			//图像格式数值

	//功能性
	char*		p_base;			//数据指针
	void*		p_private;		//私有指针
	size_t		buf_size;		//数据大小
	uint64_t	timestamp;		//时间戳
	uint64_t	frame_id;		//帧编号
	char		tl_type[32];	//tl类型
}cxp_buf_info_t;
#   pragma pack (pop)

//命令格式寄存器使能值;
typedef enum _command_gentl_
{
	CMD_GENTL_START = 1
}command_gentl_t;

//自定义相机类型
typedef enum _typedef_camera_
{
	ADIMEC_Q_12A180 = 0,
	DAHUA_AX5A22MX050 = 1,
	HAIKANG_CXP12 = 2,
	BASLER =3,
	VIEWORKS=4,
	BOPIXEL=5,
	JAI=6,
	MIK=7
}typedef_camera_t;

typedef enum _flex_io_mask_
{
	IIN11 = 0,
	IIN12 = 1,
	IIN13 = 2,
	IIN14 = 3,
	EXT_IIN11 = 4,
	EXT_IIN12 = 5,
	EXT_IIN13 = 6,
	EXT_IIN14 = 7,
	DIN11 = 8,
	DIN12 = 9,
	TTLIO11 = 10,
	TTLIO12 = 11,
	IO_NULL = 0xFF
}flex_io_mask_t;

////采集卡功能寄存器1, io control
#   pragma pack (push, 1)
typedef struct _device_io_control
{
	uint32_t			DeviceTriggerMode;					//触发模式，	0:NULL 1：软触发模式   2：硬触发模式
	double				DeviceExposureTime;					//曝光时间/脉宽, unit: 1us 
	double				DeviceFramePeriod;					//触发周期/触发脉宽 , unit: 1us
	command_gentl_t		DeviceTriggerStart;					//点击触发按钮
	uint32_t			DevicePulseNumber;					//点击DeviceTriggerStart后，产生的触发次数， =0时代表循环自动触发
	char				CardioInMask[64];					//硬件io 输入接口选择, 虽然是enum类型，但我们可以使用mw_genapi_set_string来通过字符串形式调用（严格区分大小写、空格）
	char				CardioOutMask[64];					//硬件io 输出接口选择,常用 IOUT11\IOUT12, 使用TTLIO11\TTLIO12需要CardioOutMaskOpen 开启;
	char				CardioOutMaskOpen[64];				//TTLIO11\TTLIO12从输入型接口转换成输出型；默认NULL代表不转换;
	char				CardioOutMode[64];					//Software_trigger_out软件触发转发；Hardware_trigger_out是硬件触发转发
	double				CardioGlitchTime;					//去毛刺，单位1us， 建议1us以上;
	char				SoftioTriggerReverse[8];			//软触发脉冲倒转，"OFF"\"ON"
	char				HardioTriggerReverse[8];			//硬触发脉冲倒转
	char				CardioOutReverse[8];				//硬件io输出脉冲倒转
	double				SoftioTriggerDelay;					//软触发延时,unit: 1us
	double				HardioTriggerDelay;					//硬触发延时,unit: 1us
	double				CardioOutDelay;						//硬件io输出脉冲延时，unit: 1us
}device_io_control_t;
#   pragma pack (pop)

//采集卡功能寄存器2
#   pragma pack (push, 1)
typedef struct _card_features_
{
	uint32_t mode;					//触发模式，//1：软触发模式   2：硬触发模式

	//软触发参数
	double pulse_width;				//曝光时间, unit: 1us
	double pulse_period;			//脉冲周期, unit: 1us
	uint32_t pulse_number;			//单次点击使能命令脉冲个数，设置为0，代表一直循环发送;
	command_gentl_t load_desc_en;	//点击1次使能命令，触发pulse_number个脉冲。pulse_number为0时，使能命令默认循环自动触发；

	//硬触发参数
	
	flex_io_mask_t	io_mask;		//输入接线选择;
	double glitch;					//去毛刺，单位us（微秒,精度0.1us）;建议参数1.0us
}card_features_t;
#   pragma pack (pop)

typedef enum  _cap_level
{
	CAP_SYSTEM = 0,
	CAP_CARD = 1,
	CAP_CAMERA = 2,
}cap_level_t;


typedef enum _cap_tl_info_
{
	info_id = 0,                    //id, [string, 开卡前可知]
	info_displayname = 1,           //显示名称, string型
	info_xml = 2,                   //xml文件, string型
	info_vendor = 3,                //供应商名, string型
	info_model = 4,                 //型号名称, string型

	//----------系统层分界线-----------（系统分为CXP系统、CameraLink、Gige10等）
	info_system_custom = 1000,
	info_system_pathname = 1001,    //加载的cti库目录地址, string型

	//----------接口层分界线-----------（接口数即卡的数量，一种系统可以存在多张同种卡）
	info_if_custom = 1100,
	info_if_num = 1101,             //接口数量， [uint32_t, 开卡前可知]

	//----------设备层分界线-----------（设备数即1张卡当前最多支持的设备相机数，由接口层固件和）
	info_device_custom = 1200,
	info_device_num = 1201,          //设备数量， [uint32_t, 开卡前可知]

	//----------设备-远程相机分界线---- (1个设备有连接相机、未连接相机两种状态)
	info_camera_custom = 1300,
	info_camera_xml = 1301,

	//----------数据流分界线----------- (数据流数即一个设备最多支持的流数，当前SDK支持1相机1个流)
	info_stream_custom = 1400,
	info_stream_num = 1401          //流数量， [uint32_t, 开卡前可知]

}cap_tl_info_t;

#   pragma pack (push, 1)
typedef struct _cap_system_info_
{
	char cti_url[1024];
	size_t cti_url_len;	
	char tl_type_name[128];		//TLTypeCXPName,TLTypeCLName
	size_t cti_type_len;

	char reserve[64];		//预留
}cap_system_info_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
typedef struct _cap_start_stream_param_
{
	uint32_t start_mode;	//0:auto AcquisitionStart/stop	1:user control 
	uint32_t malloc_mode;	//0:sdk malloc					1:user malloc

	char reserve[128];		//预留
}cap_start_stream_param_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
typedef struct _sim_cuda_data_
{
	void*	addr;			//cuda host alloc
	size_t	size;
	void*	private_addr;	//cuda device Pointer
}sim_cuda_data_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
typedef struct _sim_user_data_
{
	int16_t		func_flag;		//0xFFFF: sim_user_data_t	0xFFFE: ....
	void*		addr;			//user host alloc
	size_t		size;			//buffer size

	//reserve----------------------
	uint32_t	index;			//not use
	uint32_t	team_index;		//group
	bool8_t		is_available;	//can be use
	bool8_t		is_dma_reged;	//is used by dma

	char		reserve[120];	//reserve
}sim_user_data_t;
#   pragma pack (pop)

typedef enum scap_level {
	scap_interface = 0,        //采集卡层
	scap_device = 1,        //设备层
	scap_camera = 2,        //远程设备层（相机层）
	scap_data_stream = 3         //流层
} scap_level_t;

typedef enum _scap_sdk_ {
	SSDK_NULL = 0,
	SSDK_CXP = 0x01,
	SSDK_CL = 0x02,
	SSDK_COF = 0x04,
	SSDK_ALL = 0xFF
} _scap_sdk_t;
typedef int32_t scap_sdk_t;

