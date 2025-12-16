#ifndef _CXPCARD_H_
#define _CXPCARD_H_

#include "CEasyCapS.h"
#include <string>
using namespace std;
typedef struct _bg_CardFeature
{
    std::string    FWMode={"AABB"};
    int64_t     IDeviceSelector=0;
    std::string    SetCxpLinkConfiguration={"Auto"};

}bg_CardFeature_t;

class BgCxpCard
{
public:
    BgCxpCard();
    ~BgCxpCard();
    std::string   CardName={"test name"};
    CARD_HANDLE    CardHandle ;
    PORT_HANDLE    hdevice0 = NULL;
    PORT_HANDLE    hdevice1 = NULL;
    PORT_HANDLE    hdevice2 = NULL;
    PORT_HANDLE    hdevice3 = NULL;
    std::string   GetCardName(void);
    bool   BgCardCreat(void);
    void   BgCardDestroy(void);
    bool   SetCardFeature(bg_CardFeature_t CardFeatureVaule);
    CARD_HANDLE    GetCardHandle(std::string CardName);
    CARD_HANDLE    GetCardHandle(int index);
    CARD_HANDLE    GetDeviceHandle(int index);
    uint32_t    GetDeviceNumber(void);
private:
    bool    CardIsCreat=false;
    gentl_if_info_t*    CardInfo;
    bg_CardFeature_t    CardFeature;
    uint32_t     DeviceNumber=0;


};

#endif//_MV_CAMERA_H_
