/*
File: web_server.c
Language: C
Author: Andrew Poon
Date: 11/22/25
Description:
    Implements lightweight HTTP server.

    The TCP server structure and callback pattern are based on the the
    official Raspberry Pi Pico SDK WiFi examples and is licensed as follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause
    Source: https://github.com/raspberrypi/pico-examples/blob/master/pico_w/

Responsibilities:
- Accept incoming client connections 
- Close connections after responses are sent

Requires the following modules:
- web_server.h: for interface definitions
*/

#include "web_server.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include <stdio.h>


bool web_server_start(uint16_t port) {
    (void)port;
    return true;
}
