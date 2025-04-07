CC = gcc
CFLAGS = -Wall -Iinclude -Iinc

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

OBJS = $(OBJ_DIR)/dp1.o $(OBJ_DIR)/dp2.o $(OBJ_DIR)/dc.o

all: $(BIN_DIR)/DP-1 $(BIN_DIR)/DP-2 $(BIN_DIR)/DC

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/DP-1: $(OBJ_DIR)/dp1.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $<

$(BIN_DIR)/DP-2: $(OBJ_DIR)/dp2.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $<

$(BIN_DIR)/DC: $(OBJ_DIR)/dc.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
