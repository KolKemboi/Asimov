#! /usr/bin/bash
docker run --rm -it -v "$(pwd)/Engine:/Engine" -w /Engine ape_docker:latest bash
