TARGET_EXEC ?= eurorasp

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SHAREDFLAGS ?= -MMD -MP -D__LINUX_ALSA__ -rdynamic
CPPFLAGS ?= $(INC_FLAGS) $(SHAREDFLAGS) -std=c++2a
CFLAGS ?=$(INC_FLAGS) $(SHAREDFLAGS) -DUSE_BCM2835_LIB

BINDIR ?= /usr/bin
SERVICEDIR ?= /etc/systemd/system

.DEFAULT_GOAL := all

LDFLAGS ?= -L/usr/lib:/usr/local/lib
LDLIBS := -lgpiod -lasound -lpthread -levdev -lstdc++fs

ifeq ($(DISABLE_DISPLAY),true)
SHAREDFLAGS += -DDISABLE_DISPLAY
else
LDLIBS += -lbcm2835
endif


cleanall: clean all
all: $(BUILD_DIR)/$(TARGET_EXEC)

#linker
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean install

clean:
	$(RM) -r $(BUILD_DIR)

install: build
	cp $(BUILD_DIR)/$(TARGET_EXEC) $(BINDIR)
	cp util/eurorasp.service $(SERVICEDIR)
	chmod 0700 $(BINDIR)/$(TARGET_EXEC) $(SERVICEDIR)/eurorasp.service
	chown root $(BINDIR)/$(TARGET_EXEC) $(SERVICEDIR)/eurorasp.service
	systemctl enable eurorasp.service
	systemctl start eurorasp.service

-include $(DEPS)

MKDIR_P ?= mkdir -p

