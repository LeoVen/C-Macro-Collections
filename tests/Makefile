CC=gcc
CFLAGS=-Wall -Wextra -Werror -fprofile-arcs -ftest-coverage -ftime-report -g -O0
INCLUDE=-I ../src
OUTDIR=out

main:
	mkdir -p ./$(OUTDIR)
	$(CC) ./main.c $(INCLUDE) $(CFLAGS) -o ./$(OUTDIR)/$@.exe
	valgrind --leak-check=yes ./$(OUTDIR)/$@.exe
	gcov -p ./$(OUTDIR)/$@.gcda

dev: main
	mkdir -p ./coverage
	mv *.gcov ./coverage

single:
	$(CC) ./single.c $(INCLUDE) $(CFLAGS) -o ./$(OUTDIR)/$@.exe -DTEST=$(TEST)
	./$(OUTDIR)/$@.exe

clean:
	rm ./*.gcov ./*.gcda ./*.gcno ./*.exe -f
	rm ./coverage ./$(OUTDIR) -rf

lcov: dev
	lcov --capture --directory ./$(OUTDIR) --output-file ./$(OUTDIR)/coverage.info
	genhtml ./$(OUTDIR)/coverage.info --output-directory out

types:
	$(CC) ./types.c $(INCLUDE) -Wall -Wextra -O3 -ftime-report -o ./$(OUTDIR)/$@.exe
	./$(OUTDIR)/$@.exe

.PHONY: main dev clean lcov types
