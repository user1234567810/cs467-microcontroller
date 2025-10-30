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


/*
--Outline--

Functions:

led_array_init():
    - Configure GPIO for LED output
    - Return true if initialization succeeds

humidity_to_leds(humidity):
    - Convert humidity percentage (0-100) to LED count (0-8)
    - Return number of LEDs to light

led_array_set(num_leds):
    - Humidity visualization (lights num of LEDs from humidity_to_leds)

led_array_show_loading():
    - Loading visualization (cycle through LEDs)

led_array_show_error(code):
    - Error visualization (blinking red)

Testing:
    - Start in placeholder mode (no hardware)
    - Use mock humidity values
    - Will make temp main.c for local testing
*/