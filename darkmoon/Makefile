# Main configuration
LIB      := darkmoon

MKDIR    := mkdir -p
SRC      := src
OBJ      := obj
INCLUDE  := include

SANITIZE :=
# -fsanitize=address,undefined

CC       := g++
CCFLAGS  := -g -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -fPIC \
            -I$(INCLUDE)/ \
            -Ilibs/glad/include \
            $(SANITIZE)

# Detect OS (Linux vs Windows MSYS2)
ifeq ($(OS),Windows_NT)
    EXT   := dll
    LIBS  := -lglfw3 -lopengl32 -lgdi32
else
    EXT   := so
    LIBS  := -lglfw -lGLU -lGL $(SANITIZE)
endif

TARGET := lib$(LIB).$(EXT)

# Source files and objects
ALLCPP    := $(shell find $(SRC) -type f -name "*.cpp") libs/glad/src/glad.c
ALLCPPOBJ := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(filter %.cpp,$(ALLCPP))) \
             $(OBJ)/libs/glad/src/glad.o

# Derive every obj subdir that will be needed
OBJSUBDIRS := $(sort $(dir $(ALLCPPOBJ)))

# Main rule --> SHARED LIB
$(TARGET): $(ALLCPPOBJ)
	$(CC) -shared -o $@ $(ALLCPPOBJ) $(LIBS)

# Compile .cpp --> .o
$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJSUBDIRS)
	$(CC) -c $< -o $@ $(CCFLAGS)

# Compile glad.c
$(OBJ)/libs/glad/src/glad.o: libs/glad/src/glad.c | $(OBJSUBDIRS)
	$(CC) -c $< -o $@ $(CCFLAGS)

# Create all needed subdirs
$(OBJSUBDIRS):
	$(MKDIR) $@

.PHONY: clean dir

# Debug info
dir:
	$(info SRC:        $(SRC))
	$(info INCLUDE:    $(INCLUDE))
	$(info ALLCPP:     $(ALLCPP))
	$(info ALLCPPOBJ:  $(ALLCPPOBJ))
	$(info OBJSUBDIRS: $(OBJSUBDIRS))

# Clean
clean:
	rm -rf $(OBJ) $(TARGET)