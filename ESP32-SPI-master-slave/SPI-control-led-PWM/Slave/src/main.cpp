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
static constexpr size_t BUFFER_SIZE = 4;
uint8_t tx_buf[BUFFER_SIZE]{0,0,0,0};
uint8_t rx_buf[BUFFER_SIZE]{0,0,0,0};

// Led
# define LED 27
uint8_t led_channel = 1;                                   // PWM channel (16 channels available)
uint8_t led_resolution = 8;                                // 8-bit resolution (0-255)
uint32_t led_freq = 1200;                                  // PWM frecuency    

/*FUNCTION DECLARATION------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/*VOID SETUP CONFIGURATION--------------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);

  // Driving led with PWM
  pinMode(LED,OUTPUT);
  ledcSetup(led_channel,led_freq,led_resolution);          // Initialize PWM 
  ledcAttachPin(LED,led_channel);                          // Assign LED(GPIO) to channel 1
  
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

  // if no transaction is in flight and all results are handled, queue new transactions
  if (slave.hasTransactionsCompletedAndAllResultsHandled()) {
    // Receives an encoded message 
    slave.queue(NULL, rx_buf, BUFFER_SIZE);
    // Trigger transaction in the background
    slave.trigger();
  }
  
  if (slave.hasTransactionsCompletedAndAllResultsReady(QUEUE_SIZE)) {
    // Get received bytes for all transactions
    //const std::vector<size_t> received_bytes = slave.numBytesReceivedAll();
    size_t received_bytes = slave.numBytesReceived(); 
    
    // Message received from master
    ledcWrite(1,rx_buf[0]);                                // Control PWM on channel 1
    Serial.println(rx_buf[0]);
  }
}
/*--------------------------------------------------------------------------------------------------------------------------*/


/*FUNCTIONS-----------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/