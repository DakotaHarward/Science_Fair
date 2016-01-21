#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8aec68a87202468fab5f6b972bb57bb6";
int earthquakePin = 2;
int powerPin = 7;
int powerPinLight = 8;
int alertPin = 13;
int earthquakeState = 0;
boolean runDetector = false;
boolean lcdDetected = false;
boolean lcdGo = true;
WidgetLCD lcd(V6);

void setup(){
  SwSerial.begin(9600);
  Blynk.begin(auth);
  pinMode(earthquakePin, INPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(powerPinLight, OUTPUT);
  pinMode(alertPin, OUTPUT);
  // Default baud rate is 9600. You could specify it like this:
  //Blynk.begin(auth, 57600);
}

void loop()
{
  Blynk.run();
  if(runDetector){
  if(digitalRead(earthquakePin) == HIGH) {
    if(lcdGo){
      lcdDetected = false;
      lcdPrint();
      lcdGo = false;
    }
    Blynk.virtualWrite(3, 1023);
    Blynk.notify("Possible Earthquake Alert.");
    Blynk.virtualWrite(6, "Earthquake Threat Detected!");
    digitalWrite(alertPin, HIGH);
  }else{
    if(lcdGo == false){
      lcdDetected = true;
      lcdPrint();
      lcdGo = true;
    }
    Blynk.virtualWrite(3, 0);
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
     runDetector = true;
     //button presed
  } else {
    Blynk.virtualWrite(2, 0);
    digitalWrite(powerPin, LOW);
    digitalWrite(powerPinLight, LOW);
    digitalWrite(alertPin, LOW);
    runDetector = false;
    //button released
  }
}
void lcdPrint(){
  if(lcdDetected){
    lcd.clear();
    lcd.print(0, 0, "Possible Earthquake");
    lcd.print(0, 1, "Threat Detected");
  }else{
    lcd.clear();
    lcd.print(0, 0, "No Earthquake");
    lcd.print(0, 1, "Threat Detected");
  }
}

