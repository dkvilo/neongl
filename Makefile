CC := gcc -std=c99
FRAMEWORK := include
SANDBOX_SOURCE := sandbox
BUILD_DIR := build

BIN := sandbox

DEPS_DIR := deps

GLFW_DIR := $(DEPS_DIR)/glfw
GLFW_HEADERS := $(GLFW_DIR)/include/
GLFW_LIB := $(GLFW_DIR)/src/libglfw.3.4.dylib

FINAL_CFLAGS := -I./$(FRAMEWORK) -I./$(DEPS_DIR) -I./$(GLFW_HEADERS) ./$(GLFW_LIB) -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL

glfw:
	cd $(GLFW_DIR) && cmake -DBUILD_SHARED_LIBS=ON . && make

prepare:
	mkdir build

clean_bin:
	rm $(BUILD_DIR)/$(BIN)

clean:
	rm -rf $(BUILD_DIR)

build:
	make prepare && $(CC) -w $(SANDBOX_SOURCE)/*.c $(FINAL_CFLAGS) -o $(BUILD_DIR)/$(BIN)

.PHONY:
	build clean glfw prepare