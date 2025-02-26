#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

UART_device uart_dev;

int main(int argc, char *argv[]) {

  int cnt = 0;

  uint8_t msg[] = "hello uart this is a test 1234567890\r\nfjdklsfj\r\n";
  uint8_t r_data[sizeof(msg)];

  uart_begin(&uart_dev, "/dev/ttyS3", 8, 1, 'N');

  while (1) {
    uart_write(&uart_dev, msg, sizeof(msg) - 1);
    uart_read(&uart_dev, r_data, sizeof(msg) - 1);
    r_data[sizeof(msg) - 1] = '\0';
    puts((char*)r_data);
    sleep(1);
  }

  return 0;
}
