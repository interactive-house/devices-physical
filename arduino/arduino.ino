#include <LiquidCrystal_I2C.h>
#include <Servo.h> // Servo function library

Servo doorServo;
Servo windowServo;

bool isDoorOpen = false;
bool isWindowOpen = false;
bool isLightOn = false;
int buttonRightPin = 4; // Define the button in D4
int buttonRightValue;
char incomingString;

int soilValue = 0;
int MQ2 = A0; // Define MQ2 gas sensor pin at A0
int pos = 0; // Start angle of servo used in loops

LiquidCrystal_I2C mylcd(0x27, 16, 2);
void setup()
{
  doorServo.attach(9); // Define the position of the servo on D9 DOOR
  windowServo.attach(10);
  windowServo.write(0);
  doorServo.write(90);
  mylcd.init();
  mylcd.backlight();
  
  pinMode (A2, INPUT); // Soil sensor is at A2, the mode is input
  pinMode(13, OUTPUT); // Lights module at pin 13
  pinMode (MQ2, INPUT); // MQ2 gas sensor as input
  pinMode (6, OUTPUT); // Define D6 as output for fan
  pinMode (7, OUTPUT); // Define D7 as output /fan????

  Serial.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop()
{
  checkGasValue();
  if (Serial.available() > 0)
  {
    // read the incoming letter:
    incomingString = Serial.read();
    switch (incomingString) {
      case 'd':
        changeDoor();
        break;
      case 'w':
        changeWindow();
        break;
      case 'l':
        changeLights();
        break;
      case 's':
        getSoilHumidity();
        break;
      default:
        //printToScreen("Error,bad input!");
        break;
    }
  }
}

void checkGasValue(){
    int val = analogRead (MQ2); // Read the voltage value of A0 port and assign it to val
    Serial.println (val); // Serial port sends val value
    if (val> 450){ // turn on the fan
      digitalWrite (7, HIGH);
      analogWrite(6,150);
    }else{
      digitalWrite (7, LOW);
      analogWrite(6,0);
    }
    delay(1000);
  }

void getSoilHumidity(){
  while(true){
    soilValue = analogRead(A2);
    buttonRightValue = digitalRead(buttonRightPin);
    if(buttonRightValue == 0){
       mylcd.clear();
       break;
    }
    if (soilValue < 300){
      Serial.write("Dry soil \n");
      printToScreen("Dry soil: " + String(soilValue));
      delay(500);
    }else{
      printToScreen("Wet soil: " + String(soilValue));
      delay(500);
    }
  }
}


void changeLights(){
  if(isLightOn){
    digitalWrite(13, LOW);
    isLightOn = false;
  }
  else{
    digitalWrite(13, HIGH);
    isLightOn = true;
  }printToScreen(" Light: " + String(isLightOn));
}


void changeWindow(){
  if(isWindowOpen){ // close the window
    for(pos = 90; pos>=1; pos-=1) // Angle from 180 to 0 degrees
    {
      windowServo.write (pos); // The servo angle is pos
      delay (15); // Delay 15ms
    }
    isWindowOpen = false;
  }else{ // open the window
    for(pos = 0; pos < 90; pos += 1)// angle from 0 to 180 degrees
    {
      windowServo.write (pos); // The angle of the servo is pos
      delay (15); // Delay 15ms
    }
    isWindowOpen = true;
    
  }printToScreen(" Window: " + String(isWindowOpen));
}


void changeDoor(){
  if(isDoorOpen){ // close the door
    for(pos = 0; pos < 90; pos += 1)// angle from 0 to 180 degrees
    {
      doorServo.write (pos); // The servo angle is pos
      delay (15); // Delay 15ms
    }
    isDoorOpen = false;
  }else{ // open the door
    for(pos = 90; pos>=1; pos-=1) // Angle from 180 to 0 degrees
    {
      doorServo.write (pos); // The angle of the servo is pos
      delay (15); // Delay 15ms
    }
    isDoorOpen = true;
    
  }printToScreen(" Door: " + String(isDoorOpen));
}

void printToScreen(String message)
{
  mylcd.clear();
  mylcd.setCursor(1 - 1, 1 - 1);
  mylcd.print(message);
}
