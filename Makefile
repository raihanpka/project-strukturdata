# Portable Makefile for SISTEMKAI (Windows & Unix)
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Iinclude/management -Iinclude/users -Iinclude/data_structure
SRC = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)
OBJ = $(patsubst src/%,build/%,$(SRC:.cpp=.o))
TARGET = SISTEMKAI

# OS detection
ifeq ($(OS),Windows_NT)
	RM = del /s /q
	RM_DIR = if exist build rmdir /s /q build
	MKDIR = if not exist $(1) mkdir $(1)
	SEP = \
	EXE = .exe
else
	RM = rm -rf
	RM_DIR = rm -rf build
	MKDIR = mkdir -p $(1)
	SEP = /
	EXE =
endif

all: | build $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)$(EXE)

build/%.o: src/%.cpp
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	@$(call MKDIR,build)

clean:
	$(RM) build$(SEP)*.o
	$(RM) $(TARGET)$(EXE)
	$(RM_DIR)

.PHONY: all clean
