# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Output executable
TARGET := $(BIN_DIR)/main.exe

# Explicitly list main.cpp (root-level)
MAIN_SRC := main.cpp

# Collect all .cpp source files from subdirectories
SRC := $(wildcard $(SRC_DIR)/core/*.cpp) \
       $(wildcard $(SRC_DIR)/services/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp)

# Combine all source files
ALL_SRC := $(SRC) $(MAIN_SRC)

# Generate corresponding object files in obj/
OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(ALL_SRC)))

# Default target
all: $(TARGET)

# Link all object files
$(TARGET): $(OBJ) | $(BIN_DIR)
	@echo Linking $(TARGET)...
	$(CXX) $(OBJ) -o $(TARGET)

# Compile each source file by directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/core/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/services/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/utils/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ✅ Explicit rule for main.cpp (root-level)
$(OBJ_DIR)/main.o: $(MAIN_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $(OBJ_DIR)/main.o

# Ensure directories exist
$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(BIN_DIR):
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Clean build artifacts
clean:
	@if exist $(OBJ_DIR) del /Q $(OBJ_DIR)\*.o
	@if exist $(BIN_DIR)\main.exe del /Q $(BIN_DIR)\main.exe

# Run the program
run: all
	@echo Running program...
	@$(TARGET)

.PHONY: all clean run
