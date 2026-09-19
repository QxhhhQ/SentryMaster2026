#include "uart_driver_node.h"

UARTDriverNode::UARTDriverNode(std::string name) : Node(name){
    init_node();
    info_log(name, "has been built!");
}

void UARTDriverNode::init_node(){
    // 初始化串口
    declare_parameter("serial_number", "");
    std::string serial_number = get_parameter("serial_number").as_string();

    declare_parameter("serial_name", "");
    std::string serial_name_pre = get_parameter("serial_name").as_string();

    declare_parameter("baud_rate", 0);
    baud_rate = get_parameter("baud_rate").as_int();

    declare_parameter("read_buffer_size", 0);
    read_buffer_size = get_parameter("read_buffer_size").as_int();

    declare_parameter("possible_ids", std::vector<int>());
    auto int_ids = get_parameter("possible_ids").as_integer_array();

    // info_log(int_ids.size());
    for (auto int_id : int_ids){
        uint8_t id = static_cast<uint8_t>(int_id);
        possible_ids.push_back(id);
        // info_log((int)id);
    }

    try {
        bool is_find_serial_number = false;
        std::vector<serial::PortInfo> devices_found = serial::list_ports();
        for (const auto& port : devices_found) {
            if (port.hardware_id.find(serial_number) != std::string::npos) {
                serial_name = port.port;
                is_find_serial_number = true;
                info_log("match port name: ", serial_name);
                break;
            }
        }
        if (!is_find_serial_number){
            serial_name = serial_name_pre;
        }

        serial_port = std::make_shared<serial::Serial>(serial_name, baud_rate, serial::Timeout::simpleTimeout(1000));
        if (!serial_port->isOpen()) {
            serial_port->open();
        }
        is_serial_normal = true;
        info_log("open ", serial_name, "in ", baud_rate);
        uart_transfer = std::make_shared<Transfer>(this, serial_port);
        read_thread = std::thread(&UARTDriverNode::read_from_port, this);
    } catch (const std::exception& ex) {
        error_log("Error creating serial port: ", serial_number, ex.what());
        throw ex;
    }
}

void UARTDriverNode::read_from_port() {
    std::vector<uint8_t> read_buffer(read_buffer_size);
    uint8_t *buffer = read_buffer.data();

    int no_serial_data = 0; 
    int read_data_size = 0;

    while (rclcpp::ok()) {
        try {
            if (!is_serial_normal) {
                rclcpp::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            serial_port->read(buffer, 1);
            if (buffer[0] == serial_info_start) {
                read_data_size = 1;
                serial_port->read(buffer + 1, 1);

                if (std::find(possible_ids.begin(), possible_ids.end(), buffer[1]) != possible_ids.end()) {
                    uint8_t *read_end = buffer + 1;
                    read_data_size++;
                    bool have_convert = false;

                    while ((have_convert || read_end[0] != serial_info_end) && read_data_size < read_buffer_size) {
                        have_convert = false;
                        ++read_end;
                        serial_port->read(read_end, 1);

                        if (read_end[0] == 0x7f) {
                            serial_port->read(read_end, 1);
                            read_end[0] = read_end[0] + 0x7d;
                            have_convert = true;
                        }
                        ++read_data_size;
                    }

                    if (read_end[0] == serial_info_end) {
                        std::vector<uint8_t> data;
                        data.reserve(read_data_size);
                        for (int i = 0; i < read_data_size; i++) {
                            data.push_back(read_buffer[i]);
                        }
                        uart_transfer->handle_datas(data);
                        no_serial_data = 0;
                        continue;
                    }
                }
                // 解析出来start但是没有数据
                no_serial_data++;
            }

            if (no_serial_data > 5) {
                error_log("no serial data....");
                no_serial_data = 0;
                is_serial_normal = false;
                serial_port->close();
                reopen_port();
            }
        } catch (const std::exception &ex) {
            error_log("Error while receiving data: ", ex.what());
            is_serial_normal = false;
            if (serial_port && serial_port->isOpen()) {
                serial_port->close();
            }
            reopen_port();
        }
    }
}

void UARTDriverNode::reopen_port(){
    error_log("Attempting to reopen port");
    try {
        std::string cmd = "sudo chmod 666 " + serial_name + " 2>/dev/null";
        int ret = system(cmd.c_str());

        serial_port->open();
        is_serial_normal = true;
        info_log("Successfully reopened port");
    } catch (const std::exception& ex) {
        error_log("Error while reopening port: ", ex.what());
        if (rclcpp::ok()) {
            rclcpp::sleep_for(std::chrono::seconds(1));
            reopen_port();
        }
    }
}
