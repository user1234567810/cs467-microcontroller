#include "webserver.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include <stdio.h>


bool webserver_start(uint16_t port) {
    (void)port;
    return true;
}
