"""Launch the sentry decision behaviour-tree node."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description() -> LaunchDescription:
    """Create the launch description for the decision node."""

    return LaunchDescription(
        [
            Node(
                package="sentry_decision_sys",
                executable="sentry_decision_node",
                name="sentry_decision_node",
                output="both",
            ),
        ]
    )
