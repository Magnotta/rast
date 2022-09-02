#include <EEPROM.h>

const extern byte ldrUR,ldrUL,ldrDR,ldrDL;

double URval, ULval, DRval, DLval;

bool hystWindowUR[HYSTWIDTH+1], hystWindowUL[HYSTWIDTH+1],
      hystWindowDR[HYSTWIDTH+1], hystWindowDL[HYSTWIDTH+1];

bool URlit, ULlit, DRlit, DLlit;

bool printMode;

void loadLDR(){
  int16_t adr=0;
  double n1, n2, n3, n4;
  while(adr < 4032){
    EEPROM.get(adr, n1);
    adr += sizeof(double);
    EEPROM.get(adr, n2);
    adr += sizeof(double);
    EEPROM.get(adr, n3);
    adr += sizeof(double);
    EEPROM.get(adr, n4);
    adr += sizeof(double);

    Serial.print(n1);
    Serial.print(',');
    Serial.print(n2);
    Serial.print(',');
    Serial.print(n3);
    Serial.print(',');
    Serial.println(n4);
  }
}

void saveLDR(){
  if(eeAdr <= 4000){
    EEPROM.put(eeAdr, URval);
    eeAdr += sizeof(double);
    EEPROM.put(eeAdr, ULval);
    eeAdr += sizeof(double);
    EEPROM.put(eeAdr, DRval);
    eeAdr += sizeof(double);
    EEPROM.put(eeAdr, DLval);
    eeAdr += sizeof(double);
  }
}

bool binThresh(double v){
  return v>LDRTHRESHOLD;
}

void printLDR(){
  if(printMode){
    Serial.print(URlit);
    Serial.print(", ");
    Serial.print(ULlit);
    Serial.print(", ");
    Serial.print(DRlit);
    Serial.print(", ");
    Serial.println(DLlit);
  }
  else{
    Serial.print(URval);
    Serial.print(", ");
    Serial.print(ULval);
    Serial.print(", ");
    Serial.print(DRval);
    Serial.print(", ");
    Serial.println(DLval);
  }
}

void hysteresis(){
  bool winSwitch = true;
  for(int i=HYSTWIDTH-1; i>0; i--){ // Add new val at beginning of window
    hystWindowUR[i] = hystWindowUR[i-1];
  }
  hystWindowUR[0] = binThresh(URval);
  
  for(int i=0; i<HYSTWIDTH; i++){   // If a single value is == old, don't switch
    if(hystWindowUR[i] == URlit)
      winSwitch = false;
  }
  
  if(winSwitch)
    URlit = !URlit;

  winSwitch = true;
  for(int i=HYSTWIDTH-1; i>0; i--){ // Add new val at beginning of window
    hystWindowUL[i] = hystWindowUL[i-1];
  }
  hystWindowUL[0] = binThresh(ULval);
  
  for(int i=0; i<HYSTWIDTH; i++){   // If a single value is == old, don't switch
    if(hystWindowUL[i] == ULlit)
      winSwitch = false;
  }
  
  if(winSwitch)
    ULlit = !ULlit;

  winSwitch = true;
  for(int i=HYSTWIDTH-1; i>0; i--){ // Add new val at beginning of window
    hystWindowDR[i] = hystWindowDR[i-1];
  }
  hystWindowDR[0] = binThresh(DRval);
  
  for(int i=0; i<HYSTWIDTH; i++){   // If a single value is == old, don't switch
    if(hystWindowDR[i] == DRlit)
      winSwitch = false;
  }
  
  if(winSwitch)
    DRlit = !DRlit;

  winSwitch = true;
  for(int i=HYSTWIDTH-1; i>0; i--){ // Add new val at beginning of window
    hystWindowDL[i] = hystWindowDL[i-1];
  }
  hystWindowDL[0] = binThresh(DLval);
  
  for(int i=0; i<HYSTWIDTH; i++){   // If a single value is == old, don't switch
    if(hystWindowDL[i] == DLlit)
      winSwitch = false;
  }
  
  if(winSwitch)
    DLlit = !DLlit;
}

void readLDR(){
  URval = analogRead(ldrUR)*0.004883;
  ULval = analogRead(ldrUL)*0.004883;
  DRval = analogRead(ldrDR)*0.004883;
  DLval = analogRead(ldrDL)*0.004883;

  hysteresis();
}

byte pointToSun(){
  Serial.println("Searching...");

  readLDR();
  if(URlit || DRlit)
    analogWrite(driverIn2, 255);
  else
    analogWrite(driverIn1, 255);
  while(true){
    readLDR();
    if((URlit && ULlit) || (DRlit && DLlit)){
      digitalWrite(driverIn1, LOW);
      digitalWrite(driverIn2, LOW);
      break;
    }
    if(!digitalRead(rotEncSw))
      return -1;
  }
  
  Serial.println("Azim found");
  printLDR();
  delay(2000);

  readLDR();
  if(URlit || ULlit)
    analogWrite(driverIn3, 127);
  else
    analogWrite(driverIn4, 127);
  while(true){
    readLDR();
    if((URlit && DRlit) && (ULlit && DLlit)){
      digitalWrite(driverIn3, LOW);
      digitalWrite(driverIn4, LOW);
      break;
    }
    if(!digitalRead(rotEncSw))
      return -1;
  }
  
  Serial.println("Elev found");
  printLDR();
  delay(2000);

  readLDR();
  printLDR();
  if(URlit && ULlit && DRlit && DLlit)
    return 1;
  else
    return 0;
}

void ldrInit(){
  URlit = binThresh(analogRead(ldrUR)*0.004883);
  ULlit = binThresh(analogRead(ldrUL)*0.004883);
  DRlit = binThresh(analogRead(ldrDR)*0.004883);
  DLlit = binThresh(analogRead(ldrDL)*0.004883);
  for(int i=0; i<HYSTWIDTH; i++){
    // here we fill in the hysteresis filter's window of values
    hystWindowUR[i] = URlit;
    hystWindowUL[i] = ULlit;
    hystWindowDR[i] = DRlit;
    hystWindowDL[i] = DLlit;
  }
}
