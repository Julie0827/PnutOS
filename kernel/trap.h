#pragma once

#include "trap_offsets.h"

#include <stddef.h>
#include <stdint.h>

struct trap_frame {
  uint64_t ra;
  uint64_t sp;
  uint64_t gp;
  uint64_t tp;

  uint64_t t0;
  uint64_t t1;
  uint64_t t2;

  uint64_t s0;
  uint64_t s1;

  uint64_t a0;
  uint64_t a1;
  uint64_t a2;
  uint64_t a3;
  uint64_t a4;
  uint64_t a5;
  uint64_t a6;
  uint64_t a7;

  uint64_t s2;
  uint64_t s3;
  uint64_t s4;
  uint64_t s5;
  uint64_t s6;
  uint64_t s7;
  uint64_t s8;
  uint64_t s9;
  uint64_t s10;
  uint64_t s11;

  uint64_t t3;
  uint64_t t4;
  uint64_t t5;
  uint64_t t6;

  uint64_t sstatus;
  uint64_t sepc;
  uint64_t scause;
  uint64_t stval;

  uint64_t padding;
};

void trap_handler(struct trap_frame *frame);

static_assert(TRAP_FRAME_SIZE % 16 == 0);
static_assert(sizeof(struct trap_frame) == TRAP_FRAME_SIZE);

static_assert(offsetof(struct trap_frame, ra) == TF_RA);
static_assert(offsetof(struct trap_frame, sp) == TF_SP);
static_assert(offsetof(struct trap_frame, gp) == TF_GP);
static_assert(offsetof(struct trap_frame, tp) == TF_TP);

static_assert(offsetof(struct trap_frame, t0) == TF_T0);
static_assert(offsetof(struct trap_frame, t1) == TF_T1);
static_assert(offsetof(struct trap_frame, t2) == TF_T2);

static_assert(offsetof(struct trap_frame, s0) == TF_S0);
static_assert(offsetof(struct trap_frame, s1) == TF_S1);

static_assert(offsetof(struct trap_frame, a0) == TF_A0);
static_assert(offsetof(struct trap_frame, a1) == TF_A1);
static_assert(offsetof(struct trap_frame, a2) == TF_A2);
static_assert(offsetof(struct trap_frame, a3) == TF_A3);
static_assert(offsetof(struct trap_frame, a4) == TF_A4);
static_assert(offsetof(struct trap_frame, a5) == TF_A5);
static_assert(offsetof(struct trap_frame, a6) == TF_A6);
static_assert(offsetof(struct trap_frame, a7) == TF_A7);

static_assert(offsetof(struct trap_frame, s2) == TF_S2);
static_assert(offsetof(struct trap_frame, s3) == TF_S3);
static_assert(offsetof(struct trap_frame, s4) == TF_S4);
static_assert(offsetof(struct trap_frame, s5) == TF_S5);
static_assert(offsetof(struct trap_frame, s6) == TF_S6);
static_assert(offsetof(struct trap_frame, s7) == TF_S7);
static_assert(offsetof(struct trap_frame, s8) == TF_S8);
static_assert(offsetof(struct trap_frame, s9) == TF_S9);
static_assert(offsetof(struct trap_frame, s10) == TF_S10);
static_assert(offsetof(struct trap_frame, s11) == TF_S11);

static_assert(offsetof(struct trap_frame, t3) == TF_T3);
static_assert(offsetof(struct trap_frame, t4) == TF_T4);
static_assert(offsetof(struct trap_frame, t5) == TF_T5);
static_assert(offsetof(struct trap_frame, t6) == TF_T6);

static_assert(offsetof(struct trap_frame, sstatus) == TF_SSTATUS);
static_assert(offsetof(struct trap_frame, sepc) == TF_SEPC);
static_assert(offsetof(struct trap_frame, scause) == TF_SCAUSE);
static_assert(offsetof(struct trap_frame, stval) == TF_STVAL);
