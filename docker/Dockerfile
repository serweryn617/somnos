FROM alpine:3.18.4

# Toolchain
RUN apk add --no-cache \
    git \
    python3 \
    cmake \
    g++-arm-none-eabi \
    build-base \
    clang-extra-tools

# Raspberry Pi Pico C/C++ SDK 
ENV PICO_SDK_PATH=/workspace/sdk/pico-sdk

# Picotool
# RUN git clone --depth 1 --branch 1.1.2 https://github.com/raspberrypi/picotool.git /home/picotool && \
#     cd /home/picotool && \
#     mkdir build && \
#     cd build && \
#     cmake .. && \
#     make && \
#     cp /home/picotool/build/picotool /bin/picotool && \
#     rm -rf /home/picotool
