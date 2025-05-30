TARGET := ./builds
.DEFAULT_GOAL := all

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

single_thread: $(LIBS)/* $(TESTS)/single_thread_test.c gac.o gacLL.o
	$(CC) $(TEST_FLAGS) -o $(TEST_TARGET)/single_thread $(TESTS)/single_thread_test.c

.PHONY: testSweep val
testSweep: sweep_test
	$(TEST_TARGET)/sweep_test

valSweep: testSweep
	valgrind $(TEST_TARGET)/sweep_test

singleThread: single_thread
	$(TEST_TARGET)/single_thread

valSingleThread: singleThread
	valgrind $(TEST_TARGET)/single_thread

tests: testSweep singleThread

all: tests gac.o
