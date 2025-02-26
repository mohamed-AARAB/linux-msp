#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bitarray.h"
#include "msp.h"
#include "msp_protocol.h"
#include "msp_serial.h"
#include "streambuf.h"
#include "uart.h"

#define MAX_THROTTLE 1500
#define MIN_THROTL 1000

enum RcChanels {
  RC_ROLL = 0,
  RC_PITCH,
  RC_THROTTLE,
  RC_YAW,
  RC_AUX1,
  RC_AUX2,
  RC_AUX3,
  RC_AUX4,
};

UART_device uart_dev;
mspPort_t mspPort;

static mspResult_e processMSP(mspPacket_t *packet, mspPacket_t *reply,
                              mspPostProcessFnPtr *mspPostProcessFn) {
  printf("MSP data arrived with command: %d\n", packet->cmd);

  switch (packet->cmd) {
  case MSP_BOARD_INFO:
    printf("board info:\t");
    puts((char *)packet->buf.ptr);
    return MSP_RESULT_NO_REPLY;

  case MSP_ACTIVEBOXES: {
    bitarrayElement_t *arr = (bitarrayElement_t *)packet->buf.ptr;
    bool isArmed = bitArrayGet(arr, 0);
    bool isAngle = bitArrayGet(arr, 1);
    bool isHorizon = bitArrayGet(arr, 2);
    printf("isArmed: %d, isAngle: %d, isHorizon: %d\n", isArmed, isAngle,
           isHorizon);
    return MSP_RESULT_NO_REPLY;
  }

  case MSP_RC: {
    uint16_t *rcChanels = ((uint16_t *)packet->buf.ptr);
    printf("rcChanels: %3d, %3d, %3d, %3d, %3d, %3d\n", rcChanels[0],
           rcChanels[1], rcChanels[2], rcChanels[3], rcChanels[4],
           rcChanels[5]);
    return MSP_RESULT_NO_REPLY;
  }

  case MSP_SET_RAW_RC:
    printf("set raw rc!!\n");
    return MSP_RESULT_NO_REPLY;
  }

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

  uint16_t rcChanels[] = {1500, 1500, MIN_THROTL, 1500};
  uint16_t throttleIncrement = 20;
  uint16_t rollIncrement = -20;

  while (1) {
    mspSerialPush(&mspPort, MSP_BOARD_INFO, NULL, 0);
    mspSerialPush(&mspPort, MSP_ACTIVEBOXES, NULL, 0);
    mspSerialPush(&mspPort, MSP_RC, NULL, 0);
    mspSerialProcessPort(&mspPort, MSP_SKIP_NON_MSP_DATA, processMSP);
    mspSerialProcessPort(&mspPort, MSP_SKIP_NON_MSP_DATA, processMSP);
    mspSerialProcessPort(&mspPort, MSP_SKIP_NON_MSP_DATA, processMSP);

    rcChanels[RC_THROTTLE] += throttleIncrement;

    if (rcChanels[RC_THROTTLE] > MAX_THROTTLE) {
      rcChanels[RC_THROTTLE] = MAX_THROTTLE;
      throttleIncrement = -20;
    } else if (rcChanels[RC_THROTTLE] < MIN_THROTL) {
      rcChanels[RC_THROTTLE] = MIN_THROTL;
      throttleIncrement = 20;
    }

    rcChanels[RC_ROLL] += rollIncrement;
    if (rcChanels[RC_ROLL] > MAX_THROTTLE) {
      rcChanels[RC_ROLL] = MAX_THROTTLE;
      rollIncrement = -20;
    } else if (rcChanels[RC_ROLL] < MIN_THROTL) {
      rcChanels[RC_ROLL] = MIN_THROTL;
      rollIncrement = 20;
    }

    mspSerialPush(&mspPort, MSP_SET_RAW_RC, (uint8_t *)rcChanels,
                  sizeof(rcChanels));
    mspSerialProcessPort(&mspPort, MSP_SKIP_NON_MSP_DATA, processMSP);

    usleep(50 * 1000);
  }

  return 0;
}
