# A script for automation

import argparse
import os
import pathlib
import re
import shutil
import subprocess
import sys
from typing import List, Text

# Config                 Script Configuration
#   Directories          Names for directories
#   Files                Names for files
#   Compiler             Compiler Configurations
#   Macro Expansion      Configuration for the code expansion
#   Collections          All collections
#
# Variables marked as const are to not be modified

# Directories

# Output directory
# In this directory it is assumed that the following files and directories are
# included: main.c, codecov.c, unt folder
OUTPUT_DIR = './tests'
# Where the unit test files are
UNT_DIR = f'{OUTPUT_DIR}/unt'
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
# Where executables go
BIN_DIR = f'{BUILD_DIR}/bin'
# Where object files .o go
OBJ_DIR = BUILD_DIR
# Where .gcov files should go
GCOV_DIR = f'{BUILD_DIR}/gcov'

# Files
# All const

# The name of the main test
MAIN = 'main'
# The name of the code coverage file
CODECOV = 'codecov'
# The name of the single test file
SINGLE = 'single'

# Compiler

# Compiler to be used
CC = 'gcc'
# Compilation Flags
CFLAGS = ['-Wall', '-Werror', '-O3']
# Link flags
LFLAGS = []
# Coverage Flags
CVFLAGS = ['--coverage', '-O0']
# gcov flags
GCOV_FLAGS = []
# Include of the main library
# const
INCLUDE = ['-I', './src']
# Include to the test file headers
# const
TINCLUDE = ['-I', f'{INCLUDE_DIR}', '-I', f'{UNT_DIR}']
# Include to code coverage
# const
CVINCLUDE = ['-I', INCLUDE_DIR, '-I', SRC_DIR, '-I', f'{UNT_DIR}']

# Macro Expansion

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


# All collections that can be used for testing
COLLECTIONS = [
    # header, library, collection, pfx, sname, size, key, val
    {'h': '"cmc_bitset.h"',       'LIB': 'CMC', 'COLLECTION': 'BITSET',       'PFX': 'bs',  'SNAME': 'bitset',       'SIZE': '', 'K': '',       'V': ''      },
    {'h': '"cmc_deque.h"',        'LIB': 'CMC', 'COLLECTION': 'DEQUE',        'PFX': 'd',   'SNAME': 'deque',        'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_hashbidimap.h"',  'LIB': 'CMC', 'COLLECTION': 'HASHBIDIMAP',  'PFX': 'hbm', 'SNAME': 'hashbidimap',  'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc_hashmap.h"',      'LIB': 'CMC', 'COLLECTION': 'HASHMAP',      'PFX': 'hm',  'SNAME': 'hashmap',      'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc_hashmultimap.h"', 'LIB': 'CMC', 'COLLECTION': 'HASHMULTIMAP', 'PFX': 'hmm', 'SNAME': 'hashmultimap', 'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc_hashmultiset.h"', 'LIB': 'CMC', 'COLLECTION': 'HASHMULTISET', 'PFX': 'hms', 'SNAME': 'hashmultiset', 'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_hashset.h"',      'LIB': 'CMC', 'COLLECTION': 'HASHSET',      'PFX': 'hs',  'SNAME': 'hashset',      'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_heap.h"',         'LIB': 'CMC', 'COLLECTION': 'HEAP',         'PFX': 'h',   'SNAME': 'heap',         'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_intervalheap.h"', 'LIB': 'CMC', 'COLLECTION': 'INTERVALHEAP', 'PFX': 'ih',  'SNAME': 'intervalheap', 'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_linkedlist.h"',   'LIB': 'CMC', 'COLLECTION': 'LINKEDLIST',   'PFX': 'll',  'SNAME': 'linkedlist',   'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_list.h"',         'LIB': 'CMC', 'COLLECTION': 'LIST',         'PFX': 'l',   'SNAME': 'list',         'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_queue.h"',        'LIB': 'CMC', 'COLLECTION': 'QUEUE',        'PFX': 'q',   'SNAME': 'queue',        'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_sortedlist.h"',   'LIB': 'CMC', 'COLLECTION': 'SORTEDLIST',   'PFX': 'sl',  'SNAME': 'sortedlist',   'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_stack.h"',        'LIB': 'CMC', 'COLLECTION': 'STACK',        'PFX': 's',   'SNAME': 'stack',        'SIZE': '', 'K': '',       'V': 'size_t'},
    {'h': '"cmc_treemap.h"',      'LIB': 'CMC', 'COLLECTION': 'TREEMAP',      'PFX': 'tm',  'SNAME': 'treemap',      'SIZE': '', 'K': 'size_t', 'V': 'size_t'},
    {'h': '"cmc_treeset.h"',      'LIB': 'CMC', 'COLLECTION': 'TREESET',      'PFX': 'ts',  'SNAME': 'treeset',      'SIZE': '', 'K': '',       'V': 'size_t'}
]


