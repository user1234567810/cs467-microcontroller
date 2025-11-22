#include "wifi.h"
#include "pico/cyw43_arch.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"

bool wifi_start_ap(const char *ssid, const char *password) {
    // Initialize the CYW43 Wi-Fi chip
    if (cyw43_arch_init()) {
        return false;
    }

    // Power up Wi-Fi
    cyw43_arch_enable_ap_mode(ssid, password, CYW43_AUTH_WPA2_AES_PSK);

    return true;
}