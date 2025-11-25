/*
File: network.c
Language: C
Author: Andrew Poon
Date: 11/22/25
Description:
    Provides initialization and control functionality for the CYW43 WiFi
    chip on the Raspberry Pi Pico2W and also a HTTP server. This module is
    responsible for bringing up the device in WiFi access point (AP) mode so
    that other devices (laptops, phones, etc) can connect to the Pico’s network.

    Portions of WiFi initialization and lwIP/TCP logic were adapted from the
    official Raspberry Pi Pico SDK WiFi examples and is licensed as follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause
    Source: https://github.com/raspberrypi/pico-examples/blob/master/pico_w/

Responsibilities:
- Initialize CYW43 WiFi
- Start WiFi AP with given SSID and password
- Create TCP listener on configured HTTP port
- Accept incoming HTTP connection and return HTML page

Requires the following modules:
- network.h: for interface definitions
*/

#include "network.h"
#include "led_array.h"
#include "web_ui.h"

#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"

#include <stdio.h>
#include <string.h>

extern float g_latest_humidity;

#define HTTP_PORT_DEFAULT 80
#define HTTP_BODY_MAX     4096
#define HTTP_RESP_MAX     (HTTP_BODY_MAX + 512)

// TCP listener for the HTTP server
static struct tcp_pcb *http_listener_pcb = NULL;

// WiFi / Access Point (AP) setup
bool wifi_start_ap(const char *ssid, const char *password) {
    // Initialize the CYW43 Wi-Fi chip + lwIP networking stack
    if (cyw43_arch_init()) {
        printf("ERROR: cyw43_arch_init() failed\n");
        return false;
    }

    // Enable AP mode with given SSID/password
    cyw43_arch_enable_ap_mode(ssid, password, CYW43_AUTH_WPA2_AES_PSK);
    printf("WiFi AP started with SSID '%s'\n", ssid);

    return true;
}

// Lightweight HTTP server implementation

// Forward declarations of callbacks
static err_t http_accept(void *arg, struct tcp_pcb *new_pcb, err_t err);
static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void  http_err(void *arg, err_t err);
static err_t http_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

static void handle_set_request(const char *param_name, const char *param_value) {
    if (strcmp(param_name, "led") == 0) {
        if (strcmp(param_value, "on") == 0) {
            led_array_set_enabled(true);
            printf("HTTP: LED enabled via web UI\n");
        } else if (strcmp(param_value, "off") == 0) {
            led_array_set_enabled(false);
            printf("HTTP: LED disabled via web UI\n");
        }
    }
}

// Helper that closes a TCP connection
static void http_connection_close(struct tcp_pcb *tpcb) {
    if (!tpcb) return;

    cyw43_arch_lwip_begin();

    // Remove callbacks from connection
    tcp_arg(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_err(tpcb, NULL);

    // Closes connection
    tcp_close(tpcb);

    cyw43_arch_lwip_end();
}

// Start the web server and listen for HTTP connections
bool web_server_start(uint16_t port) {
    if (port == 0) {
        port = HTTP_PORT_DEFAULT;
    }

    cyw43_arch_lwip_begin();

    // Create new TCP listener
    http_listener_pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!http_listener_pcb) {
        cyw43_arch_lwip_end();
        printf("ERROR: tcp_new_ip_type() failed\n");
        return false;
    }

    // Bind listener to the given port
    err_t err = tcp_bind(http_listener_pcb, IP_ANY_TYPE, port);
    if (err != ERR_OK) {
        printf("ERROR: tcp_bind() failed: %d\n", err);
        tcp_close(http_listener_pcb);
        http_listener_pcb = NULL;
        cyw43_arch_lwip_end();
        return false;
    }

    // Convert listener to be ready to accept clients
    struct tcp_pcb *listen_pcb = tcp_listen_with_backlog(http_listener_pcb, 1);
    if (!listen_pcb) {
        printf("ERROR: tcp_listen_with_backlog() failed\n");
        tcp_close(http_listener_pcb);
        http_listener_pcb = NULL;
        cyw43_arch_lwip_end();
        return false;
    }

    http_listener_pcb = listen_pcb;

    // Set the callback that triggers when a new client connects
    tcp_arg(http_listener_pcb, NULL);
    tcp_accept(http_listener_pcb, http_accept);

    cyw43_arch_lwip_end();

    printf("HTTP server listening on port %u\n", port);
    return true;
}

// Called when a new client opens a TCP connection
static err_t http_accept(void *arg, struct tcp_pcb *new_pcb, err_t err) {
    (void)arg;
    (void)err;

    printf("HTTP: client connected\n");

    tcp_setprio(new_pcb, TCP_PRIO_MIN);
    tcp_recv(new_pcb, http_recv);
    tcp_err(new_pcb, http_err);
    tcp_sent(new_pcb, http_sent);

    return ERR_OK;
}

