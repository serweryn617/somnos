#!/usr/bin/python3

import argparse
import os
import pathlib
import subprocess
import sys

from scripts.ina219_calibration import calibrate
from scripts.udp_rcv import receiver


FILE_PATH = pathlib.Path(__file__).parent.resolve()
COMPILE_COMMANDS_PATH = FILE_PATH/'build'/'compile_commands.json'


def submodules():
    command = f'''
        git submodule update --init --depth 1 sdk/picotool sdk/pico-sdk;
        git submodule status;
        cd sdk/pico-sdk;
        git submodule update --init --depth 1 lib/tinyusb lib/lwip;
        git submodule status;
    '''.split()

    subprocess.run(' '.join(command), shell=True)


def setup():
    command = f'docker build {FILE_PATH}/docker -t pico_builder'
    subprocess.run(command, shell=True)


def build(clean=False, update_commands=True):
    command = f'''
        docker run --rm -it -u {os.getuid()}:{os.getgid()}
        -v {FILE_PATH}:/workspace
        pico_builder
        /bin/sh -c
        "cd workspace;
        {'rm -rf build;' if clean else ''}
        mkdir -p build;
        cd build;
        cmake ..;
        cmake --build . -j;"
    '''.split()

    subprocess.run(' '.join(command), shell=True)

    if update_commands:
        commands()


def commands():
    with open(COMPILE_COMMANDS_PATH, "r") as cc:
        content=cc.read()
    
    content = content.replace('/workspace', str(FILE_PATH))

    with open(COMPILE_COMMANDS_PATH, "w") as cc:
        cc.write(content)


# def picotool():
#     pass


# def upload():
#     pass


def run():
    command = f'docker run --rm -it -u {os.getuid()}:{os.getgid()} -v {FILE_PATH}:/workspace pico_builder /bin/sh'
    subprocess.run(command, shell=True)


def autoformat():
    command = f'''
        docker run --rm -it -u {os.getuid()}:{os.getgid()}
        -v {FILE_PATH}:/workspace
        pico_builder
        /bin/sh -c
        "cd workspace;
        find firmware -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -style=file -i {'{}'} \;"
    '''.split()
    # clang-format -style=mozilla -dump-config > .clang-format;

    subprocess.run(' '.join(command), shell=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers(dest='command')

    subparsers.add_parser('submodules', help='Initialize git submodules')
    subparsers.add_parser('setup', help='Setup Docker image')
    subparsers.add_parser('commands', help='Update paths in compile_commands.json')
    # subparsers.add_parser('picotool', help='Build picotool')
    # subparsers.add_parser('upload', help='Upload built binary to device')
    subparsers.add_parser('run', help='Run build container interactively')
    subparsers.add_parser('format', help='Autoformat C++ code')
    subparsers.add_parser('receive', help='Print data received from the device')

    build_parser = subparsers.add_parser('build', help='Build project')
    build_parser.add_argument('-c', '--clean', help='Perform full build from scratch', action='store_true')
    build_parser.add_argument('-s', '--skip-commands', dest='skip_commands', help='Do not update paths in compile_commands.json', action='store_true')

    calibration_parser = subparsers.add_parser('calibrate', help='Calculate INA219 calibration data')
    calibration_parser.add_argument('current_max', help='Max current in Amperes')
    calibration_parser.add_argument('shunt_resistance', help='Shunt resistor value in Ohms')

    if len(sys.argv)==1:
        parser.print_help()
        sys.exit(1)

    args = parser.parse_args()

    if args.command == 'submodules':
        submodules()
    elif args.command == 'setup':
        setup()
    elif args.command == 'build':
        build(args.clean, not args.skip_commands)
    elif args.command == 'commands':
        commands()
    # elif args.command == 'picotool':
    #     picotool()
    # elif args.command == 'upload':
    #     upload()
    elif args.command == 'run':
        run()
    elif args.command == 'format':
        autoformat()
    elif args.command == 'receive':
        receiver()
    elif args.command == 'calibrate':
        current_max = float(args.current_max)
        shunt_resistance = float(args.shunt_resistance)
        calibrate(current_max, shunt_resistance)