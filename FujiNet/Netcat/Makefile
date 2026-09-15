# necesitamos esto aqui
ifeq ($(OS),Windows_NT)
CC65_BIN := E:/Software/cc65/bin
CC65_INC := E:/Software/cc65/include
else
CC65_BIN := /usr/bin
CC65_INC := /usr/share/cc65/include
endif

MAKEFLAGS += --warn-undefined-variables

# definiciones del systema de destino
SYSTEM     		:= atari
SYSTEM_LIB 		:= atari.lib
SYSTEM_CONF		:= atari-xex.cfg
SYSTEM_START	:= 0x3000

# definiciones para mi programa
BUILD_DIR 		:= build
SRC_DIR   		:= src
DISK_DOS  		:= DosXL230
DISK_DIR  		:= Disk
FUJI_DIR  		:=/mnt/sda1/roberto/MicroComputers/Atari/fujinet/fujinet-lib-atari-4.11.2
FUJI_LIB  		:=$(FUJI_DIR)/fujinet-atari-4.11.2.lib

# definiciones de lo que generaremos
TARGET			:= netcat
TARGET_EXE    	:= $(BUILD_DIR)/$(TARGET).xex
TARGET_DISK   	:= $(DISK_DIR)/$(TARGET).xex
TARGET_MAP    	:= $(BUILD_DIR)/$(TARGET).map
TARGET_LABELS 	:= $(BUILD_DIR)/$(TARGET).labels

# los flags para las diferentes etapas
CFLAGS  		:= -t $(SYSTEM) -I $(CC65_INC) -I $(FUJI_DIR) -I $(SRC_DIR) -T -Osir
ASFLAGS 		:= -t $(SYSTEM) -I $(FUJI_DIR)
LDFLAGS 		:= --start-addr $(SYSTEM_START) -C $(SYSTEM_CONF) -m $(TARGET_MAP) -Ln $(TARGET_LABELS) -vm

SRC_C65 := $(wildcard $(SRC_DIR)/*.c)
SRC_ASM := $(wildcard $(SRC_DIR)/*.s)
OBJ_C65 := $(SRC_C65:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_ASM := $(SRC_ASM:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

# --------------------------------------------
CC := $(CC65_BIN)/cc65
CA := $(CC65_BIN)/ca65
LD := $(CC65_BIN)/ld65

.PRECIOUS: $(BUILD_DIR)/%.s
.PHONY: all dist clean

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
	dir2atr -S -b $(DISK_DOS) $(BUILD_DIR)/$(TARGET).atr $(DISK_DIR)/

clean:
	rm -fr $(BUILD_DIR)
	rm -f $(TARGET_DISK)
