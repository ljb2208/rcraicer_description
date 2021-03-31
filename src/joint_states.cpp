#include "../include/rcraicer_description/joint_states.h"

using std::placeholders::_1;

JointStates::JointStates() : Node("arduino_controller")
{
    jsPublisher = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);
    csSubscriber = this->create_subscription<rcraicer_msgs::msg::ChassisState>(
      "chassis", 10, std::bind(&JointStates::cs_callback, this, std::placeholders::_1));   // add queue size in later versions of ros2       
    
    tfBroadcaster = std::make_shared<tf2_ros::TransformBroadcaster>((std::shared_ptr<JointStates>)this);
    publishJointStates();
}

JointStates::~JointStates()
{

}

void JointStates::publishJointStates()
{
    sensor_msgs::msg::JointState msg = sensor_msgs::msg::JointState();    
    msg.header.stamp = rclcpp::Node::now();
    msg.name = {"left_steering_joint", "right_steering_joint"};
    msg.position = {0.0, 0.0};

    jsPublisher->publish(msg);
}

void JointStates::cs_callback(const rcraicer_msgs::msg::ChassisState::SharedPtr msg)
{
    std::cout << msg->throttle << "\n";

    publishJointStates();    
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JointStates>());
    rclcpp::shutdown();
    return 0;
}