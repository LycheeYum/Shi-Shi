//Water Pump Loop stuff
void pumpOn(){
 // Serial.println("Pump On");//For Debug
  digitalWrite(WATER_PUMP_PIN, LOW);
 // lcd.setCursor ( 6, 1 );
 // lcd.print("W+");
  t.after(600000, pumpOff);
}

void pumpOff(){
 // Serial.println("Pump Off");//For Debug
  //lcd.setCursor ( 6, 1 );
 // lcd.print("W-");
  digitalWrite(WATER_PUMP_PIN, HIGH);
}
