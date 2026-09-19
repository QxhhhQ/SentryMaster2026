cmake_minimum_required(VERSION 3.8)
project(uart_driver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-O3)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖
find_package(rclcpp REQUIRED)
find_package(rm_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

# 查找当前目录及子目录中的所有.cpp文件
file(GLOB_RECURSE SRC_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

set(MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM SRC_FILES ${MAIN_SRC})

# 添加库文件
set(LIB_NAME ${PROJECT_NAME}_lib)
add_library (${LIB_NAME} ${SRC_FILES})

# 添加include文件
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
)

# 添加库依赖
ament_target_dependencies(
    ${LIB_NAME}
    rclcpp
    rm_msgs
    tf2_ros
    tf2_geometry_msgs
)

# # 生成自带的msg消息
# rosidl_generate_interfaces(${PROJECT_NAME}
#   "msg/Serial.msg"
# )

# # 给当前节点添加自带的msg依赖
# rosidl_target_interfaces(${LIB_NAME} ${PROJECT_NAME} "rosidl_typesupport_cpp")

# # 安装库文件 保证其他节点可以使用自定义消息类型
# install(
#   TARGETS ${LIB_NAME}
#   EXPORT export_${PROJECT_NAME}
#   ARCHIVE DESTINATION lib
#   LIBRARY DESTINATION lib
#   RUNTIME DESTINATION bin
# )

# # 导出库文件
# ament_export_include_directories(include)
# ament_export_libraries(${LIB_NAME})
# ament_export_dependencies(rclcpp)

# 生成一个可执行文件来封装库 保证launch可以使用Node调用节点
set(EXE_NAME ${PROJECT_NAME}_node)
add_executable(${EXE_NAME} ${MAIN_SRC})
target_link_libraries(${EXE_NAME} ${LIB_NAME})
install(TARGETS ${EXE_NAME} DESTINATION lib/${PROJECT_NAME})

# 安装头文件 参数文件 启动脚本
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

ament_package()
