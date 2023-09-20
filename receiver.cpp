/*
Authors: Caleb Taylor, Maureen Sanchez, BengalBots team
Purpose: Create an Arduino combat bot to compete in LSU Bengal Brawl against Senior Design teams
Objective: Acts as the receiver and acts out the commands that the transmitter has sent to it


Code References:
- Moving components https://github.com/maureensanchez99/Arroyo-Seco-Duck-Race-2022
- Parsing through data https://github.com/maureensanchez99/CaSGC-Internship-2022
*/


//Libraries needed to use certain modules and functions to make this program work
#include <SPI.h>       //used to start communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>


//Variables
RF24 radio(7, 8); // CE, CSN


//changing variables
//int jDirectionL, jDirectionR;           //int variables to store individual values


//Constant variables
const byte address[6] = "00001";
int leftMotorPin1 = 2, leftMotorPin2 = 3, rightMotorPin1 = 4, rightMotorPin2 = 5, weaponMotor = 6;
int leftJoystick = 513, rightJoystickA = 502, rightJoystickB = 503;


// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL = 0;  //reads the command given from the left joystick
  int jDirectionR = 0;  //reads the command given from the right joystick
  int killButton = 0;   //reads the command given from the killswitch
  int weaponButton = 0; //reads the command given from the weapon button
};


Data_Package data; //Create a variable with the above structure


void setup() {
  Serial.begin(115200);  // boots serial monitor at ? bps
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(weaponMotor, OUTPUT);
}


void loop() {
 if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
  }


  if(data.weaponButton == 0 && data.killButton == 1){ //checks if the weapon needs to be activated
    digitalWrite(weaponMotor, HIGH); //activate weapon
    //don't activate kill switch
  } else if(data.weaponButton == 1 && data.killButton == 0){
    digitalWrite(data.killButton, HIGH);//activate kill switch
  } else if(data.weaponButton == 0 && data.killButton == 0){
    //kill bot and/or turn on bot
  }


  if(data.jDirectionL > leftJoystick && data.jDirectionR > 502) { //tells the bot to move forward
    digitalWrite(leftMotorPin1, HIGH); //left motor moves forward
    digitalWrite(leftMotorPin2, LOW);  //left motor moves forward
    digitalWrite(rightMotorPin1, HIGH);//right motor moves forward
    digitalWrite(rightMotorPin2, LOW); //right motor moves forward
  } else if (data.jDirectionL < leftJoystick && data.jDirectionR < 502){ //tells the bot to move backward
    digitalWrite(leftMotorPin1, LOW);  //left motor moves backward
    digitalWrite(leftMotorPin2, HIGH); //left motor moves backward
    digitalWrite(rightMotorPin1, LOW); //right motor moves backward
    digitalWrite(rightMotorPin2, HIGH);//right motor moves backward
  } else if (data.jDirectionL > leftJoystick && data.jDirectionR == 502){ //tells the bot to turn right
    digitalWrite(leftMotorPin1, HIGH); //left motor moves forward
    digitalWrite(leftMotorPin2, LOW);  //left motor moves forward
    digitalWrite(rightMotorPin1, LOW); //right motor moves backward
    digitalWrite(rightMotorPin2, HIGH);//right motor moves backward
  } else if (data.jDirectionL == leftJoystick && data.jDirectionR > 502){ //tells the bot to turn left
    digitalWrite(leftMotorPin1, LOW);  //left motor moves backward
    digitalWrite(leftMotorPin2, HIGH); //left motor moves backward
    digitalWrite(rightMotorPin1, HIGH);//right motor moves forward
    digitalWrite(rightMotorPin2, LOW); //right motor moves forward
  }
  else if (data.jDirectionL == leftJoystick && (data.jDirectionR == 502 || data.jDirectionR == 503)) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }


  /* testing to make sure the values are being collected accurately
  Serial.print("a: ");
  Serial.print(data.jDirectionL);
  Serial.print(" b: ");
  Serial.print(data.jDirectionR);
  Serial.print(" c: ");
  Serial.print(data.killButton);
  Serial.print(" d: ");
  Serial.println(data.weaponButton);
  Serial.println(" "); */
}
