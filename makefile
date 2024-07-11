# directories:
OUTPUT_DIR = ./build

# compiler stuff
CXX = g++
CXXSTD = c++20
CXXFLAGS = -Wall -Wextra -std=$(CXXSTD) -g -Wpedantic -DSFML_STATIC
INCLUDE_PATHS = -I"E:/programming_tools/SFML/SFML_Sources/include"

# linker stuff
LINKER_PATHS = -L"E:\programming_tools\SFML\SFML_Build\lib" -L"E:\programming_tools\SFML\SFML_Sources\extlibs\libs-msvc\x64"
LINKER_FLAGS = -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lsfml-main-d -lopengl32 -lwinmm -lgdi32 -lfreetype

# files
SRCS = $(shell ls *.cpp)
OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix $(OUTPUT_DIR)/, $(OBJS))

# output stuff
TARGET = $(OUTPUT_DIR)/main.exe
ARGS = 

all: $(TARGET)

# compile:
$(OUTPUT_DIR)/%.o: %.cpp
	mkdir -p $(OUTPUT_DIR); $(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_PATHS)

# link:
$(TARGET): $(OBJS) makefile
	mkdir -p $(OUTPUT_DIR); $(CXX) -o $(TARGET) $(OBJS) $(LINKER_PATHS) $(LINKER_FLAGS)

clean:
	rm -rf $(OUTPUT_DIR)

run:
	make
	$(TARGET) $(ARGS)