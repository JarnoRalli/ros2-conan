#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

class PongNode : public rclcpp::Node {
public:
    PongNode() : Node("pong_node") {
        // We listen to the "ping" and reply to the "pong"
        pub_ = this->create_publisher<sensor_msgs::msg::Image>("pong_topic", 10);
        sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "ping_topic", 10, std::bind(&PongNode::callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Pong Node has started, waiting for Ping...");
    }

private:
    void callback(const sensor_msgs::msg::Image::SharedPtr msg) {
        try {
            // 1. Convert the ROS Image to OpenCV Mat
            // toCvCopy makes a deep copy so we can modify it
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");

            // 2. "Process" the image: Let's turn the Green square into a Blue one
            // and write "PONG" on it to prove we handled it
            cv_ptr->image.setTo(cv::Scalar(255, 0, 0)); // Blue
            cv::putText(cv_ptr->image, "PONG",
                        cv::Point(20, 60),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8,
                        cv::Scalar(255, 255, 255), 2);

            RCLCPP_INFO(this->get_logger(), "Received Ping, modifying to Blue and sending Pong back!");

            // 3. Convert back to ROS message and publish
            pub_->publish(*cv_ptr->toImageMsg());

        } catch (cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        }
    }

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PongNode>());
    rclcpp::shutdown();
    return 0;
}
