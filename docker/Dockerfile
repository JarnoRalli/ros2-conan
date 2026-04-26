FROM ros:humble

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    python3-pip \
    ros-humble-cv-bridge \
    ros-humble-vision-opencv \
 && rm -rf /var/lib/apt/lists/*

# Install Conan via pip
RUN pip3 install --no-cache-dir "conan>=2.0,<3.0"

# Detect profile
RUN conan profile detect --force

# Automatically source ROS 2
RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc

CMD ["bash"]
