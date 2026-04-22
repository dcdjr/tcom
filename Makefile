CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wpedantic -Iinclude -g

SRC = src/main.c src/cpu.c
OBJ = $(SRC:.c=.o)
TARGET = tcom

TEST_SRC = tests/test_cpu.c
TEST_TARGET = test_cpu

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c include/cpu.h include/isa.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

$(TEST_TARGET): $(TEST_SRC) include/cpu.h include/isa.h src/cpu.c
	$(CC) $(CFLAGS) $(TEST_SRC) src/cpu.c -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean: 
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET)

.PHONY: all run clean test
