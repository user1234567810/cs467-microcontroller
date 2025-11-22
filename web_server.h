/*
File: web_server.h
Language: C
Author: Andrew Poon
Date: 11/22/25
Description: Provides the public interface for the web server.
*/


#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Start HTTP server
 *
 * @param port TCP port to listen on (ex/ 80)
 * @return true if server started successfully, false otherwise
 */
bool web_server_start(uint16_t port);

#endif // SERVER_H