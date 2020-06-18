# A script that automatically generates code for code coverage, updates tests
# and Makefiles, and creates necessary directories

import argparse
import os
import re
import shutil
import subprocess
import sys
from typing import List, Text

# Config                 Script Configuration
#   Directories          Names for directories
#     Output             Where the outputted code is to be put
#     Build              Where the build objects are to be located
#   Compiler             Compiler Configurations
#     Compilation        Tests, code coverage and flags
#     Macro Expansion    Configuration for the code expansion
#   Collections          All collections
#
# Variables marked as const are to not be modified

# Directories

# Output directory
# In this directory it is assumed that the following files and directories are
# included: main.c, unt folder
OUTPUT_DIR = './tests'
# All libraries
ALL_LIBS = ['CMC', 'SAC', 'TSC']
# Directory mapping
DIR_MAP = {'HEADER': 'include', 'SOURCE': 'src'}
# File extension mapping
EXT_MAP = {'HEADER': 'h', 'SOURCE': 'c'}
# Where compilation objects will be outputted
BUILD_DIR = f'{OUTPUT_DIR}/build'
# const
INCLUDE_DIR = f"{OUTPUT_DIR}/{DIR_MAP['HEADER']}"
# const
SRC_DIR = f"{OUTPUT_DIR}/{DIR_MAP['SOURCE']}"
# Where object files .o go
OBJ_DIR = f'{BUILD_DIR}/obj'
# The name of the main test executable
TEST_EXE = f'{OUTPUT_DIR}/main.exe'

# Compiler

# Compilation
CC = 'gcc'
# Compilation Flags
CFLAGS = ['-Wall', '-Werror']
# Coverage Flags
CVFLAGS = ['--coverage', '-O0']
# Include to the main Library
# const
INCLUDE = '-I ./src'
# Include to the test file headers
# const
TINCLUDE = f'-I {INCLUDE_DIR}'

# Macro expansion

# -E  Only preprocessor
# -P  No linemarkers
# -C Keep comments, except those inside macros (for UNIQUE_FLAG)
# const
EXPAND_FLAGS = '-E -P -C'
# Used to delimiter where the actual code starts and ends. Used by regex.
# Needs to be a valid C comment.
UNIQUE_FLAG = '// C_MACRO_COLLECTIONS_CODE'
# Temporary file used to expand the macros
TMP_FILE = './main.c'

# Just a realy helpfull overview
CONFIG = f'''Directory Settings
    OUTPUT_DIR = {OUTPUT_DIR}
    ALL_LIBS = {ALL_LIBS}
    DIR_MAP = {DIR_MAP}
    EXT_MAP = {EXT_MAP}
    BUILD_DIR = {BUILD_DIR}
    INCLUDE_DIR = {INCLUDE_DIR}
    SRC_DIR = {SRC_DIR}
    OBJ_DIR = {OBJ_DIR}
Compiler Settings
    CC = {CC}
    CFLAGS = {CFLAGS}
    CVFLAGS = {CVFLAGS}
    INCLUDE = {INCLUDE}
    TINCLUDE = {TINCLUDE}
Macro Expansion
    EXPAND_FLAGS = {EXPAND_FLAGS}
    UNIQUE_FLAG = {UNIQUE_FLAG}
    TMP_FILE = {TMP_FILE}'''


# All collections that can be used for testing
COLLECTIONS = [
    # header, library, collection, pfx, sname, size, key, val
    {'h': '"cmc/bitset.h"',       'LIB': 'CMC', 'COLLECTION': 'BITSET',       'PFX': 'bs',  'SNAME': 'bitset',       'SIZE': '', 'K': '',       'V': ''      },
    {'h': '"cmc/deque.h"',        'LIB': 'CMC', 'COLLECTION': 'DEQUE',        'PFX': 'd',   'SNAME': 'deque',        'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/hashbidimap.h"',  'LIB': 'CMC', 'COLLECTION': 'HASHBIDIMAP',  'PFX': 'hbm', 'SNAME': 'hashbidimap',  'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc/hashmap.h"',      'LIB': 'CMC', 'COLLECTION': 'HASHMAP',      'PFX': 'hm',  'SNAME': 'hashmap',      'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc/hashmultimap.h"', 'LIB': 'CMC', 'COLLECTION': 'HASHMULTIMAP', 'PFX': 'hmm', 'SNAME': 'hashmultimap', 'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc/hashmultiset.h"', 'LIB': 'CMC', 'COLLECTION': 'HASHMULTISET', 'PFX': 'hms', 'SNAME': 'hashmultiset', 'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/hashset.h"',      'LIB': 'CMC', 'COLLECTION': 'HASHSET',      'PFX': 'hs',  'SNAME': 'hashset',      'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/heap.h"',         'LIB': 'CMC', 'COLLECTION': 'HEAP',         'PFX': 'h',   'SNAME': 'heap',         'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/intervalheap.h"', 'LIB': 'CMC', 'COLLECTION': 'INTERVALHEAP', 'PFX': 'ih',  'SNAME': 'intervalheap', 'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/linkedlist.h"',   'LIB': 'CMC', 'COLLECTION': 'LINKEDLIST',   'PFX': 'll',  'SNAME': 'linkedlist',   'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/list.h"',         'LIB': 'CMC', 'COLLECTION': 'LIST',         'PFX': 'l',   'SNAME': 'list',         'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/queue.h"',        'LIB': 'CMC', 'COLLECTION': 'QUEUE',        'PFX': 'q',   'SNAME': 'queue',        'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/sortedlist.h"',   'LIB': 'CMC', 'COLLECTION': 'SORTEDLIST',   'PFX': 'sl',  'SNAME': 'sortedlist',   'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/stack.h"',        'LIB': 'CMC', 'COLLECTION': 'STACK',        'PFX': 's',   'SNAME': 'stack',        'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc/treemap.h"',      'LIB': 'CMC', 'COLLECTION': 'TREEMAP',      'PFX': 'tm',  'SNAME': 'treemap',      'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc/treeset.h"',      'LIB': 'CMC', 'COLLECTION': 'TREESET',      'PFX': 'ts',  'SNAME': 'treeset',      'SIZE': '', 'K': '',       'V': 'size_t'}
]


