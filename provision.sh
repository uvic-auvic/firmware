#!/bin/bash

#install all required binaries
sudo apt-get update && \
sudo apt-get install -y \
git \
make \
cmake \
gcc \
g++ \
gcc-arm-none-eabi=15:4.9.3+svn231177-1

#make cgreen
cd firmware/libs/cgreen
make
make test
sudo make install

cd ../../..

sudo ldconfig -v
