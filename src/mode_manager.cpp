#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <geometry_msgs/msg/twist.hpp>

class CmdVelSelector : public rclcpp::Node {
public:
    CmdVelSelector() : Node("cmd_vel_selector"), current_mode_(0) {
        main_cmd_vel_pub_ = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        mode_sub_ = create_subscription<std_msgs::msg::Int32>("mode", 10, std::bind(&CmdVelSelector::modeCallback, this, std::placeholders::_1));
        manual_cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "manual_cmd_vel", 10,
            std::bind(&CmdVelSelector::manualCmdVelCallback, this, std::placeholders::_1)
        );
        auto_cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "auto_cmd_vel", 10,
            std::bind(&CmdVelSelector::autoCmdVelCallback, this, std::placeholders::_1)
        );
    }

private:
    void modeCallback(const std_msgs::msg::Int32::SharedPtr msg) {
      current_mode_ = msg->data;
      //RCLCPP_INFO(this->get_logger(), "Mode set to: %d", current_mode_);
    }

    void manualCmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        if (current_mode_ == 3) {  
            main_cmd_vel_pub_->publish(*msg);
            //RCLCPP_INFO(this->get_logger(), "manual");
        }
    }

    void autoCmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        if (current_mode_ == 4) { 
            main_cmd_vel_pub_->publish(*msg);
            //RCLCPP_INFO(this->get_logger(), "auto");
        }
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr main_cmd_vel_pub_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr mode_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr auto_cmd_vel_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr manual_cmd_vel_sub_;
    int current_mode_ = 4;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CmdVelSelector>());
    rclcpp::shutdown();
    return 0;
}
