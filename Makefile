TARGET := ./builds
.DEFAULT_GOAL := testAll

LIBS := libs/objects
SOURCES := src/gac.c src/gacLL.c
HEADERS := src/gac.h
WARNINGS := -Wall -Werror -Wextra
DEBUG := -DGAC_DEBUG -ggdb

TESTS := tests
TEST_TARGET := testTargets
TEST_FLAGS := -ggdb $(WARNINGS) $(LIBS)/* -lm

gac.o: src/gac.c src/gac.h
	$(CC) $(DEBUG) $(WARNINGS) -c -o $(LIBS)/gac.o src/gac.c

gacLL.o: src/gacLL.c src/gac.h
	$(CC) $(DEBUG) $(WARNINGS) -c -o $(LIBS)/gacLL.o src/gacLL.c

sweep_test: $(LIBS)/* $(TESTS)/sweep_test.c gac.o gacLL.o
	$(CC) $(TEST_FLAGS) -o $(TEST_TARGET)/sweep_test $(TESTS)/sweep_test.c

.PHONY: testSweep val
testSweep: sweep_test
	$(TEST_TARGET)/sweep_test

valSweep: testSweep
	valgrind $(TEST_TARGET)/sweep_test

tests: testSweep

all: tests gac.o
