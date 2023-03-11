#!/bin/bash

#install all required binaries
sudo apt-get update && \
sudo apt-get install -y \
git \
make \
cmake \
gcc \
g++ \
gcc-arm-none-eabi

#make cgreen
cd firmware/libs/cgreen
make
make test
sudo make install

cd ../../..

sudo ldconfig -v
