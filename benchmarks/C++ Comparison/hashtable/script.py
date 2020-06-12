import sys
import time
import subprocess

ITERATIONS = 10
VERBOSE = False

NL = '\n'

INCLUDE = "../../../src"
CC = 'gcc'
CCPP = 'g++' # LoL
OPT = '-O3'
COUNT = 2000000

# Common args
args = ['-I', INCLUDE, OPT, f'-DCOUNT={COUNT}', '-DCMC_NO_CALLBACKS']

c_args = [CC, 'hashmap.c', '-o', 'hashmap.exe']
cpp_args = [CCPP, 'unorderedmap.cpp', '-I', INCLUDE, '-o', 'unorderedmap.exe', OPT, f'-DCOUNT={COUNT}']

c_args.extend(args)
cpp_args.extend(args)

subprocess.check_output(c_args)
subprocess.check_output(cpp_args)

total = 0
for i in range(ITERATIONS):
    print(f'{i}...', end='')
    sys.stdout.flush()
    start_time = time.time()
    subprocess.call(['./hashmap.exe'])

    delta = time.time() - start_time
    total += delta

    if VERBOSE:
        print(f'({delta})')

avg = int(((total / ITERATIONS) * 1e9) / COUNT)
print(f'{"" if VERBOSE else NL}{total / ITERATIONS} seconds for {COUNT} -> AVG[{avg} ns] [hashmap.exe]{NL if VERBOSE else ""}')

total = 0
for i in range(ITERATIONS):
    print(f'{i}...', end='')
    sys.stdout.flush()
    start_time = time.time()
    subprocess.call(['./unorderedmap.exe'])

    delta = time.time() - start_time
    total += delta

    if VERBOSE:
        print(f'({delta})')

avg = int(((total / ITERATIONS) * 1e9) / COUNT)
print(f'{"" if VERBOSE else NL}{total / ITERATIONS} seconds for {COUNT} -> AVG[{avg} ns] [unorderedmap.exe]{NL if VERBOSE else ""}')
