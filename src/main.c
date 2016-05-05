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
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "prototypes.h"
#include "e131.h"

#define SERIAL_BAUD_RATE __MAX_BAUD
#define SERIAL_BUFFER_SIZE 1024
#define MAX_EPOLL_EVENTS 10

int main(int argc, char **argv) {
  int opt;
  int epoll_fd, serial_fd, socket_udp_fd;
  char *device = NULL;
  uint16_t universe = 0;
  struct epoll_event epoll_events[MAX_EPOLL_EVENTS];
  int nfds, i;
  e131_packet_t e131_packet;
  uint8_t curr_sequence = 0, first_packet = 1;

  // program options
  while ((opt = getopt (argc, argv, "d:u:")) != -1) {
    switch (opt) {
      case 'd':
        device = optarg;
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
  if (device == NULL || universe == 0) {
    show_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  // create an epoll file descriptor
  if ((epoll_fd = epoll_create(1)) < 0) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  // open serial device and initialise
  if ((serial_fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
    perror("serial device open");
    exit(EXIT_FAILURE);
  }
  init_serial(serial_fd, SERIAL_BAUD_RATE);
  epoll_add_fd(epoll_fd, serial_fd);
  fprintf(stderr, "serial device '%s' opened\n", device);

  // open udp socket, initialise and join multicast group
  if ((socket_udp_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  init_socket_udp(socket_udp_fd, E131_DEFAULT_PORT);
  join_e131_multicast(socket_udp_fd, universe);
  epoll_add_fd(epoll_fd, socket_udp_fd);
  fprintf(stderr, "multicast UDP server listening on port %d\n", E131_DEFAULT_PORT);

  // bridge E1.31 data to AdaLight
  fprintf(stderr, "bridging E1.31 (sACN) to AdaLight, use CTRL+C to stop\n");
  for (;;) {
    // wait for an epoll event
    if ((nfds = epoll_wait(epoll_fd, epoll_events, MAX_EPOLL_EVENTS, -1)) < 0) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    // check received epoll events
    for (i=0; i<nfds; i++) {
      if (epoll_events[i].data.fd == serial_fd) { // serial port data received
        tcflush(serial_fd, TCIFLUSH);
        continue;
      }
      if (epoll_events[i].data.fd == socket_udp_fd) { // udp network data received
        if (recv(socket_udp_fd, e131_packet.raw, sizeof(e131_packet.raw), 0) < 0) {
          perror("recv");
          exit(EXIT_FAILURE);
        }
        if (e131_validate_packet(&e131_packet) != E131_ERR_NONE) {
          fprintf(stderr, "warning: invalid E1.31 packet received\n");
          continue;
        }
        if (e131_packet.sequence_number != curr_sequence++) {
          if (!first_packet)
              fprintf(stderr, "warning: out of order E1.31 packet received\n");
          curr_sequence = e131_packet.sequence_number + 1;
          continue;
        }
        if (htons(e131_packet.universe) != universe) {
          continue;
        }
        send_adalight(serial_fd, e131_packet.property_values + 1, \
          htons(e131_packet.property_value_count) - 1);
        first_packet = 0;
      }
    }
  }

  // finished
  close(socket_udp_fd);
  close(serial_fd);
  close(epoll_fd);
  exit(EXIT_SUCCESS);
}
