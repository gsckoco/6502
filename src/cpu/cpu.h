#include "stdint.h"
#include "stdio.h"

#ifndef CPU_H
#define CPU_H

#define CARRY_FLAG 0b00000001
#define ZERO_FLAG 0b00000010
#define INTERUPT_DISABLED_FLAG 0b00000100
#define DECIMAL_FLAG 0b00001000
#define BREAK_FLAG 0b00100000
#define OVERFLOW_FLAG 0b01000000
#define NEGATIVE_FLAG 0b10000000

extern uint16_t programCounter;
extern uint8_t stackPointer;     // Refered to as S
extern uint8_t accumulator;      // Refered to as A
extern uint8_t indexRegisterX;   // Refered to as X
extern uint8_t indexRegisterY;   // Refered to as Y
extern uint8_t processorStatus;  // Refered to as P

uint16_t currentAddress;

extern uint8_t CpuGetValue();

extern void CpuPushStack(uint8_t value);
extern uint8_t CpuPullStack();
extern void CpuReset();
extern void CpuStep();

extern void CpuFetch();
extern void CpuDecode();
extern void CpuExecute();

#endif  // CPU_H