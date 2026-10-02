# Memory

The RAM base address and size are discovered from the DTB during boot, and the kernel and stack ranges are provided by linker symbols.

## Memory Information

- Kernel memory range
- Stack memory range
- RAM range
- RAM size

## Software Interface

```c
void memory_init(uint64_t base, uint64_t size);
void memory_print_info(const char *prefix);
```

## Output

Example:

```text
kernel: 0x80000000 - 0x800015c0
stack: 0x800015c0 - 0x800055c0
ram: 0x80000000 - 0x88000000 (128 MiB)
```
