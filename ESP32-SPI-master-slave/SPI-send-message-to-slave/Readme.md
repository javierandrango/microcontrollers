# Description
Sends a message from Master to Slave with SPI protocol.

The code and configuration is based on a `ESP32-CAM` as Master and a `ESP32 DOIT ESP32 DEVKIT V1` as Slave using custom GPIOS for communication.
# Prerequisites 
1. The project structure, directories, packages, and code were distributed using `Platformio IDE extension` configuration to work with `VScode`. An installation tutorial can be found [here](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)
2. Library for SPI Slave Mode:
    ```
    hideakitai/ESP32SPISlave@^0.6.0
    ```
    More information an examples available in the author repository:
    [ESP32SPISlave by hideakitai](https://github.com/hideakitai/ESP32SPISlave/tree/main)
    
# Usage
1. Upload code to `ESP32-CAM`:
    - Connect `GPIO0` and `GND` 
    - Connect board or push reset button 
    - PlatformIO > General > Upload
    - Disconnect `GPIO0`
    - Push reset button 
    - PlatformIO > General > Monitor

2. Upload code to `ESP32 DOIT ESP32 DEVKIT V1`:
    - PlatformIO > General > Upload and Monitor

# Notes
> [!NOTES]
> - Libraries detailed in the `platformio.ini` file are downloaded automatically.
> - `ESP32-CAM` requires a USB to UART TTL converter of 5V.

# Results

Master-SPI send a message to Slave-Slave: 

<img src="https://github.com/javierandrango/microcontrollers/images/SPI1.png" width="50%">