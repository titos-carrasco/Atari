FUJI_DIR:=/mnt/sda1/roberto/MicroComputers/Atari/fujinet/fujinet-lib-atari-4.11.2
FUJI_LIB:=$(FUJI_DIR)/fujinet-atari-4.11.2.lib

BUILD_DIR := build
SRC_DIR   := src
DISK_DOS  := DosXL230
DISK_DIR  := Disk

TARGET        := netcat
TARGET_EXE    := $(BUILD_DIR)/$(TARGET).xex
TARGET_DISK   := $(DISK_DIR)/$(TARGET).xex
TARGET_MAP    := $(BUILD_DIR)/$(TARGET).map
TARGET_LABELS := $(BUILD_DIR)/$(TARGET).labels

SYSTEM     := atari
SYSTEM_LIB := atari.lib
CONFIG     := fujinet-xex.cfg

CFLAGS  := -t $(SYSTEM) -I $(FUJI_DIR) -I $(SRC_DIR) -T -Osir
ASFLAGS := -t $(SYSTEM) -I $(FUJI_DIR)
LDFLAGS := -C $(CONFIG) -m $(TARGET_MAP) -Ln $(TARGET_LABELS) -vm

# --------------------------------------------
CC := cc65
CA := ca65
LD := ld65

SRC_C65 := $(wildcard $(SRC_DIR)/*.c)
SRC_ASM := $(wildcard $(SRC_DIR)/*.s)
OBJ_C65 := $(SRC_C65:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_ASM := $(SRC_ASM:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

.PRECIOUS: $(BUILD_DIR)/%.s

.PHONY: all clean dist

all: dist

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

dist: $(TARGET_EXE)
	cp $(TARGET_EXE) $(DISK_DIR)
	dir2atr -S -b DosXL230 $(BUILD_DIR)/$(TARGET).atr $(DISK_DIR)/

clean:
	rm -fr $(BUILD_DIR)
	rm -f $(TARGET_DISK)
