CC = g++
CFLAGS = -g -Wall

INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)

all: main

main: main.cpp
	@echo "Compiling..."
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -o $(BIN_DIR)/main main.cpp $(SRC_FILES) 

clean:
	@echo " Cleaning..."
	$(RM) $(BIN_DIR)*