CXX ?= g++

SRC_DIR = $(shell pwd)/src

CPP_FILES = $(wildcard src/*.cc)
TEST_CPP_FILES = $(filter-out src/main.cc, $(sort $(CPP_FILES) $(wildcard test/*.cc)))

OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cc=.o)))
TEST_OBJ_FILES = $(addprefix obj/,$(notdir $(TEST_CPP_FILES:.cc=.o)))

LD_FLAGS ?= -pthread -flto
CC_FLAGS ?= -Wall -std=c++11 -O3 -march=native -flto -pthread -fno-exceptions

OBJ_DIR = obj

BIN_NAME = oNNode

all: $(OBJ_DIR) $(BIN_NAME)


$(BIN_NAME): $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cc
	$(CXX) $(CC_FLAGS) -c -o $@ $<

obj/%.o: test/%.cc
	$(CXX) $(CC_FLAGS) -I $(SRC_DIR) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TEST_BIN_NAME)
	rm -f $(BIN_NAME)