# Just a realy helpfull overview
CONFIG = f'''Directory Settings
    OUTPUT_DIR = {OUTPUT_DIR}
    UNT_DIR = {UNT_DIR}
    DIR_MAP = {DIR_MAP}
    EXT_MAP = {EXT_MAP}
    BUILD_DIR = {BUILD_DIR}
    INCLUDE_DIR = {INCLUDE_DIR}
    SRC_DIR = {SRC_DIR}
    OBJ_DIR = {OBJ_DIR}
    GCOV_DIR = {GCOV_DIR}
File Settings
    MAIN = {MAIN}
    CODECOV = {CODECOV}
    SINGLE = {SINGLE}
Compiler Settings
    CC = {CC}
    CFLAGS = {CFLAGS}
    LFLAGS = {LFLAGS}
    CVFLAGS = {CVFLAGS}
    GCOV_FLAGS = {GCOV_FLAGS}
    INCLUDE = {INCLUDE}
    TINCLUDE = {TINCLUDE}
    CVINCLUDE = {CVINCLUDE}
Macro Expansion
    EXPAND_FLAGS = {EXPAND_FLAGS}
    UNIQUE_FLAG = {UNIQUE_FLAG}
    TMP_FILE = {TMP_FILE}'''


def full_path(path: Text) -> Text:
    '''
        Returns the full path of a 'path' with ralative paths, e.g. './' or '../'
    '''
    return str(pathlib.Path(path).resolve())


def execute_command(commands: List[Text]):
    cmd = ' '.join(commands)
    print(cmd)
    subprocess.check_call(cmd, shell=True)

    # if cp.returncode != 0:
    #     print('Error when executing the last command')
    #     exit(1)


def require_executable(program: Text):
    '''
        Asserts that a given program is installed and available in PATH. Raises an exception if it doesn't.
    '''
    if shutil.which(program) is None:
        raise Exception(f'The required executable {program} was not found in your path')


def require_file(file: Text):
    '''
        Asserts that a given file exists. Raises an exception if it doesn't.
    '''
    f = pathlib.Path(file).resolve()
    if not f.is_file():
        raise Exception(f'The required file {str(f)} was not found')


def assert_dir(directory: Text):
    '''
        Given a string 'directory' that is a path to a directory, make sure that it exists.
    '''
    # TODO change this to use pathlib ?
    if not os.path.exists(directory):
        os.makedirs(directory)


def assert_structure():
    '''
        Git might not commit some required directories. This function asserts that they exist.
    '''
    assert_dir(OUTPUT_DIR)

    # include and src
    for directory in DIR_MAP.values():
        assert_dir(f'{OUTPUT_DIR}/{directory}')

    assert_dir(BUILD_DIR)
    assert_dir(OBJ_DIR)
    assert_dir(BIN_DIR)
    assert_dir(GCOV_DIR)


def format_code(source: bool, tests: bool):
    '''
        Format all source files from the main library or test files.
    '''
    if source:
        execute_command(['find', './src/', '-name', '*.h', '|', 'xargs', 'clang-format', '--style=file', '--verbose', '-i'])
    if tests:
        execute_command(['find', './tests/', '-name', '*.c', '-o', '-name', '*.h', '|', 'xargs', 'clang-format', '--style=file', '--verbose', '-i'])


