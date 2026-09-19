#pragma once 
#include "common.h" 
#include "serial_trans_tool.hpp"
#include <cstdint>

// 接收裁判系统数据包
const uint8_t serial_referee_id = 0x11;
SerialStruct RefereeData{
    uint16_t ph1;
    uint16_t ph2;
    uint16_t ph3;
    uint16_t game_time;
    uint8_t ph4;
    uint16_t ph5;
    uint8_t ph6;
    uint8_t ph7;
    uint8_t ph8;
    // 1/2 得分姿态 3移动姿态
    uint8_t sentry_posture;
    uint8_t ph9;
    uint8_t ph10;
    float ph11;
    float ph12;
};

// 接收底盘数据包
const uint8_t serial_move_id = 0x16;
SerialStruct ChassisData{
    float vx;
    float vy;
    float wz;

    float ph1;
    float ph2;

    int16_t ph3;
    uint8_t ph4;
    uint8_t ph5;
};

// 发送底盘数据包
// ph 标注的变量为占位符，全部置0即可
const uint8_t chassis_control_id = 0x20;
SerialStruct ChassisControl{
    float vx;
	float vy;
	float wz;
    uint8_t ph1;
    // 1/2 得分姿态 3移动姿态
    uint8_t sentry_posture;
    uint8_t ph2;
    uint8_t ph3;
    uint8_t ph4;
};
// 发送方式
// send_serial_packet(
//     trans_struct_uint8<ChassisControl>(chassis_control_id, 一个ChassisControl类型的变量, CRCMode::crc16),
//     "chassis packet"
// );
