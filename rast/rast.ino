#define ELEVTIMEOUTMS 1000
#define AZIMTIMEOUTMS 2500
#define LDRTHRESHOLD 4.92
#define HYSTWIDTH 4

#include <Encoder.h>
#include <DS1307RTC.h>

#include "pins.h"
#include "interface.h"
#include "motion.h"
#include "ldrutils.h"
#include "tracking.h"

volatile int16_t posElev, posAzim;

Encoder encElev(encElevD1, encElevD2);
Encoder encAzim(encAzimD1, encAzimD2);

bool pressed, axisElev, saved, finished;
unsigned long timePressed;

uint8_t machineState;

void setup() {
  Serial.begin(115200);
  delay(150);

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12)|(1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);

  ldrInit();
  trackInit(RTC);
  if(!tm.Hour && !tm.Minute && !tm.Second){
    Serial.println("RTC failed!");
    while(true){}
  }

  pinMode(driverIn1, OUTPUT);
  pinMode(driverIn2, OUTPUT);
  pinMode(driverIn3, OUTPUT);
  pinMode(driverIn4, OUTPUT);

  pinMode(encElevD1, INPUT_PULLUP);
  pinMode(encElevD2, INPUT_PULLUP);
  pinMode(encAzimD1, INPUT_PULLUP);
  pinMode(encAzimD2, INPUT_PULLUP);

  pinMode(ldrUR, INPUT);
  pinMode(ldrUL, INPUT);
  pinMode(ldrDR, INPUT);
  pinMode(ldrDL, INPUT);

  machineState = 0;   // Not necessary but good for explicitness
}

// Use the timer interrupt to periodically read encoder information
ISR(TIMER1_COMPA_vect){
  int16_t _newPosElev, _newPosAzim;
  
  _newPosElev = encElev.read();
  if(_newPosElev != posElev){
    posElev = _newPosElev;
    posElevChanged = true;
  }
  
  _newPosAzim = encAzim.read();
  if(_newPosAzim != posAzim){
    posAzim = _newPosAzim;
    posAzimChanged = true;
  }
}

void loop() {
  getCmd();
  switch(machineState){
    case 0:
      if(!digitalRead(rotEncCk)){
        // the CLK pin goes low with
        // every click of the rotary encoder
        // then the DT pin tells the direction
        if(axisElev){
          if(digitalRead(rotEncDT)) up3();
          else                      down3();
          delay(250); // for debouncing
        }else{
          if(digitalRead(rotEncDT)) cw3();
          else                      ccw3();
          delay(250);
        }
      }
      
      if(!digitalRead(rotEncSw) && !pressed){
        axisElev = !axisElev;
        pressed = true;
        timePressed = millis();
      }else if(digitalRead(rotEncSw) && pressed){
        pressed = false;
        delay(250);
      }
    
      if(pressed && millis()-timePressed>1000){
        machineState = 1;
        Serial.println("Tracking begin");
      }
      break;
      
    case 1:      
      RTC.read(tm);
      curTime = tm.Hour + tm.Minute/60.0 + tm.Second/3600.0;

      if(trackElevs[trackElevIndex]){
        // both tracking lists end with a 0.0
        if(curTime > abs(trackElevs[trackElevIndex])){
          if(trackElevs[trackElevIndex] > 0)  up3();
          else                                down3();
          trackElevIndex++;
        }
      }

      if(trackAzims[trackAzimIndex]){
        // both tracking lists end with a 0.0
        if(curTime > abs(trackAzims[trackAzimIndex])){
          if(trackAzimIndex){
            // we have to ignore the zeroth element in the azim tracking list
            // because it merely represents the time the sun shows up
            // in the horizon and not the first time to actually move
            if(trackAzims[trackAzimIndex] > 0)  cw3();
            else                                ccw3();
          }
          trackAzimIndex++;
        }
      }
      
      if(tm.Second == 0 && !saved){
        readLDR();
        saveLDR();
        saved = true;
        Serial.println("Saved!");
      }
      if(tm.Second == 1){
        saved = false;
      }
      break;
  }

  if(trackElevs[trackElevIndex] == 0.0 && trackAzims[trackAzimIndex] == 0.0){
    
    if(!finished){
      Serial.println("Reached end of trajectory!");
      finished = true;
    }
  }
  
  actOnElev();
  actOnAzim();
}
