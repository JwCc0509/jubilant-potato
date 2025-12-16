#ifndef _C_EASY_CAP_SIMPLEST_
#define _C_EASY_CAP_SIMPLEST_
/**************************************************************************************************************************************************************************
*                                                   
* 库名称：     简易编程库 CEasyCapS       
*
* 备注  ：     我们的完整功能库为GenICam GenTL库，符合欧洲机器视觉组织EMVA定义的SDK标准。gentl_capture.cpp中有我们GenTL库的一些调用演示。
*     
* 库层次：     
*              基层 -- 我们的GenTL SDK（安装目录/cti/x86_64/cxplink_gentl.cti）,包含所有功能，以C接口形式提供，具体请参考文档《GenICam_GenTL_1_6.pdf》
*              中层 -- gentl_capture.h 中的CGentlCap类(安装目录/demo/base_dll/CEasyCapS/gentl_capture.h)，适合1~n张卡，1卡1相机模式。封装了GenTL库的功能，简化了使用门槛
*              上层 -- CEasyCapS库，最简易编程库，建立于CGentlCap类和FlexIO GenTL库之上，大大简化了使用门槛。C接口形式提供，满足各种语言接入
* 
* 库目的：     
*              简易编程库CEasyCapS 的目的是帮助客户几行代码高效的完成功能，客户可根据自己的需求，增加扩充CEasyCapS的功能。我们实现了GenICam GenTL库，并开源使用流程，
*              是鼓励用户使用行业标准SDK，挖掘更多的功能需求。同时也可以让用户方便嵌入自己的算法。
*              我们也会挖掘客户的共性需求，扩充CEasyCapS库和CGentlCap类功能，简化客户使用GenTL SDK门槛。
*               
****************************************************************************************************************************************************************************/

#ifdef C_EASY_CAP_SIMPLEST
#define EASY_CAP_API  __declspec(dllexport)
#else
#define EASY_CAP_API __declspec(dllimport)
#endif

#include "GenTL.h"
#include "cxp_standard.h"
#include "load_gentl_cti_ex.h"

