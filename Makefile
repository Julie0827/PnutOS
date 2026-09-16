CC := riscv64-unknown-elf-gcc
CFLAGS := \
	-std=c23 \
	-Wall \
	-Wextra \
	-march=rv64gc \
	-mabi=lp64d \
	-mcmodel=medany \
	-ffreestanding \
	-fno-stack-protector

LDSCRIPT := linker.ld
LDFLAGS := -nostdlib -T $(LDSCRIPT)

QEMU := qemu-system-riscv64
QEMUFLAGS := -machine virt -bios none -smp 1 -nographic

BUILD := build
KERNEL := $(BUILD)/pnutos.elf

SRCS_S := $(wildcard kernel/*.S)
SRCS_C := $(wildcard kernel/*.c)
OBJS := $(SRCS_S:kernel/%.S=$(BUILD)/%.o) $(SRCS_C:kernel/%.c=$(BUILD)/%.o)

.PHONY: all run clean

all: $(KERNEL)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: kernel/%.S | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: kernel/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

run: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -kernel $(KERNEL)

clean:
	rm -rf $(BUILD)
