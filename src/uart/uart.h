#pragma once

#include <stdint.h>
#include <termios.h>
#include <unistd.h>

typedef struct {
  int fd; //
  struct termios params;
} UART_device;

int uart_begin(UART_device *uart_dev, char *UART_device, int databits,
                   int stopbits, int parity);
int uart_end(const UART_device *uart_dev);
int uart_setBaudrate(UART_device *uart_dev, uint32_t Baudrate);

ssize_t uart_write(const UART_device *uart_dev, const uint8_t *buf, uint32_t len);
ssize_t uart_read(const UART_device *uart_dev, uint8_t *buf, uint32_t len);
uint8_t uart_writeByte(const UART_device *uart_dev, uint8_t buf);
uint8_t uart_readByte(const UART_device *uart_dev);

int uart_Set(UART_device *uart_dev, int databits, int stopbits, int parity);

int uart_RxBytesWaiting(const UART_device *uart_dev);
int uart_waitTxToFinish(const UART_device *uart_dev);