def execute_command(commands: List[Text]):
    print(' '.join(commands))
    subprocess.run(commands, shell=True)


def assert_executable(program: Text):
    '''
        Asserts that a given program is installed and available in PATH
    '''
    if shutil.which(program) is None:
        raise Exception(f'The required executable {program} was not found in your path')


def assert_dir(dir: Text):
    '''
        Given a string 'dir' that is a path to a directory, make sure that it exists.
    '''
    if not os.path.exists(dir):
        os.makedirs(dir)


def assert_structure():
    '''
        Git might not commit some required directories. This function asserts that they exist.
    '''
    assert_dir(OUTPUT_DIR)

    # include and src
    for dir in DIR_MAP.values():
        assert_dir(f'{OUTPUT_DIR}/{dir}')

    # For each library, create a directory for include and src
    for lib in ALL_LIBS:
        for dir in DIR_MAP.values():
            assert_dir(f'{OUTPUT_DIR}/{dir}/{lib.lower()}')

    assert_dir(BUILD_DIR)
    assert_dir(OBJ_DIR)

    for lib in ALL_LIBS:
        assert_dir(f'{OBJ_DIR}/{lib.lower()}')


def clean_build():
    execute_command(['find', BUILD_DIR, '-type', 'f', '-delete'])
    execute_command(['find', '.', '-iname', '*.gcov', '-type', 'f', '-delete'])


def expand_code():
    # Generate Header and Source separately
    for ftype in ['HEADER', 'SOURCE']:
        for data in COLLECTIONS:
            file = open(TMP_FILE, 'w')

            file.write(
            f'''
                #include "macro_collections.h"

                {UNIQUE_FLAG}

                C_MACRO_COLLECTIONS_ALL_{ftype}({data['LIB']}, {data['COLLECTION']}, ({', '.join([data['PFX'], data['SNAME'], data['SIZE'], data['K'], data['V']])}))

                {UNIQUE_FLAG}
            ''')

            file.flush()
            file.close()

            result = subprocess.getoutput(f'{CC} {EXPAND_FLAGS} {INCLUDE} {TMP_FILE}')

            # (?s) makes '.' match anything, even '\n'
            match = re.search(fr'(?s){UNIQUE_FLAG}(?P<code>.+){UNIQUE_FLAG}', result)

            if match is None:
                os.remove(TMP_FILE)
                print('Didn\'t match FLAG. Probably because compilation failed.', file=sys.stderr)
                exit(1)

            file = open(f'{OUTPUT_DIR}/{DIR_MAP[ftype]}/{data["LIB"].lower()}/{data["SNAME"]}.{EXT_MAP[ftype]}', 'w')

            if ftype == 'HEADER':
                file.write(
                f'''
                    #ifndef CMC_{data['LIB']}_{data['COLLECTION']}_TEST_H
                    #define CMC_{data['LIB']}_{data['COLLECTION']}_TEST_H

                    #include "macro_collections.h"
                    {match.group('code')}

                    #endif /* CMC_{data['LIB']}_{data['COLLECTION']}_TEST_H */
                ''')
            else:
                file.write(
                f'''
                    #include {data["h"]}

                    {match.group("code")}
                ''')

            file.flush()
            file.close()

            print(f'Generated {data["h"]: >20} -> {OUTPUT_DIR}/{DIR_MAP[ftype]}/{data["LIB"].lower()}/{data["SNAME"]}.{EXT_MAP[ftype]}')

    os.remove(TMP_FILE)


def format_expand():
    cmd = ['clang-format', '--style=file', '--verbose', '-i']
    dirs_to_format = [f'{OUTPUT_DIR}/{DIR_MAP[ftype]}/{lib.lower()}/*.{EXT_MAP[ftype]}' for ftype in DIR_MAP.keys() for lib in ALL_LIBS]
    cmd.extend(dirs_to_format)
    execute_command(cmd)


