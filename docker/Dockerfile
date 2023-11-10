FROM alpine:3.18.4

RUN apk add --no-cache \
    git \
    python3 \
    cmake \
    g++-arm-none-eabi \
    build-base \
    clang-extra-tools

ENV PICO_SDK_PATH=/workspace/sdk/pico-sdk
