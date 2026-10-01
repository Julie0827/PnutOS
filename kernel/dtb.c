#include "dtb.h"
#include "panic.h"
#include "string.h"

#include <stddef.h>

#define DTB_MAGIC 0xd00dfeed

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

struct dtb_state {
  uint32_t address_cells;
  uint32_t size_cells;
  bool is_memory;
  bool memory_found;
};

static uint32_t read_be32(const void *ptr) {
  const uint8_t *bytes = ptr;

  return ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) | ((uint32_t)bytes[2] << 8) |
         (uint32_t)bytes[3];
}

static bool dtb_is_valid(const void *dtb) {
  return read_be32(dtb) == DTB_MAGIC;
}

static uint32_t dtb_get_header_field(const void *dtb, size_t index) {
  return read_be32((const uint8_t *)dtb + index * 4);
}

static uint32_t dtb_get_struct_offset(const void *dtb) {
  return dtb_get_header_field(dtb, 2);
}

static const void *dtb_get_struct(const void *dtb) {
  return (const uint8_t *)dtb + dtb_get_struct_offset(dtb);
}

static uint32_t dtb_get_strings_offset(const void *dtb) {
  return dtb_get_header_field(dtb, 3);
}

static const void *dtb_get_strings(const void *dtb) {
  return (const uint8_t *)dtb + dtb_get_strings_offset(dtb);
}

static uint32_t read_token(const uint8_t **ptr) {
  uint32_t token = read_be32(*ptr);

  *ptr += 4;

  return token;
}

static const uint8_t *align4(const uint8_t *ptr) {
  uintptr_t addr = (uintptr_t)ptr;

  return (const uint8_t *)((addr + 3) & ~(uintptr_t)3);
}

static bool is_memory_node(const char *name) {
  const char *expected = "memory";

  while (*expected) {
    if (*name++ != *expected++) return false;
  }

  return !*name || *name == '@';
}

static const uint8_t *handle_begin_node(const uint8_t *ptr, struct dtb_state *state) {
  const char *name = (const char *)ptr;

  state->is_memory = is_memory_node(name);

  while (*ptr++) {
  }

  return align4(ptr);
}

static uint64_t read_cells(const uint8_t *ptr, uint32_t cells) {
  if (cells > 2) panic("DTB value exceeds 64 bits");

  uint64_t val = 0;

  for (uint32_t i = 0; i < cells; i++) {
    val = (val << 32) | read_be32(ptr);
    ptr += 4;
  }

  return val;
}

static const uint8_t *handle_prop(const uint8_t *ptr, const uint8_t *strings,
                                  struct dtb_state *state, struct dtb_memory *memory) {
  uint32_t len = read_be32(ptr);
  ptr += 4;

  uint32_t nameoff = read_be32(ptr);
  ptr += 4;

  const char *name = (const char *)(strings + nameoff);

  if (!state->address_cells && !strcmp(name, "#address-cells"))
    state->address_cells = read_be32(ptr);
  else if (!state->size_cells && !strcmp(name, "#size-cells")) state->size_cells = read_be32(ptr);

  if (state->is_memory && strcmp(name, "reg") == 0) {
    memory->base = read_cells(ptr, state->address_cells);
    memory->size = read_cells(ptr + state->address_cells * 4, state->size_cells);

    state->memory_found = true;
  }

  ptr += len;

  return align4(ptr);
}

struct dtb_memory dtb_get_memory(const void *dtb) {
  if (!dtb_is_valid(dtb)) panic("Invalid DTB");

  const uint8_t *ptr = dtb_get_struct(dtb);
  const uint8_t *strings = dtb_get_strings(dtb);

  struct dtb_state state = {0};
  struct dtb_memory memory = {0};

  while (!state.memory_found) {
    switch (read_token(&ptr)) {
      case FDT_BEGIN_NODE:
        ptr = handle_begin_node(ptr, &state);
        break;

      case FDT_END_NODE:
        state.is_memory = false;
        break;

      case FDT_PROP:
        ptr = handle_prop(ptr, strings, &state, &memory);
        break;

      case FDT_NOP:
        break;

      case FDT_END:
        panic("Memory not found in DTB");

      default:
        panic("Unknown DTB token");
    }
  }

  return memory;
}
