/*
File: network.h
Language: C
Author: Andrew Poon
Date: 11/22/25
Description: Public interface for the network module. This header
    provides the functions that allow main.c to start the Pico2W
    in WiFi access point (AP) mode and launch the built-in server.
*/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize the CYW43 WiFi chip and start AP mode
 *
 * Brings up the Raspberry Pi Pico2W as a WiFi access point
 * using the given SSID and WPA2 password
 *
 * @param ssid      SSID for the access point
 * @param password  WPA2 password (8 characters minimum)
 * @return true on success, false otherwise
 */
bool wifi_start_ap(const char *ssid, const char *password);

/**
 * @brief Start a minimal HTTP server on the given port
 *
 * Listens for HTTP connections on the specified port
 * and responds with a webpage
 *
 * @param port  TCP port to listen on (use 80 for HTTP)
 * @return true on success, false otherwise
 */
bool web_server_start(uint16_t port);

#endif // NETWORK_H