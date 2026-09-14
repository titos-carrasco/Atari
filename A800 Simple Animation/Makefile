BUILD_DIR := build
SRC_DIR   := src

TARGET        := simpanim
TARGET_EXE    := $(BUILD_DIR)/$(TARGET).xex
TARGET_DISK   := $(DISK_DIR)/$(TARGET).xex
TARGET_MAP    := $(BUILD_DIR)/$(TARGET).map
TARGET_LABELS := $(BUILD_DIR)/$(TARGET).labels

CC65_PATH   := e:/Software/cc65
SYSTEM      := atari
SYSTEM_LIB  := atari.lib
SYSTEM_CONF := $(CC65_PATH)/cfg/atari-xex.cfg
SYSTEM_INC  := $(CC65_PATH)/include/

CFLAGS  := -t $(SYSTEM) -I $(SYSTEM_INC) -I $(SRC_DIR) -T -Osir
ASFLAGS := -t $(SYSTEM) 
LDFLAGS := -C $(SYSTEM_CONF) -m $(TARGET_MAP) -Ln $(TARGET_LABELS) -vm

# --------------------------------------------
CC := $(CC65_PATH)/bin/cc65
CA := $(CC65_PATH)/bin/ca65
LD := $(CC65_PATH)/bin/ld65

SRC_C65 := $(wildcard $(SRC_DIR)/*.c)
SRC_ASM := $(wildcard $(SRC_DIR)/*.s)
OBJ_C65 := $(SRC_C65:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_ASM := $(SRC_ASM:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

.PRECIOUS: $(BUILD_DIR)/%.s

.PHONY: all clean run

all: $(TARGET_EXE) run

$(TARGET_EXE): $(OBJ_C65) $(OBJ_ASM)
	$(info Generando '$(TARGET_EXE)')
	$(LD) $(LDFLAGS) $(OBJ_C65) $(OBJ_ASM) $(FUJI_LIB) $(SYSTEM_LIB) -o $@

$(BUILD_DIR)/%.s: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s | $(BUILD_DIR)
	$(CA) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CA) $(ASFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET_EXE)
	$(info RUN ...)
clean:
	rm -fr $(BUILD_DIR)
	rm -f $(TARGET_DISK)
