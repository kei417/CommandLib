#pragma once

#include "mbed.h"

/**
 * コマンド文字列を操作するための関数です
 */

int split(char* command, char* list[], int max);

int extract(const char* command, const char* startDelim, const char* endDelim, char* buffer);

int serial_print(UnbufferedSerial& serial, const char* format, ...);