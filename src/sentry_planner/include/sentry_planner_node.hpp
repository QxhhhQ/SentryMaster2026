#include "common.h"

class SentryPlannerNode : public rclcpp::Node {
public:
    SentryPlannerNode(std::string name);

private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_suber;

    rclcpp::Subscription<ChassisDataMsg>::SharedPtr chassis_data_suber;

    // 坐标变换接收者
    std::shared_ptr<tf2_ros::Buffer> tf_buffer;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener;
    
    void ChassisInfoCallback(const ChassisDataMsg::SharedPtr msg);
    void OdomCallBack(const nav_msgs::msg::Odometry::SharedPtr msg);
};
