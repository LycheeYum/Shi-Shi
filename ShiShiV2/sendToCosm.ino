void sendToCosm(){
 
    // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int tf = ((t*9)/5) + 32;
  
  int hum = 0;
  hum = digitalRead(HUM_PIN);
  
  int dni = 0;
  dni = digitalRead(LIGHT_RELAY_PIN);
  
  int vent = 0;
  vent = digitalRead(EXHAUST_PIN);
  
  
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
  Serial.println("Failed to read from DHT");
  } else {
  Serial.print(h);
  Serial.print(",");
  Serial.print(tf);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(dni);
  Serial.print(",");
  Serial.println(vent);
  
}
}
