APP 	 := Engine

MKDIR    := mkdir -p
SRC		 := src
OBJ		 := obj
ASSETS   := assets

CC 		 := g++
ifeq ($(filter clang+,$(MAKECMDGOALS)),clang+)
	CC	 := clang+
endif
	
CCFLAGS  := -g -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -Isrc/
LIBS     := 

# comment

ALLCPP      := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ   := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS     := $(shell find $(SRC) -type d)
OBJSUBDIRS  := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
		$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS)

%.o : %.cpp
		$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)

# auxiliar

$(OBJSUBDIRS) :
		$(MKDIR) $(OBJSUBDIRS)

$(ASSETS) :
		$(ASSETS) $(ASSETS)

.PHONY : dir clean game

game : $(APP)
		./$(APP)

dir :
		$(info $(ASSETS))
		$(info $(SUBDIRS))
		$(info $(OBJSUBDIRS))
		$(info $(ALLCPP))
		$(info $(ALLCPPOBJ))

clean:
		rm -r ./$(OBJ)/ ./$(APP)