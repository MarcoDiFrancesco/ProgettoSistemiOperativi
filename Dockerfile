FROM ubuntu:bionic

WORKDIR /home

RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get -y install build-essential

# COPY . /home
