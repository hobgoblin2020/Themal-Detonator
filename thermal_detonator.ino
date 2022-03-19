#include "Arduino.h"
#include "SoftwareSerial.h"
#include <DFPlayerMini_Fast.h>

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFPlayerMini_Fast myDFPlayer;
//Led Pins
int redled = 15;
int trigger = 6;
int yellowled1 = 16;
int yellowled2 = 17;
int yellowled3 = 18;

// Intial State of Yellow Led
int yellowled1state = LOW;
int yellowled2state = LOW;
int yellowled3state = LOW;

//Blink intervals
long yellowled1interval = 500;
long yellowled2interval = 1000;
long yellowled3interval = 1500;

//Timers for yellow leds 
long yellow1previousMillis = 0;
long yellow2previousMillis = 0;
long yellow3previousMillis = 0;

//Debounce values
const int debounceDelay = 100;
unsigned long lastDebounceTime = 0; 
int lastSteadyState = LOW;
int lastFlickerableState = LOW;
int currentState;

void setup()
{
  pinMode(yellowled1, OUTPUT);
  pinMode(yellowled2, OUTPUT);
  pinMode(yellowled3, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(trigger, INPUT_PULLUP);
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial, true);
  myDFPlayer.volume(1);
  Serial.begin(9600);
}

void loop(){
  currentState = digitalRead(trigger);
  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if(lastSteadyState  == HIGH && currentState == LOW)
        { 
         Serial.println("The button is pressed");
          myDFPlayer.loop(1);
          digitalWrite(redled, HIGH);
          flashLoop(yellowled1state,yellowled1,yellow1previousMillis,yellowled1interval);
          flashLoop(yellowled2state,yellowled2,yellow2previousMillis,yellowled2interval);
          flashLoop(yellowled3state,yellowled3,yellow3previousMillis,yellowled3interval);
        }
        else if(lastSteadyState  == LOW && currentState == LOW){
          Serial.println("The button is still pressed");
          flashLoop(yellowled1state,yellowled1,yellow1previousMillis,yellowled1interval);
          flashLoop(yellowled2state,yellowled2,yellow2previousMillis,yellowled2interval);
          flashLoop(yellowled3state,yellowled3,yellow3previousMillis,yellowled3interval);
        }
        else if(lastSteadyState  == LOW && currentState == HIGH){
          Serial.println("The button is released");
          myDFPlayer.stop();
          digitalWrite(redled, LOW);
          digitalWrite(yellowled1, LOW); 
          digitalWrite(yellowled2, LOW);
          digitalWrite(yellowled3, LOW);
          yellow1previousMillis = 0;
          yellow2previousMillis = 0;
          yellow3previousMillis = 0; 
        }
        else if(lastSteadyState  == LOW && currentState == LOW){
          Serial.println("The button is still released");
          myDFPlayer.stop();
          digitalWrite(redled, LOW);
          digitalWrite(yellowled1, LOW); 
          digitalWrite(yellowled2, LOW);
          digitalWrite(yellowled3, LOW);
          yellow1previousMillis = 0;
          yellow2previousMillis = 0;
          yellow3previousMillis = 0;  
        }
        lastSteadyState = currentState;
  }
}// Close Main Loop

void flashLoop(int &ledState, int &ledPin, long &previousMillis, long &interval){
 long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    if (ledState == LOW){
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
      } 
      else{
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
  }   
}
