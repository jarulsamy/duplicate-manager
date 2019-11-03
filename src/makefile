# the compiler: gcc for C program, define as g++ for C++
CC = g++-8

CFLAGS  = --std=c++1z -g -Wall -Wextra -pedantic-errors

# the build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).cpp -lstdc++fs -L/opt/ssl/lib/ -lcrypto

clean:
	$(RM) $(TARGET).out
