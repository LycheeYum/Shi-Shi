

 void tempHumCheck(){
   
   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float tf = ((t*9)/5) + 32;

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
  //Serial.println("Failed to read from DHT");
  } else {
   //Serial.println("       ");
    //Serial.print("Humidity: "); //For Debug
  //Serial.print(h);
    //Serial.println(" %\t");//For Debug
    //Serial.print("Temperature: "); //For Debug
  //Serial.print(t);
    //Serial.println(" *F");//For Debug
    
    lcd.setCursor ( 10, 1 );
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor ( 10, 0 );    
    lcd.print(tf);
    lcd.print("F");
    }
  

   if (h < 45)    //If humidity is below 50%
    {
      digitalWrite(HUM_PIN, LOW);   //Run the humidifier
    //  digitalWrite(humLED, HIGH);
      //Serial.println("Humidifing");//For Debug
      lcd.setCursor ( 3, 1 );
      lcd.print("H+");
    }
    else if (h > 50) // add a little hysteresis to prevent rapid cycling due to noise
    {
       digitalWrite(HUM_PIN, HIGH);  //Stop fan when humidity 71% or higher
   //    digitalWrite(humLED, LOW);
       //Serial.println("Humififier Off");//For Debug
       lcd.setCursor ( 3, 1 );
      lcd.print("H-");
    }
 }
