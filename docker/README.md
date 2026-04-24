# Raspberry Pi Docker

We can test the code also in a Raspberry Pi 5 using a Docker image that
has ROS 2. First we create an image in order to install Conan and other
requirements.

```bash
docker build --platform linux/arm64 -t ros2-humble-conan .
```

Then we start the container

```bash
docker run -it \
  --platform linux/arm64 \
  -v $(pwd)/ros2-conan/:/home/ros2-conan \
  ros2-humble-conan
```

After that you create the Conan package and build the workspace normally. Once built,
you start the pong node and open another terminal in the same container. First you
need to find out the ID of the running container (this is run in the host):

```bash
docker ps
```

and then you start another session by running (in the host):

```bash
docker exec -it <ID> bash
```

where <ID> is the ID of the running container.
