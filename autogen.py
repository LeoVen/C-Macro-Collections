# A script that automatically expands the macros for code coverage

import os
import re
import subprocess

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
OUTPUT_DIR = './tests/main/src'

collections = [
    # type, header, pfx, sname, key, val
    {'t': 'HASHBIDIMAP',  'h': '"cmc/hashbidimap.h"',  'pfx': 'hbm', 'sname': 'hashbidimap',  'key': 'size_t', 'val': 'size_t'},
    {'t': 'BITSET',       'h': '"cmc/bitset.h"',       'pfx': 'bs',  'sname': 'bitset',       'key': '',       'val': ''      },
    {'t': 'DEQUE',        'h': '"cmc/deque.h"',        'pfx': 'd',   'sname': 'deque',        'key': '',       'val': 'size_t'},
    {'t': 'HASHMAP',      'h': '"cmc/hashmap.h"',      'pfx': 'hm',  'sname': 'hashmap',      'key': 'size_t', 'val': 'size_t'},
    {'t': 'HASHSET',      'h': '"cmc/hashset.h"',      'pfx': 'hs',  'sname': 'hashset',      'key': '',       'val': 'size_t'},
    {'t': 'HEAP',         'h': '"cmc/heap.h"',         'pfx': 'h',   'sname': 'heap',         'key': '',       'val': 'size_t'},
    {'t': 'INTERVALHEAP', 'h': '"cmc/intervalheap.h"', 'pfx': 'ih',  'sname': 'intervalheap', 'key': '',       'val': 'size_t'},
    {'t': 'LINKEDLIST',   'h': '"cmc/linkedlist.h"',   'pfx': 'll',  'sname': 'linkedlist',   'key': '',       'val': 'size_t'},
    {'t': 'LIST',         'h': '"cmc/list.h"',         'pfx': 'l',   'sname': 'list',         'key': '',       'val': 'size_t'},
    {'t': 'MULTIMAP',     'h': '"cmc/multimap.h"',     'pfx': 'mm',  'sname': 'multimap',     'key': 'size_t', 'val': 'size_t'},
    {'t': 'MULTISET',     'h': '"cmc/multiset.h"',     'pfx': 'ms',  'sname': 'multiset',     'key': '',       'val': 'size_t'},
    {'t': 'QUEUE',        'h': '"cmc/queue.h"',        'pfx': 'q',   'sname': 'queue',        'key': '',       'val': 'size_t'},
    {'t': 'SORTEDLIST',   'h': '"cmc/sortedlist.h"',   'pfx': 'sl',  'sname': 'sortedlist',   'key': '',       'val': 'size_t'},
    {'t': 'STACK',        'h': '"cmc/stack.h"',        'pfx': 's',   'sname': 'stack',        'key': '',       'val': 'size_t'},
    {'t': 'TREEMAP',      'h': '"cmc/treemap.h"',      'pfx': 'tm',  'sname': 'treemap',      'key': 'size_t', 'val': 'size_t'},
    {'t': 'TREESET',      'h': '"cmc/treeset.h"',      'pfx': 'ts',  'sname': 'treeset',      'key': '',       'val': 'size_t'}
]

# Create output directory
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

for data in collections:
    file = open(TMP_FILE, 'w')

    if data['key'] == '' and data['val'] == '':
        # bitset, no K and no V
        type_params = ''
    elif data['key'] != '':
        # maps, K and V
        type_params = f', {data["key"]}, {data["val"]}'
    else:
        # only V
        type_params = f', {data["val"]}'

    file.write(
    f'''
    #include {data['h']}

    {UNIQUE_FLAG}

    CMC_GENERATE_{data['t']}({data['pfx']}, {data['sname']}{type_params})

    {UNIQUE_FLAG}
    ''')

    file.flush()
    file.close()

    result = subprocess.getoutput(f'{CC} {FLAGS} {INCLUDE} {TMP_FILE}')

    # (?s) makes '.' match anything, even '\n'
    match = re.search(fr'(?s){UNIQUE_FLAG}(?P<code>.+){UNIQUE_FLAG}', result)

    if match is None:
        os.remove(TMP_FILE)
        print('Didn\'t match FLAG. Probably because compilation failed.')
        exit(1)

    file = open(f'{OUTPUT_DIR}/{data["sname"]}.c', 'w')

    file.write(f'#include {data["h"]}\n{match.group("code")}')

    file.flush()
    file.close()

    print(f'Generated {data["h"]: >20} -> {OUTPUT_DIR}/{data["sname"]}.c')

# This seemed to be crashing
# subprocess.run(f'clang-format --style=file -i {OUTPUT_DIR}/*.c')

# So I guess... it will have to be called from a temp Makefile... ugh
TMP_MAKE = './temp_makefile'

data = f'''
.PHONY: codecov FORCE

codecov: FORCE
\tclang-format --style=file -i {OUTPUT_DIR}/*.c

FORCE:

'''

print(f'Creating temporary Makefile as {TMP_MAKE}')

file = open(TMP_MAKE, 'w')
file.write(data)
file.flush()
file.close()

subprocess.call(['make', '-f' , TMP_MAKE, 'codecov', '--always-make'])

os.remove(TMP_FILE)
os.remove(TMP_MAKE)