// Send a HTML page to client
static void send_http_response(struct tcp_pcb *tpcb) {

    // Buffer for the formatted HTML page
    char body[HTTP_BODY_MAX];
    
    // Check current LED state
    bool enabled = led_array_is_enabled();

    // Values inserted into the HTML template at runtime
    const char *status_text  = enabled ? "On"  : "Off";
    const char *toggle_href  = enabled ? "/set?led=off" : "/set?led=on";
    const char *toggle_label = enabled ? "Turn LEDs Off" : "Turn LEDs On";

    // Format HTML page using template and values
    int body_len_int = snprintf(body, sizeof(body),
                                PAGE_INDEX_HTML,
                                g_latest_humidity,
                                status_text,
                                toggle_href,
                                toggle_label);
    printf("HTTP: body_len_int=%d (max=%d)\n", body_len_int, HTTP_BODY_MAX);
    if (body_len_int < 0) {
        body[0] = '\0';
    }

    // Clamp buffer size to avoid overflow issues
    size_t body_len = (size_t)body_len_int;
    if (body_len >= sizeof(body)) {
        body_len = sizeof(body) - 1;
        body[body_len] = '\0';
    }

    // Standard HTTP header
    char header[256];
    int header_len = snprintf(header, sizeof(header),
                              "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html; charset=UTF-8\r\n"
                              "Connection: close\r\n"
                              "Content-Length: %zu\r\n"
                              "\r\n",
                              body_len);

    // Allocate a temporary buffer for full HTTP response (header + body)
    char response[HTTP_RESP_MAX];
    size_t total_len = (size_t)header_len + body_len;
    if (total_len > sizeof(response)) {
        total_len = sizeof(response);
    }

    // Copy header and then body after it
    memcpy(response, header, (size_t)header_len);
    memcpy(response + header_len, body, total_len - (size_t)header_len);

    // Send the full HTTP response over TCP using lwIP
    cyw43_arch_lwip_begin();
    err_t err = tcp_write(tpcb, response, (u16_t)total_len, TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        printf("ERROR: tcp_write() failed: %d\n", err);
    } else {
        err = tcp_output(tpcb);
        if (err != ERR_OK) {
            printf("ERROR: tcp_output() failed: %d\n", err);
        }
    }
    cyw43_arch_lwip_end();
}


// Callback for incoming HTTP request data
static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    (void)arg;

    printf("HTTP: received data, err=%d, p=%p\n", err, (void*)p);

    // If request is invalid or connection closes, free buffer and close
    if ((err != ERR_OK) || (p == NULL)) {
        if (p) {
            cyw43_arch_lwip_begin();
            pbuf_free(p);
            cyw43_arch_lwip_end();
        }
        http_connection_close(tpcb);
        return ERR_OK;
    }

    // Copy the entire incoming HTTP request into a local buffer
    char req[512];
    size_t copied = pbuf_copy_partial(p, req, sizeof(req) - 1, 0);
    req[copied] = '\0';

    // Free the lwIP packet buffer now that its data was copied
    cyw43_arch_lwip_begin();
    pbuf_free(p);
    cyw43_arch_lwip_end();

    printf("HTTP: raw request:\n%s\n", req);

    // Extract only the first line ("GET /path HTTP/1.1")
    char *first_line = req;
    char *line_end = strstr(first_line, "\r\n");
    if (line_end) {
        *line_end = '\0';  // terminate first line
    }

    printf("HTTP: first line: '%s'\n", first_line);

    // Parse method, path, and version
    char method[8]  = {0};
    char path[128]  = {0};
    char version[16]= {0};

    if (sscanf(first_line, "%7s %127s %15s", method, path, version) == 3) {
        printf("HTTP: parsed method=%s path=%s version=%s\n", method, path, version);

        // Check if this request is for control endpoint
        if (strncmp(path, "/set?", 5) == 0) {
            const char *query = path + 5;  // points to "led=off"

            char name[32]  = {0};
            char value[32] = {0};

            // Split "name=value" into separate strings
            const char *eq = strchr(query, '=');
            if (eq) {
                size_t name_len = (size_t)(eq - query);
                if (name_len >= sizeof(name)) name_len = sizeof(name) - 1;
                memcpy(name, query, name_len);
                name[name_len] = '\0';

                const char *val_start = eq + 1;
                size_t val_len = strlen(val_start);
                if (val_len >= sizeof(value)) val_len = sizeof(value) - 1;
                memcpy(value, val_start, val_len);
                value[val_len] = '\0';

                printf("HTTP: /set param: %s = %s\n", name, value);

                // Apply the setting (LED on/off)
                handle_set_request(name, value);
            }
        }
    } else {
        printf("HTTP: could not parse request line\n");
    }

    // Send the HTML page for any request
    send_http_response(tpcb);

    return ERR_OK;
}

// Called if a TCP error happens on the connection
static void http_err(void *arg, err_t err) {
    (void)arg;
    (void)err;
}

// Called after HTTP response is successful and acknowledged
static err_t http_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    (void)arg;
    (void)len;

    printf("HTTP: data acknowledged, closing connection\n");
    http_connection_close(tpcb);

    return ERR_OK;
}
