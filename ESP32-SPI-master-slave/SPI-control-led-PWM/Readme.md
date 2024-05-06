# Description
1. Master sends potentiometer values (0-255) to Slave.
2. Slave use potentiometer values send from master to control PWM (dim a Led). 
3. Slave sends led status (OFF, FADING, ON) to Master.

The code and configuration are based on `ESP32-CAM` as Master and `ESP32 DOIT ESP32 DEVKIT V1` as Slave using custom GPIOS for  SPI communication (VSPI).

# Prerequisites 
1. The project structure, directories, packages, and code were distributed using `Platformio IDE extension` configuration to work with `VScode`. An installation tutorial can be found [here](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)
2. Library for SPI Slave Mode:
    ```
    hideakitai/ESP32SPISlave@^0.6.0
    ```
    More information and examples are available in the author repository:
    <a hfer='https://github.com/hideakitai/ESP32SPISlave/tree/main' target="_blank">ESP32SPISlave by hideakitai</a>

# Usage
1. Diagram connection:
<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/SPI-CIRCUIT-PWM-LED.png" width='50%'>

2. Upload code to `ESP32-CAM`:
    - Connect `GPIO0` and `GND` 
    - Connect the board or push the reset button 
    - PlatformIO > General > Upload
    - Disconnect `GPIO0`
    - Push the reset button 
    - PlatformIO > General > Monitor

2. Upload code to `ESP32 DOIT ESP32 DEVKIT V1`:
    - PlatformIO > General > Upload and Monitor

# Notes
> [!NOTE]
> - Libraries detailed in the `platformio.ini` file are downloaded automatically.
> - `ESP32-CAM` requires a USB to UART TTL converter of 5V.

# Results

- Master sends: "Hello Slave!"
- Slave sends: "Hello Master!"

<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/SPI2.png">
