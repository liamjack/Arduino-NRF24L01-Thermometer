// -- Includes --

// Radio
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// -- Global variables --

RF24 radio(9,10);
int packetCounter = 0;

// -- Functions --

void setupRadio()
{
 // Initiate the radio
 radio.begin();
 
 // Define the number of retries sending a packet
 radio.setRetries(15,15);
 
 // Define the radio's broadcast channel (0 - 127)
 // Must be the same on the sensor and the master node
 radio.setChannel(30);
 
 // Define the radio's bitrate (using cards lowest bitrate)
 radio.setDataRate(RF24_250KBPS);
 
 // Define the radio's power amplifier level (RF24_PA_MIN for debugging, RF24_PA_HIGH for longest range)
 radio.setPALevel(RF24_PA_MIN);
 
 // Enable dynamic payloads for packets
 radio.enableDynamicPayloads();
 
 // Open reading pipe
 radio.openReadingPipe(1, 0xE8E8F0F0E1LL);
 
 // Start listening for packets
 radio.startListening();
}

void setup()
{
  // Initiate serial connection
  Serial.begin(57600);
  
  // RF Radio setup
  setupRadio();
}

void loop()
{ 
  // Check if the radio has received any data
  if(radio.available()) {
    float temperature = 0;
    
    // Read the packet received into the temperature variable
    radio.read(&temperature, sizeof(float));
    
    // Print the temperature
    Serial.println(temperature);
    
    // Wait for serial print to complete
    delay(100);
  }
}
