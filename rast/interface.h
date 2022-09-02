char msgByte, cmdBuff[10];
byte cmd_i = 0;

int16_t eeAdr;

extern const byte ldrUR,ldrUL,ldrDR,ldrDL;
extern volatile int posElev, posAzim;
extern bool printMode;

void up3(), down3(), cw3(), ccw3();
byte pointToSun();
void loadLDR();

void clearArray( char* arr, int len ){
  for( int i = 0; i < len; i++ ){
    arr[i] = '\0';
  }
}

void cmdExe(){
  char num[4];
  switch( cmdBuff[0] ){
    
    case 'h':
      if(cmdBuff[1] == '1')
        digitalWrite(driverIn1, HIGH);
      if(cmdBuff[1] == '2')
        digitalWrite(driverIn2, HIGH);
      if(cmdBuff[1] == '3')
        digitalWrite(driverIn3, HIGH);
      if(cmdBuff[1] == '4')
        digitalWrite(driverIn4, HIGH);
      break;

    case 'l':
      if(cmdBuff[1] == '1')
        digitalWrite(driverIn1, LOW);
      else if(cmdBuff[1] == '2')
        digitalWrite(driverIn2, LOW);
      else if(cmdBuff[1] == '3')
        digitalWrite(driverIn3, LOW);
      else if(cmdBuff[1] == '4')
        digitalWrite(driverIn4, LOW);
      else if(cmdBuff[1] == 'a'){
        digitalWrite(driverIn1, LOW);
        digitalWrite(driverIn2, LOW);
        digitalWrite(driverIn3, LOW);
        digitalWrite(driverIn4, LOW);
      }
      else if(cmdBuff[1] == 'o' && cmdBuff[2] == 'c'){
        pointToSun();
      }
      else loadLDR();
      break;

    case 'p':
      Serial.println(pointToSun());
      break;

    case 'u':
      up3();
      break;

    case 'd':
      down3();
      break;

    case 'c':
      if(cmdBuff[1] == 'w'){
        cw3();
      }else if(cmdBuff[1] == 'c' && cmdBuff[2] == 'w'){
        ccw3();
      }
      break;

    case 'v':
      printMode = !printMode;
      break;
      
    default:
      Serial.println("Command unrecognized. No action invoked.");
      break;
  }
}

void getCmd(){
  while( Serial.available() > 0 ){
    msgByte = Serial.read();
    if( msgByte != '\n' ){
      cmdBuff[cmd_i] = msgByte;
      cmd_i++;
    }
    else{
      cmdExe();
      clearArray(cmdBuff, 10);
      cmd_i = 0;
    }
  }
}