#ifdef __cplusplus
using namespace GenTL;
extern "C" {
#endif

    typedef void* CARD_HANDLE;
    typedef void* SCAP_HANDLE;  //简易抓图句柄

    typedef void (*scap_callback_t)(void* p_cb_contex, GenTL::BUFFER_HANDLE h_buf, void* p_base, size_t size);

    /*
    * 初始化库，进程唯一，单进程初始化一次；
    *  @return                  成功，返回true； 失败，通常是已初始化过，或者sdk未安装， 环境变量GENICAM_GENTL64_PATH中无本采集卡的库；或驱动安装失败，需检查驱动；
    */
    EASY_CAP_API bool __stdcall scap_init();
    /*
    * 反初始化库
    */
    EASY_CAP_API void __stdcall scap_done();

    /*
    * 初始化库增强版, 假设同时存在CXP、Camera Link下的编程；可以填入SSDK_CXP、SSDK_CL 等, 释放库需要调用scap_done_ex();
    */
    EASY_CAP_API bool __stdcall scap_init_ex(int32_t flag);
    /*
    * 反初始化库增强版，可以填入SSDK_CXP、SSDK_CL 等
    */
    EASY_CAP_API bool __stdcall scap_done_ex(int32_t flag);

    //设置语言； 0：En     1：Chn
    EASY_CAP_API void __stdcall scap_set_language(int language);

    //@param log_level: "mw_gentl_log.h"::m_log_level_t, 如：MLOG_ERROR
    EASY_CAP_API bool __stdcall scap_init_log(const char* log_folder, uint32_t log_level);

    EASY_CAP_API void __stdcall scap_uninit_log();

    //GenTL.h : TLTypeCXPName=="CXP"
    EASY_CAP_API bool __stdcall scap_get_cti_info(cap_system_info_t* p_info);

    //默认未开seh,开启windows平台SEH功能，用户程序VS工程必须也开启SEH异常，否则异常无法捕获，会导致程序崩溃；
    EASY_CAP_API void __stdcall scap_enable_seh_exception(bool enable=false);



/********************************** GenTL 工业采集最简易程序库(1卡-1相机回调) ************************************/


//-------------------------------------------------------------------
    /**************  1, 打开关闭采集卡和设备接口 ****************/
//-------------------------------------------------------------------
    /*
    *   1.1 一体化打开或关闭采集卡与设备， SCAP_HANDLE属性[index号卡、0号相机]
    */
    //根据采集卡号连接采集卡，并连接0号相机, 获取采集综合操作句柄；
    EASY_CAP_API SCAP_HANDLE __stdcall scap_create_by_card(uint32_t card_index = 0);
    //一体化关闭采集卡以及0号相机资源资源
    EASY_CAP_API bool __stdcall scap_close_by_card(SCAP_HANDLE h_cap);

    /*
    *   1.2 分接口，先打开采集卡，再连接相机设备， SCAP_HANDLE属性[card_index号卡、camera_index号相机]
    */
    //根据采集卡号连接index号采集卡，获取单采集卡操作句柄；
    EASY_CAP_API CARD_HANDLE __stdcall scap_create_card(uint32_t card_index = 0);
    //关闭此采集卡
    EASY_CAP_API bool __stdcall scap_close_card(CARD_HANDLE h_cap);

    //连接此采集卡中的camera_index相机, 获取采集综合操作句柄
    EASY_CAP_API SCAP_HANDLE  __stdcall scap_create_by_camera(CARD_HANDLE h_card, uint32_t camera_index=0);
    //关闭此采集卡中的相机
    EASY_CAP_API bool __stdcall scap_close_by_camera(SCAP_HANDLE h_cap);

//-------------------------------------------------------------------
    /**************  2, 配置读取xml参数接口 ****************/
//-------------------------------------------------------------------
    //使用配置文件配置参数；设置采集卡、设备、相机的参数,具体参考“doc/客户端Flex Io Viewer采集方法_V1.0.1.pdf”
    EASY_CAP_API bool __stdcall scap_set_param_by_file(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* config_file);

    //宽字节路径（中文路径）
    EASY_CAP_API bool __stdcall scap_set_param_by_wfile(SCAP_HANDLE h_cap, scap_level_t cap_level, const wchar_t* config_wfile);

    //使用string命令， 配置采集卡、设备、相机的所有类型参数,具体参考“doc/客户端Flex Io Viewer采集方法_V1.0.1.pdf”
    EASY_CAP_API bool __stdcall scap_set_string(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* pFeature, const char* pStr);
    //使用string命令， 读取采集卡、设备、相机的所有类型参数,具体参考“doc/客户端Flex Io Viewer采集方法_V1.0.1.pdf”
    EASY_CAP_API bool __stdcall scap_get_string(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* pFeature, char* pStr, size_t* piSize);

    //设置ommand类型参数, 设置采集卡、设备、相机的command类型参数,通常权限是只写；具体参考“doc/客户端Flex Io Viewer采集方法_V1.0.1.pdf”
    EASY_CAP_API bool __stdcall scap_set_command(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* pFeature);

    //设置整形参数， 设置采集卡、设备、相机的整形参数
    EASY_CAP_API bool __stdcall  scap_set_integer(SCAP_HANDLE h_cap, scap_level_t cap_level,const char* pFeature, int64_t iValue);
    //读取整形参数， 读取采集卡、设备、相机的整形参数
    EASY_CAP_API bool __stdcall  scap_get_integer(SCAP_HANDLE h_cap, scap_level_t cap_level,const char* pFeature, int64_t* piValue);

    //设置浮点型参数， 设置采集卡、设备、相机的浮点型参数
    EASY_CAP_API bool __stdcall  scap_set_float  (SCAP_HANDLE h_cap, scap_level_t cap_level,const char* pFeature, double fValue);
    //读取浮点型参数， 读取采集卡、设备、相机的浮点型参数
    EASY_CAP_API bool __stdcall  scap_get_float  (SCAP_HANDLE h_cap, scap_level_t cap_level,const char* pFeature, double* pfValue);

    //设置register型参数
    EASY_CAP_API bool __stdcall scap_get_register(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* pFeature, char* pStr, size_t* piSize);
    //读取register型参数
    EASY_CAP_API bool __stdcall scap_set_register(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* pFeature, const char* pStr, size_t* piSize);
    
//-------------------------------------------------------------------
    /**************  3, 获取信息辅助接口 ****************/
//-------------------------------------------------------------------
    //获取错误信息
    EASY_CAP_API GC_ERROR scap_get_last_error(SCAP_HANDLE h_cap);
    EASY_CAP_API GC_ERROR scap_get_last_error_info(char* sErrText, size_t* piSize);

    //获取采集卡数量
    EASY_CAP_API uint32_t __stdcall scap_get_card_num(); 
    //获取相机数量
    EASY_CAP_API uint32_t __stdcall scap_get_device_num(CARD_HANDLE h_cap, uint32_t card_index);    

    //打开卡前，根据卡index号，获取卡信息, 获取信息index范围：[0, card_num-1]
    EASY_CAP_API bool __stdcall scap_preread_card_info(uint32_t card_index, gentl_if_info_t* p_pre_if_info);
    //打开相机前，根据设备index号， 获取信息范围：[0, device_num-1]
    EASY_CAP_API bool __stdcall scap_preread_device_info(CARD_HANDLE h_card, uint32_t dev_index, gentl_dev_info_t* p_pre_dev_info);

    //获取相机信息(常用)
    EASY_CAP_API bool __stdcall scap_get_camera_info(SCAP_HANDLE h_cap, cxp_std_features_t* p_camera_info, bool8_t fast_mode = true);
    //获取流信息(常用)
    EASY_CAP_API bool __stdcall scap_get_data_stream_info(SCAP_HANDLE h_cap, cxp_ds_info_t* p_ds_info, bool8_t fast_mode = true);
    //获取流信息(GenTL1.6+自定义)
    //choise_mode： 从低位到高位，1标记p_ds_info的对应index参数可获取， 0标记对应不获取,默认0xFFFF，都获取
    EASY_CAP_API bool __stdcall scap_get_data_stream_info_ex(SCAP_HANDLE h_cap, gentl_ds_info_t* p_ds_info, int32_t choise_mode = 0xFFFF, bool8_t fast_mode = true);
    //获取buf相关信息(宽、高、大小、格式等)(常用)
    EASY_CAP_API bool __stdcall scap_get_buf_info(SCAP_HANDLE h_cap, BUFFER_HANDLE h_buf, cxp_buf_info_t* buf_info, bool8_t fast_mode = true);

//-------------------------------------------------------------------
    /**************  4, 控制取流接口 ****************/
//-------------------------------------------------------------------
    /*
    * 4.1回调式取流， 在scap_callback_t回调函数中获取buf
    */
    EASY_CAP_API bool __stdcall scap_set_callback_param(SCAP_HANDLE h_cap, cxp_callback_param_t* p_cb_param);
    //开启采集（回调式），内部封装开启取流线程
    EASY_CAP_API bool __stdcall scap_start_capture(SCAP_HANDLE h_cap, scap_callback_t frame_cb, void* p_cb_context, uint32_t buf_count, uint64_t cap_count = -1, bool with_control=false);
    //停止采集(回调式)， 内部封装关闭取流线程
    EASY_CAP_API bool __stdcall scap_stop_capture(SCAP_HANDLE h_cap);

    /*
    * 4.2 直取式拉流
    */
    //开启采集（直取式），内部无线程
    EASY_CAP_API bool __stdcall scap_start(SCAP_HANDLE h_cap, uint32_t buf_count);
    //关闭采集（直取式），内部无线程
    EASY_CAP_API bool __stdcall scap_stop(SCAP_HANDLE h_cap);
    //获取数据（直取式），给出指针和长度,未拷贝.注意与开启、停止多线程时加锁
    EASY_CAP_API bool __stdcall scap_get_buf(SCAP_HANDLE h_cap, uint64_t i_timeout, BUFFER_HANDLE* p_hbuf, void** pp_base, size_t* p_buf_size);
    //获取到buffer后，使用完buf, 需要调用此接口重新排队进采集卡；
    EASY_CAP_API bool __stdcall scap_back_buffer(SCAP_HANDLE h_cap, BUFFER_HANDLE h_buf);

    /*    
    * GenTL联动
    */
    EASY_CAP_API bool __stdcall scap_get_port(SCAP_HANDLE h_cap, scap_level_t gentl_level, PORT_HANDLE* p_port);

    EASY_CAP_API bool __stdcall scap_get_gentl(SCAP_HANDLE h_cap, scap_sdk_t sdk_mode, CStdLoadCtiEx** p_gentl_api);
    //EASY_CAP_API bool __stdcall scap_get_gentl_ex(SCAP_HANDLE h_cap, int32_t flag, CStdLoadCti** p_gentl_api_ex);

    /********************************** GenTL 工业采集最简易程序库(1卡-多相机) ************************************/

#ifdef __cplusplus
}
#endif

#endif//_C_EASY_CAP_SIMPLEST_