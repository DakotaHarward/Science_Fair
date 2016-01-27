//
// ScienceFair
//
// Earthquake detector.
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		Dakota Kay Harward
// 				Dakota Kay Harward
//
// Date			1/23/16 9:32 PM
// Version		<#version#>
//
// Copyright	© Dakota Kay Harward, 2016
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//


// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#   include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#   include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#   include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#   include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#   include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#   include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#   include "Arduino.h"
#elif defined(SPARK) || defined(PARTICLE) // Particle / Spark specific
#   include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#   include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#   include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#   include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#   include "Arduino.h"
#else // error
#   error Platform not defined
#endif // end IDE

#include <StopWatch.h>
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
int hits = 0;
WidgetLCD lcd(V6);
WidgetTerminal terminal(V0);
StopWatch earthquakeTimer;
StopWatch alertTimer;

void setup(){
    terminal.flush();
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
            if(alertTimer.isRunning()){
                
            }else{
                if(earthquakeTimer.isRunning()){
                    if(earthquakeTimer.elapsed() < 999000000){
                        hits = hits + 1;
                        terminal.print(hits);
                        if(hits > 3){
                            earthquakeAlert();
                            hits = 0;
                            earthquakeTimer.stop();
                            earthquakeTimer.reset();
                        }else{
                            hits = 0;
                            earthquakeTimer.stop();
                            earthquakeTimer.reset();
                        }
                    }else{
                        hits = 0;
                        earthquakeTimer.stop();
                        earthquakeTimer.reset();
                    }
                }else{
                    earthquakeTimer.start();
                    hits = hits + 1;
                }
            }
        }else{
            if(alertTimer.isRunning()){
                if(alertTimer.elapsed() > 60000000){
                    Blynk.virtualWrite(3, 0);
                    digitalWrite(alertPin, LOW);
                    alertTimer.stop();
                    alertTimer.reset();
                }else{
                    terminal.print("earthquake alert called");
                    Blynk.virtualWrite(3, 1023);
                    Blynk.notify("Possible Earthquake Alert.");
                    Blynk.virtualWrite(6, "Earthquake Threat Detected!");
                    digitalWrite(alertPin, HIGH);
                }
            }else{
                Blynk.virtualWrite(3, 0);
                digitalWrite(alertPin, LOW);
            }
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
void earthquakeAlert(){
    if(alertTimer.isRunning()){
        alertTimer.stop();
        alertTimer.reset();
        alertTimer.start();
        terminal.print("earthquake alert called");
        Blynk.virtualWrite(3, 1023);
        Blynk.notify("Possible Earthquake Alert.");
        Blynk.virtualWrite(6, "Earthquake Threat Detected!");
        digitalWrite(alertPin, HIGH);
    }else{
        alertTimer.start();
        terminal.print("earthquake alert called");
        Blynk.virtualWrite(3, 1023);
        Blynk.notify("Possible Earthquake Alert.");
        Blynk.virtualWrite(6, "Earthquake Threat Detected!");
        digitalWrite(alertPin, HIGH);
    }
}
