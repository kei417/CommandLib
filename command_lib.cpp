
#include "mbed.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "command_lib.h"


/**
 * command 文字列を スペース文字で分割し、listに分割後の文字列を設定します。
 * command 文字列は、strtokの特性によりスペース文字が NULL文字に置き換わることに
 * 注意してください。
 * @param command [in/out] コマンド文字列
 * @param list [out] スペース文字で分割後の文字列リスト
 * @param max [in] listの配列最大要素数
 * @return 分割要素数。分割できない場合は 0 を返却
 */
int split(char* command, char* list[], int max) {

	int index = 0;

	if (command == NULL || list == NULL || max == 0) {
		return 0;
	}

	// listの初期化
	for (index = 0; index < max; ++index) {
		list[index] = NULL;
	}

	// スペース文字で分割した文字列を listに設定
	index = 0;
	for (char* space = strtok(command, " "); space != NULL && index < max; space = strtok(NULL, " ")) {
		list[index++] = space;
	}

	return index;
}


/**
 * command 文字列の中の部分文字列 start から end でくくられた文字列を抽出し、
 * buffer にコピーする。bufferはあらかじめ領域を確保しておくこと
 * @param command [in] コマンド文字列
 * @param startDelim [in] 開始部分文字列
 * @param endDelim [in] 終了部分文字列
 * @param buffer [out] 抽出した文字列
 * @retval 抽出した文字数を返す。見つからない場合は 0 を返却
 */
int extract(const char* command, const char* startDelim, const char* endDelim, char* buffer) {

	buffer[0] = '\0';

	if (command == NULL || startDelim == NULL || buffer == NULL) {
		return 0;
	}
	
	int startLength = strlen(startDelim);
	const char* startAddr = strstr(command, startDelim);

	if (startAddr != NULL) {
		int bufferLength = 0;
		if (endDelim == NULL) {
			int commandLength = strlen(command);
			bufferLength = commandLength - (startAddr + startLength - command);
			strncpy(buffer, startAddr + startLength, bufferLength);

		}
		else {
			const char* endAddr = strstr(startAddr + startLength, endDelim);
			if (endAddr != NULL) {
				bufferLength = endAddr - (startAddr + startLength);
				strncpy(buffer, startAddr + startLength, bufferLength);
				buffer[bufferLength] = '\0';
			}
		}
		return bufferLength;
	}
	return 0;
}


/**
 * シリアルに書式化した文字列を出力します。printfと同様の書式化文字列で
 * シリアル出力することができます。
 * 可変長引数の処理は基本的に遅い(処理に時間がかかる)ことに注意してください。
 * よってこのメソッドは、最低限もしくはデバッグ中のみに使用してください。
 * @param serial [in/out] シリアル
 * @param format [in] フォーマット文字列
 * @param ... [in] 可変長文字列
 * @return 出力した文字数
 */
int  serial_print(UnbufferedSerial& serial, const char* format, ...) {

    char buffer[256] = {0};

    if (format == NULL) {
        return 0;
    }

    va_list args;
    va_start(args, format);

    int length = vsprintf(buffer, format, args);

    serial.write(buffer, length);

    va_end(args);

    return length;
}