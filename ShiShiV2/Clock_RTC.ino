void digitalClockDisplay(){
  /* digital clock display of the time  //This section // For Debug
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
  */
  // digital clock display of the time on LCD
  lcd.setCursor ( 0, 0 );
  lcd.print(hour() );
  lcd.print(":");
  lcd.print(minute());
  lcd.print(" ");  
    }
/*
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
    if(digits < 10)
    Serial.print('0');
      Serial.print(digits);

}
*/
