#include <Arduino.h>
#include <SPI.h>

// SPI custom pins
# define SPI_MISO 12
# define SPI_MOSI 13
# define SPI_SCK 14
# define SPI_SS 15
// VSPI or HSPI (virtual or hardward SPI)
SPIClass master(VSPI);  
// VSPI clock speed
uint32_t clock_speed = 1000000;

// Buffer size must be multiples of 4 bytes
static constexpr size_t BUFFER_SIZE = 4;
uint8_t tx_buf[BUFFER_SIZE]{0,0,0,0};
uint8_t rx_buf[BUFFER_SIZE]{0,0,0,0};

// Potentimeter
#define potPin 4
uint8_t potVal = 0;

/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  delay(2000);
  // Initialize SPI bus with defined pins as MASTER
  pinMode(SPI_SS,OUTPUT);
  digitalWrite(SPI_SS,HIGH);
  master.begin(SPI_SCK,SPI_MISO,SPI_MOSI,SPI_SS);
  // Wait for SPI to stabilize
  delay(2000);
  Serial.println("start spi master");
}
/*--------------------------------------------------------------------------------------------------------------------------*/

/*LOOP----------------------------------------------------------------------------------------------------------------------*/
void loop(){
  
  // Read Potentiometer values
  potVal = map(analogRead(potPin),0,4095,0,255);
  // Add potentiometer value to buffer
  tx_buf[0] = potVal;
  
  // Sends an encoded message 
  master.beginTransaction(SPISettings(clock_speed, MSBFIRST, SPI_MODE0));
  digitalWrite(SPI_SS, LOW);
  master.transfer(tx_buf, BUFFER_SIZE);
  //master.transferBytes(tx_buf, NULL, BUFFER_SIZE);
  digitalWrite(SPI_SS, HIGH);
  master.endTransaction();

  Serial.print("pot value: ");
  Serial.println(potVal);
}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/