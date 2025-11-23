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

#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"

#include <stdio.h>
#include <string.h>

#define HTTP_PORT_DEFAULT 80

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
    char body[512];

    snprintf(body, sizeof(body),
        "<!DOCTYPE html>\r\n"
        "<html>\r\n"
        "<head><title>Microcontroller Home Humidity Sensor - Settings</title></head>\r\n"
        "<body style=\"display:grid;place-items:center;height:100vh;margin:0\">\r\n" 
        "<h1>Microcontroller Home Humidity Sensor</h1>\r\n"
        "</body>\r\n"
        "</html>\r\n"
    );

    size_t body_len = strlen(body);

    // Standard HTTP header
    char header[256];
    int header_len = snprintf(header, sizeof(header),
                              "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html; charset=UTF-8\r\n"
                              "Connection: close\r\n"
                              "Content-Length: %zu\r\n"
                              "\r\n",
                              body_len);

    // Combine header and body
    char response[512 + 256];
    size_t total_len = (size_t)header_len + body_len;
    if (total_len > sizeof(response)) {
        total_len = sizeof(response);
    }

    memcpy(response, header, (size_t)header_len);
    memcpy(response + header_len, body, total_len - (size_t)header_len);

    // Send data through lwIP's TCP API
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

// Callback for incomping HTTP request data
static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    (void)arg;

    printf("HTTP: received data, err=%d, p=%p\n", err, (void*)p);

    // If request is invalid or connection closes, clean up
    if ((err != ERR_OK) || (p == NULL)) {
        if (p) {
            cyw43_arch_lwip_begin();
            pbuf_free(p);
            cyw43_arch_lwip_end();
        }
        http_connection_close(tpcb);
        return ERR_OK;
    }

    // Free the incoming request packet
    cyw43_arch_lwip_begin();
    pbuf_free(p);
    cyw43_arch_lwip_end();

    // Send HTML page in response
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
