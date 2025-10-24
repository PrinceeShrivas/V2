CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

TARGET := $(BIN_DIR)/main.exe

MAIN_SRC := main.cpp

SRC := $(wildcard $(SRC_DIR)/core/*.cpp) \
       $(wildcard $(SRC_DIR)/services/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp)

ALL_SRC := $(SRC) $(MAIN_SRC)

OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(ALL_SRC)))

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	@echo Linking $(TARGET)...
	$(CXX) $(OBJ) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/core/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/services/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/utils/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(MAIN_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $(OBJ_DIR)/main.o

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(BIN_DIR):
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

clean:
	@if exist $(OBJ_DIR) del /Q $(OBJ_DIR)\*.o
	@if exist $(BIN_DIR)\main.exe del /Q $(BIN_DIR)\main.exe

run: all
	@echo Running program...
	@$(TARGET)

.PHONY: all clean run
