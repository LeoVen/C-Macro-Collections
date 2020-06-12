# A script that automatically generates code for code coverage, updates tests
# and Makefiles, and creates necessary directories

import os
import sys
import re
import subprocess

# Make sure we have the specified directory
def assert_dir(dir):
    if not os.path.exists(dir):
        os.makedirs(dir)

# Compiler settings
CC = 'gcc'
INCLUDE = '-I ./src'
# -E  Only preprocessor
# -P  No linemarkers
# -C Keep comments, except those inside macros (for UNIQUE_FLAG)
FLAGS = '-E -P -C'

# Used to delimiter where the actual code starts and ends
# Used by regex
# Needs to be a valid C comment
UNIQUE_FLAG = '// C_MACRO_COLLECTIONS_CODE'

# Temporary file used to expand the macros
TMP_FILE = './main.c'

# Output directory
# In this directory it is assumed that the following files and folders are
# included: main.c, unt folder
OUTPUT_DIR = './tests'

# All libraries
ALL_LIBS = ['CMC', 'SAC', 'TSC']

# Directory mapping
DIR_MAP = {'HEADER': 'include', 'SOURCE': 'src'}

# File extension mapping
EXT_MAP = {'HEADER': 'h', 'SOURCE': 'c'}

collections = [
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

# Create output directory
assert_dir(OUTPUT_DIR)

# include and src
for dir in DIR_MAP.values():
    assert_dir(f'{OUTPUT_DIR}/{dir}')

# For each library, create a folder for include and src
for lib in ALL_LIBS:
    for dir in DIR_MAP.values():
        assert_dir(f'{OUTPUT_DIR}/{dir}/{lib.lower()}')

# Generate Header and Source separately
for ftype in ['HEADER', 'SOURCE']:
    for data in collections:
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

        result = subprocess.getoutput(f'{CC} {FLAGS} {INCLUDE} {TMP_FILE}')

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

# Format all files
from shutil import which

if which('clang-format') is not None:
    print('clang-format found. Formating files...')

    cmd = ['clang-format', '--style=file', '--verbose', '-i']
    dirs_to_format = [f'{OUTPUT_DIR}/{DIR_MAP[ftype]}/{lib.lower()}/*.{EXT_MAP[ftype]}' for ftype in DIR_MAP.keys() for lib in ALL_LIBS]

    cmd.extend(dirs_to_format)
    subprocess.call(cmd)
else:
    print('clang-format not found. Skipping code formatting...')

# Makefile
# Contains many usefull things dealing with tests, code coverage and formatting
BUILD_DIR = f'{OUTPUT_DIR}/build'
OBJ_DIR = f'{BUILD_DIR}/obj'
INCLUDE_DIR = f"{OUTPUT_DIR}/{DIR_MAP['HEADER']}"
SRC_DIR = f"{OUTPUT_DIR}/{DIR_MAP['SOURCE']}"

assert_dir(BUILD_DIR)
assert_dir(OBJ_DIR)

for lib in ALL_LIBS:
    assert_dir(f'{OBJ_DIR}/{lib.lower()}')

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
coverage: {' '.join([f'{OBJ_DIR}/{data["LIB"].lower()}/{data["SNAME"]}.o' for data in collections])} {OBJ_DIR}/main.o
\t$(CC) $^ -o {OUTPUT_DIR}/main.exe $(CFLAGS) $(CVFLAGS)
\t{OUTPUT_DIR}/main.exe
\tgcov {OBJ_DIR}/main.o {' '.join([f'{OBJ_DIR}/{data["LIB"].lower()}/{data["SNAME"]}.o' for data in collections])}

# Run without coverage
run: build
\t{OUTPUT_DIR}/main.exe

script:
\tpython autogen.py

clean:
\tfind {BUILD_DIR} -type f -delete
\trm *.gcov

# No coverage build
build: {' '.join([f'{OBJ_DIR}/{data["LIB"].lower()}/{data["SNAME"]}.o' for data in collections])} {OBJ_DIR}/main.o
\t$(CC) $^ -o {OUTPUT_DIR}/main.exe $(CFLAGS)

{OBJ_DIR}/main.o: {OUTPUT_DIR}/main.c
\t$(CC) -c $^ -o $@ -I $(INCLUDE) $(CFLAGS)
''')

# Code formatting
file.write('''
fmt_all: fmt_src fmt_tests

fmt_src: FORCE
	find ./src/ -iname *.h | xargs clang-format --style=file --verbose -i

fmt_tests: FORCE
	find ./tests/ -iname *.c -o -iname *.h | xargs clang-format --style=file --verbose -i

.PHONY FORCE:

''')

for data in collections:
    file.write(f'''
{OBJ_DIR}/{data['LIB'].lower()}/{data['SNAME']}.o: {SRC_DIR}/{data['LIB'].lower()}/{data['SNAME']}.c {INCLUDE_DIR}/{data['LIB'].lower()}/{data['SNAME']}.h
\t$(CC) -c $< -o $@ -I $(INCLUDE) $(CFLAGS) $(CVFLAGS)
''')

file.close()

print('\nDone')
