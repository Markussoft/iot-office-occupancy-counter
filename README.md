# iot-office-occupancy-counter
Senses when a person enters or exits through a door, reports via LoRa to the cloud.
A counter keeps track of how many people are in the office. When the last person is leaving this is a good way to check without going around checking every room.
## Objectives
The counter should be standalone, battery operated and wireless. To keep track of the number of people passing it will feature a laser tripwire that is setup over the only entrance to the office. To save battery and not have the laser on all the time I believe it may be neccessary to have a separate sensor, likely magnetic or sound, that keeps track of when the door is opened and then starts the laser. To keep track of if a person is leaving or entering an ultrasonic sensor will be used to check if the person breaking the beam is moving away from the door of towards it (from the inside).

Optionally I will not use the laser sensor and rely solely on the ultrasonic sensor. If possible some processing can be done on the server and then sent back to the sensor.

A subobjective of this project is to learn more about LoRaWAN, which means the sensor will report the data via LoRaWAN to a web interface where you can check how many people are in the office and how many times the door has been opened. It will also show the number of people on the display.


## Material
The material needed for this setup is:

* HELTEC Wifi LoRa 32 V3, ESP32 microcontroller.
* Laser sensor
* Light dependent resistor
* Reed sensor
* Ultrasonic distance sensor 
* Sensecap M2 LoraWAN Indoor Gateway

## Steps
1. Setup board and install sensors
2. Install SensCap LoRaWAN Gateway
3. Setup The Things Network and Datacake
    3.5 Tried using Adafruit and read up on MQTT.
4. Set up communications from Heltec ESP32 to The things Network and decode the data in Datacake
5. Merge the sensor program and the communications
6.  Set up a dashboard in datacake
7. See to it that the correct data gets passed to Datacake,
8.  add second Ultrasonic sensor 
9.  Switch from Datacake to NodeRED, set up NodeRED server at home.  <- Am here
11. 3D print holder for 2 Ultrasonic sensors and breadboard to mount over door.
12. Test setup.
13. Docker, with NodeRED, instead of datacake?
14. Try RTOS, as in this project: https://github.com/per25/P1-HAN-port-with-lora


##3D Printed part
100 mm high
150 mm deep
70 mm between sensors
Holes 43 x 17 mm

