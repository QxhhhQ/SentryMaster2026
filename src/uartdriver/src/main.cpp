#include "uart_driver_node.h"

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<UARTDriverNode>("uart_driver_node"));
    rclcpp::shutdown();
}