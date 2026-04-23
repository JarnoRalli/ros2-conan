# Ping Node

Install Conan packages:

```bash
conan install . --output-folder=build --build=missing \
  -c tools.system.package_manager:mode=install \
  -c tools.system.package_manager:sudo=True \
  -s build_type=Release
```

Build with Colcon:

```bash
colcon build \
  --cmake-args \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=$(pwd)/build/conan_toolchain.cmake
```

Before executing the software, you need to make sure that the dynamic library loader finds the Conan versions of the
libraries by executing the following

```bash
source build/conanrun.sh
```
