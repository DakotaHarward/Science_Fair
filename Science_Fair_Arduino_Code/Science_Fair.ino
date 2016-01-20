#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8aec68a87202468fab5f6b972bb57bb6";
const int buttonPin1 = 8;
const int buttonPin2 = 12;
const int powerPin = 13;
const int powerPinLight = 2;
const int alertPin = 7;
int button1State = 0;
int button2State = 0;
boolean runStuff = false;

void setup(){
  SwSerial.begin(9600);
  Blynk.begin(auth);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(powerPinLight, OUTPUT);
  pinMode(alertPin, OUTPUT);
  // Default baud rate is 9600. You could specify it like this:
  //Blynk.begin(auth, 57600);
}

void loop()
{
  Blynk.run();
  if(runStuff){
  button1State = digitalRead(buttonPin1);
  button2State = digitalRead(buttonPin2);
  if(button1State == LOW) {
    Blynk.virtualWrite(3, 1023);
    Blynk.notify("Possible Earthquake Alert.");
    Blynk.virtualWrite(6, "Earthquake Threat Detected!");
    digitalWrite(alertPin, HIGH);
  }else{
    Blynk.virtualWrite(3, 0);
    Blynk.virtualWrite(6, "No Earthquake Threat Detected.");
    digitalWrite(alertPin, LOW);
  }
  if(button2State == LOW) {
    Blynk.virtualWrite(3, 1023);
    Blynk.notify("Possible Earthquake Alert.");
    Blynk.virtualWrite(6, "Earthquake Threat Detected!");
    digitalWrite(alertPin, HIGH);
  }else{
    Blynk.virtualWrite(3, 0);
    Blynk.virtualWrite(6, "No Earthquake Threat Detected.");
    digitalWrite(alertPin, LOW);
  }
  }
}
BLYNK_WRITE(V4)  
{
  if (param.asInt()) {
     Blynk.virtualWrite(2, 1023);
     digitalWrite(powerPin, HIGH);
     digitalWrite(powerPinLight, HIGH);
     runStuff = true;
     //button presed
  } else {
    Blynk.virtualWrite(2, 0);
    digitalWrite(powerPin, LOW);
    digitalWrite(powerPinLight, LOW);
    runStuff = false;
    //button released
  }
}
