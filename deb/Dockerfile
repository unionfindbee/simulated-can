# Use an image with can-utils and a C compiler
#FROM debian:12-slim
FROM ubuntu:latest

# Install necessary packages
RUN apt-get update && apt-get install -y \
    can-utils \
    build-essential \
    iproute2 \
    kmod

# Copy your C program into the Docker image
COPY . /usr/src/vcan
WORKDIR /usr/src/vcan

# Compile your C program
RUN gcc -o vcan main.c


