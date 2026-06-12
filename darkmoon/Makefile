# ================================================================== #
#  Darkmoon Engine - Cross-platform Makefile                         #
#  Generates: libdarkmoon.so (Linux) / darkmoon.dll (Windows)        #
#                                                                    #
#  Usage:                                                            #
#      make            -> build the library                          #
#      make install    -> install into PREFIX (default: /usr/local)  #
#      make uninstall  -> uninstall                                  #
#      make clean      -> remove objects and binaries                #
#      make dist       -> create distribution tarball                #
# ================================================================== #

# ------------------ #
# Platform detection #
# ------------------ #
ifeq ($(OS),Windows_NT)
    PLATFORM     := windows
    TARGET       := darkmoon.dll
    SHARED_FLAG  := -shared
    EXT          := dll
    LIBPREFIX    :=
    SONAME_FLAG  :=
    INSTALL_LIB  := $(PREFIX)/bin
else
    UNAME := $(shell uname -s)

    ifeq ($(UNAME),Darwin)
        PLATFORM    := macos
        EXT         := dylib
        LIBPREFIX   := lib
        TARGET      := libdarkmoon.$(EXT)

        SHARED_FLAG := -dynamiclib -install_name @rpath/$(TARGET)
        SONAME_FLAG :=

        INSTALL_LIB := $(PREFIX)/lib
    else
        PLATFORM    := linux
        EXT         := so
        LIBPREFIX   := lib

        LIB_MAJOR   := 1
        LIB_MINOR   := 0
        LIB_PATCH   := 0

        LIB_VERSION := $(LIB_MAJOR).$(LIB_MINOR).$(LIB_PATCH)

        TARGET_REAL := libdarkmoon.so.$(LIB_VERSION)
        TARGET_SO   := libdarkmoon.so.$(LIB_MAJOR)
        TARGET      := libdarkmoon.so

        SHARED_FLAG := -shared
        SONAME_FLAG := -Wl,-soname,$(TARGET_SO)

        INSTALL_LIB := $(PREFIX)/lib
    endif
endif

# ----------- #
# Directories #
# ----------- #
PREFIX      ?= /usr/local

SRC_DIR     := src
INC_DIR     := include
OBJ_DIR     := obj
LIBS_DIR    := libs
DIST_DIR    := dist

GLAD_INC    := $(LIBS_DIR)/glad/include
GLAD_SRC    := $(LIBS_DIR)/glad/src/glad.c
GLAD_OBJ    := $(OBJ_DIR)/libs/glad.o

STB_INC     := $(LIBS_DIR)/stb

# ------------------ #
# Compiler and flags #
# ------------------ #
CXX         := g++
CC          := gcc

CXXFLAGS    := -std=c++2b -O2 -fPIC -Wall -Wextra
CFLAGS      := -O2 -fPIC

INCLUDES    := -I$(INC_DIR) -I$(GLAD_INC) -I$(STB_INC) -I$(LIBS_DIR)/glfw -I$(LIBS_DIR)

# Platform-specific linker flags
ifeq ($(PLATFORM),windows)
    LDFLAGS := -L$(LIBS_DIR)/glfw/lib -lglfw3 -lopengl32 -lgdi32 -lwinmm -Wl,--export-all-symbols -static-libgcc -static-libstdc++ -static
else ifeq ($(PLATFORM),macos)
    LDFLAGS := -lglfw -framework OpenGL
else
    LDFLAGS := -lglfw -lGL -ldl -lpthread
endif

# ------------------- #
# Sources and objects #
# ------------------- #

# Recursively find all engine source files
CXX_SRCS := $(shell find $(SRC_DIR) -name "*.cpp")

# Convert:
#   src/foo/bar.cpp
# into:
#   obj/foo/bar.o
CXX_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CXX_SRCS))
ALL_OBJS := $(CXX_OBJS) $(GLAD_OBJ)

# ------------ #
# Main targets #
# ------------ #
.PHONY: all clean install uninstall dist help

all: $(TARGET)

# ---------------------------------------- #
# Linux shared library versioning          #
#                                          #
# Generates:                               #
#   libdarkmoon.so.1.0.0  -> real binary    #
#   libdarkmoon.so.1      -> SONAME symlink #
#   libdarkmoon.so        -> linker symlink #
# ---------------------------------------- #
ifeq ($(PLATFORM),linux)

$(TARGET): $(TARGET_REAL)
	ln -sf $(TARGET_REAL) $(TARGET_SO)
	ln -sf $(TARGET_SO)   $(TARGET)

	@echo "  Library generated:"
	@echo "    $(TARGET_REAL)  ← real binary"
	@echo "    $(TARGET_SO)    ← SONAME symlink"
	@echo "    $(TARGET)       ← linker symlink"

