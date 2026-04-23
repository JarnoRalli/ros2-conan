#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

#include <pcl/io/pcd_io.h>

class PingNode : public rclcpp::Node {
public:
    PingNode() : Node("ping_node") {
        // Instantiate a PCDWriter in order to test dynamic linking
        pcl::PCDWriter writer;
        RCLCPP_INFO(this->get_logger(), "PCL Link Test: %p", (void*)&writer);

        pub_ = this->create_publisher<sensor_msgs::msg::Image>("ping_topic", 10);
        sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "pong_topic", 10, std::bind(&PingNode::callback, this, std::placeholders::_1));
        timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&PingNode::send_ping, this));
    }

private:
    void send_ping() {
        cv::Mat img(100, 100, CV_8UC3, cv::Scalar(0, 255, 0)); // Green square
        auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", img).toImageMsg();
        RCLCPP_INFO(this->get_logger(), "Sending Ping Image...");
        pub_->publish(*msg);
    }

    void callback(const sensor_msgs::msg::Image::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received Pong back!");
    }

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PingNode>());
    rclcpp::shutdown();
    return 0;
}
