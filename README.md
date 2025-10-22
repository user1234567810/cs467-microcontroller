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
The eight-stage LED array visually indicates the level of humidity at a glance using the following thresholds:
| **Humidity Range (%)** | **LEDs Lit** |
|-------------|-------------|
| 0–20 | 1 |
| 21–30 | 2 |
| 31–40 | 3 |
| 41–50 | 4 |
| 51–60 | 5 |
| 61–70 | 6 |
| 71–80 | 7 |
| 81–100 | 8 |

Additionally, the LED array displays different patterns to indicate system states:
| **State** | **LED Activity** | **Description** |
|-------------|-------------|-------------|
| **Startup** | All LEDs lit (yellow) | Displayed at startup and reboot |
| **System Ready** | 1-8 LEDs lit based on humidity | See table above |
| **Sensor Error** | LEDs 1-2 lit (red) | Displayed if sensor fails to respond after retries |
| **LED Error** | n/a | LED array nonfunctional, LCD indicates system error |
| **LCD Error** | LEDs 2-4 lit (red) | Displayed if LCD display fails to respond after retries |

### LCD Display Behavior
The two-line LCD Display shows the indoor humidity in numerals, system status updates, and error messages.

| **State** | **Line 1** | **Line 2** | **Description** |
|-------------|-------------|-------------|-------------|
| **Startup** | `System Init...` | `Please wait` | Displayed at startup and reboot |
| **System Ready** | `Humidity:` | `<value>%` | Updates every 5 minutes with the latest reading |
| **Sensor Error** | `Error:` | `Sensor offline` | Displayed if sensor fails to respond after retries |
| **LED Error** | `Error:` | `LED array offline` | Displayed if LED array fails to respond after retries |
| **LCD Error** | n/a | n/a | LCD display nonfunctional, LED array indicates system error |