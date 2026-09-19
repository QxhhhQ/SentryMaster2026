#pragma once
#include "crc.hpp"
#include <concepts>
#include <rclcpp/rclcpp.hpp>
#define SerialStruct struct __attribute__((packed)) 

inline constexpr uint8_t serial_info_start = 0x7d;
inline constexpr uint8_t serial_info_end = 0x7e;
inline constexpr uint8_t serial_header_size = 2;
inline constexpr uint8_t serial_tail_size16 = 3;
inline constexpr uint8_t serial_tail_size8 = 2;

template <typename T>
std::vector<uint8_t> trans_struct_uint8(uint8_t id, T info, CRCMode mode){
    // 将任意的结构体转换为符合串口通信要求的uint8数组
    std::vector<uint8_t> send_buffer_vec;
    if (mode == CRCMode::crc16){
        send_buffer_vec.resize(serial_header_size + sizeof(T) + serial_tail_size16);
    }else if (mode == CRCMode::crc8){
        send_buffer_vec.resize(serial_header_size + sizeof(T) + serial_tail_size8);
    }

    memcpy(send_buffer_vec.data(), &serial_info_start, sizeof(serial_info_start));
    memcpy(send_buffer_vec.data() + sizeof(serial_info_start), &id, sizeof(id));
    memcpy(send_buffer_vec.data() + serial_header_size, &info, sizeof(info));

    int crc_len = sizeof(info) + sizeof(id);
    uint8_t *buffer = send_buffer_vec.data();
    if (mode == CRCMode::crc16){
        uint16_t crc16 = CRC16::crc16_ccitt.check_sum(buffer + sizeof(serial_info_start), crc_len);
        memcpy(send_buffer_vec.data() + serial_header_size + sizeof(T), &crc16, sizeof(crc16));
        memcpy(send_buffer_vec.data() + serial_header_size + sizeof(T) + sizeof(crc16), &serial_info_end, sizeof(serial_info_end));

    }else if (mode == CRCMode::crc8){
        uint8_t crc8 = CRC8::crc8_maxim.check_sum(buffer + sizeof(serial_info_start), crc_len);
        memcpy(send_buffer_vec.data() + serial_header_size + sizeof(T), &crc8, sizeof(crc8));
        memcpy(send_buffer_vec.data() + serial_header_size + sizeof(T) + sizeof(crc8), &serial_info_end, sizeof(serial_info_end));
    }

    // 对数据进行转义处理,防止数据中出现头尾的uint8
    std::vector<std::pair<size_t, uint8_t>> escape_pairs;
    escape_pairs.clear();
    int has_excape_cnts = 0, buffer_len = send_buffer_vec.size();

    for(int i = 1; i < buffer_len - 1; ++i){
        if(send_buffer_vec[i] == 0x7d || send_buffer_vec[i] == 0x7e || send_buffer_vec[i] == 0x7f){
            // printf("escape %d\n", i);
            escape_pairs.emplace_back(i, (send_buffer_vec[i] - 0x7d));
        }
    }

    for(const auto& ep : escape_pairs){
        send_buffer_vec[ep.first + has_excape_cnts] = 0x7f;
        send_buffer_vec.insert(send_buffer_vec.begin() + ep.first + has_excape_cnts + 1, ep.second);
        has_excape_cnts += 1;
    }
    return send_buffer_vec;
}

inline bool buffer_check_valid(const std::vector<uint8_t> &data, CRC16& checker) {
    uint16_t crc_val;
    int crc_len = data.size() - sizeof(serial_info_start) - serial_tail_size16;
    memcpy(&crc_val, data.data() + data.size() - serial_tail_size16, sizeof(uint16_t));
    uint16_t crc_chk = checker.check_sum(data.data() + sizeof(serial_info_start), crc_len);
    if (crc_chk != crc_val){
        // 不同的crc方式
        crc_chk = checker.check_sum(data.data() + serial_header_size, crc_len - 1);
        // if (crc_chk != crc_val) std::cout << "\33[31mcrc check error\33[0m" << std::endl;
    }
    return crc_chk == crc_val;
}

inline bool buffer_check_valid(const std::vector<uint8_t> &data, CRC8& checker){
    int crc_len = data.size() - sizeof(serial_info_start) - serial_tail_size8;
    uint8_t crc_val = data[data.size() - serial_tail_size8];
    uint8_t crc_chk = checker.check_sum(data.data() + sizeof(serial_info_start), crc_len);
    if (crc_chk != crc_val){
        crc_chk = checker.check_sum(data.data() + serial_header_size, crc_len - 1);
        // if (crc_chk != crc_val) std::cout << "\33[31mcrc check error\33[0m" << std::endl;
    }
    return crc_chk == crc_val;
}

template<typename T>
T trans_uint8_struct(const std::vector<uint8_t> &data, uint8_t id, CRCMode mode){
    T default_msg;
    memset(&default_msg, 0, sizeof(default_msg));

    if (data.size() != sizeof(default_msg) + 5){
        // error_log("data size not match struct!");
        return  default_msg;
    }

    if (mode == CRCMode::crc8){
        if (buffer_check_valid(data, CRC8::crc8_maxim) &&
            data[0] == serial_info_start && data[1] == id && data.back() == serial_info_end){
            T* struct_msg = (T*)(data.data() + serial_header_size);
            return *struct_msg;
        }else {
            return default_msg;
        }
    }else if (mode == CRCMode::crc16){
        if (buffer_check_valid(data, CRC16::crc16_ccitt) &&
            data[0] == serial_info_start && data[1] == id && data.back() == serial_info_end){
            T* struct_msg = (T*)(data.data() + serial_header_size);
            return *struct_msg;
        }else {
            return default_msg;
        }
    }else{
        return default_msg;
    }
}
