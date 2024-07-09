# Specify the compiler
CXX = g++

# C++ standard
CXXSTD = c++23

# Specify the compiler flags
CXXFLAGS = -g -Wall -Wextra -std=$(CXXSTD) -DSFML_STATIC

# Specify the source files
SRCS = $(shell ls *.cpp)

# Specify the include paths
INCLUDEPATHS = -I"E:/programming_tools/SFML/SFML_Sources/include"

# Specify the dependendicies
LINKERINCLUDE = -L"E:\programming_tools\SFML\SFML_Build\lib" \
				-L"E:\programming_tools\SFML\SFML_Sources\extlibs\libs-msvc\x64"

# Linker flags
LINKERFLAGS = -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lsfml-main-d -lopengl32 -lwinmm -lgdi32 -lfreetype

# Specify the object files
OBJS = $(SRCS:.cpp=.o)

# Specify the executable file
TARGET = main.exe

# Default target
all: $(TARGET) 

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LINKERINCLUDE) $(LINKERFLAGS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDEPATHS)

# Clean target
clean:
	rm -f *.o $(TARGET)