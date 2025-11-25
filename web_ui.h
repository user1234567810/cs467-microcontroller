/*
File: web_ui.h
Language: C
Author: Andrew Poon
Date: 11/23/25
Description: Contains the HTML user interface template served by network.c

Note: This HTML is stored as a fixed size string. The current buffer
    size constants in network.c (HTTP_BODY_MAX and HTTP_RESP_MAX) must be
    increased if the file size grows past the current set maxes.
*/

#ifndef WEB_UI_H
#define WEB_UI_H

static const char PAGE_INDEX_HTML[] =
"<!DOCTYPE html>\r\n"
"<html lang=\"en\">\r\n"
"<head>\r\n"
"<meta charset=\"UTF-8\">\r\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
"<title>Microcontroller Home Humidity Sensor</title>\r\n"
"<style>\r\n"
"body{margin:0;padding:16px;font-family:-apple-system,system-ui,Segoe UI,sans-serif;"
"background:#000;color:#fff;font-size:18px;}\r\n"
"h1{font-size:24px;margin-bottom:12px;}\r\n"
"p{margin:8px 0;}\r\n"
"a{color:#4da3ff;text-decoration:none;}\r\n"
"a:hover{text-decoration:underline;}\r\n"
"</style>\r\n"
"</head>\r\n"
"<body>\r\n"
"<h1>Humidity Sensor</h1>\r\n"
"<p>Current humidity: %.1f%%</p>\r\n"
"<p>LEDs are currently: %s</p>\r\n"
"<p><a href=\"%s\">%s</a></p>\r\n"
"</body>\r\n"
"</html>\r\n";

#endif // WEB_UI_H