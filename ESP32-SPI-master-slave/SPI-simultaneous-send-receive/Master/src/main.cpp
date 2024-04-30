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
static constexpr size_t BUFFER_SIZE = 16;
uint8_t tx_buf[BUFFER_SIZE]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t rx_buf[BUFFER_SIZE]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/
void strToBuffer(String , uint8_t* , int);
String bufferToStr(uint8_t* ,int );
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
  
  // Message to send
  String data = "Hello Slave!";
  strToBuffer(data,tx_buf,BUFFER_SIZE);
  
  // Sends an encoded message 
  master.beginTransaction(SPISettings(clock_speed, MSBFIRST, SPI_MODE0));
  digitalWrite(SPI_SS, LOW);
  master.transfer(tx_buf, BUFFER_SIZE);
  //master.transferBytes(tx_buf, NULL, BUFFER_SIZE);
  digitalWrite(SPI_SS, HIGH);
  master.endTransaction();
  
  // Reveices an encoded message 
  master.beginTransaction(SPISettings(clock_speed, MSBFIRST, SPI_MODE0));
  digitalWrite(SPI_SS, LOW);
  master.transfer(rx_buf, BUFFER_SIZE);
  //master.transferBytes(NULL, rx_buf, BUFFER_SIZE);
  digitalWrite(SPI_SS, HIGH);
  master.endTransaction();

  // Message received from slave
  String slave_message = bufferToStr(rx_buf,BUFFER_SIZE);
  Serial.println(slave_message);

}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/
void strToBuffer(String data, uint8_t* buffer, int bufferSize){
  /**
   * Converts(encode) a String into a buffer of unsigned 8 bits integers
   * input:
   * - data(String): the string value
   * - buffer(uint8_t): an empty pre-sized buffer
   * - bufferSize(int): the size of the buffer
   * output:
   * - buffer(uint8_t): the filled buffer 
  */
  char dataBuffer[bufferSize];
  data.toCharArray(dataBuffer,bufferSize+1);
  for (int i=0;i<bufferSize;i++){
    if (i <data.length()){
      buffer[i]=static_cast<uint8_t>(dataBuffer[i]);
    }
    else{
      // Fill free buffer space
      buffer[i]=static_cast<uint8_t>('\0');
    }
    
  }
}

String bufferToStr(uint8_t* buffer,int bufferSize){
  /**
   * Convert(decode) the (uint8_t)buffer into a String
   * input:
   * - buffer(uint8_t): buffer with unsigned 8 bits integers(0-255) values
   * output:
   * - result(String): decoded String
  */
  String result="";
  for (int i=0;i<bufferSize;i++){
    if (buffer[i]==0){
      result+="";
    }
    else{
      result+=(char)buffer[i];
    }
  }
  return result;
}
/*--------------------------------------------------------------------------------------------------------------------------*/