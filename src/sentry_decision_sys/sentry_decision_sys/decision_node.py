import rclpy
from rclpy.node import Node
from tf2_ros import Buffer, TransformListener
from rm_msgs.msg import RefereeData, ChassisData
from nav_msgs.msg import Odometry

class DecisionNode(Node):
    def __init__(self) -> None:
        super().__init__("sentry_decision_node")
        # TF 监听器
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # 订阅话题
        self.referee_suber = self.create_subscription(
            msg_type=RefereeData,
            topic="/referee_data",
            callback=self.referee_callback,
            qos_profile=10,
        )

        self.odom_suber = self.create_subscription(
            msg_type=Odometry,
            topic="/odin1/odometry_highfreq",
            callback=self.odom_callback,
            qos_profile=10,
        )

        self.chassis_suber = self.create_subscription(
            msg_type=ChassisData,
            topic="/chassis_data",
            callback=self.chassis_callback,
            qos_profile=10,
        )

        print("\33[32m sentry_decision_node has been built \33[0m", flush=True)

    def referee_callback(self, msg : RefereeData) -> None:
        pass

    def odom_callback(self, msg : Odometry) -> None:
        pass

    def chassis_callback(self, msg : ChassisData) -> None:
        pass

def cleanup_node(node: Node) -> None:
    node.destroy_node()
    if rclpy.ok():
        rclpy.shutdown()

def spin_node(node: Node) -> None:
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

def main(args=None) -> None:
    rclpy.init(args=args)
    node = DecisionNode()
    try:
        spin_node(node)
    finally:
        cleanup_node(node)
