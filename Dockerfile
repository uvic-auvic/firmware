FROM ubuntu

RUN apt-get update && \
    apt-get -y install git \
    gcc \
    g++ \
    make \
    cmake \
    python3.7

WORKDIR /root/build_tools

RUN apt-get -y install protobuf-compiler \
                       python-protobuf \
		       gcc-arm-none-eabi

COPY firmware/libs/cgreen /root/build_tools/cgreen

RUN cd cgreen && \
    make -j4

COPY firmware/libs/nanopb /root/build_tools/nanopb

WORKDIR /root/firmware

CMD ["make", "all"]