def format_code(source: bool, tests: bool):
    if source:
        execute_command(['find', './src/', '-iname', '*.h', '|', 'xargs', 'clang-format', '--style=file', '--verbose', '-i'])
    if tests:
        execute_command(['find', './tests/', '-iname', '*.c', '-o', '-iname', '*.h', '|', 'xargs', 'clang-format', '--style=file', '--verbose', '-i'])


def build_tests(with_coverage: bool):
    cflags = CFLAGS.copy()

    if with_coverage:
        cflags += CVFLAGS

    print("WIP")


def run_tests():
    execute_command([TEST_EXE])


def unused():
    # Makefile
    # Contains many usefull things dealing with tests, code coverage and formatting

    print('Updating Makefile...')

    file = open(f'Makefile', 'w')

    file.write(f'''# This file was generated by a Python script
    CC = {CC}
    CFLAGS = -std=c11 -Wall -Wextra
    CFLAGS += -Wno-missing-braces -DCMC_TEST_COLOR
    CVFLAGS = --coverage -O0 -g3
    INCLUDE = {INCLUDE_DIR} -I src

    # Runs the script to update tests, run all tests and code coverage
    all: script coverage

    tests: script build
    \t{OUTPUT_DIR}/main.exe

    # Like build but with coverage
    coverage: {' '.join([f'{OBJ_DIR}/{data["LIB"].lower()}/{data["SNAME"]}.o' for data in COLLECTIONS])} {OBJ_DIR}/main.o
    \t$(CC) $^ -o {OUTPUT_DIR}/main.exe $(CFLAGS) $(CVFLAGS)
    \t{OUTPUT_DIR}/main.exe
    \tgcov {OBJ_DIR}/main.o {' '.join([f'{OBJ_DIR}/{data["LIB"].lower()}/{data["SNAME"]}.o' for data in COLLECTIONS])}

    # Run without coverage
    run: build
    \t{OUTPUT_DIR}/main.exe

    script:
    \tpython autogen.py

    clean:
    \tfind {BUILD_DIR} -type f -delete
    \trm *.gcov

    # No coverage build
    build: {' '.join([f'{OBJ_DIR}/{data["LIB"].lower()}/{data["SNAME"]}.o' for data in COLLECTIONS])} {OBJ_DIR}/main.o
    \t$(CC) $^ -o {OUTPUT_DIR}/main.exe $(CFLAGS)

    {OBJ_DIR}/main.o: {OUTPUT_DIR}/main.c
    \t$(CC) -c $^ -o $@ -I $(INCLUDE) $(CFLAGS)
    ''')

    for data in COLLECTIONS:
        file.write(f'''
    {OBJ_DIR}/{data['LIB'].lower()}/{data['SNAME']}.o: {SRC_DIR}/{data['LIB'].lower()}/{data['SNAME']}.c {INCLUDE_DIR}/{data['LIB'].lower()}/{data['SNAME']}.h
    \t$(CC) -c $< -o $@ -I $(INCLUDE) $(CFLAGS) $(CVFLAGS)
    ''')

    file.close()
    print('\nDone')


if __name__ == '__main__':
    p = argparse.ArgumentParser('autogen.py', usage='%(prog)s [options]')

    p.add_argument('-sc', '--show-config',
                    help='Displays the current configuration',
                    action='store_true',
                    dest='config')

    tests = p.add_argument_group('tests', 'For dealing with tests')

    tests.add_argument('-c', '--clean',
                        help='Cleans all build objects',
                        default=False,
                        action='store_true',
                        dest='clean')
    tests.add_argument('-e', '--expand',
                       help='Expands the header and source files',
                       default=False,
                       action='store_true',
                       dest='expand')
    tests.add_argument('-nf', '--no-format',
                       help='Does not format the output of expand. Can be used when you might not have clang-format',
                       default=False,
                       action='store_true',
                       dest='noformat')
    tests.add_argument('-r', '--run',
                        help='Runs tests without code coverage',
                        default=False,
                        action='store_true',
                        dest='run')
    tests.add_argument('-cv', '--coverage',
                        help='Compiles and runs tests with code coverage',
                        default=False,
                        action='store_true',
                        dest='coverage')
    tests.add_argument('-rr', '--rerun',
                        help='Builds tests without code coverage and runs it',
                        default=False,
                        action='store_true',
                        dest='rerun')

    source = p.add_argument_group('source', 'For dealing with the source files and formatting')

    source.add_argument('-fs', '--format-source',
                        help='Format the source code',
                        default=False,
                        action='store_true',
                        dest='fmt_source')
    source.add_argument('-ft', '--format-tests',
                        help='Format tests files',
                        default=False,
                        action='store_true',
                        dest='fmt_tests')

    args = p.parse_args()

    if args.config:
        print(CONFIG)
        exit(0)

    if args.clean:
        clean_build()

    assert_structure()
    assert_executable(CC)

    if args.expand:
        expand_code()
        if not args.noformat:
            assert_executable('clang-format')
            format_expand()

    if args.fmt_source or args.fmt_tests:
        assert_executable('find')
        assert_executable('xargs')
        format_code(args.fmt_source, args.fmt_tests)

    if args.rerun:
        build_tests(False)
        run_tests()
    elif args.run:
        run_tests()
    elif args.coverage:
        build_tests(True)
        run_tests()
