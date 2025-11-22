/*
File: wifi.h
Language: C
Author: Andrew Poon
Date: 11/22/25
Description: Provides the public interface for the WiFi connectivity
    and HTTP server module.
*/

#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>

/**
 * @brief Initialize the WiFi and start an access point (AP)
 *
 * @param ssid     SSID to broadcast (ex/ "MHHaTS")
 * @param password WPA2 password (must be at least 8 characters)
 * @return true if AP started successfully, false otherwise
 */
bool wifi_start_ap(const char *ssid, const char *password);

#endif // WIFI_H