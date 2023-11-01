#!/usr/bin/python3

import click
import os
import subprocess
import pathlib


FILE_PATH = pathlib.Path(__file__).parent.resolve()


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
        cmake --build .;"
    '''.split()

    subprocess.run(' '.join(command), shell=True)


@build.command(help='Setup Docker')
def setup():
    command = f'docker build {FILE_PATH}/docker -t pico_builder'
    subprocess.run(command, shell=True)


if __name__ == '__main__':
    build()