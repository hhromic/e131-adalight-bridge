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
#include <stdlib.h>

// initialise and return an AdaLight buffer
unsigned char * init_adalight_buffer(int num_leds) {
  // buffer size = header + 3 bytes per LED
  unsigned char *buffer = (unsigned char *)calloc(6 + (num_leds * 3), 1);
  if (buffer == NULL) {
    perror("buffer calloc");
    exit(EXIT_FAILURE);
  }

  // initialise header
  buffer[0] = 'A'; // Magic word
  buffer[1] = 'd';
  buffer[2] = 'a';
  buffer[3] = (num_leds - 1) >> 8;          // LED count high byte
  buffer[4] = (num_leds - 1) & 0xff;        // LED count low byte
  buffer[5] = buffer[3] ^ buffer[4] ^ 0x55; // Checksum
  return buffer;
}
