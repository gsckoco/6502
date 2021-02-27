#include "cpu.h"
#include "memory.h"

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

static void zeroFlagCheck() {
    if (accumulator == 0) {
        processorStatus |= ZERO_FLAG;
    } else {
        processorStatus &= ~(ZERO_FLAG);
    }
}
static void negativeFlagCheck() {
    if (accumulator >> 7 == 0b1) {
        processorStatus |= NEGATIVE_FLAG;
    } else {
        processorStatus &= ~(NEGATIVE_FLAG);
    }
}
static void setFlag(int value, int flag) {
    if (value) {
        processorStatus |= flag;
    } else {
        processorStatus &= ~(flag);
    }
}

static int checkFlag(int flag) { return processorStatus & flag; }

// Add with carry
static void ADC() {
    uint16_t addition = CpuReadMemory(currentAddress) + (processorStatus & CARRY_FLAG);
    addition += accumulator;
    setFlag(addition > 255, CARRY_FLAG);
    accumulator = (uint8_t)(addition & 0xFF);
    zeroFlagCheck();
}

// Logical AND
static void AND() {
    accumulator &= CpuReadMemory(currentAddress);
    zeroFlagCheck();
    negativeFlagCheck();
}

// Arithmetic Shift Left
static void ASL() {
    uint8_t lastBit = (CpuGetValue() & 0b10000000) >> 7;
    setFlag(lastBit, CARRY_FLAG);
    accumulator <<= 1;
    zeroFlagCheck();
    negativeFlagCheck();
}

// Branch if Carry Clear
static void BCC() {
    if (checkFlag(CARRY_FLAG) == 0) {
        programCounter = currentAddress;
    }
}

// Branch if Carry Set
static void BCS() {
    if (checkFlag(CARRY_FLAG) > 0) {
        programCounter = currentAddress;
    }
}

// Branch if Equal
static void BEQ() {
    if (checkFlag(ZERO_FLAG) > 0) {
        programCounter = currentAddress;
    }
}

// Bit Test
static void BIT() {
    uint8_t bitTest = accumulator & CpuReadMemory(currentAddress);
    setFlag((bitTest & 0b01000000) >> 6, OVERFLOW_FLAG);
    setFlag((bitTest & 0b10000000) >> 7, NEGATIVE_FLAG);
}

// Branch if Minus
static void BMI() {
    if (checkFlag(NEGATIVE_FLAG)) {
        programCounter = currentAddress;
    }
}

// Branch if Not Equal
static void BNE() {
    if (checkFlag(ZERO_FLAG) == 0) {
        programCounter = currentAddress;
    }
}

// Branch if Positive
static void BPL() {
    if (checkFlag(NEGATIVE_FLAG) == 0) {
        programCounter = currentAddress;
    }
}

// Force Interrupt
static void BRK() {
    CpuPushStack((uint8_t)(programCounter & 0xFF00) >> 8);
    CpuPushStack((uint8_t)(programCounter & 0x00FF));
    CpuPushStack(processorStatus);
    uint16_t vector = CpuReadMemory(0xFFFE) + (CpuReadMemory(0xFFFF) << 8);
    programCounter = vector;
}

// Branch if Overflow Clear
static void BVC() {
    if (checkFlag(OVERFLOW_FLAG) == 0) {
        programCounter = currentAddress;
    }
}

// Branch if Overflow Set
static void BVS() {
    if (checkFlag(OVERFLOW_FLAG) > 0) {
        programCounter = currentAddress;
    }
}

// Clear Carry Flag
static void CLC() { setFlag(0, CARRY_FLAG); }

// Clear Decimal Mode
static void CLD() { setFlag(0, DECIMAL_FLAG); }

// Clear Interrupt Disabled
static void CLI() { setFlag(0, INTERUPT_DISABLED_FLAG); }

// Clear Overflow Flag
static void CLV() { setFlag(0, OVERFLOW_FLAG); }

