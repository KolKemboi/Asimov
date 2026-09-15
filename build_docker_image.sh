#! /usr/bin/bash
docker build --build-arg USERNAME=ape --build-arg UID=$(id -u) --build-arg GID=$(id -g) -t ape_docker .
