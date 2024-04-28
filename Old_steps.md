
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
