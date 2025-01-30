CC = gcc
CFLAGS = -Wall -g
TARGET = test

SRC = map.c test.c
OBJ = $(SRC:.c=.o)

INC_DIRS = .

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIRS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

