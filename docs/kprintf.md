# kprintf

`kprintf()` provides formatted output for the PnutOS kernel.

## Interface

```c
void kprintf(const char *fmt, ...);
```

- `fmt` - format string
- `...` - arguments corresponding to the format specifiers

```c
void vkprintf(const char *fmt, va_list args);
```

- `fmt` - format string
- `args` - arguments corresponding to the format specifiers, stored in a `va_list`

## Supported Format Specifiers

| Specifier | Description |
|---|---|
| `%c` | Character |
| `%s` | String |
| `%d` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` | Unsigned hexadecimal integer |
| `%ld` | Signed long decimal integer |
| `%lu` | Unsigned long decimal integer |
| `%lx` | Unsigned long hexadecimal integer |
| `%p` | Pointer |
| `%%` | Literal `%` |

## Design

`kprintf()` initializes a `va_list` and delegates formatting to `vkprintf()`.

`vkprintf()` parses the format string and writes output through the UART interface.
