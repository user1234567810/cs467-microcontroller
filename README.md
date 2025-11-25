# Microcontroller Home Humidity Sensor
The Home Humidity Sensor is an embedded system built with C on the Raspberry Pi Pico. It measures humidity using a DHT20 sensor and provides both LED based and LCD text feedback to communicate humidity, system state, and errors. The project emphasizes modular firmware design, low level hardware interaction, and clear visualization of sensor data.

[Full Project description](https://eecs.engineering.oregonstate.edu/capstone/submission/pages/viewSingleProject.php?id=a0MOgZxGy3ZSn4Yr)

### File Descriptions:
1. `main.c` - Initializes hardware and runs the main program. Calls function to read the sensor, update the LED array, and update the display.
2. `sensor.c` - Contains function to initialize and read data from the humidity sensor.
3. `led_array.c` - Contains functions to initialize the LED array and set their state based on humidity levels.
4. `display.c` - Contains functions to initialize and update the display with the current humidity level.
5. `network.c` - Contains functions to initialize a Pico2W with WiFi access point (AP) mode and launch a built-in server.
6. `CMakeLists.txt` - Build configuration file using CMake.

### Building the Firmware
Run the following commands in the GitHub Codespaces terminal:

**Option A: Base Firmware (Raspberry Pi Pico)**
```bash
rm -rf build
cmake -S . -B build -DPICO_BOARD=pico
cmake --build build
```
**Option B: WiFi Firmware (Raspberry Pi Pico 2 W)**
```bash
rm -rf build
cmake -S . -B build -DPICO_BOARD=pico2_w -DENABLE_WIFI=ON
cmake --build build
```

**Flashing the Device**
1. Download the `.uf2` file generated in the `build/` folder.
2. Unplug the Pico from your computer.
3. Hold down the **BOOTSEL** button.
4. Plug the Pico back in (continue holding BOOTSEL until the drive appears).
5. The Pico should mount as a storage device named `RPI-RP2` (or `RP2350` for a Pico2W).
6. Drag and drop the `.uf2` file into that storage device.

**Connecting to the Pico's WiFi Network**
1. Connect to the Pico’s access point `PICO2W-AP`
2. Configure your device with a **manual IP**:  
  - IP: `192.168.4.2`  
  - Subnet: `255.255.255.0`  
  - Gateway: `192.168.4.1`
3. Open a browser and navigate to: `http://192.168.4.1/`
<img src="https://github.com/user-attachments/assets/44a09844-e6ad-410b-84ad-dcfd0804f988" width="400">
<img src="https://github.com/user-attachments/assets/20bb3c1b-d041-4d53-89e2-2347a1271887" width="400">


## Wiring Diagram
![Wiring Diagram](docs/wiring-diagram-v01.jpg)
_Note: All GND pins on the Pico are electrically equivalent. The diagram shows specific GND pins for clarity, but any GND pin will work. Also, this wiring diagram is applicable to both Pico and Pico2W_

## Visualization Logic

### LED Patterns by Humidity Level
The eight-stage LED array visually indicates the level of humidity at a glance using the following thresholds:
| **Humidity Range (%)** | **LEDs Lit** |
|-------------|-------------|
| 0–19 | 1 |
| 20–29 | 2 |
| 30–39 | 3 |
| 40–49 | 4 |
| 50–59 | 5 |
| 60–69 | 6 |
| 70–79 | 7 |
| 80–100 | 8 |

### System States
Additionally, the LED array displays different patterns to indicate system states:
| **State** | **LED Activity** | **Description** |
|-------------|-------------|-------------|
| **System Ready** | 1-8 LEDs lit based on humidity | See table above |
| **Sensor Error** | LED #2 blinks | Displayed if sensor fails to respond after retries |
| **LED Error** | LED #3 blinks (nonfunctional) | LCD indicates system error |
| **LCD Error** | LED #4 blinks | Displayed if LCD display fails to respond after retries |

### LCD Display Behavior
The two-line LCD Display shows the indoor humidity in numerals, system status updates, and error messages.

| **State** | **Line 1** | **Line 2** | **Description** |
|-------------|-------------|-------------|-------------|
| **System Ready** | `Humidity: <value>%` | `n/a` | Updates with the latest reading |
| **Sensor Error** | `Error:` | `Sensor offline` | Displayed if sensor fails to respond after retries |
| **LED Error** | `Error:` | `LED array offline` | Displayed if LED array fails to respond after retries |
| **LCD Error** | n/a | n/a | LCD display nonfunctional, LED array indicates system error |
