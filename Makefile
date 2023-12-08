
## By my CHATGPT bro
CC = gcc
CFLAGS = -Wall
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
BIN_DIR = bin
BINARIES = $(addprefix $(BIN_DIR)/,$(notdir $(SRC_FILES:%.c=%)))

all: $(BINARIES)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean