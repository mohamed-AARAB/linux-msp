#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "msp.h"
#include "msp_protocol.h"
#include "msp_serial.h"
#include "streambuf.h"
#include "uart.h"

UART_device uart_dev;
mspPort_t mspPort;

static mspResult_e processMSP(mspPacket_t *cmd, mspPacket_t *reply,
                              mspPostProcessFnPtr *mspPostProcessFn) {
  printf("MSP data arrived with command: %d\n", cmd->cmd);
  printf("Data:\n\t");

  puts((char *)cmd->buf.ptr);

  printf("\n");

  return MSP_RESULT_NO_REPLY;
}

void exitHandler(int signo) {
  uart_end(&uart_dev);
  printf("\r\nInterrupt: exiting...\r\n");
  exit(0);
}

int main(int argc, char *argv[]) {

  signal(SIGINT, exitHandler);

  if (uart_begin(&uart_dev, "/dev/ttyS3", 8, 1, 'N') != 0) {
    perror("failed to setup serial port");
  }
  mspSerialInit(&mspPort, &uart_dev, false);

  uint8_t msg[] = "hello msp";
  uint8_t r_data[sizeof(msg)];

  while (1) {
    mspSerialPush(&mspPort, MSP_API_VERSION, msg, sizeof(msg) - 1);
    mspSerialProcessPort(&mspPort, MSP_SKIP_NON_MSP_DATA, processMSP);
    sleep(1);
  }

  return 0;
}
