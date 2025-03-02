tests: bloom_filter.o tests.c
	gcc -lm -o tests tests.c bloomfilter.o murmur3/murmur3.c

bloomfilter.o: bloom_filter.c bloom_filter.h
	gcc -ggdb -o bloomfilter.o -c bloom_filter.c

.PHONY:
	all

all: bloomfilter.o
