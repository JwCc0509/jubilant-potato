#include "cxpCard.h"
#include  <qDebug>
#include <QMessageBox>

BgCxpCard::BgCxpCard()
{
    scap_init();
    qDebug()<<"cxp card cti lib init ";
    //打开SEH异常！！！这里设置为true后，请在编译配置里开启SEH异常，否则正常的c++抛异常不能捕获，且会崩溃报错！
 //   scap_enable_seh_exception("true");

    //打开日志
//    scap_init_log("C:/mw_log", mw_gentl_log::MLOG_ALL);

    //设置信息语言为英文
    scap_set_language(MLAN_EN);

}
BgCxpCard::~BgCxpCard()
{
    qDebug()<<"cxp card lib destroy ";
    if(CardIsCreat)
    {
       BgCardDestroy();
    }
    scap_done();
}

bool BgCxpCard::BgCardCreat()
{
    if(CardIsCreat)
    {
        return true;
    }
    int card_index = 0;
    uint32_t card_num= scap_get_card_num();
    qDebug()<<"have "<<card_num <<"card";

    qDebug()<<"the BgCxpCard class get the Card hanele address:"<<CardHandle;
    if (!CardHandle)
    {
        QMessageBox::warning(0,"警告","打开采集卡失败！");
        qDebug()<<"Cann`t find card";
        return false;
    }

    // gentl_if_info_t* CardInfo = new(std::nothrow) gentl_if_info_t[0];
    // if (!CardInfo)
    // {
    //     qDebug()<<QString::asprintf("Malloc gentl_if_info_t Error\n");
    //     return -2;
    // }

    // if (false == scap_preread_card_info(0,  CardInfo  ))
    // {
    //     qDebug()<<QString::asprintf("Get card%d info error\n", 0);
    // }
    // else
    // {
    //     qDebug()<<QString::asprintf("Get card%d id: %s\n", 0, CardInfo->if_id);
    // }

    // CardName=CardInfo->if_id;

//    qDebug()<<"the card name :"<<CardName;
    bool b_res = false;
    CardHandle = scap_create_card(card_index);


    int64_t ivalue = 0;
    scap_get_integer(CardHandle, scap_interface, "FirmwareMaxDevices", &ivalue);

    //设置0号卡为"AAAA"模式,单设备-单相机模式
//    b_res = scap_set_string(CardHandle, scap_interface, "FirmwareMode", CardFeature.FWMode.c_str());
    b_res = scap_set_string(CardHandle, scap_interface, "FirmwareMode", "AAAA");
    qDebug()<<"Ser(FirmwareMode):bool res = "<< b_res;
    //并配置device0连接速率为"Auto"，实际编程建议“Default”模式，强检查线路路数
//    b_res = scap_set_integer(CardHandle, scap_interface, "DeviceSelector", CardFeature.IDeviceSelector);//选择0号设备
    b_res = scap_set_integer(CardHandle, scap_interface, "DeviceSelector", 0);
    qDebug()<<"Ser(DeviceSelector):bool res = "<< b_res;

    int configcount=0;
    do
    {
      //  b_res = scap_set_string(CardHandle, scap_interface, "CxpLinkConfiguration", "Auto");//连接速率设置为Auto
//        b_res = scap_set_string(CardHandle, scap_interface,
//                 "CxpLinkConfiguration", CardFeature.SetCxpLinkConfiguration.c_str());//连接速率设置为Auto

        b_res = scap_set_string(CardHandle, scap_interface,
                  "CxpLinkConfiguration","Auto");//连接速率设置为Auto

        if(b_res==NULL)   {
            QMessageBox::warning(0,"警告","CxpLinkConfiguration CXP12模式配置失败");
            configcount++;
            qDebug()<<"配置cxp速度失败"<<configcount;
        }  else  {
            qDebug()<<"init the card is sucess:bool res = "<< b_res;
            break;
        }
        if(configcount>10) {

            return false;
        }

        qDebug()<<"Ser(CxpLinkConfiguration):bool res = "<< b_res;
    }
    while(true);
    DeviceNumber=scap_get_device_num(CardHandle,0);

    qDebug()<<"this card have "<<DeviceNumber<<"device *****";

    CardIsCreat=true;
    return true;
}
void BgCxpCard::BgCardDestroy()
{
    if(CardIsCreat)
        scap_close_card(CardHandle);//关掉卡
    CardIsCreat=false;
    return ;
}
bool BgCxpCard::SetCardFeature(bg_CardFeature_t CardFeatureVaule)
{
    CardFeature=CardFeatureVaule;
    bool b_res = false;

    b_res = scap_set_string(CardHandle, scap_interface, "FirmwareMode", CardFeature.FWMode.c_str());

    qDebug()<<"Ser(FirmwareMode):bool res = "<< b_res;
    //并配置device0连接速率为"Auto"，实际编程建议“Default”模式，强检查线路路数
    b_res = scap_set_integer(CardHandle, scap_interface, "DeviceSelector", CardFeature.IDeviceSelector);//选择0号设备
    qDebug()<<"Ser(DeviceSelector):bool res = "<< b_res;

    int configcount=0;
    do
    {
        //  b_res = scap_set_string(CardHandle, scap_interface, "CxpLinkConfiguration", "Auto");//连接速率设置为Auto
        b_res = scap_set_string(CardHandle, scap_interface,
                                "CxpLinkConfiguration", CardFeature.SetCxpLinkConfiguration.c_str());//连接速率设置为Auto
        if(b_res==NULL)   {
            QMessageBox::warning(0,"警告","CxpLinkConfiguration CXP12模式配置失败");
            configcount++;
            qDebug()<<"配置cxp速度失败"<<configcount;
        }  else  {
            qDebug()<<"init the card is sucess:bool res = "<< b_res;
            break;
        }
        if(configcount>10) {

            return false;
        }

        qDebug()<<"Ser(CxpLinkConfiguration):bool res = "<< b_res;
    }
    while(true);
    return true;
}
CARD_HANDLE  BgCxpCard::GetCardHandle(std::string CardName)
{
    return CardHandle;

}
CARD_HANDLE    BgCxpCard::GetCardHandle(int index)
{
    return CardHandle;
}

