CXX ?= g++
RC ?= rc
XRES ?= xres
MIMESET ?= mimeset
PACKAGE ?= package

TARGET := ReName
VERSION := 0.14
BUILD_DIR := build
SOURCE_DIR := source
TEST_DIR := tests
TRACKER_ADDONS_DIR = $(shell finddir B_USER_NONPACKAGED_ADDONS_DIRECTORY)/Tracker
PACKAGE_NAME := rename
PACKAGE_REVISION := 1
PACKAGE_ARCH = $(shell getarch -p)
PACKAGE_ROOT := $(BUILD_DIR)/package-root
PACKAGE_INFO_TEMPLATE := packaging/PackageInfo.in
PACKAGE_FILE = $(BUILD_DIR)/$(PACKAGE_NAME)-$(VERSION)-$(PACKAGE_REVISION)-$(PACKAGE_ARCH).hpkg

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

.PHONY: all clean install package test

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
	mkdir -p "$(TRACKER_ADDONS_DIR)"
	copyattr --data -- $(TARGET) "$(TRACKER_ADDONS_DIR)/$(TARGET)"

package: test $(TARGET) $(PACKAGE_INFO_TEMPLATE) ../ReName-Old/License
	rm -rf "$(PACKAGE_ROOT)"
	mkdir -p "$(PACKAGE_ROOT)/apps"
	mkdir -p "$(PACKAGE_ROOT)/add-ons/Tracker"
	mkdir -p "$(PACKAGE_ROOT)/documentation/packages/$(PACKAGE_NAME)"
	copyattr --data -- $(TARGET) "$(PACKAGE_ROOT)/apps/$(TARGET)"
	ln -s ../../apps/$(TARGET) "$(PACKAGE_ROOT)/add-ons/Tracker/$(TARGET)"
	cp ../ReName-Old/License "$(PACKAGE_ROOT)/documentation/packages/$(PACKAGE_NAME)/COPYING"
	sed -e 's/@VERSION@/$(VERSION)/g' \
		-e 's/@REVISION@/$(PACKAGE_REVISION)/g' \
		-e 's/@ARCHITECTURE@/$(PACKAGE_ARCH)/g' \
		$(PACKAGE_INFO_TEMPLATE) > "$(PACKAGE_ROOT)/.PackageInfo"
	rm -f "$(PACKAGE_FILE)"
	$(PACKAGE) create -C "$(PACKAGE_ROOT)" "$(abspath $(PACKAGE_FILE))"
	@echo "Created $(PACKAGE_FILE)"

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
