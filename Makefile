INCLUDE_DIRS := libc common

CC := riscv64-unknown-elf-gcc
CFLAGS := \
	-std=c23 \
	-Wall \
	-Wextra \
	-march=rv64gc \
	-mabi=lp64d \
	-mcmodel=medany \
	-ffreestanding \
	-fno-stack-protector \
	$(foreach dir,$(INCLUDE_DIRS),-I$(dir))

LDSCRIPT := linker.ld
LDFLAGS := -nostdlib -T $(LDSCRIPT)

QEMU := qemu-system-riscv64
QEMUFLAGS := -machine virt -bios none -smp 1 -nographic

BUILD := build
KERNEL := $(BUILD)/pnutos.elf

SRC_DIRS := kernel libc

SRCS_S := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.S))
SRCS_C := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

OBJS := $(SRCS_S:%.S=$(BUILD)/%.o) $(SRCS_C:%.c=$(BUILD)/%.o)

.PHONY: all run clean

all: $(KERNEL)

$(BUILD)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

run: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -kernel $(KERNEL)

clean:
	rm -rf $(BUILD)
