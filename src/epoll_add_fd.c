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
#include <sys/epoll.h>

// add a file descriptor to an epoll instance
void epoll_add_fd(int epoll_fd, int fd) {
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0) {
    perror("epoll_ctl EPOLL_CTL_ADD");
    exit(EXIT_FAILURE);
  }
}
