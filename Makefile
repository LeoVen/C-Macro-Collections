
help:
	@echo Welcome to the C Macro Collections Library. Targets:
	@echo
	@echo - tests: run tests
	@echo - format: formats the entire project

tests:
	make -C ./tests

tests_dev:
	make -C ./tests dev

format:
	- find ./ -name *.h -type f | xargs clang-format --style=file --verbose -i

bench:
	gcc ./benchmarks/benchmark.c -o ./benchmarks/benchmark.exe -I ./src -O3
	./benchmarks/benchmark.exe
