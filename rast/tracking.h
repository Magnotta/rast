// The line below should be the definition of trackAzims[]
const double trackAzims[] = {6.3515,-7.054,-7.6673,-8.1934,-8.6423,-9.0259,-9.3557,-9.6418,-9.8918,-10.113,-10.31,-10.487,-10.648,-10.795,-10.93,-11.056,-11.173,-11.284,-11.389,-11.488,-11.584,-11.676,-11.765,-11.852,-11.938,-12.021,-12.105,-12.187,-12.27,-12.353,-12.436,-12.521,-12.608,-12.697,-12.789,-12.884,-12.983,-13.087,-13.197,-13.314,-13.438,-13.572,-13.718,-13.876,-14.051,-14.246,-14.463,-14.71,-14.991,-15.315,-15.691,-16.13,-16.646,-17.248,-17.94,0.0};
const double trackElevs[] = {7.1968,7.4098,7.6237,7.839,8.0554,8.2732,8.4929,8.7145,8.939,9.1662,9.3976,9.6337,9.8765,10.128,10.391,10.674,10.988,11.371,-13.396,-13.711,-13.993,-14.257,-14.508,-14.751,-14.987,-15.219,-15.446,-15.67,-15.892,-16.112,-16.33,-16.546,-16.761,-16.975,-17.188,0.0};
// The line above should be the definition of trackElevs[]


tmElements_t tm;

byte trackElevIndex, trackAzimIndex;

double curTime;

void trackInit(DS1307RTC RTC){
  RTC.read(tm);

  curTime = tm.Hour + tm.Minute/60.0 + tm.Second/3600.0;
  
  while(curTime > abs(trackElevs[trackElevIndex])){
    if(trackElevs[trackElevIndex] == 0.0)
      break;
    trackElevIndex++;
  }

  while(curTime > abs(trackAzims[trackAzimIndex])){
    if(trackAzims[trackAzimIndex] == 0.0)
      break;
    trackAzimIndex++;
  }

  Serial.println(curTime);
  Serial.println(trackElevs[trackElevIndex]);
  Serial.println(trackAzims[trackAzimIndex]);
}
