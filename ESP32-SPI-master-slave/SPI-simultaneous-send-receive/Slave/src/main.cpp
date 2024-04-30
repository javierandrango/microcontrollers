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
static constexpr size_t QUEUE_SIZE = 2;
// Buffer size must be multiples of 4 bytes
static constexpr size_t BUFFER_SIZE = 16;
uint8_t tx_buf[BUFFER_SIZE]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t rx_buf[BUFFER_SIZE]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// Custom delay 
unsigned long currentTime = 0;             
unsigned long previousTime = 0;
int time_ms = 5;            

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
  // Message to send
  String data = "Hello Master!";
  strToBuffer(data,tx_buf,BUFFER_SIZE);

  // if no transaction is in flight and all results are handled, queue new transactions
  if (slave.hasTransactionsCompletedAndAllResultsHandled()) {
    // Receives an encoded message 
    slave.queue(NULL, rx_buf, BUFFER_SIZE);
    delay(5);
    // Sends an encoded message 
    slave.queue(tx_buf, NULL, BUFFER_SIZE);

    // Trigger transaction in the background
    slave.trigger();
  }
  
  if (slave.hasTransactionsCompletedAndAllResultsReady(QUEUE_SIZE)) {
    // Get received bytes for all transactions
    const std::vector<size_t> received_bytes = slave.numBytesReceivedAll(); 
    // Message received from master
    String rx_message = bufferToStr(rx_buf,received_bytes[0]); 
    Serial.println(rx_message);
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