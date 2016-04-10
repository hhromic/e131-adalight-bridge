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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/socket.h>
#include <functions.h>

#define BAUD_RATE B115200
#define E131_PORT 5568
#define UDP_BUFFER_SIZE 2048

#define OFST_UNIV_HIGH 113
#define OFST_UNIV_LOW 114
#define OFST_DATA 126

int main(int argc, char **argv) {
  int opt, serial_fd, socket_fd;
  int num_leds = -1, universe = -1;
  char *device = NULL;
  unsigned char *adalight_buffer = NULL;
  unsigned char udp_buffer[UDP_BUFFER_SIZE];
  ssize_t rbytes, adalight_buffer_size;

  // program options
  while ((opt = getopt (argc, argv, "d:n:u:")) != -1) {
    switch (opt) {
      case 'd':
        device = optarg;
        break;
      case 'n':
        num_leds = atoi(optarg);
        if (num_leds < 1 || num_leds > 170) {
          fprintf(stderr, "error: number of LEDs must be between 1-170\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'u':
        universe = atoi(optarg);
        if (universe < 1 || universe > 63999) {
          fprintf(stderr, "error: universe must be between 1-63999\n");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        show_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (device == NULL || num_leds == -1 || universe == -1) {
    show_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  // open serial device and initialise
  if ((serial_fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
    perror("serial device open");
    exit(EXIT_FAILURE);
  }
  init_serial(serial_fd, BAUD_RATE);
  fprintf(stderr, "serial device '%s' opened\n", device);

  // initialise AdaLight buffer
  adalight_buffer = init_adalight_buffer(num_leds);
  adalight_buffer_size = 6 + (num_leds * 3);

  // open socket, initialise and join multicast group
  if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  init_socket_udp(socket_fd, E131_PORT);
  join_e131_multicast(socket_fd, universe);
  fprintf(stderr, "multicast UDP server listening on port %d\n", E131_PORT);

  // receive socket data and forward to the serial port
  fprintf(stderr, "bridging packets from E1.31 to AdaLight, use CTRL+C to stop\n");
  for (;;) {
    if ((rbytes = recv(socket_fd, udp_buffer, UDP_BUFFER_SIZE , 0)) < 0) {
      perror("recv");
      exit(EXIT_FAILURE);
    }

    // check if enough data was received
    if (rbytes < OFST_DATA + (num_leds * 3)) {
      fprintf(stderr, "not enough UDP data received (%d bytes)\n", rbytes);
      continue;
    }

    // check received universe
    if (((udp_buffer[OFST_UNIV_HIGH] << 8) | udp_buffer[OFST_UNIV_LOW]) != universe) {
      continue;
    }

    // copy RGB data from the udp buffer to the AdaLight buffer
    memcpy(&adalight_buffer[6], &udp_buffer[OFST_DATA], adalight_buffer_size);
    send_buffer(serial_fd, adalight_buffer, adalight_buffer_size);
  }

  // finished
  close(serial_fd);
  exit(EXIT_SUCCESS);
}
