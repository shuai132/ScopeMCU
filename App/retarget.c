/**
 * 重定向c库printf 请确保此文件参与编译
 */

#include <stdio.h>
#include "usart.h"

/**
 * for armcc (勾选MicroLIB)
 */
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
    return ch;
}

/**
 * for gcc
 */
int _write(int fd, char *pBuffer, int size) {
    HAL_UART_Transmit(&huart1, (uint8_t*)pBuffer, size, 0xFFFF);
    return size;
}
