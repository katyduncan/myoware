// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*
 * Serial Port over BLE
 * Create UART service compatible with Nordic's *nRF Toolbox* and Adafruit's *Bluefruit LE* iOS/Android apps.
 *
 * BLESerial class implements same protocols as Arduino's built-in Serial class and can be used as it's wireless
 * replacement. Data transfers are routed through a BLE service with TX and RX characteristics. To make the
 * service discoverable all UUIDs are NUS (Nordic UART Service) compatible.
 *
 * Please note that TX and RX characteristics use Notify and WriteWithoutResponse, so there's no guarantee
 * that the data will make it to the other end. However, under normal circumstances and reasonable signal
 * strengths everything works well.
 */


// Import libraries (BLEPeripheral depends on SPI)
//#include <SPI.h>
#include <BLEPeripheral.h>
//#include <BLEHIDPeripheral.h>
//#include <BLEMultimedia.h>
#include "BLESerial.h"

// define pins (varies per shield/board)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

// create ble serial instance, see pinouts above
BLESerial BLESerial(BLE_REQ, BLE_RDY, BLE_RST);

bool sw1 = true;
bool sw2 = true;
bool sw3 = true;
bool sw4 = true;

bool sw1Old = true;
bool sw2Old = true;
bool sw3Old = true;
bool sw4Old = true;

//BLEHIDPeripheral bleHIDPeripheral = BLEHIDPeripheral(BLE_REQ, BLE_RDY, BLE_RST);
//BLEMultimedia bleMultimedia;

void setup() {

  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  
  pinMode(17, INPUT);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT);
  pinMode(20, INPUT_PULLUP); 
    
  digitalWrite(21, HIGH);
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  
  // custom services and characteristics can be added as well
  BLESerial.setLocalName("TeleStoneMk0");

Serial.begin(115200);
  BLESerial.begin();

  //bleHIDPeripheral.setLocalName("HID Volume");
  //bleHIDPeripheral.addHID(bleMultimedia);

 // bleHIDPeripheral.begin();
  
}

void loop() {

 /*if (digitalRead(17) == LOW) {
  sw1 = LOW;
 }
 if (digitalRead(20) == LOW) {
  sw1 = HIGH;
 }
 */
 //Read input pins on switches
 sw1 = digitalRead(17);
 sw2 = digitalRead(18); 
 sw3 = digitalRead(19); 
 sw4 = digitalRead(20); 

/*
 //Used to debug switch inputs 
 digitalWrite(21, sw1);
 digitalWrite(22, sw2);
 digitalWrite(23, sw3);
 digitalWrite(24, sw4);
*/
  BLESerial.poll();
 // loopback();
  
  if (sw1 != sw1Old) { //Runs once on switch change for sw1---------------------

     if (sw1 == LOW) { //Runs when switch is false (pressed to GND)
       digitalWrite(21, LOW); //Turns LED on
       BLESerial.write("SwitchOne"); //Sends data to phone
       //bleMultimedia.write(MMKEY_VOL_UP);
       //digitalWrite(22, HIGH);
       //digitalWrite(23, LOW);
     }
     if (sw1 == HIGH) { //Runs when switch is true (Pullup resistor)
      
       digitalWrite(21, HIGH); //Turns off LED 
       //digitalWrite(21, LOW);
       //digitalWrite(22, LOW);
     }
     sw1Old = sw1; //Writes current switch state to Old var
  }

  if (sw2 != sw2Old) { //Runs once on switch change for sw2---------------------

     if (sw2 == LOW) { //Runs when switch is false (pressed to GND)
       digitalWrite(22, LOW); //Turns LED on
       BLESerial.write("SwitchTwo"); //Sends data to phone
     }
     if (sw2 == HIGH) { //Runs when switch is true (Pullup resistor)
       digitalWrite(22, HIGH); //Turns off LED 
     }
     sw2Old = sw2; //Writes current switch state to Old var
  }

  if (sw3 != sw3Old) { //Runs once on switch change for sw3---------------------

     if (sw3 == LOW) { //Runs when switch is false (pressed to GND)
       digitalWrite(23, LOW); //Turns LED on
       BLESerial.write("SwitchThree"); //Sends data to phone
     }
     if (sw3 == HIGH) { //Runs when switch is true (Pullup resistor)
       digitalWrite(23, HIGH); //Turns off LED 
     }
     sw3Old = sw3; //Writes current switch state to Old var
  }

  if (sw4 != sw4Old) { //Runs once on switch change for sw4---------------------

     if (sw4 == LOW) { //Runs when switch is false (pressed to GND)
       digitalWrite(24, LOW); //Turns LED on
       BLESerial.write("SwitchFour"); //Sends data to phone
     }
     if (sw4 == HIGH) { //Runs when switch is true (Pullup resistor)
       digitalWrite(24, HIGH); //Turns off LED 
     }
     sw4Old = sw4; //Writes current switch state to Old var
  }

  //digitalWrite(23, LOW);
  //delay (500);
  //digitalWrite(23, HIGH);
  //delay (500);
  
//Reads data send from phone
if (BLESerial) {//runs if new data is in receave buffer.
    int byte;
    while((byte = BLESerial.read()) > 0){
      if(byte == 0x41 || byte == 0x61){ //Tests in input is A or a
        digitalWrite(21, LOW); //Turns on LED
        delay(500); //Wait...
        digitalWrite(21, HIGH); //Tune off LED
        byte = 0; //Sets data to 0 to stop loop
      }
      if(byte == 0x42 || byte == 0x62){ //Tests in input is B or b
        digitalWrite(22, LOW);
        delay(500);
        digitalWrite(22, HIGH);
        byte = 0;
      }
      if(byte == 0x43 || byte == 0x63){ //Tests in input is C or c
        digitalWrite(23, LOW);
        delay(500);
        digitalWrite(23, HIGH);
        byte = 0;
      }
      if(byte == 0x44 || byte == 0x64){ //Tests in input is D or d
        digitalWrite(24, LOW);
        delay(500);
        digitalWrite(24, HIGH);
        byte = 0;
      }
    } 
  }

  

  //forward();
  

}


// forward received from Serial to BLESerial and vice versa
void forward() {
  if (BLESerial && Serial) {
    int byte;
    while ((byte = BLESerial.read()) > 0) Serial.write((char)byte);
    while ((byte = Serial.read()) > 0) BLESerial.write((char)byte);
  }
}

// echo all received data back
void loopback() {
  if (BLESerial) {
    int byte;
    while((byte = BLESerial.read()) > 0){
      BLESerial.write(byte);
      if(byte == 0x74){
        digitalWrite(22, LOW);
        delay(500);
        digitalWrite(22, HIGH);
        byte = 0;
      }
      
    }
    
    
/*    
    while ((byte = BLESerial.read()) > 0) BLESerial.write(byte);
    if(byte == 0x74){
      digitalWrite(21, LOW);
      delay(500);
      digitalWrite(21, HIGH);
      byte = 0;
    }
*/
    
  }
}


