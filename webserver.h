#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Start HTTP server
 *
 * @param port TCP port to listen on (ex/ 80)
 * @return true if server started successfully, false otherwise
 */
bool webserver_start(uint16_t port);

#endif // SERVER_H