
#include <BLEPeripheral.h>
#include "BLESerial.h"

// define pins
#define BLE_REQ   10 //May not need?
#define BLE_RDY   2  //May not need?
#define BLE_RST   9  //May not need?
#define pinSw1    14 //Up
#define pinSw2    16 //Down
#define pinSw3    12 //Center
#define pinSw4    13 //Left
#define pinSw5    15 //Right
#define pinOut1   17 //Red LED
#define pinOut2   19 //Green LED
#define pinOut3   18 //Blue LED
#define pinOut4   8  //White LED (power)
#define pinOut5   25 //Buzzer
#define pinOut6   24  //Vibe Motor

//create BLE serial instance
BLESerial BLESerial(BLE_REQ, BLE_RDY, BLE_RST);

int i;
int dataIn;
int dataOut;
int swNum = 5; //number of active switch inputs
int swRead[9] = {0, 0, 0, 0, 0, 0, 0, 0};
int swPos[9] = {0, 0, 0, 0, 0, 0, 0, 0};
int swReadOld[9] = {0, 0, 0, 0, 0, 0, 0, 0};
int pinSwArray[] = {pinSw1, pinSw2, pinSw3, pinSw4, pinSw5};
int pinOutArray[] = {pinOut1, pinOut2, pinOut3, pinOut4, pinOut5, pinOut6};
unsigned long swTime;
unsigned long swTimeOld;
bool sendData = false;
bool swTimeOut = false;
bool buzz = false;


void setup() {

  pinMode(pinSw1, INPUT);  //Up
  pinMode(pinSw1, INPUT_PULLUP);
  pinMode(pinSw2, INPUT);  //Right
  pinMode(pinSw2, INPUT_PULLUP);
  pinMode(pinSw3, INPUT);  //Down
  pinMode(pinSw3, INPUT_PULLUP);
  pinMode(pinSw4, INPUT);  //Left
  pinMode(pinSw4, INPUT_PULLUP);
  pinMode(pinSw5, INPUT);  //Center
  pinMode(pinSw5, INPUT_PULLUP);


  pinMode(pinOut1, OUTPUT);  //Red LED
  digitalWrite(pinOut1, LOW);
  pinMode(pinOut2, OUTPUT);  //Green LED
  digitalWrite(pinOut2, LOW);
  pinMode(pinOut3, OUTPUT);  //Blue LED
  digitalWrite(pinOut3, LOW);
  pinMode(pinOut4, OUTPUT);  //White LED (power)
  digitalWrite(pinOut4, HIGH);
  pinMode(pinOut5, OUTPUT);  //Buzzer
  digitalWrite(pinOut5, LOW);
  pinMode(pinOut6, OUTPUT);  //Vibe Motor
  digitalWrite(pinOut6, LOW);
  
  BLESerial.setLocalName("TeleStoneMk1");

  BLESerial.begin();

  analogWrite(pinOut4,12);//Dimms power LED to save power.
  
/*
  analogWrite(pinOut4,8);//Dimms power LED to save power.
  delay(333);
  analogWrite(pinOut4,24);
  delay(333);
  analogWrite(pinOut4,96);
  delay(333);
  analogWrite(pinOut4,0);
*/
}

