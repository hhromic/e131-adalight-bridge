/**
 * E1.31 to AdaLight Bridge Server
 * Hugo Hromic - http://github.com/hhromic
 *
 * Copyright 2016 Hugo Hromic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <termios.h>

// initialise a serial device file descriptor
void init_serial(int fd, speed_t baud_rate) {
  // serial port config swiped from RXTX library (rxtx.qbang.org):
  struct termios tty;
  tcgetattr(fd, &tty);
  tty.c_iflag = INPCK;
  tty.c_lflag = 0;
  tty.c_oflag = 0;
  tty.c_cflag = CREAD | CS8 | CLOCAL;
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 0;
  cfsetispeed(&tty, baud_rate);
  cfsetospeed(&tty, baud_rate);
  tcsetattr(fd, TCSANOW, &tty);
}
