# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Iinclude/management -Iinclude/users -Iinclude/data_structure
SRC = \
    src/main.cpp \
    src/data.cpp \
    src/menu.cpp \
    src/data_structure/queue.cpp \
    src/data_structure/stack.cpp \
    src/management/management.cpp \
    src/users/admin.cpp \
    src/users/user.cpp
OBJ = $(patsubst src/%,build/%,$(SRC:.cpp=.o))
TARGET = SISTEMKAI

all: | build $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

build/%.o: src/%.cpp
	@if not exist $(subst /,\,$(@D)) mkdir $(subst /,\,$(@D))
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	@if not exist build mkdir build

clean:
	del /Q build $(TARGET) 2>nul || rm -rf build $(TARGET)
