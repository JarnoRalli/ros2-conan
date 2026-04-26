[![build](https://github.com/JarnoRalli/ros2-conan/actions/workflows/build.yml/badge.svg?branch=main&event=push)](https://github.com/JarnoRalli/ros2-conan/actions/workflows/build.yml)
[![pre-commit](https://github.com/JarnoRalli/ros2-conan/actions/workflows/pre-commit.yml/badge.svg?branch=main&event=push)](https://github.com/JarnoRalli/ros2-conan/actions/workflows/pre-commit.yml)

# ROS2 with Conan

This repository studies how Conan can be used for managing 3rd party libraries with the ROS 2 framework. The challenge
we are trying to solve is how to build, link against, and distribute 3rd party C/C++ libraries that are used in ROS 2 applications.
Conan manages building and caching of C/C++ libraries, it does not natively support packaging of libraries in debian or related packages.
This does not mean that it cannot be used as part of a toolchain used for packaging C/C++ libraries, but this requires additional
steps and tools.

Using Conan with the ROS 2 framework is complicated by the fact that there is some overlap between how ROS 2 and Conan
handle 3rd party libraries. The situation is further complicated when vendors like NVIDIA distribute optimized binary
versions of libraries such as OpenCV for the Jetson platform. The tool used for managing the 3rd party libraries should support the following:

* Building and maintaining different versions of the same library:
  * Different architectures, e.g. x86_64, arm and arm64 need to be supported
  * Different build switches need to be supported (e.g. release vs debug)
  * ABI compatibility must be maintained
* Libraries where only binary artifacts exist need to be supported
* The tool must integrate with the ROS 2 build system based on `colcon`
  * ROS 2 C/C++ packages use CMake/Ament
* Libraries built with the tool need to be linkable using CMake
  * Conan uses generators that product the required scaffolding for several different builds system

Conan uses a combination of profile files and `conanfile.py` in order to deduce what libraries and which versions, and
what build-system and compiler flags are used to build the libraries. Following is an example of a profile file

```shell
[settings]
arch=x86_64
build_type=Release
compiler=gcc
compiler.cppstd=gnu17
compiler.libcxx=libstdc++11
compiler.version=13
os=Linux

[options]
*:shared=True
```

## 1 Consistent Library Versions

Conan uses files called `conanfile.py` to define which libraries the software being built depends on. Following shows a typical ROS 2
workspace.

```shell
ros2_ws/
├── project.repos
├── conanfile.py
└── src/
    ├── ping/
    │   ├── include/
    │   │   └── ping_node/
    │   │       └── ping.hpp
    │   ├── src/
    │   │   └── ping_node.cpp
    │   ├── CMakeLists.txt
    │   ├── package.xml
    │   └── conanfile.py
    └── pong/
        ├── include/
        │   └── pong_node/
        │       └── pong.hpp
        ├── src/
        │   └── pong_node.cpp
        ├── CMakeLists.txt
        ├── package.xml
        └── conanfile.py
```

The file `project.repos` contains the information required to clone the `ping` and `pong` repositories
into the `src/` folder. Since `ping` and `pong` are being developed in their respective repositories, they need
to be testeable separately (in their own repos). This simply means that both `ping` and `pong` need to handle
their own dependencies when being built and tested independently. This means that we have three separate build and test contexts:
* `ros2_ws`: builds and tests everything.
* `ping`: builds and tests `ping` separately.
* `pong`: builds and tests `pong` separetely.

In order to achieve this, each of the build contexts have their own `conanfile.py` that defines the dependencies.
This creates a problem since each of the Conan files can define different versions of the same library, like OpenCV, leading
to poor testeability of the system. For example, `pong` might build and pass the tests when tested in its own repo (due to
having correct dependencies), whereas it might fail to even build in the `ros2_ws` context. Another problem is that having
to define the library versions consistenly across different files increases cognitive load and the probability of an error.

There are at least two different ways to tackle the problem:
* Using [lockfiles](https://docs.conan.io/2/tutorial/versioning/lockfiles.html)
* Using a central Conan package called `robot_base_config` that defines the versions

In the approach based on a central Conan package (`robot_base_config`) if we change the version of a dependency in that package,
then the version is changed everywhere. Following section shows what `robot_base_config` would look like.

### 1.1 Robot Base Config

`robot_base_config` is a normal Conan package with the following files:

```shell
robot_base_config
├── conanfile.py
└── version_logic.py
```

Contents of `conanfile.py` is as follows:

```python
from conan import ConanFile

class RobotBaseConfig(ConanFile):
    name = "robot_base_config"
    version = "1.0"

    # This tells Conan which files to bring from your disk into the recipe
    exports = "version_logic.py"

# Import the class into the global namespace of this file
from version_logic import RobotVersions
```

Contents of `version_logic.py` is as follows:

```python
import os
from typing import Dict

class RobotVersions:
    @staticmethod
    def get_versions() -> Dict[str, str]:
        ros_distro: str = os.getenv("ROS_DISTRO", "humble")
        if ros_distro == "kilted":
            return {
                "eigen": "3.4.0",
                "pcl": "1.14.1",
                "nlohmann_json": "3.11.3"
            }

        if ros_distro == "humble":
            return {
                "eigen": "3.4.0",
                "pcl": "1.14.1",
                "nlohmann_json": "3.11.3"
            }

        # Default fallback for rolling or unknown distributions
        return {
            "eigen": "3.4.0",
            "pcl": "1.14.1",
            "nlohmann_json": "3.11.3"
        }
```

All the Conan files in that particular project would use the version numbers defined in the `RobotVersions`. Following is
an example how this works in practice.

```python
from conan import ConanFile
from conan.tools.files import copy
import os

class PingNode(ConanFile):
    name = "ping_node"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    python_requires = "robot_base_config/1.0@einherjar/stable"

    def requirements(self) -> None:
        """
        Define requirements by pulling pinned versions from the central config.
        """
        versions = self.python_requires["robot_base_config"].module.RobotVersions.get_versions()

        self.requires(f"pcl/{versions['pcl']}")
        self.requires(f"eigen/{versions['eigen']}")
        self.requires(f"nlohmann_json/{versions['nlohmann_json']}")
```

Bumping up a version number would happen in the `robot_base_config` package.

## 2. How to Test

Following are the instructions for testing this approach in Ubuntu 24.04 with ROS 2 Kilted. First you need to install
C/C++ compiler so that Conan can detect which compiler you are using.

### 2.1 Install Conan

In Ubuntu 24.04 you can install Conan by running the following commands:

```bash
sudo apt update
sudo apt install pipx -y
pipx ensurepath
pipx install conan
conan profile detect
```

### 2.2 Install ROS 2 Kilted

Instructions for installing ROS Kilted can be found [https://docs.ros.org/en/kilted/Installation.html](here).

### 2.3 Install CMake

Install CMake as per these [instructions](https://cmake.org/download/).

### 2.4 Create the `robot_base_config` Package

Create the `robot_base_config` package in local cache (no Conan server required) as per the instructions at [./conan/README.md](./conan/README.md).

### 2.5 Build and Run

Build the and the nodes are per the instructions at [ros2_ws](./ros2_ws/README.md).

## 3. Repo Contents

Contents are as follows:

* [artifactory/](./artifactory/README.md)
  * This directory contains instructions for running Artifactory (Community Edition) locally.
* [conan/](./conan/README.md)
  * This directory contains instructions for running Conan (Community Edition) locally.
   * `robot_base_config` Conan package.
* [docker/](./docker/README.md)
  * Instructions for testing out the code in Raspberry Pi 5 using a Docker container.
* [license](./LICENSE)
  * BSD 3-Clause License file.
* [ros2_ws](./ros2_ws/README.md)
  * ROS 2 workspace with the nodes.
