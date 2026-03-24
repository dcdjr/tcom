CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wpedantic -Iinclude -g
SRC = src/main.c src/cpu.c
OBJ = $(SRC:.c=.o)
TARGET = tcom

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c include/cpu.h include/isa.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean: 
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
