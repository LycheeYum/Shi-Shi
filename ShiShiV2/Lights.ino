// light methods

void lightOn(){
  digitalWrite(LIGHT_RELAY_PIN, LOW);
 // digitalWrite(lightLED, HIGH);
  //Serial.println("Light should be on");//For Debug
  lcd.setCursor ( 0, 1 );
  lcd.print("L+");
}

void lightOff(){
  digitalWrite(LIGHT_RELAY_PIN, HIGH);
//  digitalWrite(lightLED, LOW);
 // Serial.println("Light should be off");//For Debug
  lcd.setCursor ( 0, 1 );
  lcd.print("L-");
}
// end light methods


