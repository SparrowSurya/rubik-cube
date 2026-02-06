# -------- Config --------
BUILD_DIR := build
TARGET    := app
CORES     := $(shell nproc)

# -------- Default --------
all: build

# -------- Configure --------
configure:
	cmake -S . -B $(BUILD_DIR)

# -------- Build --------
build: configure
	cmake --build $(BUILD_DIR) -j$(CORES)

# -------- Run --------
run: build
	./$(BUILD_DIR)/$(TARGET)

# -------- Clean build artifacts --------
clean:
	rm -rf $(BUILD_DIR)

# -------- Reconfigure from scratch --------
rebuild: clean build

# -------- Release build --------
release:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) -j$(CORES)

# -------- Debug build --------
debug:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR) -j$(CORES)

.PHONY: all configure build run clean rebuild release debug
