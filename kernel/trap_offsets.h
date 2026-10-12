#pragma once

#define TRAP_FRAME_SIZE 288

#define TF_RA 0
#define TF_SP 8
#define TF_GP 16
#define TF_TP 24

#define TF_T0 32
#define TF_T1 40
#define TF_T2 48

#define TF_S0 56
#define TF_S1 64

#define TF_A0 72
#define TF_A1 80
#define TF_A2 88
#define TF_A3 96
#define TF_A4 104
#define TF_A5 112
#define TF_A6 120
#define TF_A7 128

#define TF_S2 136
#define TF_S3 144
#define TF_S4 152
#define TF_S5 160
#define TF_S6 168
#define TF_S7 176
#define TF_S8 184
#define TF_S9 192
#define TF_S10 200
#define TF_S11 208

#define TF_T3 216
#define TF_T4 224
#define TF_T5 232
#define TF_T6 240

#define TF_SSTATUS 248
#define TF_SEPC 256
#define TF_SCAUSE 264
#define TF_STVAL 272
