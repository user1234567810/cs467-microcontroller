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
"<meta http-equiv=\"refresh\" content=\"3\">\r\n"
"<title>Microcontroller Home Humidity Sensor</title>\r\n"
"<style>\r\n"
"body{margin:0;font-family:Inter,-apple-system,system-ui,Segoe UI,sans-serif;"
"background:#202020;color:#ffffff;}\r\n"
".page{padding:20vh 24px 48px 24px;max-width:500px;margin:0 auto;}\r\n"
".label{font-size:clamp(24px,5vw,36px);margin-bottom:4px;}\r\n"
".primary-value{font-size:clamp(72px,18vw,128px);"
"font-weight:600;color:#d73f09;line-height:1.0;margin-bottom:24px;}\r\n"
".status-text{font-size:20px;margin:32px 0 6px 0;}\r\n"
"a{color:#d73f09;text-decoration:none;font-size:20px;}\r\n"
"a:hover{text-decoration:underline;}\r\n"
"</style>\r\n"
"</head>\r\n"
"<body>\r\n"
"<div class=\"page\">\r\n"
"    <div class=\"label\">Humidity &#37;</div>\r\n"
"    <div class=\"primary-value\">%.1f</div>\r\n"
"\r\n"
"    <div class=\"label\">Temperature</div>\r\n"
"    <div class=\"primary-value\">%.1f&#176;F</div>\r\n"
"\r\n"
"    <p class=\"status-text\">LEDs are currently: %s</p>\r\n"
"    <p><a href=\"%s\">%s</a></p>\r\n"
"</div>\r\n"
"</body>\r\n"
"</html>\r\n";

#endif // WEB_UI_H