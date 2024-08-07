#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// OLED configuration
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {

  // Initialize peripheral 
  u8g2.begin();
}
/*--------------------------------------------------------------------------------------------------------------------------*/

/*LOOP----------------------------------------------------------------------------------------------------------------------*/
void loop(){
  // Choose a suitable font
  u8g2.setFont(u8g2_font_ncenB08_tr);	
  for (int x=-70;x<=128;x++){
    u8g2.clearBuffer();
    // Write something to the internal memory
    u8g2.drawStr(x,36,"Hello User!");
    u8g2.sendBuffer(); 
  }
}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/