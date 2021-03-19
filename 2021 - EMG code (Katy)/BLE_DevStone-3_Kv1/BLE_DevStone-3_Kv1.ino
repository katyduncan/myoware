
#include <BLEPeripheral.h>
#include "BLESerial.h"

// define pins
#define BLE_REQ   10 //May not need?
#define BLE_RDY   2  //May not need?
#define BLE_RST   9  //May not need?
#define pinSw1    17
#define pinSw2    18
#define pinSw3    19
#define pinSw4    20
#define pinOut1   21
#define pinOut2   22
#define pinOut3   23
#define pinOut4   24

//Katy
//#define analogInPin 3 // A2
// pin for LED is 21 is pinOut1
#define sensorPin A2
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
//End Katy

//create BLE serial instance
BLESerial BLESerial(BLE_REQ, BLE_RDY, BLE_RST);

int i;
int dataIn;
int dataOut;
int swNum = 4; //number of active switch inputs
int swRead[9] = {0, 0, 0, 0, 0, 0, 0, 0};
int swPos[9] = {0, 0, 0, 0, 0, 0, 0, 0};
int swReadOld[9] = {0, 0, 0, 0, 0, 0, 0, 0};
int pinSwArray[] = {pinSw1, pinSw2, pinSw3, pinSw4};
int pinOutArray[] = {pinOut1, pinOut2, pinOut3, pinOut4};
unsigned long swTime;
unsigned long swTimeOld;
bool sendData = false;
bool swTimeOut = false;



void setup() {
//  Katy
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
//End Katy

  pinMode(pinSw1, INPUT);
  pinMode(pinSw1, INPUT_PULLUP);
  pinMode(pinSw2, INPUT);
  pinMode(pinSw2, INPUT_PULLUP);
  pinMode(pinSw3, INPUT);
  pinMode(pinSw3, INPUT_PULLUP);
  pinMode(pinSw4, INPUT);
  pinMode(pinSw4, INPUT_PULLUP);

  pinMode(pinOut1, OUTPUT);
  digitalWrite(pinOut1, HIGH);
  pinMode(pinOut2, OUTPUT);
  digitalWrite(pinOut2, HIGH);
  pinMode(pinOut3, OUTPUT);
  digitalWrite(pinOut3, HIGH);
  pinMode(pinOut4, OUTPUT);
  digitalWrite(pinOut4, HIGH);
  
  BLESerial.setLocalName("DevStoneMk1KD");

  BLESerial.begin();


}

void loop() {
//  Katy
  // read the analog in value:
  sensorValue = analogRead(sensorPin);


//example/test
////   turn the ledPin on
//  digitalWrite(pinOut1, HIGH);
////   stop the program for <sensorValue> milliseconds:
//  delay(sensorValue);
////   turn the ledPin off:
//  digitalWrite(pinOut1, LOW);
////   stop the program for for <sensorValue> milliseconds:
//  delay(sensorValue);

//or
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
//  BLESerial.write(outputValue);
//  // change the analog out value:
//  analogWrite(pinOut1, outputValue);
//
//  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
//  delay(2);
//End Katy

  

  //Read input pins
  for (i = 0; i < swNum; ++i){
    swRead[i] = digitalRead(pinSwArray[i]);
  }

  //Polls BLE stack
  BLESerial.poll();

  for (i = 0; i < swNum; ++i){
    if (swRead[i] != swReadOld[i]){
      
      //Runs when switch is false (pressed to GND)
      if (swRead[i] == LOW){ 
        digitalWrite(pinOutArray[i], LOW); //Turns LED on      
        swTime = millis();
        swTimeOut = false;
        ++swPos[i]; //Adds +1 to switch count  
                   
      }
      //Runs when switch is NOT pressed (Pullup resistor)  
      if (swRead[i] == HIGH){     
        digitalWrite(pinOutArray[i], HIGH); //Turns off LED 
        sendData = true;    
      }
      swReadOld[i] = swRead[i]; //Writes current switch state to Old var
      
    }
    //Times out switch if pressed more then 2sec
    if (millis() - swTime > 2000){
      swTimeOut = true;
      swPos[i] = 0;
    }
    
    if (millis() - swTime > 500 && swPos[i] != 0 && sendData == true){
      if (i == 0){
        BLESerial.write("1"); //Sends data to phone
      }
      if (i == 1){
        BLESerial.write("2"); //Sends data to phone
      }
      if (i == 2){
        BLESerial.write("3"); //Sends data to phone
      }
      if (i == 3){
        BLESerial.write("4"); //Sends data to phone
      }
      if (i == 4){
        BLESerial.write("5"); //Sends data to phone
      }
      if (i == 5){
        BLESerial.write("6"); //Sends data to phone
      }
      if (i == 6){
        BLESerial.write("7"); //Sends data to phone
      }
      if (i == 7){
        BLESerial.write("8"); //Sends data to phone
      }
      if (swPos[i] == 1){
        BLESerial.write("A"); //Sends data to phone
//        BLESerial.write(sensorValue);
//        BLESerial.write(outputValue);
      }
      if (swPos[i] == 2){
        BLESerial.write("B"); //Sends data to phone
      }
      if (swPos[i] == 3){
        BLESerial.write("C"); //Sends data to phone
      }
      if (swPos[i] == 4){
        BLESerial.write("D"); //Sends data to phone
      }
      if (swPos[i] == 5){
        BLESerial.write("E"); //Sends data to phone
      }
      sendData = false;
      swPos[i] = 0; //Resets switch count to zero
      delay(50); //adds time for stability (debounce)
    }    
  }


//Reads data send FROM phone
  //runs if new data is in receave buffer.
  if (BLESerial){
    while((dataIn = BLESerial.read()) > 0){
      if(dataIn == 0x41 || dataIn == 0x61){ //Tests in input is A or a
        digitalWrite(pinOutArray[0], LOW); //Turns on LED
        delay(500); //Wait...
        digitalWrite(pinOutArray[0], HIGH); //Tune off LED
        dataIn = 0; //Sets data to 0 to stop loop
      }
      if(dataIn == 0x42 || dataIn == 0x62){ //Tests in input is B or b
        digitalWrite(pinOutArray[1], LOW);
        delay(500);
        digitalWrite(pinOutArray[1], HIGH);
        dataIn = 0;
      }
      if(dataIn == 0x43 || dataIn == 0x63){ //Tests in input is C or c
        digitalWrite(pinOutArray[2], LOW);
        delay(500);
        digitalWrite(pinOutArray[2], HIGH);
        dataIn = 0;
      }
      if(dataIn == 0x44 || dataIn == 0x64){ //Tests in input is D or d
        digitalWrite(pinOutArray[3], LOW);
        delay(500);
        digitalWrite(pinOutArray[3], HIGH);
        dataIn = 0;
      }
    } 
  }

























}
