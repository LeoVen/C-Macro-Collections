INCLUDE=-I ../../..
CFLAGS=-O3 -Wall -Wextra -Werror -DCOUNT=20000000 -DCMC_NO_CALLBACKS

# Install hyperfine
# https://github.com/sharkdp/hyperfine

run: build
	hyperfine --warmup 2 --runs 5 './hashmap.exe'
	hyperfine --warmup 2 --runs 5 './hashmultimap.exe'
	hyperfine --warmup 2 --runs 5 './unorderedmap.exe'

build:
	gcc hashmap.c -o hashmap.exe $(INCLUDE) $(CFLAGS)
	gcc hashmultimap.c -o hashmultimap.exe $(INCLUDE) $(CFLAGS)
	g++ unorderedmap.cpp $(INCLUDE) -o unorderedmap.exe $(CFLAGS)

clean:
	rm *.exe -f
	rm *.txt -f
