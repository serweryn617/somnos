FROM alpine:3.18.4

# Toolchain
RUN apk add --no-cache \
        git \
        python3 \
        cmake \
        g++-arm-none-eabi \
        build-base

# Raspberry Pi Pico C/C++ SDK 
ARG SDK_PATH=/opt/pico_sdk
RUN git clone --depth 1 --branch 1.5.1 https://github.com/raspberrypi/pico-sdk $SDK_PATH && \
    cd $SDK_PATH && \
    git submodule update --init lib/tinyusb lib/lwip

ENV PICO_SDK_PATH=$SDK_PATH

# Picotool
# RUN git clone --depth 1 --branch 1.1.2 https://github.com/raspberrypi/picotool.git /home/picotool && \
#     cd /home/picotool && \
#     mkdir build && \
#     cd build && \
#     cmake .. && \
#     make && \
#     cp /home/picotool/build/picotool /bin/picotool && \
#     rm -rf /home/picotool