void loop(){    
  Alarm.delay(1000);

  t.update();

     
//displays clock through serial monitor // when not debugging
  digitalClockDisplay(); 

//This is the call to check the exhaust
  exhaustCheck();
  
//Checks the Temp/Humidity from DHT11
  tempHumCheck();
  
 //Send data to Cosm/Xively
   sendToCosm();
 
//Alarms for sunrise & set  w/examples
Alarm.alarmRepeat(lightOnHr,lightOnMin,0,lightOn);  // Sunrise
Alarm.alarmRepeat(lightOffHr,lightOffMin,0,lightOff); // Sunset
Alarm.timerRepeat(12,0,5,calculateAlarmTimes); // Get new rise and set after midnight

//Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
//Alarm.timerOnce(10, OnceOnly);             // called once after 10 seconds 


}