def clean_build(cleanfull: bool):
    '''
        Removes all files generated by compilation and code coverage along with folders.
    '''
    # -f prevents 'rm' from failing
    execute_command(['rm', '-r', '-f', f'./*.gcov'])
    execute_command(['rm', '-r', '-f', f'{OUTPUT_DIR}/*.exe'])
    execute_command(['rm', '-r', '-f', BUILD_DIR])
    execute_command(['rm', '-f', './*.gcda'])
    execute_command(['rm', '-f', './*.gcno'])

    if cleanfull:
        execute_command(['rm', '-r', SRC_DIR])
        execute_command(['rm', '-r', INCLUDE_DIR])


def expand_code():
    '''
        Expands code to be used in tests. This is necessary because code coverage doesn't work with macros.
    '''
    # Generate Header and Source separately
    for ftype in ['HEADER', 'SOURCE']:
        for data in COLLECTIONS:
            file = open(TMP_FILE, 'w')

            file.write(
            f'''
                #include "macro_collections.h"

                {UNIQUE_FLAG}

                C_MACRO_COLLECTIONS_ALL_{ftype}({data['LIB']}, {data['COLLECTION']}, PUBLIC, ({', '.join([data['PFX'], data['SNAME'], data['SIZE'], data['K'], data['V']])}))

                {UNIQUE_FLAG}
            ''')

            file.flush()
            file.close()

            result = subprocess.getoutput(f'{CC} {EXPAND_FLAGS} {" ".join(INCLUDE)} {TMP_FILE}')

            # (?s) makes '.' match anything, even '\n'
            match = re.search(fr'(?s){UNIQUE_FLAG}(?P<code>.+){UNIQUE_FLAG}', result)

            if match is None:
                os.remove(TMP_FILE)
                print('Didn\'t match FLAG. Probably because compilation failed.', file=sys.stderr)
                exit(1)

            file = open(f'{OUTPUT_DIR}/{DIR_MAP[ftype]}/tst_{data["LIB"].lower()}_{data["SNAME"]}.{EXT_MAP[ftype]}', 'w')

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
                    #include {'"tst_' + data["h"][1:]}

                    {match.group("code")}
                ''')

            file.flush()
            file.close()

            print(f'''Generated {'"tst_' + data["h"][1:]: >24} -> {OUTPUT_DIR}/{DIR_MAP[ftype]}/tst_{data["LIB"].lower()}_{data["SNAME"]}.{EXT_MAP[ftype]}''')

    os.remove(TMP_FILE)


def format_expand():
    '''
        Format the files outputted by 'expand_code'.
    '''
    cmd = ['clang-format', '--style=file', '--verbose', '-i']
    dirs_to_format = [f'{OUTPUT_DIR}/{DIR_MAP[ftype]}/*.{EXT_MAP[ftype]}' for ftype in DIR_MAP.keys()]
    cmd.extend(dirs_to_format)
    execute_command(cmd)


def build_main():
    '''
        Build all object files and main.c for tests.
    '''
    for data in COLLECTIONS:
        cmd = [CC]
        cmd += TINCLUDE # This one needs to go first
        cmd += INCLUDE
        cmd += ['-c', f"{SRC_DIR}/tst_{data['LIB'].lower()}_{data['SNAME']}.c"]
        cmd += ['-o', f"{OBJ_DIR}/tst_{data['LIB'].lower()}_{data['SNAME']}.o"]
        execute_command(cmd)

    # Build OUTPUT_DIR/main.c
    cmd = [CC]
    cmd += TINCLUDE # This one needs to go first
    cmd += INCLUDE
    cmd += ['-c', f'{OUTPUT_DIR}/{MAIN}.c']
    cmd += ['-o', f'{OBJ_DIR}/{MAIN}.o']
    execute_command(cmd)

    # Link everything producing OUTPUT_DIR/main.exe
    cmd = [CC]
    cmd += LFLAGS
    cmd += ['-o', f'{BIN_DIR}/{MAIN}.exe']
    cmd += [f'{OBJ_DIR}/main.o']
    cmd += [f'{OBJ_DIR}/tst_{data["LIB"].lower()}_{data["SNAME"]}.o' for data in COLLECTIONS]
    execute_command(cmd)


def run_main():
    '''
        Runs main.exe
    '''
    execute_command([full_path(f'{BIN_DIR}/{MAIN}.exe')])


def build_codecov():
    '''
        Builds codecov.c for code coverage.
    '''
    cmd = [CC]
    cmd += CVFLAGS
    cmd += CVINCLUDE # This one needs to go first
    cmd += INCLUDE
    cmd += [f'{OUTPUT_DIR}/{CODECOV}.c']
    cmd += ['-o', f'{BIN_DIR}/{CODECOV}.exe']
    execute_command(cmd)


def run_codecov():
    '''
        Runs codecov.exe and then gcov on the generated gcda files. All gcov files then go to GCOV_DIR.
    '''
    execute_command([full_path(f'{BIN_DIR}/{CODECOV}.exe')])
    execute_command(['gcov'] + GCOV_FLAGS + [f'{CODECOV}.gcda'])
    execute_command(['rm', '*.h.gcov'])


def tidy():
    '''
        Places files to their respective directory.
    '''
    execute_command(['mv', '*.gcov', GCOV_DIR])


if __name__ == '__main__':
    p = argparse.ArgumentParser('autogen.py', usage='%(prog)s [options]')

    p.add_argument('-sc', '--show-config',
                    help='Displays the current configuration',
                    default=False,
                    action='store_true',
                    dest='config')

    source = p.add_argument_group('source', 'For dealing with the source files and formatting')

    expand = p.add_argument_group('expand', 'For dealing with code generation and formatting for tests')

    task = p.add_argument_group('task', 'Specify a task (mutually exclusive)')
    task_ = task.add_mutually_exclusive_group()

    action = p.add_argument_group('action', 'Actions on task')

    task_.add_argument('-cv', '--codecov',
                        help='Designates task as code coverage (uses codecov.c)',
                        default=False,
                        action='store_true',
                        dest='codecov')
    task_.add_argument('-m', '--main',
                        help='Designates task as the main tests (uses main.c)',
                        default=False,
                        action='store_true',
                        dest='main')
    task_.add_argument('-s', '--single',
                        help='Designates task as an individual test (uses single.c)',
                        dest='single')

    expand.add_argument('-c', '--clean',
                        help='Cleans all build objects',
                        default=False,
                        action='store_true',
                        dest='clean')
    expand.add_argument('-cf', '--clean-full',
                        help='Cleans all build objects, source and header files, and removes folders',
                        default=False,
                        action='store_true',
                        dest='cleanfull')
    expand.add_argument('-e', '--expand',
                       help='Expands the header and source files',
                       default=False,
                       action='store_true',
                       dest='expand')
    expand.add_argument('-nf', '--no-format',
                       help='Does not format the output of expand. Can be used when you might not have clang-format',
                       default=False,
                       action='store_true',
                       dest='noformat')
    expand.add_argument('-nt', '--no-tidy',
                        help='Don\'t move files around',
                        default=False,
                        action='store_true',
                        dest='notidy')

    action.add_argument('-b', '--build',
                        help='Build task',
                        default=False,
                        action='store_true',
                        dest='build')
    action.add_argument('-r', '--run',
                        help='Run task',
                        default=False,
                        action='store_true',
                        dest='run')

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

    has_target = args.main or args.codecov or args.single

    if args.config:
        print(CONFIG)
        exit(0)

    if args.fmt_source or args.fmt_tests:
        require_executable('find')
        require_executable('xargs')
        format_code(args.fmt_source, args.fmt_tests)
        # Prevent mixing code
        exit(0)

    if not args.cleanfull:
        assert_structure()
        if args.clean:
            clean_build(False)
    else:
        clean_build(True)
        # Nothing else will work, so just exit
        exit(0)

    if args.expand:
        expand_code()
        if not args.noformat:
            require_executable('clang-format')
            format_expand()
        else:
            # In case the next task is to build the code from source
            # This warning is a very annoying one from gcc
            CFLAGS += ['-Wno-misleading-indentation']

    if args.build:
        require_executable(CC)
    if args.codecov:
        require_executable('gcov')

    if args.codecov:
        require_file(f'{OUTPUT_DIR}/{CODECOV}.c')
        if args.build:
            build_codecov()
        if args.run:
            run_codecov()
    elif args.main:
        require_file(f'{OUTPUT_DIR}/{MAIN}.c')
        if args.build:
            build_main()
        if args.run:
            run_main()
    elif args.single:
        require_file(f'{OUTPUT_DIR}/{SINGLE}.c')
        # TODO

    if not args.notidy and args.codecov:
        tidy()
