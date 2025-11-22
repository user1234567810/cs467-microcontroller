/*
File: wifi.c
Language: C
Author: Andrew Poon
Date: 11/22/25
Description:
    Provides initialization and control functionality for the CYW43 WiFi
    chip on the Raspberry Pi Pico2W. This module is responsible for bringing up
    the device in WiFi access point (AP) mode so that other devices can connect
    to the Pico’s network.

    Portions of WiFi initialization flow were adapted from the official Raspberry
    Pi Pico SDK WiFi examples and is licensed as follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause
    Source: https://github.com/raspberrypi/pico-examples/blob/master/pico_w/

Responsibilities:
- Initialize CYW43 WiFi
- Start WiFi AP with given SSID and password

Requires the following modules:
- wifi.h: for interface definitions
*/

#include "wifi.h"
#include "pico/cyw43_arch.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"

bool wifi_start_ap(const char *ssid, const char *password) {
    // Initialize the CYW43 WiFi chip
    if (cyw43_arch_init()) {
        return false;
    }

    // Power up WiFi
    cyw43_arch_enable_ap_mode(ssid, password, CYW43_AUTH_WPA2_AES_PSK);

    return true;
}