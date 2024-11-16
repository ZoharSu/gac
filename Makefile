TARGET := ./builds
.DEFAULT_GOAL := main

LIBS := libs/objects
SOURCES := src/gac.c src/gacLL.c
HEADERS := src/gac.h
WARNINGS := -Wall -Werror -Wextra

gac.o: src/gac.c src/gac.h
	$(CC) -ggdb $(WARNINGS) -c -o $(LIBS)/gac.o src/gac.c

gacLL.o: src/gacLL.c src/gac.h
	$(CC) -ggdb $(WARNINGS) -c -o $(LIBS)/gacLL.o src/gacLL.c

main: $(LIBS)/* src/main.c gac.o gacLL.o
	$(CC) -ggdb $(WARNINGS) $(LIBS)/* -lm -o $(TARGET)/main src/main.c

.PHONY: run val
run: main
	$(TARGET)/main

val: main
	valgrind $(TARGET)

all: main gac.o
