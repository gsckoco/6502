#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <curses.h>

#include "../cpu/memory.h"
#include "../main.h"

int byte = 0;
uint8_t nibble = 0;
int row = 0;
int cursorRow = 0;

void DebuggerInitTerminal() {
    initscr();
    raw();
    keypad(stdscr, 1);
    noecho();
    start_color();

    CpuWriteMemory(0, 0xFF);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
}

void DebuggerEndTerminal() { endwin(); }

char tmp[11];
int curRow = 0;
int curChar = 0;
void DebuggerDrawTerminal() {
    wbkgd(stdscr, COLOR_PAIR(1));

    for (int y = 0; y < LINES - 1; y++) {
        curRow = (y + row) * 16;
        sprintf(tmp, "0x%04X |", curRow);
        mvaddstr(y, 0, tmp);
        for (int x = 0; x < 16; x++) {
            sprintf(tmp, "%02X", CpuReadMemory(curRow + x));
            mvaddstr(y, 9 + (x * 4), tmp);
        }
        mvaddstr(y, 9 + (16 * 4) - 1, "|");
    }
    move(LINES - 1, 0);
    printw("-------*    0x%04X/0xFFFF", ((row + cursorRow) * 16) + byte);

    move(cursorRow, 9 + (byte * 4) + nibble);
    refresh();
}

int isHexKey(int key) {
    switch (key) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return 1;
        default:
            return 0;
    }
}

/**
 if (key == KEY_RIGHT) {
        if (nibble == 0) {
            nibble = 1;
        } else {
            nibble = 0;
            if (byte + 1 < 16) {
                byte++;
            } else {
                byte = 0;
                cursorRow++;
            }
        }
    } else if (key == KEY_LEFT) {
        if (nibble == 1) {
            nibble = 0;
        } else {
            nibble = 1;
            if (byte - 1 >= 0) {
                byte--;
            } else {
                if (cursorRow - 1 >= 0) {
                    byte = 15;
                    cursorRow--;
                }
            }
        }
    } else if (key == KEY_UP) {
        if (cursorRow - 1 >= 0) {
            cursorRow--;
        } else {
            if (row - 1 >= 0) {
                row--;
            }
        }
    } else if (key == KEY_DOWN) {
        if (cursorRow + 1 < LINES-1) {
            cursorRow++;
        } else {
            if (row + 1 <= 0xFFF0) {
                row++;
            }
        }
    }
    */

int DebuggerHandleKeyPress(int key) {
    if (key == 27) {
        running = 0;
        return 0;
    }
    return 1;
}

#endif  // !DEBUGGER_H