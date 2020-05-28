FROM ubuntu:18.04

WORKDIR /root

# RUN runs on docker build
# CMD runs on docker run
RUN apt-get update && apt-get install -y \
    build-essential

# Copy files when NOT mounting volume
#COPY . /home
 
CMD ["/bin/bash"]
