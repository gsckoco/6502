#include "memory.h"

uint8_t internalMemory[0x7FFF];
uint8_t rom[0x8000];
uint8_t outOfBounds; // Gets returned in CpuGetMemoryPointer instead of producing a segfault

uint8_t* CpuGetMemoryPointer(uint16_t address) {
    if (address >= 0x0000 && address <= 0x7FFF) {
        return &internalMemory[address];
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        return &rom[address - 0x8000];
    }
    return &outOfBounds; // Segfault go away
}

uint8_t CpuReadMemory(uint16_t address) {
    return *CpuGetMemoryPointer(address);
}

void CpuWriteMemory(uint16_t address, uint8_t value) {
    *CpuGetMemoryPointer(address) = value;
}