CXX ?= g++
RC ?= rc
XRES ?= xres
MIMESET ?= mimeset

TARGET := ReName
VERSION := 0.12
BUILD_DIR := build
SOURCE_DIR := source
TEST_DIR := tests

SOURCES := \
	$(SOURCE_DIR)/Application.cpp \
	$(SOURCE_DIR)/MainWindow.cpp \
	$(SOURCE_DIR)/RenameEngine.cpp \
	$(SOURCE_DIR)/main.cpp
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)
RESOURCE_DEFINITION := $(SOURCE_DIR)/ReName.rdef
RESOURCE_FILE := $(BUILD_DIR)/ReName.rsrc
TEST_TARGET := $(BUILD_DIR)/RenameEngineTest

CPPFLAGS := -I$(SOURCE_DIR)
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic
LDLIBS := -lbe -lroot

.PHONY: all clean install test

all: $(TARGET)

$(TARGET): $(OBJECTS) $(RESOURCE_FILE)
	$(CXX) $(OBJECTS) -o $@ $(LDLIBS)
	$(XRES) -o $@ $(RESOURCE_FILE)
	$(MIMESET) -f $@
	chmod 755 $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(RESOURCE_FILE): $(RESOURCE_DEFINITION) | $(BUILD_DIR)
	$(RC) -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

$(TEST_TARGET): $(TEST_DIR)/RenameEngineTest.cpp $(SOURCE_DIR)/RenameEngine.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test: $(TEST_TARGET)
	$(TEST_TARGET)

install: $(TARGET)
	mkdir -p "$(shell finddir B_USER_ADDONS_DIRECTORY)/Tracker"
	copyattr --data -- $(TARGET) "$(shell finddir B_USER_ADDONS_DIRECTORY)/Tracker/$(TARGET)"

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
