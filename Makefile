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
TEST_KERNEL := $(BUILD)/pnutos-test.elf

SRC_DIRS := kernel libc common

SRCS_S := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.S))
SRCS_C := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

OBJS := $(SRCS_S:%.S=$(BUILD)/%.o) $(SRCS_C:%.c=$(BUILD)/%.o)

TEST_SRCS := $(wildcard test/*.c) $(foreach dir,$(SRC_DIRS),$(wildcard test/$(dir)/*.c))
TEST_OBJS := $(filter-out $(BUILD)/kernel/main.o,$(OBJS)) $(TEST_SRCS:%.c=$(BUILD)/%.o)

.PHONY: all run test clean

all: $(KERNEL)

$(BUILD)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/test/%.o: CFLAGS += -Itest -Ikernel -I.

$(KERNEL): $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TEST_KERNEL): $(TEST_OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(TEST_OBJS) -o $@

run: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -kernel $(KERNEL)

test: $(TEST_KERNEL)
	$(QEMU) $(QEMUFLAGS) -kernel $(TEST_KERNEL)

clean:
	rm -rf $(BUILD)