$(TARGET_REAL): $(ALL_OBJS)
	$(CXX) $(SHARED_FLAG) $(SONAME_FLAG) -o $@ $^ $(LDFLAGS)

# --------------------- #
# Windows / macOS build #
# --------------------- #
else

$(TARGET): $(ALL_OBJS)
	$(CXX) $(SHARED_FLAG) -o $@ $^ $(LDFLAGS)

	@echo ""
	@echo "  Library generated: $(TARGET)"

endif

# ------------------------ #
# Compile C++ source files #
# ------------------------ #
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ----------------------- #
# Compile glad (C source) #
# ----------------------- #
$(GLAD_OBJ): $(GLAD_SRC)
	@mkdir -p $(OBJ_DIR)/libs
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ------------------------------- #
# Automatic dependency generation #
# ------------------------------- #
DEPS := $(ALL_OBJS:.o=.d)

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# ------------------- #
# Install / uninstall #
# ------------------- #
install: all
	@echo "Installing into $(PREFIX)..."

	install -d $(DESTDIR)$(INSTALL_LIB)
	install -d $(DESTDIR)$(PREFIX)/include/darkmoon

ifeq ($(PLATFORM),linux)
	install -m 755 $(TARGET_REAL) $(DESTDIR)$(INSTALL_LIB)/
	ln -sf $(TARGET_REAL) $(DESTDIR)$(INSTALL_LIB)/$(TARGET_SO)
	ln -sf $(TARGET_SO)   $(DESTDIR)$(INSTALL_LIB)/$(TARGET)
	ldconfig $(DESTDIR)$(INSTALL_LIB) 2>/dev/null || true
else
	install -m 755 $(TARGET) $(DESTDIR)$(INSTALL_LIB)/
endif

	cp -r $(INC_DIR)/darkmoon/. \
	      $(DESTDIR)$(PREFIX)/include/darkmoon/

	@echo "Installation completed."

uninstall:
	@echo "Uninstalling..."

ifeq ($(PLATFORM),linux)
	rm -f $(DESTDIR)$(INSTALL_LIB)/libdarkmoon.so*
else ifeq ($(PLATFORM),macos)
	rm -f $(DESTDIR)$(INSTALL_LIB)/libdarkmoon.dylib
else
	rm -f $(DESTDIR)$(INSTALL_LIB)/darkmoon.dll
endif

	rm -rf $(DESTDIR)$(PREFIX)/include/darkmoon

	@echo "Uninstalled."

# -------------------- #
# Distribution package #
# -------------------- #
DIST_NAME := darkmoon-$(LIB_VERSION)

dist: all
	@echo "Generating distribution package..."

	rm -rf $(DIST_DIR)/$(DIST_NAME)

	mkdir -p $(DIST_DIR)/$(DIST_NAME)/{lib,include/darkmoon}

ifeq ($(PLATFORM),linux)
	cp $(TARGET_REAL) $(DIST_DIR)/$(DIST_NAME)/lib/
	cd $(DIST_DIR)/$(DIST_NAME)/lib && ln -sf $(TARGET_REAL) $(TARGET_SO) && ln -sf $(TARGET_SO) $(TARGET)
else
	cp $(TARGET) $(DIST_DIR)/$(DIST_NAME)/lib/
endif

	cp -r $(INC_DIR)/darkmoon/. $(DIST_DIR)/$(DIST_NAME)/include/darkmoon/

	cp README.md $(DIST_DIR)/$(DIST_NAME)/ 2>/dev/null || true

	cd $(DIST_DIR) && tar -czf $(DIST_NAME).tar.gz $(DIST_NAME)

	rm -rf $(DIST_DIR)/$(DIST_NAME)

	@echo "  -> $(DIST_DIR)/$(DIST_NAME).tar.gz"

# ------- #
# Cleanup #
# ------- #
clean:
	rm -rf $(OBJ_DIR)

	rm -f libdarkmoon.so*
	rm -f libdarkmoon.dylib
	rm -f darkmoon.dll

	rm -rf $(DIST_DIR)

	@echo "Clean completed."

# ---- #
# Help #
# ---- #
help:
	@echo ""
	@echo "  Available targets:"
	@echo "    make              -> build the shared library"
	@echo "    make install      -> install into PREFIX"
	@echo "    make uninstall    -> uninstall the library"
	@echo "    make dist         -> create distribution package"
	@echo "    make clean        -> remove objects and binaries"
	@echo ""
	@echo "  Variables:"
	@echo "    PREFIX=/path      -> installation directory"
	@echo "    DESTDIR=/staging  -> staging root for packages"
	@echo ""
	@echo "  Examples:"
	@echo "    make PREFIX=~/.local install"
	@echo "    make DESTDIR=/tmp/pkg install"
	@echo ""