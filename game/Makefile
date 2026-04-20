# Main configuration
GAME     := game

MKDIR    := mkdir -p
SRC      := src
OBJ      := obj

SANITIZE :=
# -fsanitize=address,undefined

CC       := g++
CCFLAGS  := -g -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -fPIC \
             -Isrc/ -I../darkmoon/src/ -I../darkmoon/libs/glad/include $(SANITIZE)

# Detect OS
ifeq ($(OS),Windows_NT)
    EXT    := exe
    LIBS   := -lglfw3 -lopengl32 -lgdi32
else
    EXT    := out
    LIBS   := -lglfw -lGLU -lGL $(SANITIZE)
endif

TARGET := $(GAME).$(EXT)

# Source files and objects
ALLCPP     := $(shell find $(SRC) -type f -iname *.cpp)
ALLOBJ     := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(ALLCPP))

SUBDIRS    := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

# Main rule --> EXECUTABLE
$(TARGET): $(OBJSUBDIRS) $(ALLOBJ)
	$(CC) -o $@ $(ALLOBJ) -L. -ldarkmoon $(LIBS) $(SANITIZE)

# Compile .cpp --> .o
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(CCFLAGS)

# Create obj directories
$(OBJSUBDIRS):
	$(MKDIR) $(OBJSUBDIRS)

.PHONY: clean dir

run: $(TARGET)
	./$(TARGET)

# Debug info
dir:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLOBJ))

# Clean
clean:
	rm -rf $(OBJ) $(TARGET)