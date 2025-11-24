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
"<html>\r\n"
"<head>\r\n"
"<meta charset=\"UTF-8\">\r\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
"<title>Microcontroller Home Humidity Sensor</title>\r\n"
"<style>\r\n"
"body{margin:0;font-family:-apple-system,system-ui,Segoe UI,sans-serif;"
"background:#000;color:#fff;font-size:18px;}\r\n"
".page{padding:16px;max-width:480px;margin:0 auto;min-height:100vh;"
"display:flex;flex-direction:column;}\r\n"
".card{background:#1c1c1e;border-radius:18px;padding:18px 20px;"
"display:flex;align-items:center;justify-content:space-between;"
"text-decoration:none;color:inherit;}\r\n"
".label-title{font-size:18px;margin-bottom:6px;}\r\n"
".label-sub{font-size:14px;color:#8e8e93;}\r\n"
".toggle{width:64px;height:36px;border-radius:18px;background:#3a3a3c;"
"position:relative;transition:background .2s;}\r\n"
".toggle.on{background:#d73f09;}\r\n"
".knob{width:32px;height:32px;border-radius:50%%;background:#fff;"
"position:absolute;top:2px;left:2px;transition:left .2s;}\r\n"
".toggle.on .knob{left:30px;}\r\n"
"</style>\r\n"
"</head>\r\n"
"<body>\r\n"
"<div class=\"page\">\r\n"
"<div style=\"font-size:20px;font-weight:600;margin-bottom:16px;\">Settings</div>\r\n"
"<a class=\"card\" href=\"%s\">\r\n"
"  <div>\r\n"
"    <div class=\"label-title\">LEDs</div>\r\n"
"    <div class=\"label-sub\">Currently: %s</div>\r\n"
"  </div>\r\n"
"  <div class=\"%s\"><div class=\"knob\"></div></div>\r\n"
"</a>\r\n"
"</div>\r\n"
"</body>\r\n"
"</html>\r\n";

#endif // WEB_UI_H