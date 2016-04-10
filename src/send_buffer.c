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
#include <termios.h>
#include <unistd.h>

// send a buffer to a file descriptor
void send_buffer(int fd, unsigned char *buffer, size_t size) {
  ssize_t bytes_sent, bytes_to_go, i;
  tcdrain(fd);
  for (bytes_sent=0, bytes_to_go=size; bytes_to_go > 0;) {
    if ((i = write(fd, &buffer[bytes_sent], bytes_to_go)) > 0) {
      bytes_to_go -= i;
      bytes_sent += i;
    }
  }
}
