cmake_minimum_required(VERSION 3.10)
project(ping)

# Find ROS 2 packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)

# Find Conan packages
find_package(yaml-cpp REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(node src/ping_node.cpp)

# Link everything
target_include_directories(node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(node
  rclcpp::rclcpp
  sensor_msgs::sensor_msgs_library
  cv_bridge::cv_bridge
  yaml-cpp::yaml-cpp
  Eigen3::Eigen
)

# Install the targets
install(TARGETS
  node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
