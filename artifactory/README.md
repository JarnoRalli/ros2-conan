# Artifactory

This directory contains information related to running Artifactory.

## Contents

* [./docker-compose.yaml](./docker-compose.yaml)
  * Docker compose file for starting the server

## Instructions

You can start a local Artifactory (CE) server using the following

```bash
docker compose up -d
```

, if you want to see how the different services start, omit `-d`.
Once the server is up and running, you can access it at [http://localhost:8082/](http://localhost:8082/).
Default user credentials are:

* user: admin
* password: password
