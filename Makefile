# This file is useful to format all the source files and header in this library using clang-format

.PHONY
main:
	clang-format --style=file -i ./src/cmc/*.h
