from pathlib import Path

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def _create_planner_node(context):
    params_file = LaunchConfiguration("params_file").perform(context)
    parameters = [
        params_file,
    ]


    return [
        Node(
            package="sentry_planner",
            executable="sentry_planner_node",
            name="sentry_planner_node",
            output="screen",
            parameters=parameters,
        )
    ]


def generate_launch_description():
    planner_share = FindPackageShare("sentry_planner")

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "params_file",
                default_value=PathJoinSubstitution(
                    [planner_share, "config", "global_planning.yaml"]
                ),
                description="Path to the planner parameter file.",
            ),
            OpaqueFunction(function=_create_planner_node),
        ]
    )
