# ROS2 Workspace

## 1. Build the Nodes

In order to make sure that all ROS 2 related dependencies etc. are available, you need to call the setup function:

```bash
source /opt/ros/<ROS-VERSION>/setup.bash
```

, replace `<ROS-VERSION>` with the ROS version that you are using.

Build and install the 3rd party libraries using Conan:

```bash
conan install . --output-folder=build --build=missing \
  -c tools.system.package_manager:mode=install \
  -c tools.system.package_manager:sudo=True \
  -s build_type=Release
```

After that you can build the ROS 2 nodes:

```bash
colcon build \
  --cmake-args \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=$(pwd)/build/conan_toolchain.cmake
```

Before executing the software, you need to make sure that the dynamic library loader finds the Conan versions of the
libraries by executing the following

## 2. Start Pong Node

```bash
cd src/pong/
source ./build/conanrun.sh
source ./install/setup.bash
ros2 run pong node
```

## 3. Start Ping Node

```bash
cd src/ping/
source ./build/conanrun.sh
source ./install/setup.bash
ros2 run ping node
```
