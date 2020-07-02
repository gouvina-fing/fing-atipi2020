CC = g++
CFLAGS = -g -Wall -w --std=c++17

INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)

all: main run diffpnm

main: main.cpp
	@echo "Compiling main.cpp (Denoiser)..."
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -o $(BIN_DIR)/main main.cpp $(SRC_FILES) 

run: run.cpp
	@echo "Compiling run.cpp (Multiple runs)..."
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -o $(BIN_DIR)/run run.cpp $(SRC_FILES) 

diffpnm: src/utils/diffpnm.c
	@echo "Compiling diffpnm.c (For comparing PSNR)..."
	$(CC) $(CFLAGS) src/utils/diffpnm.c -o $(BIN_DIR)/diffpnm

clean:
	@echo " Cleaning..."
	$(RM) $(BIN_DIR)*