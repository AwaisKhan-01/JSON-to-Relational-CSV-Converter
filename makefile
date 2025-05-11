CC = gcc
LEX = flex
YACC = bison

SRC_DIR = src
BUILD_DIR = build
OUTPUT_DIR = output
TEST_DIR = tests

CFLAGS = -g -Wall -I$(SRC_DIR)

# Ensure build directory exists as a prerequisite
$(BUILD_DIR)/.dir:
	@mkdir -p $(BUILD_DIR)

# Ensure output directory exists
$(OUTPUT_DIR)/.dir:
	@mkdir -p $(OUTPUT_DIR)

all: $(BUILD_DIR)/json2relcsv

$(BUILD_DIR)/json2relcsv: $(BUILD_DIR)/scanner.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/json2relcsv.o $(BUILD_DIR)/ast.o $(BUILD_DIR)/schema.o $(BUILD_DIR)/csv.o
	$(CC) -o $@ $^

$(BUILD_DIR)/scanner.o: $(BUILD_DIR)/scanner.c $(BUILD_DIR)/parser.h | $(BUILD_DIR)/.dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/parser.o: $(BUILD_DIR)/parser.c | $(BUILD_DIR)/.dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/scanner.c: $(SRC_DIR)/scanner.l | $(BUILD_DIR)/.dir
	$(LEX) -o $@ $<

$(BUILD_DIR)/parser.c $(BUILD_DIR)/parser.h: $(SRC_DIR)/parser.y | $(BUILD_DIR)/.dir
	$(YACC) -d -o $(BUILD_DIR)/parser.c $<

$(BUILD_DIR)/json2relcsv.o: $(SRC_DIR)/json2relcsv.c $(SRC_DIR)/json2relcsv.h $(BUILD_DIR)/parser.h | $(BUILD_DIR)/.dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/ast.o: $(SRC_DIR)/ast.c $(SRC_DIR)/ast.h | $(BUILD_DIR)/.dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/schema.o: $(SRC_DIR)/schema.c $(SRC_DIR)/schema.h $(SRC_DIR)/ast.h | $(BUILD_DIR)/.dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/csv.o: $(SRC_DIR)/csv.c $(SRC_DIR)/csv.h $(SRC_DIR)/schema.h | $(BUILD_DIR)/.dir
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(OUTPUT_DIR)/*

test: $(BUILD_DIR)/json2relcsv $(OUTPUT_DIR)/.dir
	./$(BUILD_DIR)/json2relcsv $(TEST_DIR)/large.json --out-dir $(OUTPUT_DIR)
