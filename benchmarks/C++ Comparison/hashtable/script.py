import os
import sys
import time
import subprocess

ITERATIONS = 10
COUNT = 20000000
VERBOSE = False

NL = '\n'

INCLUDE = "../../../src"
CC = 'gcc'
CCPP = 'g++' # LoL
OPT = '-O3'

print(f'Using {CC} compiler')


def execute_command(commands):
    print(' '.join(commands))
    subprocess.run(commands, shell=True)


# Common args
args = ['-I', INCLUDE, OPT, f'-DCOUNT={COUNT}', '-DCMC_NO_CALLBACKS']

map_args = [CC, 'hashmap.c', '-o', 'hashmap.exe']
mmap_args = [CC, 'hashmultimap.c', '-o', 'hashmultimap.exe']
cpp_args = [CCPP, 'unorderedmap.cpp', '-I', INCLUDE, '-o', 'unorderedmap.exe', OPT, f'-DCOUNT={COUNT}']

map_args.extend(args)
mmap_args.extend(args)
cpp_args.extend(args)

execute_command(map_args)
execute_command(mmap_args)
execute_command(cpp_args)

programs = ['hashmap.exe', 'hashmultimap.exe', 'unorderedmap.exe']

for prog in programs:
    total = 0
    for i in range(ITERATIONS):
        print(f'{i}...', end='')
        sys.stdout.flush()
        start_time = time.time()
        subprocess.run(prog, shell=True)

        delta = time.time() - start_time
        total += delta

        if VERBOSE:
            print(f'({delta})')

    avg = total / ITERATIONS
    print(f'{"" if VERBOSE else NL}{avg} seconds for {COUNT} -> AVG[{int((avg * 1e9) / COUNT)} ns] [{prog}]{NL if VERBOSE else ""}')
