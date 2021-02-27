#ifndef MEMORY_H
#define MEMORY_H
#include "stdint.h"
#include "stdio.h"

/**
 *
 *  Define your own memory groups here.
 *  Once you have defined your own memory groups, you will need to edit
 *  CpuGetMemoryPointer to return the correct pointers for the correct addresses.
 *
 */

extern uint8_t internalMemory[0x7FFF];
extern uint8_t rom[0x8000];
extern uint8_t outOfBounds; // Gets returned in CpuGetMemoryPointer instead of producing a segfault

extern uint8_t* CpuGetMemoryPointer(uint16_t address);

extern uint8_t CpuReadMemory(uint16_t address);

extern void CpuWriteMemory(uint16_t address, uint8_t value);

#endif // !MEMORY_H