/*
File: led_array.c
Language: C
Author: Andrew Poon
Date: 10/30/25
Description: Provides initialization & control functionality for the
    WS2812 RGB 8 LED Strip connected to the Raspberry Pi Pico.

Responsibilities:
- Initialize and configure the LED array
- Light LEDs based on humidity levels given by the sensor
- Display loading and error animations for debugging and status indication
- Support placeholder testing

Requires the following modules:
- led_array.h: for interface definitions

Wiring configuration
** WS2812 RGB 8 LED Strip **
GPIO 2 (pin 4) -> DIN (data in) on LED strip
3.3V (pin 36)  -> Power rail (+)  -> VCC on LED strip
GND  (pin 38)  -> Ground rail (-) -> GND on LED strip
*/

