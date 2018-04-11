CC = g++

SRC_DIR = src
BIN_DIR = bin

PROGRAM_NAME = render.out

#####################################################################

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

HEADERS = $(wildcard $(call rwildcard, $(SRC_DIR), *.h))

SRCS = $(wildcard $(call rwildcard, $(SRC_DIR), *.cpp))

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

all: bin_dir_exists $(PROGRAM_NAME)
	./$(PROGRAM_NAME)
	#display *.ppm

$(PROGRAM_NAME): $(HEADERS) $(OBJS)
	$(CC) -o $@ $^ 


$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC)  -c $< -o $@

bin_dir_exists:
	mkdir -p $(BIN_DIR)

run: all
	./$(PROGRAM_NAME)
	display *.ppm
clean:
	rm -f $(PROGRAM_NAME)
	rm -rf $(BIN_DIR)
	rm -f *.ppm
	rm -f *.png
