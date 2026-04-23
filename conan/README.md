# Conan

This directory contains information related running Conan in a Docker container and how to install
the `robot_base_config` Conan package.

You don't need a remote server for testing how the `robot_base_config` can be used. You can simply create it in the local cache
in order to make it available as per the instructions below.

## 1. Contents

* [./docker-compose.yaml](./docker-compose.yaml)
  * Docker compose file for starting the server
* [./robot_base_config](robot_base_config)
  * `robot_base_config` Conan package

## 2. Instructions

You can start a local Conan (CE) server using the following command

```bash
docker compose up -d
```

, if you want to see how the different services start, omit `-d`.
Once the server is up and running, you can access it at [http://localhost:8082/](http://localhost:8082/).
Default user credentials are:

* **user:** admin
* **password:** password

## 3. Create the Robot Base Config

Run the following command in order to create a `robot_base_config` Conan package in the local cache.

```bash
cd robot_base_config
conan create . --user einherjar --channel stable
```

### 3.1 Upload to Remote

You can push the package to a remote server using:

```bash
conan upload robot_base_config/1.0@einherjar/stable --remote=myremote
```
