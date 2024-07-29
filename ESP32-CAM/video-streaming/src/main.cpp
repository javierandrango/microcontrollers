#include <Arduino.h>
// esp32cam with async web server
#include <AsyncWebCamera.h>
#include <ESPAsyncWebServer.h>
// wifi credentials (create your own file on include/Secrets.h)
#include <Secrets.h>
// wifi library
#include <WiFi.h>
// upload files to filesystem
#include <LittleFS.h>

//Web server credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
AsyncWebServer server(80);

//LittleFs filesystem
#define FORMAT_LITTLEFS_IF_FAILED true

//esp32-cam internal led
int led_channel = 2;
int led_resolution = 8;
int led_frequency = 980;
int led_duty = 0;
int max_intensity = 255;


/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/
void notFound(AsyncWebServerRequest *request);
void StreamJpg(AsyncWebServerRequest *request);
void SetCameraVar(AsyncWebServerRequest *request);
void GetCameraStatus(AsyncWebServerRequest *request);
void SetXclkValue(AsyncWebServerRequest *request);
void SetupCameraLed();
/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  // initialize camera led
  SetupCameraLed();
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
    Serial.println("WiFi connected succesfully!");
    // shows web page IP
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  
  // Mount LittleFS filesystem
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LITTLEFS Mount Failed");
    return;
  }
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  /*
  // normal web files
  server.serveStatic("/static/css/style.css", LittleFS, "/static/css/style.css");
  server.serveStatic("/static/js/main.js", LittleFS, "/static/js/main.js");
  server.serveStatic("/static/icons/", LittleFS, "/static/icons/");
  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/stream.html","text/html", false);
  });
  */
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  // compressed web files
  server.on("/static/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/static/js/main.js.gz", "application/javascript");
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });
  server.on("/static/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/static/css/style.css.gz", "text/css");
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });
  server.serveStatic("/static/icons/", LittleFS, "/static/icons/");
  //server.serveStatic("/", LittleFS, "/").setDefaultFile("stream.html.gz").setCacheControl("max-age=200");
  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/stream.html.gz", "text/html");
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Handle URLs
  server.onNotFound(notFound);
  server.on("/stream", HTTP_GET, StreamJpg);
  server.on("/control", HTTP_GET, SetCameraVar);
  server.on("/status", HTTP_GET, GetCameraStatus);
  server.on("/xclk", HTTP_GET, SetXclkValue);
  server.begin();

}
/*--------------------------------------------------------------------------------------------------------------------------*/

