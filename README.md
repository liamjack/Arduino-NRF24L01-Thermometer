## The plan

Create a cheap and low-power Arduino sensor device (currently for measuring the temperature but could be easily adapted to other Arduino sensors) using an NRF24L01+ 2.4Ghz transceiver to send temperature measurements every X minutes.

The idea is to have reliable RF communications, using small packet sizes, low transmission rates, whilst maximising communication range and battery life, by using the Arduino's and the transceiver's "Power-Down" modes.

## Requirements
* Two Arduino Nanos
* Two NRF24L01+ transceivers
* A DS18B20 One-Wire digital thermometer
* A 4.7KΩ resistor
* Jumper cables
* USB cables
* The Arduino IDE
* Maniacbug's [RF24 Library](https://github.com/maniacbug/RF24)
* Paul Stoffregen's [One-Wire library](http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip)
* Miles Burton's [DallasTemperature library](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* Basic C knowledge

## Sensor wiring

#### Layout
![](http://i.imgur.com/KO40IwZ.png)

#### NRF24L01+ Module

![](http://g02.a.alicdn.com/kf/UT8_ZyHXolbXXagOFbXp/120207292/UT8_ZyHXolbXXagOFbXp.jpg)

* VCC -> 3v3
* GND -> GND
* CSN -> D10
* CE -> D9
* MOSI -> D11
* SCK -> D13
* IRQ is not required for our use case, it's an interrupt pin that goes LOW whenever a packet arrives, a packet is sent or the maximum retry count for sending a packet is reached. You can find out more about that [here](https://www.insidegadgets.com/2012/08/22/using-the-nrf24l01-wireless-module/)
* MISO -> D12

#### DS18B20 Thermometer

![](http://37.59.100.119/wp-content/uploads/DS18B20-pinout.jpg)

* GND -> GND
* D -> D2
* VDD -> 3v3

Place a 4.7KΩ resistor between D and VCC.

## Master node wiring

#### Layout

![](http://i.imgur.com/vb4ZlIy.png)

#### NRF24L01+ Module

* VCC -> 3v3
* GND -> GND
* CSN -> D10
* CE -> D9
* MOSI -> D11
* SCK -> D13
* IRQ is not used
* MISO -> D12

## Getting it to work

With the Arduino IDE compile and upload `sensor.ino` to the sensor node, and `master.ino` to the master node. Leave the master node connected to the computer via USB, and open up the the Arduino IDE Serial console (CTRL+SHIFT+M).

Power up the sensor node a few meters away for testing purposes via USB power or with a direct 5V input on the 5V pin.

Every 8 seconds you should see the temperature (in °C) displayed on the serial monitor of the master node.

## Todo

* Reduce power usage
* Add GPRS module to the Master node to post temperature to a data logging API
* Test out other sensors (PIR Sensor, humidity, air pressure, smoke detector...)
* Add more sensors all communicating with a single master node which bundles up the data and posts it to the API
* Test battery life using 2 AA batteries and a 5V Step-up module
* Test maximum range in open area, within a building...

## Contributing

Any contribution is welcome, whether it's new ways to keep power usage to a minimum, how to send radio packets in a more reliable manner, how to communicate at an even lower bitrate, how to increase the RF range... Feel free to open up an issue if you have a problem, question, bug or idea.