// Compare
static void CMP() {
    uint8_t value = CpuReadMemory(currentAddress);
    uint8_t result = accumulator - value;
    if (accumulator >= value) {
        setFlag(1, CARRY_FLAG);
    }
    if (accumulator == value) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Compare X Register
static void CPX() {
    uint8_t value = CpuReadMemory(currentAddress);
    uint8_t result = indexRegisterX - value;
    if (accumulator >= value) {
        setFlag(1, CARRY_FLAG);
    }
    if (indexRegisterX == value) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Compare Y Register
static void CPY() {
    uint8_t value = CpuReadMemory(currentAddress);
    uint8_t result = indexRegisterY - value;
    if (accumulator >= value) {
        setFlag(1, CARRY_FLAG);
    }
    if (indexRegisterY == value) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Decrement Memory
static void DEC() {
    uint8_t result = CpuReadMemory(currentAddress) - 1;
    CpuWriteMemory(currentAddress, result);
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Decrement X Register
static void DEX() {
    uint8_t result = indexRegisterX - 1;
    indexRegisterX = result;
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Decrement Y Register
static void DEY() {
    uint8_t result = indexRegisterY - 1;
    indexRegisterY = result;
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Exclusive OR
static void EOR() {
    uint8_t result = accumulator ^ CpuReadMemory(currentAddress);
    accumulator = result;
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Increment
static void INC() {
    uint8_t result = CpuReadMemory(currentAddress) + 1;
    CpuWriteMemory(currentAddress, result);
    zeroFlagCheck();
    negativeFlagCheck();
}

// Increment X Register
static void INX() {
    uint8_t result = indexRegisterX + 1;
    indexRegisterX = result;
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Increment Y Register
static void INY() {
    uint8_t result = indexRegisterY + 1;
    indexRegisterY = result;
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Jump
static void JMP() {
    programCounter = (uint16_t)CpuReadMemory(currentAddress) |
                     ((uint16_t)CpuReadMemory(currentAddress + 1) << 8);
}

// Jump to Subroutine
static void JSR() {
    CpuPushStack((uint8_t)((programCounter - 1) & 0xFF00) >> 8);
    CpuPushStack((uint8_t)((programCounter - 1) & 0x00FF));
    programCounter = (uint16_t)CpuReadMemory(currentAddress) |
                     ((uint16_t)CpuReadMemory(currentAddress + 1) << 8);
}

// Load Accumulator
static void LDA() {
    accumulator = CpuReadMemory(currentAddress);
    zeroFlagCheck();
    negativeFlagCheck();
}

// Load X Register
static void LDX() {
    indexRegisterX = CpuReadMemory(currentAddress);
    if (indexRegisterX == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(indexRegisterX >> 7, NEGATIVE_FLAG);
}

// Load Y Register
static void LDY() {
    indexRegisterY = CpuReadMemory(currentAddress);
    if (indexRegisterY == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(indexRegisterY >> 7, NEGATIVE_FLAG);
}

// Logical Shift Right
static void LSR() {
    uint8_t lastBit = (CpuGetValue() & 0b00000001);
    setFlag(lastBit, CARRY_FLAG);
    accumulator >>= 1;
    zeroFlagCheck();
    negativeFlagCheck();
}

// No Operation
static void NOP() {
    // Do nothing
}

// Logical Inclusive OR
static void ORA() {
    uint8_t result = accumulator | CpuReadMemory(currentAddress);
    accumulator = result;
    if (result == 0) {
        setFlag(1, ZERO_FLAG);
    } else {
        setFlag(0, ZERO_FLAG);
    }
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Push Accumulator
static void PHA() {
    CpuPushStack(accumulator);
}

// Pull Accumulator
static void PLA() {
    accumulator = CpuPullStack();
    zeroFlagCheck();
    negativeFlagCheck();
}

// Push Processor Status
static void PHP() {
    CpuPushStack(processorStatus);
}

// Pull Processor Status
static void PLP() {
    processorStatus = CpuPullStack();
}

// Rotate Left
static void ROL() {
    uint8_t value = CpuGetValue();
    uint8_t result = (value << 1) + (processorStatus & CARRY_FLAG);
    setFlag(value >> 7, CARRY_FLAG);
    setFlag(result == 0, ZERO_FLAG);
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Rotate Right
static void ROR() {
    uint8_t value = CpuGetValue();
    uint8_t result = (value >> 1) + ((processorStatus & CARRY_FLAG) << 7);
    setFlag(value & 0b00000001, CARRY_FLAG);
    setFlag(result == 0, ZERO_FLAG);
    setFlag(result >> 7, NEGATIVE_FLAG);
}

// Return from Interupt
static void RTI() {
    uint16_t lo = CpuPullStack();
    uint16_t hi = CpuPullStack();
    processorStatus = CpuPullStack();
    programCounter = (hi << 8) + lo;
}

// Return from Subrotine
static void RTS() {
    uint16_t lo = CpuPullStack();
    uint16_t hi = CpuPullStack();
    programCounter = (hi << 8) + lo;
}

// Subtract With Carry
static void SBC() {
    uint16_t addition = CpuReadMemory(currentAddress) - (1 - (processorStatus & CARRY_FLAG));
    addition -= accumulator;
    setFlag(addition > 255, CARRY_FLAG);
    accumulator = (uint8_t)(addition & 0xFF);
    zeroFlagCheck();
    negativeFlagCheck();
}

// Set Carry Flag
static void SEC() {
    setFlag(1, CARRY_FLAG);
}

// Set Decimal Flag
static void SED() {
    setFlag(1, DECIMAL_FLAG);
}

// Set Decimal Flag
static void SEI() {
    setFlag(1, INTERUPT_DISABLED_FLAG);
}

// Store Accumulator
static void STA() {
    accumulator = CpuReadMemory(currentAddress);
}

// Store X Register
static void STX() {
    indexRegisterX = CpuReadMemory(currentAddress);
}

// Store Y Register
static void STY() {
    indexRegisterY = CpuReadMemory(currentAddress);
}

// Transfer Accumulator to X
static void TAX() {
    indexRegisterX = accumulator;
    setFlag(indexRegisterX == 0, ZERO_FLAG);
    setFlag(indexRegisterX >> 7, NEGATIVE_FLAG);
}

// Transfer Accumulator to Y
static void TAY() {
    indexRegisterY = accumulator;
    setFlag(indexRegisterY == 0, ZERO_FLAG);
    setFlag(indexRegisterY >> 7, NEGATIVE_FLAG);
}

// Transfer Stack Pointer to X
static void TSX() {
    indexRegisterX = stackPointer;
    setFlag(indexRegisterX == 0, ZERO_FLAG);
    setFlag(indexRegisterX >> 7, NEGATIVE_FLAG);
}

// Transfer X to Accumulator
static void TXA() {
    accumulator = indexRegisterX;
    setFlag(accumulator == 0, ZERO_FLAG);
    setFlag(accumulator >> 7, NEGATIVE_FLAG);
}

// Transfer X to Stack Pointer
static void TXS() {
    stackPointer = indexRegisterX;
    setFlag(stackPointer == 0, ZERO_FLAG);
    setFlag(stackPointer >> 7, NEGATIVE_FLAG);
}

// Transfer Y to Accumulator
static void TYA() {
    accumulator = indexRegisterY;
    setFlag(accumulator == 0, ZERO_FLAG);
    setFlag(accumulator >> 7, NEGATIVE_FLAG);
}

#endif  // !INSTRUCTIONS_H