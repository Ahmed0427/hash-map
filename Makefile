CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

SRC = map.c test.c
OBJ = map.o test.o
EXEC = test

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
