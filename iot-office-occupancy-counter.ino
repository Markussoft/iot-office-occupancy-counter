/*
  Button

  https://www.sensingthecity.com/detecting-movement-direction-with-two-ultrasonic-distance-sensors/
  https://www.electrokit.com/avstandsmatare-ultraljud-hc-sr04-2-400cm

  Turns on and off internal LED,
  when pressing a pushbutton attached to pin 19.

  Original example created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button

  Also:
  Ultrasonic Sensor HC-SR04 and Arduino Tutorial

  by Dejan Nedelkovski,
  www.HowToMechatronics.com

  LoRaWan Library
  https://github.com/rgot-org/LoRaWan_esp32/tree/main
*/

//Start Display stuff
#include <Wire.h>
#include "HT_SSD1306Wire.h"
#include "images.h"

//Include LoaWAN
#include <LoRaWan_CayenneLPP.h>
#include <LoRaWan_esp32.h>
#include "LoRaWan_config.h"

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 19;  // the number of the pushbutton pin
const int ledPin = 35;     // the number of the LED pin
const int trigPin = 20;    // ultrasonic sensor trigger
const int echoPin = 26;    // ultrasonic sensor receiver
const int reedPin = 48;    // reed sensor (magnetic)
//const int IRPin = 6; // IR distance sensor
//const int IRPin = 6; // IR distance sensor
const int trigPin2 = 5;  // ultrasonic sensor trigger
const int echoPin2 = 6;  // ultrasonic sensor receiver


//variables to keep track of the timing of recent interrupts
unsigned long reed_time = 0;
unsigned long last_reed_time = 0;

unsigned long UV_time = 0;
unsigned long last_UV_time = 0;
unsigned long UV_time_reset = 0;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int reedState = 0;    // variable for reading the reed sensor status
unsigned short IRvalue;
long duration;
//int distance;
int distance = 0;
int distance1 = 0;
int distance2 = 0;
long t1 = 0;
long t2 = 0;
unsigned long start_time;
float max_distance = 50;
int polling_rate = 3;
bool LoRaWan_send = false;
size_t appDataSize = 4;
uint8_t LoraPayload[] = { 0x06, 0x07, 0x08, 0x09};


//int reed_changes = 0;
volatile int reed_changes;
volatile int IR_changes;
int door_openings;
int person_in = 0;
int person_out = 0;
int persons_office = 0;

const char* devEui = "70B3D57ED8002B0E"; // Change to TTN Device EUI
const char* appEui = "0000000000000000"; // Change to TTN Application EUI
const char* appKey = "CB7CAC5EDCD1A1D9D4C9BB7D625AFE65"; // Chaneg to TTN Application Key

LoRaWan_esp32 ttn;
LoRaWan_CayenneLPP lpp;
#ifndef AUTO_PIN_MAP // AUTO_PIN_MAP is set if board is defined in the file target-config.h
#include "board_config.h"
#endif // !AUTO_PINS
void message(const uint8_t* payload, size_t size, uint8_t port, int rssi)
{
	Serial.println("-- MESSAGE");
	Serial.printf("Received %d bytes on port %d (RSSI=%ddB) :", size, port, rssi);
	for (int i = 0; i < size; i++)
	{
		Serial.printf(" %02X", payload[i]);
	}
	Serial.println();
}

SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);  // addr , freq , i2c group , resolution , rst

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

//display functions below
void drawFontFaceDemo() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "# of door openings: " + String(door_openings));
  //display.setFont(ArialMT_Plain_10);
  display.drawString(0, 12, "Distance1 [cm]: " + String(distance1));
  display.drawString(0, 24, "Distance2 [cm]: " + String(distance2));
  //display.drawString(0, 36, "Reedstate: " + String(reedState));
  display.drawString(0, 36, "IRCounter: " + String(IR_changes));
  //display.setFont(ArialMT_Plain_16);

  //display.drawString(0, 24, String(distance1));
}

void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void)  //Vext default OFF
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}


Demo demos[] = { drawFontFaceDemo };

int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;
//End Displaystuff


