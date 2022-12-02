.PHONY: all clean run
CXX = g++
CFLAGS = -std=c++11
OPTFLAGS = -O3
LDFLAGS = -lm
TRAIN_ITER = 100

SRC_DIR = src/
INC_DIR = inc/
BIN_DIR = bin/
OBJ_DIR = build/

EXEC = train test train_and_test
INC = -I$(INC_DIR)
SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJ = $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

.PHONY: all clean

all: directories $(EXEC)

directories: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

train: $(filter-out $(OBJ_DIR)train_and_test.o, $(filter-out $(OBJ_DIR)test.o, $(OBJ)))
	$(CXX) $(CFLAGS) $(OPTFLAGS) $(LDFLAGS) $^ -o $(BIN_DIR)train

test: $(filter-out $(OBJ_DIR)train_and_test.o, $(filter-out $(OBJ_DIR)train.o, $(OBJ)))
	$(CXX) $(CFLAGS) $(OPTFLAGS) $(LDFLAGS) $^ -o $(BIN_DIR)test

train_and_test: $(filter-out $(OBJ_DIR)train.o, $(filter-out $(OBJ_DIR)test.o, $(OBJ)))
	$(CXX) $(CFLAGS) $(OPTFLAGS) $(LDFLAGS) $^ -o $(BIN_DIR)train_and_test

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo ">> Compiling $<"
	$(CXX) $(CFLAGS) $(OPTFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

