#include "cpu.h"

#include "addressingmodes.h"
#include "instructions.h"
#include "memory.h"

// Instruction grid
static void (*addressingModeTable[256])() = {
    impl, xind, impl, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm,  acc, impl, abso, abso, abso,impl,
     rel, indy, impl, impl,  zpg, zpgx, zpgx,  zpg, impl, absy,  acc, impl, abso, absx, absx,impl,
    abso, xind, impl, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm,  acc, impl, abso, abso, abso,impl,
     rel, indy, impl, impl, zpgx, zpgx, zpgx,  zpg, impl, absy,  acc, impl, absx, absx, absx,impl,
    impl, xind, impl, impl, impl,  zpg,  zpg,  zpg, impl,  imm,  acc, impl, abso, abso, abso,impl,
     rel, indy, impl, impl, impl, zpgx, zpgx,  zpg, impl, absy, impl, impl, impl, absx, absx,impl,
    impl, xind, impl, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm,  acc, impl,  ind, abso, abso,impl,
     rel, indy, impl, impl, zpgx, zpgx, zpgx,  zpg, impl, absy, impl, impl, impl, absx, absx,impl,
     rel, xind, impl, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm, impl, impl, abso, abso, abso,impl,
     rel, indy, impl, impl, zpgx, zpgx, zpgy,  zpg, impl, absy, impl, impl, abso, absx, absx,impl,
     imm, xind,  imm, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm, impl, impl, abso, abso, abso,impl,
     rel, indy, impl, impl, zpgx, zpgx, zpgy,  zpg, impl, absy, impl, impl, absx, absx, absy,impl,
     imm, xind, impl, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm, impl, impl, abso, abso, abso,impl,
     rel, indy, impl, impl, impl, zpgx, zpgx,  zpg, impl, absy, impl, impl, impl, absx, absx,impl,
     imm, xind, impl, impl,  zpg,  zpg,  zpg,  zpg, impl,  imm, impl, impl, abso, abso, abso,impl,
     rel, indy, impl, impl, impl, zpgx, zpgx,  zpg, impl, absy, impl, impl, impl, absx, absx,impl
};

static void (*instructionsTable[256])() = {
    BRK,  ORA,  NOP,  NOP,  NOP,  ORA,  ASL, NOP,  PHP,  ORA,  ASL,  NOP,  NOP,  ORA,  ASL, NOP,
    BPL,  ORA,  ORA,  NOP,  NOP,  ORA,  ASL, NOP,  CLC,  ORA,  INC,  NOP,  NOP,  ORA,  ASL, NOP,
    JSR,  AND,  NOP,  NOP,  BIT,  AND,  ROL, NOP,  PLP,  AND,  ROL,  NOP,  BIT,  AND,  ROL, NOP,
    BMI,  AND,  AND,  NOP,  BIT,  AND,  ROL, NOP,  SEC,  AND,  DEC,  NOP,  BIT,  AND,  ROL, NOP,
    RTI,  EOR,  NOP,  NOP,  NOP,  EOR,  LSR, NOP,  PHA,  EOR,  LSR,  NOP,  JMP,  EOR,  LSR, NOP,
    BVC,  EOR,  EOR,  NOP,  NOP,  EOR,  LSR, NOP,  CLI,  EOR,  NOP,  NOP,  NOP,  EOR,  LSR, NOP,
    RTS,  ADC,  NOP,  NOP,  NOP,  ADC,  ROR, NOP,  PLA,  ADC,  ROR,  NOP,  JMP,  ADC,  ROR, NOP,
    BVS,  ADC,  ADC,  NOP,  NOP,  ADC,  ROR, NOP,  SEI,  ADC,  NOP,  NOP,  JMP,  ADC,  ROR, NOP,
    NOP,  STA,  NOP,  NOP,  STY,  STA,  STX, NOP,  DEY,  BIT,  TXA,  NOP,  STY,  STA,  STX, NOP,
    BCC,  STA,  STA,  NOP,  STY,  STA,  STX, NOP,  TYA,  STA,  TXS,  NOP,  NOP,  STA,  NOP, NOP,
    LDY,  LDA,  LDX,  NOP,  LDY,  LDA,  LDX, NOP,  TAY,  LDA,  TAX,  NOP,  LDY,  LDA,  LDX, NOP,
    BCS,  LDA,  LDA,  NOP,  LDY,  LDA,  LDX, NOP,  CLV,  LDA,  TSX,  NOP,  LDY,  LDA,  LDX, NOP,
    CPY,  CMP,  NOP,  NOP,  CPY,  CMP,  DEC, NOP,  INY,  CMP,  DEX,  NOP,  CPY,  CMP,  DEC, NOP,
    BNE,  CMP,  CMP,  NOP,  NOP,  CMP,  DEC, NOP,  CLD,  CMP,  NOP,  NOP,  NOP,  CMP,  DEC, NOP,
    CPX,  SBC,  NOP,  NOP,  CPX,  SBC,  INC, NOP,  INX,  SBC,  NOP,  NOP,  CPX,  SBC,  INC, NOP,
    BEQ,  SBC,  SBC,  NOP,  NOP,  SBC,  INC, NOP,  SED,  SBC,  NOP,  NOP,  NOP,  SBC,  INC, NOP
};


// Registers
uint16_t programCounter = 0x00;
uint8_t stackPointer = 0x00;     // Refered to as S
uint8_t accumulator = 0x00;      // Refered to as A
uint8_t indexRegisterX = 0x00;   // Refered to as X
uint8_t indexRegisterY = 0x00;   // Refered to as Y
uint8_t processorStatus = 0x00;  // Refered to as P
uint16_t currentAddress = 0x0000;

uint8_t currentOpcode = 0x00;

uint8_t CpuGetValue() {
    if (addressingModeTable[currentAddress] == acc) {
        return accumulator;
    }
    return CpuReadMemory(currentAddress);
}

void CpuPushStack(uint8_t value) {
    CpuWriteMemory(0x100 + stackPointer, value);
    stackPointer--;
}

uint8_t CpuPullStack() {
    stackPointer++;
    return CpuReadMemory(0x100 + stackPointer);
}

void CpuReset() {
    processorStatus = 0;
    programCounter = 0;
    uint8_t hi = CpuReadMemory(0xFFFD);
    uint8_t lo = CpuReadMemory(0xFFFC);
    programCounter = ((uint16_t)hi << 8) | lo;
}

// Function
void CpuStep() {
    CpuWriteMemory(0xFFFD, 0x01);
    CpuReset();
    printf("%04X\n", programCounter);
}

void CpuFetch() {}

void CpuDecode() {}

void CpuExecute() {}
