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
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MCAST_GROUP_TEMPLATE "239.255.%d.%d"

// join UDP socket to an E1.31 multicast group on all interfaces
void join_e131_multicast(int fd, uint16_t universe) {
  char mcast_group[16];
  struct ip_mreq mreq;

  // compute multicast group from the universe number
  uint8_t high = (universe & 0xff00) >> 8;
  uint8_t low = universe & 0x00ff;
  sprintf(mcast_group, MCAST_GROUP_TEMPLATE, high, low);

  // join the multicast group
  mreq.imr_multiaddr.s_addr = inet_addr(mcast_group);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    perror("setsockopt IP_ADD_MEMBERSHIP");
    exit(EXIT_FAILURE);
  }
}