void IRAM_ATTR revCounter() {
  reed_time = millis();
  if (reed_time - last_reed_time > 250) {
    reed_changes++;
    door_openings = reed_changes / 2;
    last_reed_time = reed_time;
  }
}

int readUVSensor(int tPin, int ePin) {

  //Also run the distance sensor while button pushed
  // Clears the trigPin
  int i;
  int sum = 0;
  digitalWrite(tPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(tPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(tPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ePin, HIGH);
  // Calculating the distance
  for (i = 0; i < polling_rate; i++) {
    distance = duration * 0.034 / 2;
    sum = sum + distance;
    delay(10);
  }
  return (sum / polling_rate);
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(reedPin, INPUT);
  //pinMode(IRPin, INPUT);
  pinMode(trigPin, OUTPUT);   // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT);   // Sets the echoPin as an Input
  Serial.begin(9600);         // Starts the serial communication

  //Add interrupt tp reedPin to count door openings
  attachInterrupt(reedPin, revCounter, FALLING);

  //Serial.begin(115200);
  //Serial.println();
  //Serial.println();
  VextON();
  delay(100);

  // Initialising the UI will init the display too.
  display.init();

  display.setFont(ArialMT_Plain_10);

//Setup LoraWan
  	Serial.println("Starting");
#ifndef AUTO_PIN_MAP
	SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
#endif // !AUTO_PIN_MAP
	ttn.begin();
	ttn.onMessage(message); // Declare callback function for handling downlink
	// messages from server
	ttn.join(devEui, appEui, appKey);
	Serial.print("Joining TTN ");
	while (!ttn.isJoined())
	{
		Serial.print(".");
		delay(500);
	}
	Serial.println("\njoined !");
	ttn.showStatus();
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  reedState = digitalRead(reedPin);


  //reedState == Low when there is no magnetic fiels, i.e. when the door is open.
  if (reedState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    //Also run the distance sensor while button pushed

    distance1 = readUVSensor(trigPin, echoPin);
    distance2 = readUVSensor(trigPin2, echoPin2);

    UV_time = millis();

    if (UV_time - last_UV_time > 1000) {
      // if either sensor distance drops below limit, record timestamp
      if (distance1 < max_distance) {
        t1 = millis();
        UV_time_reset = millis();
      } else if (distance2 < max_distance) {
        t2 = millis();
        UV_time_reset = millis();
      }

      //Reset the sensors if one is triggered but not the other.
      if (t1 > 0 || t2 > 0) {
        if (UV_time - UV_time_reset  > 500 ) {
              t1 = 0;
            t2 = 0;
            Serial.println("Sensor reset before counting");
        }
      }


      if (t1 > 0 && t2 > 0) {               // if both sensors have nonzero timestamps
        if (t1 < t2) {                      // if left sensor triggered first
          Serial.println("In from door");  // direction is left to right
          person_in++;
        } else if (t2 < t1) {               // if right sensor triggered first
          Serial.println("Out of door");  // direction is right to left
          person_out++;
        } else {
          Serial.println("");  // else print nothing (shouldn't happen)
        }

        // after printing direction, reset both timestamps to 0
        t1 = 0;
        t2 = 0;

        last_UV_time = UV_time;
        persons_office = person_in - person_out;
        Serial.println(persons_office);
        //Serial.println(person_in);
        //Serial.println(person_out);
        LoRaWan_send = true;
        
      }
    }

  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    //reed_changes++;
  }
  //istance1 = distance;
  // clear the display
  display.clear();
  // draw the current demo method
  demos[demoMode]();

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();

  if (LoRaWan_send){
  //  	static float nb = 18.2;
	//nb += 0.1;
	//lpp.reset();
	//lpp.addTemperature(1, nb);
  LoraPayload[0] = door_openings;
  LoraPayload[1] = persons_office;
  ttn.sendBytes(LoraPayload,appDataSize);
	if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize()))
	//{
	//	Serial.printf("Temp: %f TTN_CayenneLPP: %d %x %02X%02X\n", nb, lpp.getBuffer()[0], lpp.getBuffer()[1],
	//		lpp.getBuffer()[2], lpp.getBuffer()[3]);
	//}
  LoRaWan_send = false;
  }

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1) % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(10);
}
