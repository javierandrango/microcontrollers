# Description
Send data from client HTML to ESP32 server to activate internal led
# Prerequisites
1. The code and configuration is based on ESP32 DOIT ESP32 DEVKIT V1 board
2. The project structure, directories, packages, and code were distributed using `Platformio IDE extension` configuration to work with `VScode`. An installation tutorial can be found [here](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)

# Usage
1. Open the file `LITTLEFS.cpp` and change the code explained in **Notes** section:
    ```bash
    .pio > libdeps > esp32doit-devkit-v1 > LittleFS_esp32 > LITTLEFS.cpp
    ```
2. Create a `secrets.h` file into `src` folder to manage wifi credentials and add your wifi credentials:
    ```arduino
    #define WIFI_SSID "your_wifi_name"
    #define WIFI_PASSWORD "your_password"
    ```
3. Upload files to the filesystem :
    ```bash
    PlatformIO > Platform > Build Filesystem
    PlatformIO > Platform > Upload Filesystem
    ```
4. Upload code to ESP32:
    ```bash
    PlatformIO > General > Upload and Monitor
    ```
# Notes
> [!NOTE]
> Libraries detailed in the `platformio.ini` file are downloaded automatically.
> A compilation error occurs in the build process. The original library (LittleFS_esp32@^1.0.6) `LITTLEFS.cpp` was changed:

✖️ removed:
```cpp
bool LITTLEFSImpl::exists(const char* path)
{
    File f = open(path, "r");
    return (f == true);
}
```
✔️ changed:
```cpp
bool LITTLEFSImpl::exists(const char* path)
{
    File f = open(path, "r",false);
    return (f == true);
}
```
# Results

HTML (websocket connection): 

<img src="https://github.com/javierandrango/mobile_robot/blob/main/images/websocket.png" width="50%">