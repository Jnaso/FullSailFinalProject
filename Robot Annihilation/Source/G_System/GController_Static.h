#ifndef GCONTROLLER_STATIC_H
#define GCONTROLLER_STATIC_H
#include "../../Interface/G_System/GControllerInputDefines.h"

#define MAX_CONTROLLER_INDEX 16
#define MAX_XBOX_CONTROLLER_INDEX 4
#define MAX_GENENRAL_INPUTS 20
#define MAX_XBOX_INPUTS 20
#define MAX_PS4_INPUTS 20
#define MAX_XBOX_THUMB_AXIS 32768
#define MIN_XBOX_THUMB_AXIS -32768
#define MAX_PS4_THUMB_AXIS 255
#define MIN_PS4_THUMB_AXIS 0
#define MAX_XBOX_TRIGGER_AXIS 255
#define XINPUT_MAX_VIBRATION 65535
#define MAX_LINUX_THUMB_AXIS 32768
#define MAX_GENENRAL_TRIGGER_AXIS 255
#define GENENRAL_TRIGGER_THRESHOLD 51

#define SONY_VENDOR_ID 0x054C
#define MICROSOFT_VENDOR_ID 0x045E


#define MAX_AXIS 0
#define MIN_AXIS 1
static const int ControllerAxisRanges[2][3] = {
    //Genenral  //Xbox  //PS4
    {MAX_XBOX_THUMB_AXIS, MAX_XBOX_THUMB_AXIS, MAX_PS4_THUMB_AXIS},
    {MIN_XBOX_THUMB_AXIS, MIN_XBOX_THUMB_AXIS, MIN_PS4_THUMB_AXIS},
};

static const unsigned int Mac_ControllerCodes[][3] = {
	  //Genenral      //Xbox		   //PS4
	{ G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 0
    { G_SOUTH_BTN,           G_SOUTH_BTN,          G_WEST_BTN}, // 1
    { G_EAST_BTN,            G_EAST_BTN,           G_SOUTH_BTN}, // 2
    { G_WEST_BTN,            G_WEST_BTN,           G_EAST_BTN}, // 3
    { G_NORTH_BTN,           G_NORTH_BTN,          G_NORTH_BTN}, // 4
    { G_LEFT_SHOULDER_BTN,   G_LEFT_SHOULDER_BTN,  G_LEFT_SHOULDER_BTN}, // 5
    { G_RIGHT_SHOULDER_BTN,  G_RIGHT_SHOULDER_BTN, G_RIGHT_SHOULDER_BTN}, // 6
    { G_LEFT_THUMB_BTN,      G_LEFT_THUMB_BTN,     G_UNKOWN_INPUT}, // 7
    { G_RIGHT_THUMB_BTN,     G_RIGHT_THUMB_BTN,    G_UNKOWN_INPUT}, // 8
    { G_START_BTN,           G_START_BTN,          G_SELECT_BTN}, // 9
    { G_SELECT_BTN,          G_SELECT_BTN,         G_START_BTN}, // 10
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_LEFT_THUMB_BTN}, // 11
    { G_DPAD_UP_BTN,         G_DPAD_UP_BTN,        G_RIGHT_THUMB_BTN}, // 12
    { G_DPAD_DOWN_BTN,       G_DPAD_DOWN_BTN,      G_UNKOWN_INPUT}, // 13
    { G_DPAD_LEFT_BTN,       G_DPAD_LEFT_BTN,      G_UNKOWN_INPUT}, // 14
    { G_DPAD_RIGHT_BTN,      G_DPAD_RIGHT_BTN,     G_UNKOWN_INPUT}, // 15
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 16
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 17
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 18
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 19
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 20
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 21
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 22
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 23
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 24
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 25
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 26
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 27
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 28
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 29
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 30
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 31
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 32
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 33
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 34
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 35
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 36
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 37
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 38
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 39
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 40
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 41
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 42
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 43
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 44
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 45
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 46
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 47
    { G_LX_AXIS,             G_LX_AXIS,            G_LX_AXIS}, // 48
    { G_LY_AXIS,             G_LY_AXIS,            G_LY_AXIS}, // 49
    { G_LEFT_TRIGGER_AXIS,   G_LEFT_TRIGGER_AXIS,  G_RX_AXIS}, // 50
    { G_RX_AXIS,             G_RX_AXIS,            G_LEFT_TRIGGER_AXIS}, // 51
    { G_RY_AXIS,             G_RY_AXIS,            G_RIGHT_TRIGGER_AXIS}, // 52
    { G_RIGHT_TRIGGER_AXIS,  G_RIGHT_TRIGGER_AXIS, G_RY_AXIS}, // 53
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 54
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 55
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 56
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_DPAD_LEFT_BTN /* Dpad value reported as 0-360*/ }, // 57
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 58
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 59
    { G_UNKOWN_INPUT,        G_UNKOWN_INPUT,       G_UNKOWN_INPUT}, // 60
};

static const unsigned int Linux_ControllerCodes[] = {
G_SOUTH_BTN, G_EAST_BTN, G_UNKOWN_INPUT, G_NORTH_BTN,
G_WEST_BTN,  G_UNKOWN_INPUT, G_LEFT_SHOULDER_BTN, G_RIGHT_SHOULDER_BTN,
G_UNKOWN_INPUT, G_UNKOWN_INPUT, G_SELECT_BTN, G_START_BTN,
G_UNKOWN_INPUT, G_LEFT_THUMB_BTN, G_RIGHT_THUMB_BTN
};


#endif
