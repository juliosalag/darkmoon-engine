APP		   := Motor
CCACHE 	   := ccache
CC         := g++
CCFLAGS    := -std=c++2b -g -Wall -Wpedantic -Wextra -Wconversion -I/usr/include/freetype2

LIBS 	   := -lglfw -lGLEW -lGLU -lGL -lm -lassimp -lfreetype -lgif

SANITIZE   := -fsanitize=address,undefined

# agregar g++ | clang++
# -pg

MKDIR      := mkdir -p
SRC  	   := src
OBJ  	   := obj

ALLCPP     := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ  := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS    := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

# Regla principal (enlazado de los .o)
$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CCACHE) $(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE) -Wl,-rpath=libs

# Regla que compila los .cpp
%.o : %.cpp
	$(CCACHE) $(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) $(SANITIZE)

# Reglas auxiliares que crean carpetas

$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)

# Otras reglas

.PHONY : dir clean engine

engine: $(APP)
	./$(APP)

dir:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLCPPOBJ))

clean:
	rm -r ./$(OBJ)/ ./$(APP)