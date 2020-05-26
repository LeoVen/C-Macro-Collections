# A script that automatically expands the macros for code coverage

import os
import sys
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
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

for data in collections:
    file = open(TMP_FILE, 'w')

    file.write(
    f'''
    #include {data['h']}

    {UNIQUE_FLAG}

    CMC_{data['LIB']}_{data['COLLECTION']}_CORE(({', '.join([data['PFX'], data['SNAME'], data['SIZE'], data['K'], data['V']])}))

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

    file = open(f'{OUTPUT_DIR}/{data["SNAME"]}.c', 'w')

    file.write(
    f'''#ifndef CMC_TEST_SRC_{data['COLLECTION']}
        #define CMC_TEST_SRC_{data['COLLECTION']}

        #include {data["h"]}\n{match.group("code")}

        #endif /* CMC_TEST_SRC_{data['COLLECTION']} */
    ''')

    file.flush()
    file.close()

    print(f'Generated {data["h"]: >20} -> {OUTPUT_DIR}/{data["SNAME"]}.c')

os.remove(TMP_FILE)

# Format all files
print('Formating files...')
subprocess.call(['clang-format', '--style=file', '-i', f'{OUTPUT_DIR}/*.c', '--verbose'])
print('Done')
