# 保证libusb库的最新版可以被找到
export LD_PRELOAD=/usr/local/lib/libusb-1.0.so

# 获取livox消息类型
sudo chmod 777 /dev/ttyACM0
cd ..
source install/setup.bash
ros2 launch sentry_launch chassis.launch.py





