# iot-office-occupancy-counter
## Senses when a person enters or exits through a door, reports via LoRaWAN to a wordpress website.
A counter keeps track of how many people are in the office. When the last person is leaving this is a good way to check without going around checking every room.

**Table of Contents**

1. [Objectives](#Objectives)
2. [Material](#Material)
3. [Environment setup](#Environment-setup)
4. [Putting everything together](#Putting-everything-together)
5. [Platforms and infrastructure](#Platforms-and-infrastructure)
6. [The code](#The-code)
7. [The physical network layer](#The-physical-network-layer)
8. [Visualisation and user interface](#Visualisation-and-user-interface)
9. [Finalizing the design](#Finalizing-the-design)
9. [References](#References)


### Objectives
The counter should be standalone, battery operated and wireless. To keep track of the number of people passing it will feature a laser tripwire that is setup over the only entrance to the office. To save battery and not have the laser on all the time I believe it may be neccessary to have a separate sensor, likely magnetic or sound, that keeps track of when the door is opened and then starts the laser. To keep track of if a person is leaving or entering an ultrasonic sensor will be used to check if the person breaking the beam is moving away from the door of towards it (from the inside).

Optionally I will not use the laser sensor and rely solely on the ultrasonic sensor. If possible some processing can be done on the server and then sent back to the sensor.

A subobjective of this project is to learn more about LoRaWAN, which means the sensor will report the data via LoRaWAN to a web interface where you can check how many people are in the office and how many times the door has been opened. It will also show the number of people on the display.


### Steps
1. Setup board and install sensors
2. Install SensCap LoRaWAN Gateway
3. Setup The Things Network and Datacake
3.5 Tried using Adafruit and read up on MQTT.
4. Set up communications from Heltec ESP32 to The things Network and decode the data in Datacake
5. Merge the sensor program and the communications
6.  Set up a dashboard in datacake
7. See to it that the correct data gets passed to Datacake,
8. Add second Ultrasonic sensor 
9. Switch from Datacake to Docker, with NodeRED and MQTT. Have set up NodeRED server in a Debian virtual machine at home. The Debian machine is running on Proxmox.
10. Set up a Wordpress site that can receive the data from NodeRED and display it do the internet. Uses digitalocean and a domain from Hover.
11. 3D print holder for 2 Ultrasonic sensors and breadboard to mount over door.
13. Test setup.

    
14. Try RTOS, as in this project: https://github.com/per25/P1-HAN-port-with-lora
    Tried it, I cant't find a LoRaWan library that works for HELTEC Wifi LoRa 32 V3.

<!---
Give a short and brief overview of what your project is about.
What needs to be included:

- [ ] Title
- [ ] Your name and student credentials (xx666xxx)
- [ ] Short project overview
- [ ] How much time it might take to do (an approximation)
-->

The material needed for this setup is:

* HELTEC Wifi LoRa 32 V3, ESP32 microcontroller.
* 2x ultrasonic distance sensors
* Reed sensor
* Ultrasonic distance sensor 
* Sensecap M2 LoraWAN Indoor Gateway

| Product | Where to buy | Description | Price |
| --------- | ---------------- | ---------------- | ----------------: |
| NodeMCU ESP32 Heltec | [link](https://www.amazon.se/dp/B08243JHMW?ref_=pe_24982401_518009621_302_E_DDE_dt_1) | Microcontroller supporting WiFi and Lora. Built in OLED display. | 350SEK |
| Vibration sensor high sensitivity | [link](https://www.electrokit.com/produkt/vibrationssensor-hog-kanslighet/) | Measures vibration through digital output | 42SEK |
| Jumper wires male-male | [link](https://www.electrokit.com/produkt/labbsladd-40-pin-30cm-hane-hane/) | Wires to connect the circuits | 49SEK |
| Jumper wires female-male | [link](https://www.electrokit.com/produkt/labbsladd-40-pin-30cm-hona-hane/) | Wires to connect the circuits | 49SEK |
| USB to Micro USB cable | [link](https://www.kjell.com/se/produkter/kablar-kontakter/usb-kablar/linocell-micro-usb-kabel-svart-05-m-p93424?gclid=Cj0KCQiAsdKbBhDHARIsANJ6-jdFMu6K6bP9FJbrX_VwUeSgRLyFK9sPdiU4-TL19HrHKeCEr88ER2IaAqSyEALw_wcB&gclsrc=aw.ds) | Cable to program the device | 110SEK |
| Battery | [link](https://www.kjell.com/se/produkter/el-verktyg/laddare/mobilladdare/powerbank/linocell-powerbank-10000-mah-p89256) | Power supply | 199SEK |
| Breadboard | [link](https://sizable.se/P.TVY7M/Kopplingsdack-med-830-punkter) | Breadboard to connect device and sensor during development | 59SEK |

### Environment setup

This project was programmed in Arduino and to get you started you will need to set up the environment.

##### 1. Install drivers

 + Connect the Heltec board to your computer using a USB to micro USB cable
 + Download drivers and install preferred version from https://www.silabs.com/documents/public/software/CP210x_Windows_Drivers.zip

##### 3. Install IDE on computer

 + For this project I ended up using Thonny for development which is lightweight IDE for microcontrollers but I would suggest you try using either VS Code or Atom with the PyMakr plugin installed. I had some stability issues uploading new code when I used VS Code with PyMakr but it's definitely a more modern setup. You can download Thonny from here: https://thonny.org/

##### 4. Verify your installation

 + Open Thonny and hit the stop/restart button. You should se the following statement when the device boots up 
 > Pycom MicroPython 1.20.2.r4 [v1.20.1.r2-392-g172cf5d0-dirty] on 2021-01-19; Heltec Wireless Stick with ESP32


### Putting everything together

To get the physical setup in place we need to connect our vibration sensor and LoRa antenna. To make things a little bit easier I used a breadboard to connect the circuits. 

#### Steps

##### 1. Disconnect any power supply (computer or battery)
##### 2. Connect LoRa antenna
##### 3. Connect vibration sensor
+ Connect one wire from **5V** on the board to pin **VCC** on the sensor 
+ Connect one wire from **GND** on the board to pin **GND** o the sensor
+ Connect one wire from **PIN10** on the board to pin **DO** (digital output)
##### 4. As power supply I used a power bank with **5V DC** connected with USB


### Platforms and infrastructure

As the device supports wireless communication using both WiFi and LoRA there are two ways to connect the device to the internet. Both the database and the web server is hosted on the **Azure** Platform so if you use WiFi these are the services required to be set up. If you instead want to use LoRa you also need to set up a **Helium** account and configure your device and integration. Read more about the platform setup under "The physical network layer".

#### Cost



### The code


### The physical network layer


#### Setting up communication using LoRa with The Things Network

To set the device up using LoRa and The Things Network follow the steps found in this guide

???

### Visualisation and user interface

#### NodeRED server hoted on local computer
NodeRED is subscribed to an MQTT server at The Things Network. This requires setting up a username and password under the MQTT tab in The Things Network.
These credentials then have to be entered in the MQTTin node in NodeRED.

![NodeRED](https://github.com/Markussoft/iot-office-occupancy-counter/blob/main/Assets/NodeRED_flow.png)

![User](https://github.com/Markussoft/iot-office-occupancy-counter/blob/main/Assets/NodeRED_MQTTuser.png)

This is the template posted to the website.
```javascript=
"message":"test",
"title":"Office Occupancy Counter",
"excerpt":"Door openings: {{flow.doorOpenings}} \n People in office: {{flow.peoplePassing}} ",
"content":"Door openings: {{flow.doorOpenings}} \n People in office: {{flow.peoplePassing}} \n \n This data is send via LoRaWan and The Things Network. The data is decoded in NodeRED before it is posted to this site using the wordpress REST API. ",
"status":"publish"
```
The http request looks like this, following the instructions for the Wordpress REST API.
![User](https://github.com/Markussoft/iot-office-occupancy-counter/blob/main/Assets/NodeRED_post.png)

#### Wordpress website hosted on Digitalocean


### Finalizing the design



I'm happy with the end result of this project but I haven't reached the end goal.

To be continued...

### References

https://www.hostinger.in/tutorials/wordpress-rest-api