CARD_HANDLE    BgCxpCard::GetDeviceHandle(int index)
{
    qDebug()<<" get the device handle address******";

    switch  (index){
        case 0:
            hdevice0 = scap_create_by_camera( CardHandle, index);
            if(hdevice0==NULL)
            {
                qDebug()<<"open device0 fail";
                return NULL;
            }
            qDebug()<<"the device0 address:"<<hdevice0;
            return hdevice0;
        case 1:
            hdevice1 = scap_create_by_camera( CardHandle, index);
            if(hdevice1==NULL)
            {
                qDebug()<<"open device1 fail";
                return NULL;
            }
            qDebug()<<"the device1 address:"<<hdevice1;
            return hdevice1;
        case 2:
            hdevice2 = scap_create_by_camera( CardHandle, index);
            if(hdevice2==NULL)
            {
                qDebug()<<"open device2 fail";
                return NULL;
            }
            qDebug()<<"the device2 address:"<<hdevice2;
            return hdevice2;
        case 3:
            hdevice3 = scap_create_by_camera( CardHandle, index);
            if(hdevice3==NULL)
            {
                qDebug()<<"open device3 fail";
                return NULL;
            }
            qDebug()<<"the device3 address:"<<hdevice3;
            return hdevice3;
    }
    return NULL;


}
uint32_t  BgCxpCard::GetDeviceNumber(void)
{
    DeviceNumber=scap_get_device_num(CardHandle,0);
    return DeviceNumber;
}


std::string BgCxpCard::GetCardName(void)
{
    return CardName;
}
