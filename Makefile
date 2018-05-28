#TODO: MDL was a little rushed and everything got messed up. Pls fix

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

# Old, C program:
#all: bin_dir_exists $(PROGRAM_NAME)
#	./$(PROGRAM_NAME)
#	#display *.ppm
#
#$(PROGRAM_NAME): parser $(HEADERS) $(OBJS) $(BIN_DIR)/print_pcode.o
#	$(CC) -o $@ $(OBJS) $(BIN_DIR)/print_pcode.o $(HEADERS) $(SRC_DIR)/y.tab.h $(CFLAGS) 


all: bin_dir_exists parser

parser: $(SRC_DIR)/y.tab.c $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.h $(OBJS) $(BIN_DIR)/print_pcode.o
	$(CC) -o mdl $(CFLAGS) $(SRC_DIR)/y.tab.c $(SRC_DIR)/lex.yy.c $(BIN_DIR)/print_pcode.o $(OBJS) $(LDFLAGS)

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/mdl.l $(SRC_DIR)/y.tab.h
	flex -I -o $@ $(SRC_DIR)/mdl.l 

$(SRC_DIR)/y.tab.c: $(SRC_DIR)/mdl.y $(SRC_DIR)/symtab.h $(SRC_DIR)/parser.h
	bison --defines=$(SRC_DIR)/y.tab.h -o $@ -y $(SRC_DIR)/mdl.y

$(SRC_DIR)/y.tab.h: $(SRC_DIR)/mdl.y
	bison --defines=$@ -o $@ -y $(SRC_DIR)/mdl.y

$(BIN_DIR)/print_pcode.o: $(SRC_DIR)/print_pcode.c $(SRC_DIR)/parser.h $(SRC_DIR)/matrix.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/print_pcode.c -o $@

# uh
#$(BIN_DIR)/y.tab.o: $(SRC_DIR)/y.tab.c
#	$(CC) -c $(CFLAGS) $(SRC_DIR)/y.tab.c -o $(BIN_DIR)/y.tab.o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC)  -c $< -o $@ $(CFLAGS)

bin_dir_exists:
	mkdir -p $(BIN_DIR)

run: all
	./mdl

clean:
	rm -f ./y.tab.c #TODO: Why is this file generated? Figure that out!
	rm -f $(SRC_DIR)/y.tab.c $(SRC_DIR)/y.tab.h
	rm -f $(SRC_DIR)/lex.yy.c
	rm -rf $(SRC_DIR)/mdl.dSYM
	rm -f $(PROGRAM_NAME)
	rm -rf $(BIN_DIR)
	rm -f mdl
	rm -f *.ppm
	rm -f *.png
