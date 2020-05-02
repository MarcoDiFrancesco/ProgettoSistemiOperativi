#!/bin/bash

# Check if docker is running
if ! [[ $(systemctl status docker.service | grep 'active (running)') ]]; then
    echo "Docker not running"
    echo "Enable it with 'sudo systemctl start docker'"
    exit 1
fi

# Check if image exists
if ! [[ $(ls | grep Dockerfile) ]]; then
    echo "You are not in the project directory!"
    exit 1
fi

if ! [[ $(docker image ls | grep osproject) ]]; then
    echo "osproject image does not exist"
    echo "Building..."
    docker image build --tag osproject:latest .
fi

if [[ $(docker container ls --filter "status=running" | grep osproject) ]]; then
    docker exec --interactive --tty osproject bash
else
    docker container run --name osproject --interactive --tty --rm --volume $(pwd):/home osproject:latest
fi

