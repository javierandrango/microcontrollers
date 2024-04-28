#include <Arduino.h>
#include<ESP32SPISlave.h>

// SPI custom pins
# define SPI_MISO 22
# define SPI_MOSI 23
# define SPI_SCK 35
# define SPI_SS 34
// SPI settings
ESP32SPISlave slave;
// Queued transactions
static constexpr size_t QUEUE_SIZE = 1;
// Buffer size must be multiples of 4 bytes
static constexpr size_t BUFFER_SIZE = 12;
uint8_t rx_buf[BUFFER_SIZE];


/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/
void strToBuffer(String, uint8_t*, int);
String bufferToStr(uint8_t*, int);
/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  delay(2000);
  //Initialize SPI bus with defined pins as SLAVE
  slave.setDataMode(SPI_MODE0); 
  slave.setQueueSize(QUEUE_SIZE);
  // VSPI or HSPI (virtual or hardward SPI)
  slave.begin(VSPI,SPI_SCK,SPI_MISO,SPI_MOSI,SPI_SS); 
  delay(2000);
  Serial.println("start spi slave");
}
/*--------------------------------------------------------------------------------------------------------------------------*/

/*LOOP----------------------------------------------------------------------------------------------------------------------*/
void loop(){
  
  if (slave.hasTransactionsCompletedAndAllResultsHandled()) {
    // execute transaction in the background and wait for completion
    slave.queue(NULL, rx_buf, BUFFER_SIZE);
    slave.trigger();
  }
  if (slave.hasTransactionsCompletedAndAllResultsReady(QUEUE_SIZE)) {
      // get the oldeest transfer result
      size_t received_bytes = slave.numBytesReceived();
      // decode message received
      String rx_string = bufferToStr(rx_buf,received_bytes);
      Serial.println(rx_string);
  }

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