/**
 * E1.31 (sACN) to AdaLight Bridge Server
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

#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>

static uint8_t header_data[] = {'A', 'd', 'a', 0x00, 0x00, 0x00};

// send a data buffer to a serial device
static void _send_serial_data(int fd, uint8_t *data, size_t size) {
  ssize_t bytes_sent, bytes_to_go, nbytes;
  for (bytes_sent=0, bytes_to_go=size; bytes_to_go > 0;) {
    if ((nbytes = write(fd, &data[bytes_sent], bytes_to_go)) > 0) {
      bytes_to_go -= nbytes;
      bytes_sent += nbytes;
    }
  }
}

// send a buffer with RGB data to an AdaLight serial device
void send_adalight(int fd, uint8_t *rgb_data, size_t size) {
  uint16_t led_count;
  tcdrain(fd);
  led_count = (size / 3) - 1;
  header_data[3] = led_count >> 8;   // LED count high byte
  header_data[4] = led_count & 0xff; // LED count low byte
  header_data[5] = header_data[3] ^ header_data[4] ^ 0x55; // checksum
  _send_serial_data(fd, header_data, 6);
  _send_serial_data(fd, rgb_data, size);
}
