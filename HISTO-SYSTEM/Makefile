CC = gcc
CFLAGS = -Wall -Iinc -Iinc/producer -Iinc/consumer

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

PRODUCER_SRC = $(SRC_DIR)/producer/dp1.c $(SRC_DIR)/producer/dp1_utils.c $(SRC_DIR)/producer/dp2.c $(SRC_DIR)/producer/dp2_utils.c
CONSUMER_SRC = $(SRC_DIR)/consumer/dc.c $(SRC_DIR)/consumer/process_utils.c $(SRC_DIR)/consumer/signal_utils.c
SHARED_SRC = $(SRC_DIR)/shared.c

SRCS = $(PRODUCER_SRC) $(CONSUMER_SRC) $(SHARED_SRC)

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(BIN_DIR)/DP-1 $(BIN_DIR)/DP-2 $(BIN_DIR)/DC

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/DP-1: $(OBJ_DIR)/producer/dp1.o $(OBJ_DIR)/producer/dp1_utils.o $(OBJ_DIR)/shared.o	
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^

$(BIN_DIR)/DP-2: $(OBJ_DIR)/producer/dp2.o $(OBJ_DIR)/producer/dp2_utils.o $(OBJ_DIR)/shared.o	
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^

$(BIN_DIR)/DC: $(OBJ_DIR)/consumer/dc.o $(OBJ_DIR)/consumer/process_utils.o $(OBJ_DIR)/consumer/signal_utils.o $(OBJ_DIR)/shared.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
