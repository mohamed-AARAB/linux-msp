#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "uart.h"

int uart_begin(UART_device *uart_dev, char *UART_device, int databits,
               int stopbits, int parity) {

  // device
  if ((uart_dev->fd = open(UART_device, O_RDWR | O_NOCTTY)) < 0) { // open UART
    perror("Failed to open UART device.\n");
    return 1;
  }

  printf("open : %s\r\n", UART_device);

  uart_setBaudrate(uart_dev, 115200);
  uart_Set(uart_dev, databits, stopbits, parity);

  return 0;
}

int uart_end(const UART_device *uart_dev) {
  if (close(uart_dev->fd) != 0) {
    perror("Failed to close UART device.\n");
    return -1;
  }
  return 0;
}

int uart_setBaudrate(UART_device *uart_dev, uint32_t Baudrate) {
  uint16_t err;
  uint32_t baud;
  tcflush(uart_dev->fd, TCIOFLUSH); // Clear the data buffer

  switch (Baudrate) // Set the number of data bits
  {
  case B230400:
  case 230400:
    baud = B230400;
    break;
  case B115200:
  case 115200:
    baud = B115200;
    break;
  case B57600:
  case 57600:
    baud = B57600;
    break;
  case B38400:
  case 38400:
    baud = B38400;
    break;
  case B19200:
  case 19200:
    baud = B19200;
    break;
  case B9600:
  case 9600:
    baud = B9600;
    break;
  case B4800:
  case 4800:
    baud = B4800;
    break;
  case B2400:
  case 2400:
    baud = B2400;
    break;
  case B1800:
  case 1800:
    baud = B1800;
    break;
  case B1200:
  case 1200:
    baud = B1200;
    break;
  case B600:
  case 600:
    baud = B600;
    break;
  case B300:
  case 300:
    baud = B300;
    break;
  case B200:
  case 200:
    baud = B200;
    break;
  case B150:
  case 150:
    baud = B150;
    break;
  case B134:
  case 134:
    baud = B134;
    break;
  case B110:
  case 110:
    baud = B110;
    break;
  case B75:
  case 75:
    baud = B75;
    break;
  case B50:
  case 50:
    baud = B50;
    break;
  default:
    baud = B9600;
    break;
  }

  printf("Baud rate setting\r\n");
  if (cfsetispeed(&uart_dev->params, baud) != 0) {
    perror("Baud rate setting failed 1");
  }
  if (cfsetospeed(&uart_dev->params, baud) != 0) {
    perror("Baud rate setting failed 2");
  }
  err = tcsetattr(uart_dev->fd, TCSANOW, &uart_dev->params);
  if (err != 0) {
    perror("Setting the terminal baud rate failed");
    return 1;
  }

  return 0;
}

uint8_t uart_writeByte(const UART_device *uart_dev, uint8_t buf) {
  uint8_t sbuf[1] = {0};
  sbuf[0] = buf;
  write(uart_dev->fd, sbuf, 1);
  return 0;
}

ssize_t uart_write(const UART_device *uart_dev, const uint8_t *buf,
                   uint32_t len) {
  return write(uart_dev->fd, buf, len);
}

uint8_t uart_readByte(const UART_device *uart_dev) {
  uint8_t buf[1] = {0};
  read(uart_dev->fd, buf, 1);
  return buf[0];
}

ssize_t uart_read(const UART_device *uart_dev, uint8_t *buf, uint32_t len) {
  return read(uart_dev->fd, buf, len);
}

int uart_Set(UART_device *uart_dev, int databits, int stopbits, int parity) {
  if (tcgetattr(uart_dev->fd, &uart_dev->params) != 0) {
    perror("tcgetattr fd");
    printf("Failed to get terminal parameters\r\n");
    return -1;
  }

  switch (databits) // Set the number of data bits
  {
  case 5:
    uart_dev->params.c_cflag &= ~CSIZE;
    uart_dev->params.c_cflag |= CS5;
    break;
  case 6:
    uart_dev->params.c_cflag &= ~CSIZE;
    uart_dev->params.c_cflag |= CS6;
    break;
  case 7:
    uart_dev->params.c_cflag &= ~CSIZE;
    uart_dev->params.c_cflag |= CS7;
    break;
  case 8:
    uart_dev->params.c_cflag &= ~CSIZE;
    uart_dev->params.c_cflag |= CS8;
    break;
  default:
    fprintf(stderr, "Unsupported data size.\n");
    return 0;
  }

  switch (parity) // Set check digit
  {
  case 'n':
  case 'N':
    uart_dev->params.c_cflag &= ~PARENB; // Clear check digit
    uart_dev->params.c_iflag &= ~INPCK;  // enable parity checking
    break;
  case 'o':
  case 'O':
    uart_dev->params.c_cflag |= PARENB; // enable parity
    uart_dev->params.c_cflag |= PARODD; // Odd parity
    uart_dev->params.c_iflag |= INPCK;  // disable parity checking
    break;
  case 'e':
  case 'E':
    uart_dev->params.c_cflag |= PARENB;  // enable parity
    uart_dev->params.c_cflag &= ~PARODD; // Even parity
    uart_dev->params.c_iflag |= INPCK;   // disable pairty checking
    break;
  case 's':
  case 'S':
    uart_dev->params.c_cflag &= ~PARENB; // Clear check digit
    uart_dev->params.c_cflag &= ~CSTOPB;
    uart_dev->params.c_iflag |= INPCK; // disable pairty checking
    break;
  default:
    fprintf(stderr, "Unsupported parity.\n");
    return -1;
  }
  switch (stopbits) // Set stop bit  1   2
  {
  case 1:
    uart_dev->params.c_cflag &= ~CSTOPB;
    break;
  case 2:
    uart_dev->params.c_cflag |= CSTOPB;
    break;
  default:
    fprintf(stderr, "Unsupported stopbits.\n");
    return -1;
  }
  uart_dev->params.c_cflag |= (CLOCAL | CREAD);
  uart_dev->params.c_iflag &=
      ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  uart_dev->params.c_oflag &=
      ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OLCUC | OPOST);

  uart_dev->params.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  uart_dev->params.c_oflag &= ~OPOST;
  uart_dev->params.c_oflag &= ~(ONLCR | OCRNL);

  tcflush(uart_dev->fd, TCIFLUSH);
  uart_dev->params.c_cc[VTIME] = 150; // Set timeout
  uart_dev->params.c_cc[VMIN] = 1;    //
  if (tcsetattr(uart_dev->fd, TCSANOW, &uart_dev->params) <
      0) // Update the uart_dev->params and do it now
  {
    printf("Setting terminal parameters failed\n");
    return 0;
  }
  printf("Set terminal parameters successfully\n");
  return 0;
}

int uart_RxBytesWaiting(const UART_device *uart_dev) {
  int bytes_available = -1;
  if (ioctl(uart_dev->fd, FIONREAD, &bytes_available) < 0) {
    return -1;
  }
  return bytes_available;
}

int uart_waitTxToFinish(const UART_device *uart_dev) {
  return tcdrain(uart_dev->fd);
}
