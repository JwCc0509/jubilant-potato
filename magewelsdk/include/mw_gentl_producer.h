#pragma once
#include "GenTL.h"

#ifdef __cplusplus
extern "C" {
    namespace GenTL {
        namespace mw_custom {
#endif
            typedef enum _custom_cxp_speed_ {
               CUSTOM_CXP_1 = 0x28,     //1.250
               CUSTOM_CXP_2 = 0x30,     //2.500
               CUSTOM_CXP_3 = 0x38,     //3.125
               CUSTOM_CXP_5 = 0x40,     //5.000
               CUSTOM_CXP_6 = 0x48,     //6.250
               CUSTOM_CXP_10 = 0x50,    //10.000
               CUSTOM_CXP_12 = 0x58     //12.500
            } custom_cxp_speed_t;

            enum _custom_link_speed_
            {
                CUSTOM_CXP_DEFAULT = 0x00,
                CUSTOM_CXP_AUTO = 0x01,
                CUSTOM_CXP1_X1 = 0x010000 + CUSTOM_CXP_1,//296
                CUSTOM_CXP3_X1 = 0x010000 + CUSTOM_CXP_3,//312
                CUSTOM_CXP6_X1 = 0x010000 + CUSTOM_CXP_6,//328
                CUSTOM_CXP10_X1 = 0x010000 + CUSTOM_CXP_10,//336
                CUSTOM_CXP12_X1 = 0x010000 + CUSTOM_CXP_12,//344
                CUSTOM_CXP1_X2 = 0x020000 + CUSTOM_CXP_1,//552
                CUSTOM_CXP3_X2 = 0x020000 + CUSTOM_CXP_3,//568
                CUSTOM_CXP6_X2 = 0x020000 + CUSTOM_CXP_6,//584
                CUSTOM_CXP10_X2 = 0x020000 + CUSTOM_CXP_10,//592
                CUSTOM_CXP12_X2 = 0x020000 + CUSTOM_CXP_12,//600
                CUSTOM_CXP1_X3 = 0x030000 + CUSTOM_CXP_1,//808
                CUSTOM_CXP3_X3 = 0x030000 + CUSTOM_CXP_3,//824
                CUSTOM_CXP6_X3 = 0x030000 + CUSTOM_CXP_6,//840
                CUSTOM_CXP10_X3 = 0x030000 + CUSTOM_CXP_10,//848
                CUSTOM_CXP12_X3 = 0x030000 + CUSTOM_CXP_12,//856
                CUSTOM_CXP1_X4 = 0x040000 + CUSTOM_CXP_1,//1064
                CUSTOM_CXP3_X4 = 0x040000 + CUSTOM_CXP_3,//1080
                CUSTOM_CXP6_X4 = 0x040000 + CUSTOM_CXP_6,//1096
                CUSTOM_CXP10_X4 = 0x040000 + CUSTOM_CXP_10,//1104
                CUSTOM_CXP12_X4 = 0x040000 + CUSTOM_CXP_12,//1112

                //...   0x000N0000+BR_CXP
            };
            typedef uint32_t custom_link_speed_t;

            enum _event_custom_type_
            {
                //EVENT_CUSTOM_BASE_BASE = EVENT_CUSTOM_ID,                                 //EVENT_CUSTOM_ID + [1, 999]
                //...
                //EVENT_CUSTOM_SYS_BASE = EVENT_CUSTOM_ID + 1000,                           //EVENT_CUSTOM_ID + [1001, 1999]
                //...
                //EVENT_CUSTOM_IF_BASE = EVENT_CUSTOM_ID + 2000,                            //EVENT_CUSTOM_ID + [2001, 2999]
                //...
                //EVENT_CUSTOM_DEVICE_BASE = EVENT_CUSTOM_ID + 3000,                        //EVENT_CUSTOM_ID + [3001, 3999]， Device+Remote Device
                EVENT_CUSTOM_CXP_CAMERA_ONLINE                  = EVENT_CUSTOM_ID+3001,     //DEVICE: CXP/COF 相机上线事件
                //EVENT_CUSTOM_CXP_CAMERA_OFFLINE               = EVENT_CUSTOM_ID+3002,     //DEVICE: CXP/COF 相机下线事件
                EVENT_CUSTOM_CXP_CAMERA_LINK_LOST               = EVENT_CUSTOM_ID+3003,     //DEVICE: CXP/COF 相机链接丢失错误事件(其中主链接丢失，直接下线)
                EVENT_CUSTOM_CXP_CAMERA_CONNECT_SUCCESS         = EVENT_CUSTOM_ID+3004,     //DEVICE: CXP/COF 相机链接成功事件
                EVENT_CUSTOM_CXP_CAMERA_CONNECT_FAILED          = EVENT_CUSTOM_ID+3005,     //DEVICE: CXP/COF 相机链接失败事件
           
                //EVENT_CUSTOM_DS_BASE = EVENT_CUSTOM_ID + 5000,                
            };
            typedef int32_t event_custom_type_t;

            /* Event data info command */
            enum _event_data_custom_type_
            {
                // EVENT_DATA_CUSTOM_DEVICE_BASE = EVENT_CUSTOM_ID + 3000,
                EVENT_DATA_CUSTOM_CXP_1 = EVENT_DATA_CUSTOM_ID + 3001,  //
            };
            typedef int32_t event_data_custom_type_t;

            enum  _custom_buffer_info_cmd_list_
            {
                //BUFFER_INFO_CUSTOM_ID = 1000 /* Starting value for GenTL Producer custom IDs. */
                BUFFER_INFO_CUSTOM_THREAD_ID = 1001,   //UINT32, Thread ID for creating this buffer
            };
            typedef int32_t custom_buffer_info_cmd_list_t;

            //在打开设备后，online启动; 另外offline掉线后，只要设备在,会启动循环检查;
#pragma pack(push, 1)
            typedef struct _cxp_camera_online_info_
            {
                uint64_t        timestamp;      //时间戳ns
                DEV_HANDLE      h_device;       //事件设备句柄
                bool8_t         is_online;      //是否在线

                uint8_t         reserve[64];//预留
            }cxp_camera_online_info_t;
#   pragma pack (pop)

            //在打开设备链接相机成功弄后，会启动循环检查,错误只报1次
#pragma pack(push, 1)
            typedef struct _camera_linklost_info_
            {
                uint64_t        timestamp;      //时间戳ns
                DEV_HANDLE      h_device;       //事件设备句柄
                uint32_t        link_status;    //错误链接状态, 从最低位开始，[0,3]对应了采集卡的A、B、C、D通道；值为1表示出现断线

                uint8_t         reserve[64];//预留
            }cxp_camera_linklost_info_t;
#   pragma pack (pop)

            //在打开设备链接相机成功弄后，启动
#pragma pack(push, 1)
            typedef struct _camera_link_info_
            {
                uint64_t        timestamp;      //时间戳ns
                DEV_HANDLE      h_device;       //事件设备句柄
                uint32_t        link_status;    //暂时为0
                bool8_t         is_online;      //是否在线
                char            speed_str[32];  //当前实际链接速率字符串
                uint32_t        speed;          //当前实际链接速率custom_link_speed_t

                uint8_t         reserve[64];//预留
            }cxp_camera_link_info_t;
#   pragma pack (pop)

#pragma pack(push, 1)
            typedef struct _dev_lost_info_
            {
                EVENTSRC_HANDLE handle;
                uint32_t        lost_connect_index; //0~7;

                uint8_t         reserve[64];//预留
            }devlost_info_t;
#   pragma pack (pop)
            
            //EVENT_CUSTOM_CAMERA_LOST，对应 p_out_info 为 devlost_info_t*
            typedef void (__stdcall *event_callback_t)(void* p_cb_contex, void* p_out_info);

            /*
            *   Mark the fifo that needs to be released
            *   param@ hDataStream: data stream handle
            *   param@ thread_id:   The thread number for creating this fifo, if(thread_id==0), do nothing
            *   return: GC_ERR_SUCCESS(hDataStream is avaliable)
            */
            GC_API vp_fifo_free_start(DS_HANDLE hDataStream, uint32_t thread_id);

            /*
            *   Abort the fifo that needs to be released, if you have not DSRevokeBuffer, you can use this fifo again
            *   param@ hDataStream: data stream handle
            *   param@ thread_id:   The thread number for creating this fifo, if(thread_id==0), do nothing
            *   return: GC_ERR_SUCCESS(hDataStream is avaliable)
            */
            GC_API vp_fifo_free_cancel(DS_HANDLE hDataStream, uint32_t thread_id);
#ifdef __cplusplus
        }
    }
}
#endif