.PHONY: clean realclean all

BUILD_DIR := build
DIST_DIR := dist

TARGET := $(DIST_DIR)/Nova.iso
ELF := $(BUILD_DIR)/fullkernel.bin

CC := clang
AS := nasm
LD := ld.lld
OBJCOPY := llvm-objcopy
GRUB := grub-mkrescue

CFLAGS := -ffreestanding -m32 -g -c -target i686-none-elf
AFLAGS := -f elf -g
LDFLAGS := -Ttext 07E00 --oformat binary -error-limit=0
GFLAGS := /usr/lib/grub/i386-pc

# Collect all source and header files
ASMSRC := $(wildcard Boot/*.asm)
CSRC := $(wildcard Kernel/*.c Graphics/*.c Include/*.c Font/*.c Memory/*.c \
        Interupts/*.c Timer/*.c Drivers/*.c Hardware/*.c Filesystem/*.c \
        Shell/*.c Userspace/*.c Userspace/GUI/*.c)
HEADERS := $(wildcard Kernel/*.h Graphics/*.h Include/*.h Font/*.h Memory/*.h \
           Interupts/*.h Timer/*.h Drivers/*.h Hardware/*.h Filesystem/*.h \
           Shell/*.h Userspace/*.h Userspace/GUI/*.h)

# Directories containing header files
INCLUDE_DIRS := Kernel Graphics Include Font Memory Interupts Timer Drivers \
                Hardware Filesystem Shell Userspace Userspace/GUI

# Add include directories to CFLAGS
CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

# Map source files to object files in the build directory
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(CSRC)) \
       $(patsubst %.asm,$(BUILD_DIR)/%.o,$(ASMSRC))

# Default target
all: $(TARGET)

# Build ISO target
$(TARGET): $(ELF)
	@mkdir -p $(DIST_DIR)
	$(GRUB) $(GFLAGS) -o $@ $(DIST_DIR)

# Link object files into kernel binary
$(ELF): $(OBJ)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

# Compile assembly files
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(AFLAGS) $< -o $@

# Compile C source files
$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)

# Clean everything
realclean: clean
	@rm -rf $(DIST_DIR)
