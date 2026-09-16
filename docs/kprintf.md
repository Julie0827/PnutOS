# kprintf

`kprintf()` provides formatted output for the PnutOS kernel.

## Interface

```c
void kprintf(const char *fmt, ...);
```

- `fmt` - format string
- `...` - arguments corresponding to the format specifiers

## Supported Format Specifiers

| Specifier | Description |
|---|---|
| `%c` | Character |
| `%s` | String |
| `%d` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` | Hexadecimal integer |
| `%p` | Pointer |
| `%%` | Literal `%` |

## Design

`kprintf()` parses the format string and writes output through the UART interface.
