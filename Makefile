TARGET := ./builds/main

SOURCES := src/main.c src/gac.c
HEADERS := src/gac.h

main: $(SOURCES) $(HEADERS)
	$(CC) -o $(TARGET) $(SOURCES)

.PHONY: run val
run: main
	$(TARGET)

val: main
	valgrind $(TARGET)
