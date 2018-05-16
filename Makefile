CC = g++

SRC_DIR = src
BIN_DIR = bin

PROGRAM_NAME = render.out

CFLAGS = -g -std=c++0x
LDFLAGS= -lm

#####################################################################

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

HEADERS = $(wildcard $(call rwildcard, $(SRC_DIR), *.h))

SRCS = $(wildcard $(call rwildcard, $(SRC_DIR), *.cpp))

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

all: bin_dir_exists $(PROGRAM_NAME)
	./$(PROGRAM_NAME)
	#display *.ppm

$(PROGRAM_NAME): parser $(HEADERS) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) 

parser: $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(SRC_DIR)/y.tab.h $(OBJS)
	$(CC) -o mdl $(CFLAGS) $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(OBJS) $(LDFLAGS)

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/mdl.l $(SRC_DIR)/y.tab.h
	flex -I -o $@ $(SRC_DIR)/mdl.l 

$(SRC_DIR)/y.tab.c: $(SRC_DIR)/mdl.y $(SRC_DIR)/symtab.h $(SRC_DIR)/parser.h
	bison --defines=$(SRC_DIR)/y.tab.h -o $@ -y $(SRC_DIR)/mdl.y

$(SRC_DIR)/y.tab.h: $(SRC_DIR)/mdl.y
	bison --defines=$@ -y $(SRC_DIR)/mdl.y

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC)  -c $< -o $@ $(CFLAGS)

bin_dir_exists:
	mkdir -p $(BIN_DIR)

run: all
	./$(PROGRAM_NAME)
	display *.ppm
clean:
	rm -f ./y.tab.c #TODO: Why is this file generated? Figure that out!
	rm -f $(SRC_DIR)/y.tab.c $(SRC_DIR)/y.tab.h
	rm -f $(SRC_DIR)/lex.yy.c
	rm -rf $(SRC_DIR)/mdl.dSYM
	rm -f $(PROGRAM_NAME)
	rm -rf $(BIN_DIR)
	rm -f *.ppm
	rm -f *.png
