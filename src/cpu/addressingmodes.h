/**
 *
 * addressingmodes.h
 *
 */
#include "cpu.h"
#include "memory.h"

#ifndef ADDRESSINGMODES_H
#define ADDRESSINGMODES_H

// Inspired by fake6502

static void acc() {
    // Do nothing, implied to be accumulator
}

// Points to an absolute memory location
static void abso() {
    currentAddress = ((uint16_t)CpuReadMemory(programCounter++)) | ((uint16_t)CpuReadMemory(programCounter+2) << 8);
}

// Absolute X indexed with carry
static void absx() {
    currentAddress += ((uint16_t)CpuReadMemory(programCounter++)) | ((uint16_t)CpuReadMemory(programCounter+2) << 8);
    currentAddress += indexRegisterX;
}

// Absolute Y indexed with carry
static void absy() {
    currentAddress += ((uint16_t)CpuReadMemory(programCounter++)) | ((uint16_t)CpuReadMemory(programCounter+2) << 8);
    currentAddress += indexRegisterY;
}

// Implied. No operand
static void imm() {
    currentAddress = programCounter++;
}

// Implied. No operand
static void impl() {
    // Do nothing, no operand
}

// Indirect
static void ind() {
    abso();
    currentAddress = (uint16_t)CpuReadMemory(currentAddress) | ((uint16_t)CpuReadMemory(currentAddress + 1) << 8);
}

// X Indexed, Indirect
static void xind() {
    currentAddress = (uint16_t)(CpuReadMemory(programCounter++) + indexRegisterX);
}

// Indirect, Y Indexed
static void indy() {
    currentAddress = (uint16_t)CpuReadMemory(programCounter++) + indexRegisterY;
}

// Relative
static void rel() {
    currentAddress = programCounter + (int8_t)CpuReadMemory(programCounter++);
}

// Zeropage
static void zpg() {
    currentAddress = (uint16_t)CpuReadMemory(programCounter++);
}

// Zeropage, X Indexed
static void zpgx() {
    currentAddress = (uint8_t)CpuReadMemory(programCounter++) + indexRegisterX;
}

// Zeropage, Y Indexed
static void zpgy() {
    currentAddress = (uint8_t)CpuReadMemory(programCounter++) + indexRegisterY;
}

#endif // !ADDRESSINGMODES_H