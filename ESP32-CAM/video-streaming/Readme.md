# Description
Website video streaming using asyncronus webserver.

GPIO configuration are based on `ESP32-CAM AI-THINKER` microcontroller with a `OV2640` sensor camera.

# Prerequisites 
1. The project structure, directories, packages, and code were distributed using `Platformio IDE extension` configuration to work with `VScode`. An installation tutorial can be found [here](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/). 


# Usage
1. Circuit diagram connection:

    <img src="https://github.com/javierandrango/microcontrollers/blob/main/images/VIDEO-STREAM-CIRCUIT.png" width='70%'>

2. Upload code and web files: 
    - Connect `GPIO0` and `GND` 
    - Connect the board or push the reset button 
    - Upload code to `ESP32-CAM`:
        - Go to: PlatformIO > General > Upload
    - Upload web files(.html, .css, .js, .svg) to filesystem: 
        - Push the reset button 
        - Go to: PlatformIO > Platform > Upload Filesystem Image
    - Inspect the website:    
        - Disconnect `GPIO0`
        - Push the reset button 
        - PlatformIO > General > Monitor
        - Open a web-browser and copy the IP address that the serial monitor indicates.

3. Delete everything from the microcontroller:
    - Connect `GPIO0` and `GND` 
    - Connect the board or push the reset button
    - Go to: PlatformIO > Platform > Erase Flash

# Notes
> [!NOTE]
> - Libraries detailed in the `platformio.ini` file are downloaded automatically.
> - `ESP32-CAM` requires a USB to UART TTL converter of 5V.
> - The main code is based on the [AsyncWebCamera.cpp](https://gist.github.com/me-no-dev/d34fba51a8f059ac559bf62002e61aa3) that uses an asyncronous web server to handle the camera response, and the Arduino example for ESP32-CAM, available on the Arduion-IDE `Examples > ESP32 > Camera > CameraWebServer`.
> - Web files(.html, .css, .js, .svg) were uploaded to the filesystem using `LittleFS.h` library, the library is now part of the Arduino core for ESP32, no additional installation is required.
> - The stream URL is linked directly to the src of the`<img>` tag, this allows the browser to handle the streaming more efficiently mainly when using high-resolution images.
> - The web files (.html,.css,.js) were compressed to `gzip` files using a free [online compressor](https://gzip.swimburger.net/)(The response from the server is faster using compressed files). In order to use normal web files uncomment the section `//normal web files` on the `main.cpp` and comment the section `//compressed web files`.

# Camera sensor values
- External clock(xclk): value necessary for the camera's internal operation, including capturing images and processing data.
- Auto exposure control(aec): how much light the sensor collects during each frame,can help achieve better image quality under different lighting conditions
- Automatic gain control(agc): amplify the signal from the camera sensor which can help improve the brightness and visibility of images in low-light conditions
- Auto white balance(awb): white balance for capturing images

# Results
Website on  mobile phone, landspace mode:

|Main page|Camera menu|Fullscreen|
|-|-|-|
|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream1-min.jpg" style="width=50%">|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream2-min.jpg" style="width=50%">|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream4-min.jpg" style="width=50%">|

Website on mobile phone, portrait mode:

|Camera Menu|Fullscreen|Fit image to screen with pinch gesture|
|-|-|-|
|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream3-min.jpg" style="width=50%">|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream5-min.jpg" style="width=50%">|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream6-min.jpg" style="width=50%">|

