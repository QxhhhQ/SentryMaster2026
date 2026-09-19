#pragma once
#include <serial/serial.h>
#include <rclcpp/rclcpp.hpp>

#include <rm_msgs/msg/referee_data.hpp>
using RefereeMsg = rm_msgs::msg::RefereeData;

#include <rm_msgs/msg/chassis_data.hpp>
using ChassisMsg = rm_msgs::msg::ChassisData;

#include <tf2_ros/transform_broadcaster.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

inline void info_log() {std::cout << "\33[0m" << std::endl;}
// 发布绿色信息
template<typename T, typename... Args>
void info_log(T element, Args... args) {
    std::cout << "\33[32m" << element << " "; 
    info_log(args...);         
}

inline void error_log() {std::cout << "\33[0m" << std::endl;}
// 发布红色信息
template<typename T, typename... Args>
void error_log(T element, Args... args) {
    std::cout << "\33[31m" << element << " "; 
    error_log(args...);         
}
