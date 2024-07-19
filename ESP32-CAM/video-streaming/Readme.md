# Description
1. 

 
# Prerequisites 
1. GPIO configuration are based on `ESP32-CAM AI-THINKER` microcontroller with a `OV2640` sensor camera.
2. The project structure, directories, packages, and code were distributed using `Platformio IDE extension` configuration to work with `VScode`. An installation tutorial can be found [here](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)


# Usage
1. Circuit diagram connection:
    <img src="https://github.com/javierandrango/microcontrollers/blob/main/images/SPI-CIRCUIT-PWM-LED.png" width='70%'>

2. Upload code to `ESP32-CAM`:
    - Connect `GPIO0` and `GND` 
    - Connect the board or push the reset button 
    - PlatformIO > General > Upload
    - Disconnect `GPIO0`
    - Push the reset button 
    - PlatformIO > General > Monitor
3. Upload web files(.html, .css, .js, .svg) to filesystem 
    - Connect `GPIO0` and `GND` 
    - Connect the board or push the reset button
    - ....
    
# Notes
> [!NOTE]
> - Libraries detailed in the `platformio.ini` file are downloaded automatically.
> - `ESP32-CAM` requires a USB to UART TTL converter of 5V.
> - The main code was based on the [AsyncWebCamera.cpp](https://gist.github.com/me-no-dev/d34fba51a8f059ac559bf62002e61aa3) that use an asyncronous web server to handle the camera response, and the arduino example for esp32-cam, vailable on the Arduion-IDE `Examples > ESP32 > Camera > CameraWebServer`.
> - Files used by the server (.html, .css, .js, .svg) were uploaded to the filesystem using `LittleFS.h` library, the library is now part of the Arduino core for ESP32, no additional installation is required.
> - The stream url is linked directly to the src of the`<img>` tag, this allows the browser to handle the streaming more efficiently mainly when using high-resolution images.
> - The web files (.html,.css,.js) were compressed to `gzip files` using a free [online compressor](https://gzip.swimburger.net/)(The response from the server is faster using compressed files). In order to use normal web files uncomment the section `//normal web files` on the `main.cpp` and comment the section `//compressed web files`.

# Camera sensor
- auto exposure control (aec): how much light the sensor collects during each frame,can help achieve better image quality under different lighting conditions
- automatic gain control (agc): amplify the signal from the camera sensor which can help improve the brightness and visibility of images in low-light conditions

# Results
...
<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/SPI3.png">