void loop() {

  //Read input pins
  for (i = 0; i < swNum; ++i){
    swRead[i] = digitalRead(pinSwArray[i]);
  }

  //Polls BLE stack
  BLESerial.poll();

  if (swRead[0] == LOW){
    //analogWrite(pinOut6,63);
  }
  else {
    if (swRead[1] == LOW){
      //analogWrite(pinOut6,127);
    }
    else {
      if (swRead[2] == LOW){
        //analogWrite(pinOut6,192);
        //digitalWrite(pinOut2,HIGH);
      }
      else {
        if (swRead[3] == LOW){
          //analogWrite(pinOut6,255);
          //digitalWrite(pinOut1,HIGH);
        }
        else {
          if (swRead[4] == LOW){
            //analogWrite(pinOut5,127);
            /*  
            if (buzz == false){    //Buzz by cycle time
              digitalWrite(pinOut5,HIGH);
              buzz = true;
            }
            else {
              digitalWrite(pinOut5,LOW);
              buzz = false;
            }
            */
            //digitalWrite(pinOut3,HIGH);
            //analogWrite(pinOut3,63);
            //digitalWrite(pinOut3,HIGH);
          }
          else {
            //analogWrite(pinOut5,0);
            //digitalWrite(pinOut5,LOW);  //Buzz by cycle time
            //analogWrite(pinOut6,0);
            //digitalWrite(pinOut3,LOW);
            //analogWrite(pinOut3,0);
            //digitalWrite(pinOut1,LOW);
            //digitalWrite(pinOut2,LOW);
            //digitalWrite(pinOut3,LOW);
          }
        }
      }
    }
  }

  for (i = 0; i < swNum; ++i){
    if (swRead[i] != swReadOld[i]){
      
      //Runs when switch is false (pressed to GND)
      if (swRead[i] == LOW){ 
        //digitalWrite(pinOutArray[i], HIGH); //Turns LED on      
        swTime = millis();
        swTimeOut = false;
        ++swPos[i]; //Adds +1 to switch count  
                   
      }
      //Runs when switch is NOT pressed (Pullup resistor)  
      if (swRead[i] == HIGH){     
        //digitalWrite(pinOutArray[i], LOW); //Turns off LED 
        sendData = true;    
      }
      swReadOld[i] = swRead[i]; //Writes current switch state to Old var
      
    }
    //Times out switch if pressed more then 2sec
    if (millis() - swTime > 2500){
      sendData = false;
      swTimeOut = true;
      swPos[i] = 0;
    }

    if (millis() - swTime > 2000 && swTimeOut == false && sendData == false && swRead[i] == LOW){
      swPos[i] = 8;
      sendData = true;
      swTimeOut = true;
    }
    
    if (millis() - swTime > 500 && swPos[i] != 0 && sendData == true){
      if (i == 0){
        BLESerial.write("A"); //Sends data to phone
      }
      if (i == 1){
        BLESerial.write("B"); //Sends data to phone
      }
      if (i == 2){
        BLESerial.write("C"); //Sends data to phone
      }
      if (i == 3){
        BLESerial.write("D"); //Sends data to phone
      }
      if (i == 4){
        BLESerial.write("E"); //Sends data to phone
      }
      if (i == 5){
        BLESerial.write("F"); //Sends data to phone
      }
      if (i == 6){
        BLESerial.write("G"); //Sends data to phone
      }
      if (i == 7){
        BLESerial.write("H"); //Sends data to phone
      }
      if (swPos[i] == 1){
        BLESerial.write("1"); //Sends data to phone
      }
      if (swPos[i] == 2){
        BLESerial.write("2"); //Sends data to phone
      }
      if (swPos[i] == 3){
        BLESerial.write("3"); //Sends data to phone
      }
      if (swPos[i] == 4){
        BLESerial.write("4"); //Sends data to phone
      }
      if (swPos[i] == 5){
        BLESerial.write("5"); //Sends data to phone
      }
      if (swPos[i] == 6 ){
        BLESerial.write("6"); //Sends data to phone
      }
      if (swPos[i] == 7){
        BLESerial.write("7"); //Sends data to phone
      }
      if (swPos[i] == 8){
        BLESerial.write("8"); //Sends data to phone
      }
      if (swPos[i] >= 9){
        BLESerial.write("9"); //Sends data to phone
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
      if(dataIn == 0x54 || dataIn == 0x74){ //Tests in input is T or t
        analogWrite(pinOut5,127);  //Turn buzzer on
        delay(1000);  //Wait...
        analogWrite(pinOut5,0);  //Turn buzzer off
        dataIn = 0; //Sets data to 0 to stop loop
      }
      if(dataIn == 0x56 || dataIn == 0x76){ //Tests in input is V or v
        analogWrite(pinOut6,80);  //Turn vibe motor on at 31.25%       
        delay(500);  //Wait...
        analogWrite(pinOut6,0);  //Turn vibe motor off
        dataIn = 0;
      }
      if(dataIn == 0x4C || dataIn == 0x6C){ //Tests in input is L or l
        digitalWrite(pinOut3, HIGH);  //Turns blue LED on
        delay(500);  //Wait...
        digitalWrite(pinOut3, LOW);  //Turns blue LED off
        dataIn = 0;
      }
      if(dataIn == 0x44 || dataIn == 0x64){ //Tests in input is D or d
        //digitalWrite(pinOutArray[3], LOW);
        //delay(500);
        //digitalWrite(pinOutArray[3], HIGH);
        dataIn = 0;
      }
    } 
  }




}
