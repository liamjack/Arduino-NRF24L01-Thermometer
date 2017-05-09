// -- Includes --

// Sleep
#include <avr/sleep.h>
#include <avr/power.h>

// Watchdog timer
#include <avr/wdt.h>

// Radio
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Thermometer
#include <OneWire.h>
#include <DallasTemperature.h>

// -- Structures --

typedef struct {
  uint32_t sensorId;
  float temperature; 
} dataPacket;

// -- Constants --

const uint32_t sensorId = 1;

const rf24_datarate_e radioDataRate = RF24_250KBPS;
const rf24_crclength_e radioCRCLength = RF24_CRC_8;
const uint8_t radioPALevel = RF24_PA_MAX;
const uint8_t radioChannel = 50;
const uint8_t radioAddress[4] = "n01";

const int thermometerResolution = 10;

// -- Global variables --

volatile int f_wdt = 1;
int counter = 0;
int packetCounter = 0;

RF24 radio(9, 10);
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

// -- Functions --

void counterHandler()
{
  // Increment the sleep counter
  counter++;

  // Should be 75 for 10 minutes (75 * 8 = 600 seconds = 10 minutes)
  // Use 1 for debugging purposes
  
  if(counter == 75) {    
    // Reset the counter to 0
    counter = 0;
    
    // Power up components
    power_all_enable();
    
    // Power up the radio
    radio.powerUp();
    
    // Wait for radio to power up
    delay(2);
  } else {
    // Sleep time isn't over yet, sleep some more
    enterSleep();
  } 
}

void enterSleep()
{
  // Start the watchdog timer
  f_wdt = 0;
  
  // Power down the radio
  radio.powerDown();
  
  // Enter sleep
  sleep_enable();
  sleep_mode();
  
  // Wake from sleep
  sleep_disable();
  
  // Increment the interrupt counter
  counterHandler();
}

ISR(WDT_vect)
{
  // Stop the watchdog timer
  f_wdt = 1;
}

void setupWDT()
{
  // Setup the Watchdog timer for an interruption every 8 seconds
  
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3;
  WDTCSR |= _BV(WDIE);
}

void setupRadio()
{
 // Initiate the radio
 radio.begin();
 
 // Define the number of retries sending a packet
 radio.setRetries(15, 15);
 
 // Define the radio's broadcast channel (0 - 127)
 radio.setChannel(radioChannel);
 
 // Define the radio's bitrate (using cards lowest bitrate)
 radio.setDataRate(radioDataRate);
 
 // Define the radio's power amplifier level (RF24_PA_MIN for debugging, RF24_PA_MAX for longest range)
 radio.setPALevel(radioPALevel);
 
 // Define the size of packets
 radio.setPayloadSize(sizeof(dataPacket));

 radio.setCRCLength(radioCRCLength);

 radio.setAddressWidth(3);
}

void setupThermometer()
{
  // Initiate the sensor interface
  sensors.begin();
  
  // Set the bit resolution to 9 for fast conversion times (9 - 12)
  sensors.setResolution(thermometerResolution);
}

void setup()
{ 
  // Disable Brown out detection (uses power)
  sleep_bod_disable();
  
  // Sleep mode setup
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // Watchdog timer setup
  setupWDT();
  
  // RF Radio setup
  setupRadio();
  
  // Thermometer setup
  setupThermometer();
}

void loop()
{
  dataPacket packet;

  // Request the sensors temperatures
  sensors.requestTemperatures();
  
  // Get the first sensor's temperature
  packet.sensorId = sensorId;
  packet.temperature = sensors.getTempCByIndex(0);
  
  // Open a writing pipe on the radio
  radio.openWritingPipe(address);
  
  // Write the temperature to the pipe
  radio.write(&packet, sizeof(dataPacket));
  
  // Sleep time
  enterSleep();
}
