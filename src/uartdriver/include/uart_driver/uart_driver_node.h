#pragma once
#include "common.h"
#include <thread>
#include "transfer.h"

// 串口驱动节点
class UARTDriverNode : public rclcpp::Node{ 
private:
    std::thread read_thread;
    std::string serial_name;
    std::vector<uint8_t> possible_ids;

    int baud_rate = 0;
    int read_buffer_size;
    bool is_serial_normal = false;

    std::shared_ptr<Transfer> uart_transfer;
    std::shared_ptr<serial::Serial> serial_port;
    
    void init_node();
    void read_from_port();
    void reopen_port();

public:
    UARTDriverNode(std::string name);
};