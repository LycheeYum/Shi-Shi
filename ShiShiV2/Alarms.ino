/**
 * Calculates alarm times used for scheduling and init state.  Also checks if the light shoud be on or off.
 */
void calculateAlarmTimes(){
  int nowHour = hour(),
   nowMinute = minute(),
   nowDay = day(),
   nowMonth = month(),
   nowYear = year();


  byte timeLordSunRise[]  = {0, 0, 0, nowDay, nowMonth, nowYear};
  byte timeLordSunSet[]  = {0, 0, 0, nowDay, nowMonth, nowYear};

  LightTimeLord.SunRise(timeLordSunRise);
  LightTimeLord.SunSet(timeLordSunSet);   
  
  /*  This section // for Debug
    Serial.print("sunrise: ");
    Serial.print(timeLordSunRise[2]);
    Serial.print(":");
    Serial.println(timeLordSunRise[1]);
    Serial.print("sunset: ");
    Serial.print(timeLordSunSet[2]);
    Serial.print(":");
    Serial.println(timeLordSunSet[1]);
  */
  
  lightOnHr = timeLordSunRise[2];
  lightOnMin = timeLordSunRise[1];
  lightOffHr = timeLordSunSet[2];
  lightOffMin = timeLordSunSet[1];
 
  
   // If the light is supposed to be on turn it on
  if(hour() < lightOffHr && hour() >=lightOnHr)
  {
  //Serial.println("RECOVER"); //For Debug
  lightOn();
  }
  else
  {
  lightOff();
  //Serial.println("Light Checked"); //For Debug
  }
 
  }
