TARGET_EXEC := main

BUILD_DIR := bin
SRC_DIRS := src

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJS := $(SRCS:%.cpp=%.o)

INC_DIRS := include
INC_FLAGS := $(addprefix -I ,$(INC_DIRS))

CPPFLAGS := -Wall -Wextra -std=c++2a $(INC_FLAGS) -MMD -MP
CXX := g++

#
# Debug build settings
#
DBG_DIR := $(BUILD_DIR)/debug
DBG_EXE := $(DBG_DIR)/$(TARGET_EXEC)
DBG_OBJS := $(addprefix $(DBG_DIR)/, $(OBJS))
DBG_FLAGS = -g -O0 -DDEBUG
DBG_DEPS = $(DBG_OBJS:.o=.d)

#
# Release build settings
#
REL_DIR := $(BUILD_DIR)/release
REL_EXE := $(REL_DIR)/$(TARGET_EXEC)
REL_OBJS := $(addprefix $(REL_DIR)/, $(OBJS))
REL_FLAGS = -Ofast -DNDEBUG
REL_DEPS = $(REL_OBJS:.o=.d)

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBG_EXE)

$(DBG_EXE): $(DBG_OBJS)
	$(CXX) $(CPPFLAGS) $(DBG_FLAGS) -o $(DBG_EXE) $^

$(DBG_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(DBG_FLAGS) -c -o $@ $<

#
# Release rules
#
release: $(REL_EXE)

$(REL_EXE): $(REL_OBJS)
	$(CXX) $(CPPFLAGS) $(REL_FLAGS) -o $(REL_EXE) $^

$(REL_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(REL_FLAGS) -c -o $@ $<


#
# Other rules
#
prep:
	mkdir -p $(DBG_DIR) $(REL_DIR)

remake: clean all

clean:
	$(RM) -r $(BUILD_DIR)

# not quite happy with this solution to deal with dependencies - Lars
-include $(REL_DEPS)
-include $(DBG_DEPS)

# https://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds
# https://stackoverflow.com/questions/2394609/makefile-header-dependencies