/*LOOP----------------------------------------------------------------------------------------------------------------------*/
void loop(){
  //camera led control
  ledcWrite(led_channel,led_duty);  
}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/
// URL not found
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// stream jpg images over async web server
void StreamJpg(AsyncWebServerRequest *request){
  AsyncJpegStreamResponse *response = new AsyncJpegStreamResponse();
  if(!response){
    // not implemented : server not support the functionality
    request->send(501);
    return;
  }
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

// set xclk(external clock) value. 

void SetXclkValue(AsyncWebServerRequest *request){
  if(!request->hasArg("xclk")){
    request->send(404, "text/plain", "Not found");
    return;
  }
  int xclk = atoi(request->arg("xclk").c_str());
  sensor_t * s = esp_camera_sensor_get();
  if(s == NULL){
    // not implemented : server not support the functionality
    request->send(501);
    return;
  }
  // xclk value in MHz, default 20MHz
  int res = s->set_xclk(s,LEDC_TIMER_0,xclk);
  
  AsyncWebServerResponse * response = request->beginResponse(200);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

// configure stream images over async web server
void SetCameraVar(AsyncWebServerRequest *request){
  if(!request->hasArg("var") || !request->hasArg("val")){
    request->send(404,"text/plain", "Not found");
    return;
  }
  String var = request->arg("var");
  const char * variable = var.c_str();
  int val = atoi(request->arg("val").c_str());
  sensor_t * s = esp_camera_sensor_get();
  if(s == NULL){
    // not implemented : server not support the functionality
    request->send(501);
    return;
  }
  int res = 0;
  if(!strcmp(variable, "framesize")) res = s->set_framesize(s, (framesize_t)val);
  else if(!strcmp(variable, "quality")) res = s->set_quality(s, val);
  else if(!strcmp(variable, "contrast")) res = s->set_contrast(s, val);
  else if(!strcmp(variable, "brightness")) res = s->set_brightness(s, val);
  else if(!strcmp(variable, "saturation")) res = s->set_saturation(s, val);
  else if(!strcmp(variable, "gainceiling")) res = s->set_gainceiling(s, (gainceiling_t)val);
  else if(!strcmp(variable, "colorbar")) res = s->set_colorbar(s, val);
  else if(!strcmp(variable, "awb")) res = s->set_whitebal(s, val);
  else if(!strcmp(variable, "agc")) res = s->set_gain_ctrl(s, val);
  else if(!strcmp(variable, "aec")) res = s->set_exposure_ctrl(s, val);
  else if(!strcmp(variable, "hmirror")) res = s->set_hmirror(s, val);
  else if(!strcmp(variable, "vflip")) res = s->set_vflip(s, val);
  else if(!strcmp(variable, "awb_gain")) res = s->set_awb_gain(s, val);
  else if(!strcmp(variable, "agc_gain")) res = s->set_agc_gain(s, val);
  else if(!strcmp(variable, "aec_value")) res = s->set_aec_value(s, val);
  else if(!strcmp(variable, "aec2")) res = s->set_aec2(s, val);
  else if(!strcmp(variable, "dcw")) res = s->set_dcw(s, val);
  else if(!strcmp(variable, "bpc")) res = s->set_bpc(s, val);
  else if(!strcmp(variable, "wpc")) res = s->set_wpc(s, val);
  else if(!strcmp(variable, "raw_gma")) res = s->set_raw_gma(s, val);
  else if(!strcmp(variable, "lenc")) res = s->set_lenc(s, val);
  else if(!strcmp(variable, "special_effect")) res = s->set_special_effect(s, val);
  else if(!strcmp(variable, "wb_mode")) res = s->set_wb_mode(s, val);
  else if(!strcmp(variable, "ae_level")) res = s->set_ae_level(s, val);
  else if(!strcmp(variable, "led_intensity")){
    led_duty = val;
  }
  else {
    request->send(404);
    return;
    //log_e("unknown setting %s", var.c_str());
  }
  //log_d("Got setting %s with value %d. Res: %d", var.c_str(), val, res);
  AsyncWebServerResponse * response = request->beginResponse(200);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
};

// get stream images configuration over async web server
void GetCameraStatus(AsyncWebServerRequest *request){
  static char json_response[1024];
  sensor_t * s = esp_camera_sensor_get();
  if(s == NULL){
    // not implemented : server not support the functionality
    request->send(501);
    return;
  }
  char * p = json_response;
  *p++ = '{';
  p+=sprintf(p, "\"framesize\":%u,", s->status.framesize);
  p+=sprintf(p, "\"quality\":%u,", s->status.quality);
  p+=sprintf(p, "\"brightness\":%d,", s->status.brightness);
  p+=sprintf(p, "\"contrast\":%d,", s->status.contrast);
  p+=sprintf(p, "\"saturation\":%d,", s->status.saturation);
  p+=sprintf(p, "\"special_effect\":%u,", s->status.special_effect);
  p+=sprintf(p, "\"wb_mode\":%u,", s->status.wb_mode);
  p+=sprintf(p, "\"awb\":%u,", s->status.awb);
  p+=sprintf(p, "\"awb_gain\":%u,", s->status.awb_gain);
  p+=sprintf(p, "\"aec\":%u,", s->status.aec);
  p+=sprintf(p, "\"aec2\":%u,", s->status.aec2);
  p+=sprintf(p, "\"ae_level\":%d,", s->status.ae_level);
  p+=sprintf(p, "\"aec_value\":%u,", s->status.aec_value);
  p+=sprintf(p, "\"agc\":%u,", s->status.agc);
  p+=sprintf(p, "\"agc_gain\":%u,", s->status.agc_gain);
  p+=sprintf(p, "\"gainceiling\":%u,", s->status.gainceiling);
  p+=sprintf(p, "\"bpc\":%u,", s->status.bpc);
  p+=sprintf(p, "\"wpc\":%u,", s->status.wpc);
  p+=sprintf(p, "\"raw_gma\":%u,", s->status.raw_gma);
  p+=sprintf(p, "\"lenc\":%u,", s->status.lenc);
  p+=sprintf(p, "\"hmirror\":%u,", s->status.hmirror);
  p+=sprintf(p, "\"vflip\":%u,", s->status.vflip);
  p+=sprintf(p, "\"dcw\":%u,", s->status.dcw);
  p+=sprintf(p, "\"colorbar\":%u", s->status.colorbar);
  p+=sprintf(p, ",\"led_intensity\":%u", led_duty);
  *p++ = '}';
  *p++ = 0;
  
  AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}


// esp32-cam internal led configuration
void SetupCameraLed(){
  pinMode(LED_GPIO_NUM,OUTPUT);
  ledcSetup(led_channel, led_frequency, led_resolution);
  ledcAttachPin(LED_GPIO_NUM, led_channel);    
}

/*--------------------------------------------------------------------------------------------------------------------------*/