#include <StopWatch.h>
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3);
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

char auth[] = "8aec68a87202468fab5f6b972bb57bb6";
int earthquakePin = 2;
int hitLight = 4; 
int powerPin = 7;
int powerPinLight = 8;
int alertPin = 13;
int earthquakeState = 0;
int sensitivity = 1;
int notificationTime = 5;
boolean runDetector = false;
int hits = 0;
WidgetLCD lcd(V6);
WidgetTerminal terminal(V0);
StopWatch earthquakeTimer(StopWatch::SECONDS);
StopWatch alertTimer(StopWatch::SECONDS);
StopWatch measuring;

void setup(){
  terminal.flush();
  SwSerial.begin(9600);
  Blynk.begin(auth);
  pinMode(earthquakePin, INPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(powerPinLight, OUTPUT);
  pinMode(alertPin, OUTPUT);
  pinMode(hitLight, OUTPUT);
  digitalWrite(powerPin, LOW);
  digitalWrite(powerPinLight, LOW);
  digitalWrite(alertPin, LOW);
}

void loop()
{
  Blynk.run();
  if(alertTimer.isRunning()){
    if(alertTimer.elapsed() >= notificationTime){
      Blynk.virtualWrite(3, 0);
      digitalWrite(alertPin, LOW);
      alertTimer.stop();
      alertTimer.reset();
    }
  }
  if(runDetector){
    if(digitalRead(earthquakePin) == HIGH) {
      measuring.stop();
      measuring.reset();
      measuring.start();
      digitalWrite(hitLight, HIGH);
      if(alertTimer.isRunning()){
        terminal.print(" * still running * ");
        alertTimer.stop();
        alertTimer.reset();
        alertTimer.start();
      }else{
      if(earthquakeTimer.isRunning()){
        if(earthquakeTimer.elapsed() < 1){
          hits = hits + 1;
          terminal.print(hits);
          if(hits > sensitivity){
            terminal.print(" %*& ALERT &*% ");
            earthquakeAlert();
            resetTime();
          }
        }else{
          resetTime();
        }
      }else{
        earthquakeTimer.start();
        hits = hits + 1;
      }
      }
    }else{
      digitalWrite(hitLight, LOW);
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
  }
}
BLYNK_WRITE(V1){
  sensitivity = param.asInt();
}
BLYNK_WRITE(V5){
  notificationTime =  param.asInt();
}
BLYNK_WRITE(V0){
  if (String("clear") == param.asStr()) {
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
    terminal.println("");
  }
  if (String("resetlits") == param.asStr()) {
    Blynk.virtualWrite(3, 0);
    digitalWrite(alertPin, LOW);
  }
}
void earthquakeAlert(){
  if(alertTimer.isRunning()){
    alertTimer.stop();
    alertTimer.reset();
    alertTimer.start();
    terminal.print("earthquake alert called");
    Blynk.virtualWrite(3, 1023);
    Blynk.notify("Possible Earthquake Alert.");
    digitalWrite(alertPin, HIGH);
  }else{
    alertTimer.start();
    terminal.print("earthquake alert called");
    Blynk.virtualWrite(3, 1023);
    Blynk.notify("Possible Earthquake Alert.");
    digitalWrite(alertPin, HIGH);
    measuring.stop();
    terminal.println("");
    terminal.println("Arduino Speed: ");
    terminal.print(measuring.elapsed());
    terminal.println("");
    terminal.flush();
  }
}
void resetTime(){
    hits = 0;
    earthquakeTimer.stop();
    earthquakeTimer.reset();
}

