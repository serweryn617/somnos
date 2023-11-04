#!/usr/bin/python3

import click
import os
import subprocess
import pathlib


FILE_PATH = pathlib.Path(__file__).parent.resolve()
COMPILE_COMMANDS_PATH = FILE_PATH/'build'/'compile_commands.json'


@click.group(invoke_without_command=True)
@click.pass_context
@click.option('-c', '--clean', default=False, is_flag=True, help='Perform full build from scratch')
def build(ctx, clean):
    if ctx.invoked_subcommand is not None:
        return

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
    commands()


@build.command(help='Setup Docker')
def setup():
    command = f'docker build {FILE_PATH}/docker -t pico_builder'
    subprocess.run(command, shell=True)


@build.command(help='Update compile commands')
def commands():
    with open(COMPILE_COMMANDS_PATH, "r") as cc:
        content=cc.read()
    
    content = content.replace('/workspace', str(FILE_PATH))

    with open(COMPILE_COMMANDS_PATH, "w") as cc:
        cc.write(content)


@build.command(help='Run interactive')
def run():
    command = f'docker run --rm -it -v {FILE_PATH}:/workspace pico_builder /bin/sh'
    subprocess.run(command, shell=True)



@build.command(help='Run interactive')
def format():
    command = f'''
        docker run --rm -it -u {os.getuid()}:{os.getgid()}
        -v {FILE_PATH}:/workspace
        pico_builder
        /bin/sh -c
        "cd workspace;
        find src inc lwip -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -style=file -i {'{}'} \;"
    '''.split()
    # clang-format -style=mozilla -dump-config > .clang-format;

    subprocess.run(' '.join(command), shell=True)


@build.command(help='Setup Docker')
def submodules():
    command = f'''
        git submodule update --init --depth 1 picotool pico-sdk;
        git submodule status;
        cd pico-sdk;
        git submodule update --init --depth 1 lib/tinyusb lib/lwip;
        git submodule status;
    '''.split()

    subprocess.run(' '.join(command), shell=True)


if __name__ == '__main__':
    build()