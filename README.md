# cs467-microcontroller
[Project description](https://eecs.engineering.oregonstate.edu/capstone/submission/pages/viewSingleProject.php?id=a0MOgZxGy3ZSn4Yr)

**File Descriptions:**
1. `main.c` - Initializes hardware and runs the main program. Calls function to read the sensor, update the LED array, and update the display.
2. `sensor.c` - Contains function to initialize and read data from the humidity sensor.
3. `led_array.c` - Contains functions to initialize the LED array and set their state based on humidity levels.
5. `display.c` - Contains functions to initialize and update the display with the current humidity level.
6. `CMakeLists.txt` - Build configuration file using CMake.

## UI Output Visualization Logic

### LED Array Behavior
The six-stage LED array visually indicates the level of humidity at a glance using the following thresholds:
- 0–20%: 0 LEDs
- 21–30%: 1 LEDs
- 31–40%: 2 LEDs
- 41–50%: 3 LEDs
- 51–60%: 4 LEDs
- 61-70%: 5 LEDs
- 71-100%: 6 LEDs

### LCD Display Behavior
The two-line LCD Display shows the indoor humidity in numerals, system status updates, and error messages.

| **State** | **Line 1** | **Line 2** | **Description** |
|------------|-------------|-------------|--------------------------|
| **Startup** | `System Init...` | `Please wait` | Displayed at startup and reboot |
| **System Ready** | `Humidity:` | `<value>%` | Updates every 5 minutes with the latest reading |
| **Sensor Error** | `Error:` | `Sensor offline` | Displayed if sensor fails to respond after retries |
| **LED Error** | `Error:` | `LED array offline` | Displayed if LED array fails to respond after retries |
| **LCD Error** | `n/a` | `n/a` | LCD display nonfunctional, LED array indicates system error |