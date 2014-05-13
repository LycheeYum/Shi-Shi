void exhaustCheck(){
 
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   float tf = ((t*9)/5) + 32;
  
   if (h > 60 || tf > 82)    //If humidity is above 60% or if the temp is above 82
    {
      digitalWrite(EXHAUST_PIN, LOW);   //Run the fan
    //  digitalWrite(humLED, HIGH);
      //Serial.println("Venting");//For Debug
      //lcd.setCursor ( 3, 1 );  //save for later finetuning
      //lcd.print("H+");
    }
    else if (h < 61 || tf < 79) // add a little hysteresis to prevent rapid cycling due to noise
    {
       digitalWrite(EXHAUST_PIN, HIGH);  //Stop fan when humidity 71% or higher
   //    digitalWrite(humLED, LOW);
       //Serial.println("venting Off");//For Debug
      //lcd.setCursor ( 3, 1 );
      //lcd.print("H-");
    } 
}
