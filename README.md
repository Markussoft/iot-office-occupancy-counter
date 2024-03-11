# iot-office-occupancy-counter
Senses when a person enters or exits through a door, reports via LoRa to the cloud.
A counter keeps track of how many people are in the office. When the last person is leaving this is a good way to check without going around checking every room.
## Objectives
The counter should be standalone, battery operated and wireless. To keep track of the number of people passing it will feature a laser tripwire that is setup over the only entrance to the office. To save battery and not have the laser on all the time I believe it may be neccessary to have a separate sensor, likely magnetic or sound, that keeps track of when the door is opened and then starts the laser. To keep track of if a person is leaving or entering an ultrasonic sensor will be used to check if the person breaking the beam is moving away from the door of towards it (from the inside).

A subobjective of this project is to learn more about LoRaWAN, which means the sensor will report the data via LoRaWAN to a web interface where you can check how many people are in the office and how many times the door has been opened. It will also show the number of people on the display.


## Material
The material needed for this setup is:

* HELTEC Wifi LoRa 32 V3, ESP32 microcontroller.
* Laser sensor
* Light dependent resistor
* Reed sensor
* Ultrasonic distance sensor 
* Sensecap M2 LoraWAN Indoor Gateway
