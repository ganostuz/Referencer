CXX = g++
CC = gcc
WINDRES ?= windres
PKG_CONFIG ?= pkg-config

PACKAGES := glfw3 libcurl assimp
HOST_SYSTEM := $(shell uname -s 2>/dev/null)
WINDOWS_BUILD := $(if $(filter Windows_NT,$(OS)),1,$(if $(findstring MINGW,$(HOST_SYSTEM)),1,))
APP_SOURCES := $(shell find Referencer/src -name '*.cpp' -print)
APP_HEADERS := $(shell find Referencer/src -name '*.h' -print)
IMGUI_SOURCES := \
	Referencer/vendor/imGui/imgui.cpp \
	Referencer/vendor/imGui/imgui_demo.cpp \
	Referencer/vendor/imGui/imgui_draw.cpp \
	Referencer/vendor/imGui/imgui_tables.cpp \
	Referencer/vendor/imGui/imgui_widgets.cpp
IMGUI_HEADERS := $(shell find Referencer/vendor/imGui -maxdepth 2 -name '*.h' -print)

INCLUDES := \
	-IReferencer/src \
	-IReferencer/vendor/Glad/include \
	-IReferencer/vendor/imGui \
	-IReferencer/vendor/GLFW/include \
	-IReferencer/vendor/glm \
	-IReferencer/vendor/toml/toml11-master \
	-IReferencer/vendor/STB

ifeq ($(WINDOWS_BUILD),1)
	PLATFORM := windows
	TARGET := bin/Referencer.exe
	BUILD_DIR := bin-int/windows
	PLATFORM_CPPFLAGS := -D_WIN32_WINNT=0x0601
	PLATFORM_LDFLAGS := -mwindows
	PLATFORM_LIBS := -lopengl32 -lcomdlg32 -lole32 -luuid -lshell32
	PLATFORM_RESOURCES := $(BUILD_DIR)/Referencer.res.o
	DIST_DIR := dist/Referencer-windows-x64
else
	PLATFORM := linux
	TARGET := bin/Referencer
	BUILD_DIR := bin-int/linux
	PLATFORM_LIBS := -lGL -ldl -pthread
endif

RESOURCE_STAMP := bin/resources/.copied
PACKAGE_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(PACKAGES) 2>/dev/null)
PACKAGE_LIBS := $(shell $(PKG_CONFIG) --libs $(PACKAGES) 2>/dev/null)

CPPFLAGS := -DGLFW_INCLUDE_NONE $(PLATFORM_CPPFLAGS) $(INCLUDES) $(PACKAGE_CFLAGS)
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra
LDFLAGS += $(PLATFORM_LDFLAGS)
LDLIBS := $(PACKAGE_LIBS) $(PLATFORM_LIBS)

.PHONY: all clean debug package run check-deps

all: $(TARGET)

debug: CXXFLAGS := -std=c++20 -O0 -g -Wall -Wextra
debug: all

check-deps:
	@$(PKG_CONFIG) --exists $(PACKAGES) || \
		(echo "Missing GLFW, Assimp, or libcurl development files. See README.md."; exit 1)

$(BUILD_DIR)/glad.o: Referencer/vendor/Glad/src/glad.c Makefile
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -O2 -c $< -o $@

ifeq ($(PLATFORM),windows)
$(BUILD_DIR)/Referencer.res.o: Referencer/src/OS/Referencer.rc icon.ico Makefile
	@mkdir -p $(BUILD_DIR)
	$(WINDRES) -I. $< -O coff -o $@
endif

$(RESOURCE_STAMP): $(shell find Referencer/resources -type f -print)
	@mkdir -p bin/resources
	cp -R Referencer/resources/. bin/resources/
	@touch $@

$(TARGET): Makefile $(APP_SOURCES) $(APP_HEADERS) $(IMGUI_SOURCES) $(IMGUI_HEADERS) $(BUILD_DIR)/glad.o $(PLATFORM_RESOURCES) $(RESOURCE_STAMP) | check-deps
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(APP_SOURCES) $(IMGUI_SOURCES) \
		$(BUILD_DIR)/glad.o $(PLATFORM_RESOURCES) -o $@ $(LDFLAGS) $(LDLIBS)

run: all
	./$(TARGET)

ifeq ($(PLATFORM),windows)
package: all
	rm -rf $(DIST_DIR)
	mkdir -p $(DIST_DIR)
	cp $(TARGET) $(DIST_DIR)/
	cp -R bin/resources $(DIST_DIR)/
	@ldd $(TARGET) | awk '$$3 ~ /^\/(mingw64|ucrt64)\/bin\/.*\.dll$$/ { print $$3 }' | \
		while IFS= read -r dll; do cp "$$dll" $(DIST_DIR)/; done
	@echo "Windows artifact prepared at $(DIST_DIR)"
else
package:
	@echo "The package target currently prepares Windows artifacts under MSYS2."
	@exit 1
endif

clean:
	rm -rf bin bin-int dist
