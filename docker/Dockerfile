FROM ros:humble

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    python3-pip \
    ros-humble-cv-bridge \
    ros-humble-vision-opencv \
 && rm -rf /var/lib/apt/lists/*

# Install Conan via pip
RUN pip3 install --no-cache-dir conan

CMD ["bash"]