CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
TARGET = calculadora

SRCS = src/calculadora.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c src/calculadora.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
