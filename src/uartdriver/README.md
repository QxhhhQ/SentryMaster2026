# UART串口通信驱动

## 使用教程
1. 正确地打开串口
- 在命令行输入 `ls /dev/tty*`查找串口对应的文件，例如找到了**dev/ttyUSB0**

- 命令行输入`sudo udevadm info --query=all --name=/dev/ttyUSB0`查找串口文件的详细信息

- 找到**ID_SERIAL_SHORT=c6371f124814eb119da4437c2d17fe54**信息所在位置，将后面的序列号复制保存到config文件里面
