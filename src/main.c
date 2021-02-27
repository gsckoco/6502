/**
 *
 * main.c
 *
 */
#include "main.h"
#include "cpu/cpu.h"
#include "cpu/memory.h"
#include "debugger/debugger.h"
#include "stdio.h"

int running = 1;

int main(void) {
    DebuggerInitTerminal();
    DebuggerDrawTerminal();
    while (DebuggerHandleKeyPress(getch()) && running) {
        DebuggerDrawTerminal();
    }
    DebuggerEndTerminal();
    return 0;
}