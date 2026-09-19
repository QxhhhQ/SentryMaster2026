import os
from launch import LaunchDescription
from launch.actions import ExecuteProcess, Shutdown, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.descriptions import ComposableNode
from launch_ros.actions import ComposableNodeContainer, Node
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    uart_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([
                FindPackageShare('uart_driver'), 
                'launch',
                'driver.launch.py'
            ])
        ])
    )

    sentry_planner = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([
                FindPackageShare('sentry_planner'), 
                'launch',
                'global_planning.launch.py'
            ])
        ])
    )

    odin1 = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([
                FindPackageShare('odin_ros_driver'), 
                'launch',
                'odin1_ros2.launch.py'
            ])
        ])
    )

    robot_description = Command(
        [
            "xacro ",
            os.path.join(
                get_package_share_directory("sentry_launch"),
                "urdf",
                "sentry_chassis.xacro",
            ),
        ]
    )
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[
            {"robot_description": robot_description, "publish_frequency": 500.0}
        ],
    )

    sentry_decision = Node(
        package='sentry_decision_sys',
        executable='sentry_decision_node',
        name='sentry_decision_node',
        output='both',
    )

    return LaunchDescription([
        # 机器人固定坐标变换发布节点
        robot_state_publisher,
        # USB驱动
        # uart_driver,
        # Odin1驱动
        # odin1,
        # 导航节点
        sentry_planner,
        # 决策节点
        sentry_decision,
    ])