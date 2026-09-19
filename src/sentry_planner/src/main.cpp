#include "sentry_planner_node.hpp"

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SentryPlannerNode>("sentry_planner_node"));
    rclcpp::shutdown();
}