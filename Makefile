CC := gcc
CFLAGS := -Wall -O2
BUILD_DIR=build
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(addprefix $(BUILD_DIR)/,$(OBJECTS))
OUT_EXEC := hw1

all: $(BUILD_DIR) $(OUT_EXEC)

clean:
	rm -rf $(BUILD_DIR)/ $(OUT_EXEC)

$(OUT_EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

build/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.c: %.h

%.h:

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
