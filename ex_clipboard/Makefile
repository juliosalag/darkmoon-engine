# ======================================== #
#  Project Makefile                        #
#  Compiles all .cpp in src/ into game.exe #
# ======================================== #

# ----------- #
# Directories #
# ----------- #
SRC_DIR     := src
OBJ_DIR     := obj

DARKMOON_DIR := ../darkmoon

# -------- #
# Platform #
# -------- #
ifeq ($(OS),Windows_NT)
    TARGET  := game.exe
    LDFLAGS := -L$(DARKMOON_DIR) -ldarkmoon
else
    TARGET  := game
    LDFLAGS := -L$(DARKMOON_DIR) -ldarkmoon -Wl,-rpath,$(DARKMOON_DIR)
endif

# -------------------- #
# Compiler and flags   #
# -------------------- #
CXX      := g++
CXXFLAGS := -std=c++2b -O2 -Wall -Wextra
INCLUDES := -I$(DARKMOON_DIR)/include

# ----------------------- #
# Sources and objects     #
# ----------------------- #
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# ------------- #
# Main targets  #
# ------------- #
.PHONY: all clean run help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo ""
	@echo "  Built: $(TARGET)"

# ---------------------- #
# Compile .cpp -> .o     #
# ---------------------- #
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# -------------------- #
# Dependency tracking  #
# -------------------- #
-include $(OBJS:.o=.d)

# ------- #
# Targets #
# ------- #
run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo "  Clean completed."

help:
	@echo ""
	@echo "  Targets:"
	@echo "    make       -> compile $(TARGET)"
	@echo "    make run   -> compile and run"
	@echo "    make clean -> remove objects and binary"
	@echo ""