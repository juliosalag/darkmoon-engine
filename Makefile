# Main configuration
APP 	 := Engine

MKDIR    := mkdir -p
SRC		 := src
OBJ		 := obj
ASSETS   := assets

SANITIZE :=

CC 		 := g++
CCFLAGS  := -g -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -Isrc/ -Ilibs/glad/include $(SANITIZE)
LIBS     := -lglfw -lGLU -lGL $(SANITIZE)

# Source files and objects
ALLCPP      := $(shell find $(SRC) -type f -iname *.cpp) libs/glad/src/glad.c
ALLCPPOBJ   := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS     := $(shell find $(SRC) -type d)
OBJSUBDIRS  := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

# Main rule
$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
		$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS)

# Compile each .cpp to .o
%.o : %.cpp
		$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)

# Additional
$(OBJSUBDIRS) :
		$(MKDIR) $(OBJSUBDIRS)
$(ASSETS) :
		$(ASSETS) $(ASSETS)
.PHONY : dir clean game

# Run the engine
game : $(APP)
		./$(APP)

# Show project structure info
dir :
		$(info $(ASSETS))
		$(info $(SUBDIRS))
		$(info $(OBJSUBDIRS))
		$(info $(ALLCPP))
		$(info $(ALLCPPOBJ))

# Clean generated files
clean:
		rm -rf ./$(OBJ)/ ./$(APP)