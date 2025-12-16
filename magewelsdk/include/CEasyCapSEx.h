#pragma once
#include "CEasyCapS.h"
#include "mw_genapi_producer.h"
#include "mw_gentl_log.h"
#include "mw_gentl_producer.h"
/*
* Cap补充/定制化接口
* 1，定制化为主
* 2，暂不会放入CEasyCapS.h中；
*/


#ifdef __cplusplus
using namespace GenTL;
using namespace mw_genapi;
using namespace mw_gentl_log;
using namespace mw_custom;

extern "C" {
#endif
    //-------------------------------------------------------------------
    /**************  1, 打开关闭采集卡和设备接口 (补充)****************/
    //-------------------------------------------------------------------
    //查找设备（前置函数 scap_create_card）使用get_device_info可以查找更多信息，待讨论
    EASY_CAP_API uint32_t get_device_count(CARD_HANDLE h_card);

    //新增，打开相机前提前获取信息



    //-------------------------------------------------------------------
    /**************   2, 配置读取xml参数接口 (补充) ****************/
    //-------------------------------------------------------------------
    //获取xml/zip文件

    //GenApi辅助接口
    EASY_CAP_API bool __stdcall scap_get_xml(SCAP_HANDLE h_cap, scap_level_t cap_level, char* xml_buf, size_t* psize);

    /*
    * @param p_node_info:  feature 类型、访问模式、最小值、最大值、步进值等
    * return
    */
    EASY_CAP_API bool __stdcall scap_get_node_info(SCAP_HANDLE h_cap, scap_level_t cap_level, const char* node_name, mw_node_info_t* p_node_info);

    EASY_CAP_API bool scap_create_buffer(size_t need_size, char** p_buf);

    EASY_CAP_API bool scap_destroy_buffer(char* p_buf);

    //-------------------------------------------------------------------
    /**************  4, 控制取流接口 (补充)****************/
//-------------------------------------------------------------------

    EASY_CAP_API bool8_t scap_user_malloc(SCAP_HANDLE h_cap, uint64_t buf_size, uint32_t buf_count);
    EASY_CAP_API void    scap_user_free(SCAP_HANDLE h_cap);

    EASY_CAP_API bool8_t scap_set_start_param(SCAP_HANDLE h_cap, cap_start_stream_param_t* pparam);

    //labview 3维数据输入， DsAnnounceBuffer, 输入输出
    EASY_CAP_API bool8_t scap_register_vi_buffer(SCAP_HANDLE h_cap, uint8_t* buf_array, uint64_t all_size, uint32_t num);
    EASY_CAP_API bool8_t scap_unregister_vi_buffer(SCAP_HANDLE h_cap);
    EASY_CAP_API bool8_t scap_get_vi_one_buffer(SCAP_HANDLE h_cap, uint64_t i_timeout, BUFFER_HANDLE* p_hbuf, uint32_t* p_index);

    //labview 2维数据输入，被拷贝数据后，输出
    EASY_CAP_API bool8_t scap_copy_one_buffer(SCAP_HANDLE h_cap, uint64_t i_timeout, uint32_t* p_array, size_t* p_buf_size);

    //cuda buffer input, DsAnnounceBuffer
    EASY_CAP_API bool8_t scap_register_cuda_buffer(SCAP_HANDLE h_cap, sim_cuda_data_t* p_cuda_data_array, uint32_t buf_count);
    EASY_CAP_API bool8_t scap_unregister_cuda_buffer(SCAP_HANDLE h_cap);
    //EASY_CAP_API bool8_t scap_get_cuda_one_buffer(SCAP_HANDLE h_cap, uint64_t i_timeout, BUFFER_HANDLE* p_hbuf, uint32_t* p_index);

    //user malloc input, DsAnnounceBuffer
    EASY_CAP_API bool8_t scap_register_user_buffer(SCAP_HANDLE h_cap, sim_user_data_t* p_user_data_array, uint32_t buf_count);
    EASY_CAP_API bool8_t scap_unregister_user_buffer(SCAP_HANDLE h_cap);
    //EASY_CAP_API bool8_t scap_get_user_one_buffer(SCAP_HANDLE h_cap, uint64_t i_timeout, BUFFER_HANDLE* p_hbuf, uint32_t* p_index);

#ifdef __cplusplus
}
#endif