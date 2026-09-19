import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    driver_config = os.path.join(
        get_package_share_directory("uart_driver"),
        "config",
        "driver_config.yaml",
    )

    uart_driver_node = Node(
        package='uart_driver',
        executable='uart_driver_node',
        name='uart_driver_node',
        parameters=[driver_config],
        output='both'
    )
    
    return LaunchDescription([
        uart_driver_node
    ])