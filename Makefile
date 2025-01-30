CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ggdb
TARGET = test

$(TARGET):
	$(CC) $(CFLAGS) -o test test.c map.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)

