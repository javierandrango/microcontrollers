#include <Arduino.h>
// esp32cam with async web server
#include <AsyncWebCamera.h>
// wifi credentials (create your own file on include/Secrets.h)
#include <Secrets.h>
// wifi library
#include <WiFi.h>
// upload files to file system
#include <FS.h>
#include <LittleFS.h>


//Web server credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
AsyncWebServer server(80);


/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/
void streamJpg(AsyncWebServerRequest *request);
/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  // initialize camera (ESP32-CAM AI-Thinker configuration)
  Camera_init_cofig();
  // web server as Station mode:
  // we can  request information from the internet 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  else{
    Serial.printf("WiFi connected succesfully!");
  }
  // shows web page IP
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // stream video
  server.on("/stream", HTTP_GET, streamJpg);
  server.begin();

}
/*--------------------------------------------------------------------------------------------------------------------------*/

/*LOOP----------------------------------------------------------------------------------------------------------------------*/
void loop(){

}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/
void streamJpg(AsyncWebServerRequest *request){
    AsyncJpegStreamResponse *response = new AsyncJpegStreamResponse();
    if(!response){
        request->send(501);
        return;
    }
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}
/*--------------------------------------------------------------------------------------------------------------------------*/