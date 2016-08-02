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

#ifndef _PROTOTYPES_H
#define _PROTOTYPES_H

#include <stdint.h>
#include <termios.h>

extern void epoll_add_fd(int epoll_fd, int socket_fd);
extern void init_serial(int fd, speed_t baud_rate);
extern speed_t parse_baud_rate(char *str);
extern void send_adalight(int fd, uint8_t *rgb_data, size_t size);
extern void show_usage(char *prog_name);

#endif
