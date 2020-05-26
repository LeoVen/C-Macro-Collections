# This file is useful to format all the source files and header in this library using clang-format

ARGS = --verbose

all: src tests

src: FORCE
	find ./src/ -iname *.h | xargs clang-format --style=file --verbose -i

tests: FORCE
	find ./tests/ -iname *.c -o -iname *.h | xargs clang-format --style=file --verbose -i

FORCE:
