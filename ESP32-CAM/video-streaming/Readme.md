# Description
Website video streaming using an asynchronous web server.

GPIO configurations are based on `ESP32-CAM AI-THINKER` microcontroller with an `OV2640` sensor camera.

# Prerequisites 
1. The project structure, directories, packages, and code were distributed using the `Platformio IDE extension` configuration to work with `VScode`. An installation tutorial can be found <a href="https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/" target="_blank">here</a>.


# Usage
1. Circuit diagram connection:

    <img src="https://github.com/javierandrango/microcontrollers/blob/main/images/VIDEO-STREAM-CIRCUIT.png" width='50%'>

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
        - Open a web-browser and copy the IP address the serial monitor indicates.

3. Delete everything from the microcontroller:
    - Connect `GPIO0` and `GND` 
    - Connect the board or push the reset button
    - Go to: PlatformIO > Platform > Erase Flash

4. Files description:
    - The Folder `src` contains the main code as `.cpp` file
    - The folder `include` contains the camera setup and GPIOS configuration as `.h` files. 
    - An additional file is required on `include/Secrets.h` directory. the file was EXCLUDED on purpose because it contains WiFi credentials. To avoid compiling errors create your file or replace WiFi credentials with your own 
    >[!TIP]
    >
    >Secrets.h
    >```
    >#define WIFI_SSID "******"
    >#define WIFI_PASSWORD "******"
    >```
    - The folder `data` contains the web files (.html, .js, .css, .svg) and their compressed version.

# Notes
> [!NOTE]
> - Libraries detailed in the `platformio.ini` file are downloaded automatically.
> - `ESP32-CAM` requires a USB to UART TTL converter of 5V.
> - The main code is based on the <a href="https://gist.github.com/me-no-dev/d34fba51a8f059ac559bf62002e61aa3" target="_blank">AsyncWebCamera.cpp</a> that uses an asynchronous web server to handle the camera response, and the Arduino example for ESP32-CAM, available on the Arduion-IDE `Examples > ESP32 > Camera > CameraWebServer`.
> - Web files(.html, .css, .js, .svg) were uploaded to the filesystem using the `LittleFS.h` library, the library is now part of the Arduino core for ESP32, and no additional installation is required.
> - The stream URL is linked directly to the src of the`<img>` tag, this allows the browser to handle the streaming more efficiently mainly when using high-resolution images.
> - The web files (.html,.css,.js) were compressed to `gzip` files using a free <a href="https://gzip.swimburger.net/" target="blank">online compressor</a>(The response from the server is faster using compressed files). To use normal web files uncomment the section `//normal web files` on the `main.cpp` and comment the section `//compressed web files`.

# Camera sensor values
- External clock(xclk): value necessary for the camera's internal operation, including capturing images and processing data.
- Auto exposure control(aec): how much light the sensor collects during each frame, can help achieve better image quality under different lighting conditions
- Automatic gain control(agc): amplify the signal from the camera sensor which can help improve the brightness and visibility of images in low-light conditions
- Auto white balance(awb): white balance for capturing images

# Results
Website on  mobile phone, landscape mode:

|Main page|Camera menu|Fullscreen|
|:-:|:-:|:-:|
|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream1-min.jpg" width='50%'>|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream2-min.jpg" width='50%'>|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream4-min.jpg" width='50%'>|

Website on mobile phone, portrait mode:

|Camera Menu|Fullscreen|Fit image to screen with pinch gesture|
|:-:|:-:|:-:|
|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream3-min.jpg">|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream5-min.jpg">|<img src="https://github.com/javierandrango/microcontrollers/blob/main/images/stream6-min.jpg">|

