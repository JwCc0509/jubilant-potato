#pragma once
#include "GenTL.h"

#ifdef C_CONV_SIMPLEST
#define CONV_API  __declspec(dllexport)
#else
#define CONV_API __declspec(dllimport)
#endif

#ifdef __cplusplus
    extern "C" {
        namespace GenTL {
            namespace mw_converter {
#endif

                enum GC_CONV_ERROR_LIST
                {
                    GC_CONV_ERR_CUSTOM_ID = -11000,// == GenTL::GC_ERR_CUSTOM_ID 

                    GC_ERR_OPEN_FILE_FAILED = -11001,   //打开文件失败
                    GC_ERR_WRITE_FILE_FAILED = -11002,  //文件无写权限
                    GC_ERR_FILE_EXIST = -11003,         //文件已存在
                };

                //Equal PFNC:PfncFormat
                enum _raw_pixelfmt_list_
                {
                    r_mono8 = 0x01080001,
                    r_mono10 = 0x01100003,
                    r_mono12 = 0x01100005,
                    r_bayer_rg8 = 0x01080009,
                    r_bayer_rg10 = 0x0110000D,
                    r_bayer_rg12 = 0x01100011,
                    //PfncFormat::BayerBG8      //PFNC.h
                    //PfncFormat::BayerGR8      //PFNC.h
                    //PfncFormat::BayerGB8      //PFNC.h
                    //PfncFormat::RGB8          //PFNC.h
                    //PfncFormat::BGR8          //PFNC.h
                };
                typedef int32_t raw_pixelfmt_t;

                enum _conv_file_type_
                {
                    conv_raw = 0,
                    conv_tiff = 1,
                    conv_mim = 2,
                    conv_opencv_bmp = 3,
                };
                typedef int32_t conv_file_type_t;

#   pragma pack(push, 1)
                typedef struct _raw_to_file_param_t
                {
                    raw_pixelfmt_t pixelfmt;
                    uint8_t* p_data;
                    uint64_t    data_len;
                    uint32_t    width;
                    uint32_t    height;
                    conv_file_type_t    conv_file_type;

                    uint8_t     reserve[64];//预留
                } raw_to_file_param_t;
#   pragma pack (pop)

                CONV_API GC_ERROR mw_conv_raw_to_file(raw_to_file_param_t* conv_param, const char* output_filename);

#ifdef __cplusplus
            }
    }
 }
#endif