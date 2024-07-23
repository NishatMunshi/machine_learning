# directories:
SOURCE_DIR = ./src
OUTPUT_DIR = ./build

# compiler stuff
CXX = g++
CXXSTD = c++20
CXXFLAGS = -Wall -Wextra -std=$(CXXSTD) -g -Wpedantic -DSFML_STATIC
INCLUDE_PATHS =

# linker stuff
LINKER_PATHS =
LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# files
SRCS = $(shell ls $(SOURCE_DIR)/*.cpp)
OBJS = $(patsubst $(SOURCE_DIR)/%.cpp, $(OUTPUT_DIR)/%.o, $(SRCS))

# output stuff
TARGET = $(OUTPUT_DIR)/main
ARGS =

all: $(TARGET)

# compile:
$(OUTPUT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(OUTPUT_DIR); $(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_PATHS)

# link:
$(TARGET): $(OBJS) makefile
	mkdir -p $(OUTPUT_DIR); $(CXX) -o $(TARGET) $(OBJS) $(LINKER_PATHS) $(LINKER_FLAGS)

clean:
	rm -rf $(OUTPUT_DIR)

run:
	$(TARGET) $(ARGS)