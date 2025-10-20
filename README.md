# cs467-microcontroller
[Project description](https://eecs.engineering.oregonstate.edu/capstone/submission/pages/viewSingleProject.php?id=a0MOgZxGy3ZSn4Yr)

File Descriptions:
1. main.c - Initializes hardware and runs the main program. Calls function to read the sensor, update the LED array, and update the display.
2. sensor.c - Contains function to initialize and read data from the humidity sensor.
3. led_array.c - Contains functions to initialize the LED array and set their state based on humidity levels.
      Map humidity to LED stages:
      0–20%: 1 LED
      21–40%: 2 LEDs
      41–60%: 3 LEDs
      61–80%: 4 LEDs
      81–100%: 5–6 LEDs
5. display.c - Contains functions to initialize and update the display with the current humidity level.
6. CMakeLists.txt - Build configuration file using CMake.
