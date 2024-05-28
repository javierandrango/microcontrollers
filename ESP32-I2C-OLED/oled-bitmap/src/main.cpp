#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <eyes.h>
// OLED configuration
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// time delay
unsigned long time_now_normal = 0;
int period_normal = 200;
unsigned long time_now_blink = 0;
int period_blink = 2000;

/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {

  // Initialize peripheral 
  display.begin();
  // shows original image colors
  display.setDrawColor(1);
  
  display.clearBuffer();
  // display happy eyes image
  display.drawXBMP(0,0,128,64,eyes_happy);
  display.sendBuffer();
  delay(300);
  // display happy eyes eyebrowns up image
  display.drawXBMP(0,0,128,64,eyes_happy_eyebrowns_up);
  display.sendBuffer();
  delay(300);
  // display happy eyes image
  display.drawXBMP(0,0,128,64,eyes_happy);
  display.sendBuffer();
  delay(300);
  // display happy eyes eyebrowns up image
  display.drawXBMP(0,0,128,64,eyes_happy_eyebrowns_up);
  display.sendBuffer();
  delay(300);
  // display happy eyes image
  display.drawXBMP(0,0,128,64,eyes_happy);
  display.sendBuffer();
  delay(300);
}
/*--------------------------------------------------------------------------------------------------------------------------*/

/*LOOP----------------------------------------------------------------------------------------------------------------------*/
void loop(){
  display.clearBuffer();
  // display normal eyes image
  if (millis() > time_now_normal+period_normal){
    time_now_normal = millis();
    display.drawXBMP(0,0,128,64,eyes_normal);
    display.sendBuffer();
  }
  // shows blink eyes image
  if (millis() > time_now_blink+period_blink){
    time_now_blink = millis();
    display.drawXBMP(0,0,128,64,eyes_blink);
    display.sendBuffer();
  }
}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/