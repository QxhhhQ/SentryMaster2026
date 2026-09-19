cmake_minimum_required(VERSION 3.8)
project(sentry_planner)

# 设置编译标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-O3)

# 查找依赖
find_package(PCL REQUIRED)
find_package(fmt REQUIRED)
find_package(tf2 REQUIRED)
find_package(ompl REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(rclcpp REQUIRED)
find_package(message_filters REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rm_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system serialization thread date_time regex timer chrono)

# 查找当前目录及子目录中的所有.cpp文件
file(GLOB_RECURSE SRC_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
)

# 添加库文件
set(LIB_NAME ${PROJECT_NAME}_lib)
add_library (${LIB_NAME} ${SRC_FILES})

# 添加include文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加库依赖
ament_target_dependencies(
    ${LIB_NAME}
    tf2
    PCL
    Boost
    rclcpp
    message_filters
    Eigen3
    OpenCV
    rm_msgs
    tf2_ros
    nav_msgs
    std_msgs
    tf2_eigen
    pcl_conversions
    tf2_geometry_msgs
    visualization_msgs
)

target_link_libraries(${LIB_NAME} ${OMPL_LIBRARIES} fmt)

# 生成一个可执行文件来封装库 保证launch可以使用Node调用节点
set(EXE_NAME ${PROJECT_NAME}_node)
add_executable(${EXE_NAME} ${SRC_FILES})
target_link_libraries(${EXE_NAME} ${LIB_NAME})
install(TARGETS ${EXE_NAME} DESTINATION lib/${PROJECT_NAME})

# 安装头文件 参数文件 启动脚本
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY config launch map DESTINATION share/${PROJECT_NAME})

ament_package()
