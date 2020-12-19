TARGET_EXEC ?= main

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := ./include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= -Wall -g -std=c++2a $(INC_FLAGS) -MMD -MP
CXX = g++

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CPPFLAGS)

# c++ source
$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p