
help:
	@echo Welcome to the C Macro Collections Library. Targets:
	@echo
	@echo - tests: run tests
	@echo - tests_dev: run tests for dev
	@echo - format: formats the entire project
	@echo - bench: runs benchmarks
	@echo - docbuild: builds documentation to ./docs
	@echo - docserve: serves mdbook docs
	@echo - install_dev_deps: install dev dependencies

tests:
	make -C ./tests

tests_dev:
	make -C ./tests dev

format:
	- find ./ -name *.h -type f | xargs clang-format --style=file --verbose -i

bench:
	make -C ./benchmarks benchmark

docbuild:
	mdbook build -d ../docs documentation

docserve:
	mdbook serve documentation

install_dev_deps:
	sudo apt install gcc make valgrind lcov clang-format hyperfine
	cargo install mdbook

.PHONY: tests tests_dev format bench docbuild docserve install_dev_deps
