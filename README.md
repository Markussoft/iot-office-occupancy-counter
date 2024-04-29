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
The counter should be standalone, battery operated and wireless. To keep track of the number of people passing it will use ultrasonic sensors and be mounted over the doorway that is the only entrance to the office. Both to save battery (by not running the ultrasonic sensors all the time) and to count door openings, a Reed sensor is used to detect if the door is opened. To keep track of if a person is leaving or entering there are two ultrasonic sensors. By keeping track of which of them is reached first the direction of movement can be deducted.

A subobjective of this project is to learn more about LoRaWAN, which means the sensor will report the data via LoRaWAN to a web interface where you can check how many people are in the office and how many times the door has been opened.

<!---
Give a short and brief overview of what your project is about.
What needs to be included:

- [ ] Title
- [ ] Your name and student credentials (xx666xxx)
- [ ] Short project overview
- [ ] How much time it might take to do (an approximation)
-->
### Material

The material needed for this setup is:

* HELTEC Wifi LoRa 32 V3, ESP32 microcontroller.
* 2x ultrasonic distance sensors
* Reed sensor
* Sensecap M2 LoraWAN Indoor Gateway
* Battery
* 

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

### 1. Install drivers and Arudino IDE

 + Connect the Heltec board to your computer using a USB-C cable
 + The Heltec docs were a very good help https://docs.heltec.org/en/node/esp32/quick_start.html

#### LoRaWAN Library
The library included in the Heltec Board I fould very complicated, if forced the chip to go inte deep sleep between sending to function correctly.
This library was much easier to use. https://www.arduino.cc/reference/en/libraries/lorawan_esp32/

![LoraWAN](https://github.com/Markussoft/iot-office-occupancy-counter/blob/main/Assets/Arduino_LoRaWAN_esp32.png)

### Putting everything together

To get the physical setup in place we need to connect our vibration sensor and LoRa antenna. To make things a little bit easier I used a breadboard to connect the circuits. 

#### Steps

##### 1. Disconnect any power supply (computer or battery)
##### 2. Connect LoRa antenna
##### 3. Connect ??? sensor

### Platforms and infrastructure


#### Cost



### The code


### The physical network layer


#### Setting up communication using LoRa with The Things Network

To set the device up using LoRa and The Things Network follow the steps found in this guide

???

### Visualisation and user interface
There are several ways to do this, but using NodeRED was a nice experience. I host a NodeRED server on a server in my apartment. NodeRED is running inside a virtual Debian machine, which is in turn hosted on Proxmox.
The website is hosted on DigitalOcean using one of their Wordpress droplets. I bough a domain on Hover and got the DNS settings sorted out.
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

The result is that the only post is updated:

![Markussoft](https://github.com/Markussoft/iot-office-occupancy-counter/blob/main/Assets/Markusoft.png)

### Finalizing the design

I believe I have learned a lot about IoT networks communications during this project. There is definitely several possible ways to improve this project, but I'm happy with the proof of concept.

* Improve the accuracy of the ultrasonic counters. Likely possible to improve both the physical positioning and the signal processing.
* Improve the website and make it nicer.
* Implement a downlink reset command, to clear every night if something has gone wrong.

### References
https://docs.heltec.org/en/node/esp32/quick_start.html

https://www.arduino.cc/reference/en/libraries/lorawan_esp32/

https://www.hostinger.in/tutorials/wordpress-rest-api

https://www.sensingthecity.com/detecting-movement-direction-with-two-ultrasonic-distance-sensors/
