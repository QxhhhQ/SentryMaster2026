#pragma once
#include <rclcpp/rclcpp.hpp>

#include <tf2/utils.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/create_timer_ros.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_eigen/tf2_eigen.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include <nav_msgs/msg/odometry.hpp>
using OdomMsg = nav_msgs::msg::Odometry;

#include "rm_msgs/msg/chassis_data.hpp"
using ChassisDataMsg = rm_msgs::msg::ChassisData;

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

enum SentryPosture{
    ATTACK = 1,
    DEFENSE = 2,
    MOVE = 3
};
