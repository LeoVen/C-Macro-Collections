
help:
	@echo Welcome to the C Macro Collections Library. Targets:
	@echo
	@echo - tests: run tests
	@echo - tests_dev: run tests for dev
	@echo - format: formats the entire project
	@echo - bench: runs benchmarks
	@echo - docbuild: builds documentation to ./docs
	@echo - docserve: serves mdbook docs

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

.PHONY: tests tests_dev format bench docbuild docserve
