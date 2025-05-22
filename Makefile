CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Iinclude/management -Iinclude/users -Iinclude/data_structure
SRC = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)

OBJ = $(patsubst src/%,build/%,$(SRC:.cpp=.o))
TARGET = SISTEMKAI

# Detect OS
UNAME_S := $(shell uname -s)

all: | build $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	@mkdir -p build

clean:
ifeq ($(UNAME_S),Linux)
	rm -rf build $(TARGET)
else ifeq ($(UNAME_S),Darwin)
	rm -rf build $(TARGET)
else
	del /Q build $(TARGET) 2>nul || rmdir /S /Q build
endif
