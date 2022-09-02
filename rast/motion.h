extern const byte driverIn1;
extern const byte driverIn2;
extern const byte driverIn3;
extern const byte driverIn4;

const byte pots[] = {127,133,146,165,190,215,235,248,255,255,255,255,255,248,235,215,190,165,146,133,127};
byte iPotsElev = 0, iPotsAzim = 0;

bool posElevChanged = false, posAzimChanged = false;
bool movingElev = false, movingAzim = false;

unsigned long lastAzimMoveTime, lastElevMoveTime;

bool up, cw;

void up3(){
  if(movingElev == false){
    movingElev = true;
    up = true;
  }
}

void down3(){
  if(movingElev == false){
    movingElev = true;
    up = false;
  }
}

void cw3(){
  if(movingAzim == false){
    movingAzim = true;
    cw = true;
  }
}

void ccw3(){
  if(movingAzim == false){
    movingAzim = true;
    cw = false;
  }
}

void actOnElev(){
  if(movingElev && iPotsElev == 0){
    if(up)  analogWrite(driverIn1, pots[iPotsElev]);
    else    analogWrite(driverIn2, pots[iPotsElev]);
    lastElevMoveTime = millis();
    iPotsElev++;
  }

  if(movingElev && posElevChanged){
    if(up)  analogWrite(driverIn1, pots[iPotsElev]);
    else    analogWrite(driverIn2, pots[iPotsElev]);
    lastElevMoveTime = millis();
    iPotsElev++;
    posElevChanged = false;
  }

  if(iPotsElev == 21){
    iPotsElev = 0;
    movingElev = false;
    digitalWrite(driverIn1, LOW);
    digitalWrite(driverIn2, LOW);
  }

  if(movingElev && (millis() - lastElevMoveTime > ELEVTIMEOUTMS)){
    iPotsElev = 0;
    movingElev = false;
    digitalWrite(driverIn1, LOW);
    digitalWrite(driverIn2, LOW);
  }
}

void actOnAzim(){
  if(movingAzim && iPotsAzim == 0){
    if(cw)  analogWrite(driverIn4, pots[iPotsAzim]);
    else    analogWrite(driverIn3, pots[iPotsAzim]);
    lastAzimMoveTime = millis();
    iPotsAzim++;
  }

  if(movingAzim && posAzimChanged){
    if(cw)  analogWrite(driverIn4, pots[iPotsAzim]);
    else    analogWrite(driverIn3, pots[iPotsAzim]);
    lastAzimMoveTime = millis();
    iPotsAzim++;
    posAzimChanged = false;
  }

  if(iPotsAzim == 21){
    iPotsAzim= 0;
    movingAzim = false;
    digitalWrite(driverIn4, LOW);
    digitalWrite(driverIn3, LOW);
  }

  if(movingAzim && (millis() - lastAzimMoveTime > AZIMTIMEOUTMS)){
    iPotsAzim= 0;
    movingAzim = false;
    digitalWrite(driverIn4, LOW);
    digitalWrite(driverIn3, LOW);
  }
}
