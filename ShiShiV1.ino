/* 
 *Lychee Shishi.pde
 * This if code is intended to control the growing area for an indoor growing area
 *kugiyv
 *Working Lights 
 *RTC Time retreival
 *Shenzhen Sun Rise/Set Calculations
 *Temp sensor reading
 *
 * A Processing example sketch to automatically send the messages is inclided in the download
 *  NOW WORKING - REMOVED THE TEMP STUFF
 *  8/11/13 add  exhaust fan control
 *  8/14/13 lower humidity level to 55 
 */ 
#include <Arduino.h>
#include <Wire.h>  
#include <TimeLord.h>
#include <Time.h>  
#include <TimeAlarms.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include "Timer.h"
#include "DHT.h"
#include <SD.h>
#include <Wire.h> 
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

Timer t;


// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()




#define DHTPIN 6     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


// Location constants  - Shenzhen, China
const float LATITUDE = 22.608103,
    LONGITUDE = 113.939965;
const int TIMEZONE = 8;
 
 
//SD Card stuff - for maker shield 
const int chipSelect = 10;
File logfile;
void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  while(1);
}

// Time variables
TimeLord LightTimeLord;
byte   lightOnHr, lightOnMin, lightOffHr, lightOffMin;


//Pins that are used to switch on components
int WATER_PUMP_PIN = 2;
int LIGHT_RELAY_PIN = 3;
int HUM_PIN = 4;
int EXHAUST_PIN = 5;

//LED Indocator Pins
//int waterLED = 6;
//int lightLED = 7;
//int humLED = 8;
//int led8 = 9;

#define I2C_ADDR    0x3F  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7


LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);



void setup()  {
  Serial.begin(9600);
  
  lcd.begin (16,2);
  
// Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();                   // go home
  
  
// initialize the digital pins as an output.
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  pinMode(HUM_PIN, OUTPUT);
  pinMode(EXHAUST_PIN, OUTPUT);
  
  
  
  // initialize the digital pins as an output.
//  pinMode(waterLED, OUTPUT);
//  pinMode(lightLED, OUTPUT);
//  pinMode(humLED, OUTPUT);
//  pinMode(led8, OUTPUT);
  
 //Turns off the all Non-Light Pins on start up
digitalWrite(HUM_PIN, HIGH);
digitalWrite(EXHAUST_PIN, HIGH);
digitalWrite(WATER_PUMP_PIN, HIGH); 
 
  
// Gets the time from the RTC
  setSyncProvider(RTC.get);   
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
    else
     Serial.println("RTC has set the system time");      
     
// TimeLord Stuff  - Do not Touch
  LightTimeLord.TimeZone(TIMEZONE * 60);
  LightTimeLord.Position(LATITUDE, LONGITUDE);
  
//This is the call that sets the sunrise and sunset times
  calculateAlarmTimes(); 
  

  
//water Pump stuff
Alarm.timerRepeat(10800, pumpOn); //Cycle the water pump once every 3 hours

//Alarms for sunrise & set  w/examples
Alarm.alarmRepeat(lightOnHr,lightOnMin,0,lightOn);  // Sunrise
Alarm.alarmRepeat(lightOffHr,lightOffMin,0,lightOff); // Sunset
Alarm.timerRepeat(12,0,0,calculateAlarmTimes); // Get new rise and set after midnight

//Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
//Alarm.timerOnce(10, OnceOnly);             // called once after 10 seconds 



dht.begin();

/*
 // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("couldnt create file");
  }
  
  Serial.print("Logging to: ");
  Serial.println(filename);

  // connect to RTC
  Wire.begin();  
 if(timeStatus()!= timeSet){
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL
  }
  
  logfile.println("millis,stamp,datetime,light,temp,vcc");    
#if ECHO_TO_SERIAL
  Serial.println("millis,stamp,datetime,light,temp,vcc");
#endif //ECHO_TO_SERIAL

*/



}


void loop(){    
  Alarm.delay(1000);


  
  
float Wcd = 180000 - millis()/1000;
int LWcd = Wcd/1000.;
lcd.setCursor ( 6, 1 );
lcd.print(LWcd);
 
 
  t.update();

     
//displays clock through serial monitor // when not debugging
  digitalClockDisplay(); 

 //This is the call to check the exhaust
    exhaustCheck();
 
//Alarms for sunrise & set  w/examples
Alarm.alarmRepeat(lightOnHr,lightOnMin,0,lightOn);  // Sunrise
Alarm.alarmRepeat(lightOffHr,lightOffMin,0,lightOff); // Sunset
Alarm.timerRepeat(12,0,5,calculateAlarmTimes); // Get new rise and set after midnight

//Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
//Alarm.timerOnce(10, OnceOnly);             // called once after 10 seconds 


}


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
  
  
    Serial.print("sunrise: ");
    Serial.print(timeLordSunRise[2]);
    Serial.print(":");
    Serial.println(timeLordSunRise[1]);
    Serial.print("sunset: ");
    Serial.print(timeLordSunSet[2]);
    Serial.print(":");
    Serial.println(timeLordSunSet[1]);
  
  
  lightOnHr = timeLordSunRise[2];
  lightOnMin = timeLordSunRise[1];
  lightOffHr = timeLordSunSet[2];
  lightOffMin = timeLordSunSet[1];
 
  
   // If the light is supposed to be on turn it on
  if(hour() < lightOffHr && hour() >=lightOnHr)
  {
  Serial.println("RECOVER");
  lightOn();
  }
  else
  {
  lightOff();
  Serial.println("Light Checked");
  }
 
  }


// light methods

void lightOn(){
  digitalWrite(LIGHT_RELAY_PIN, LOW);
 // digitalWrite(lightLED, HIGH);
  Serial.println("Light should be on");
  lcd.setCursor ( 0, 1 );
  lcd.print("L+");
}

void lightOff(){
  digitalWrite(LIGHT_RELAY_PIN, HIGH);
//  digitalWrite(lightLED, LOW);
  Serial.println("Light should be off");
  lcd.setCursor ( 0, 1 );
  lcd.print("L-");
}
// end light methods

 
//void Repeats(){
//  Serial.println("15 second timer");    
//  digitalWrite(WATER_PUMP_PIN, LOW);
//  digitalWrite(LIGHT_RELAY_PIN, LOW);
//  digitalWrite(HUM_PIN, LOW);
// digitalWrite(led4, LOW); 
//}

//Water Pump Loop stuff
void pumpOn(){
  Serial.println("Pump On");
  digitalWrite(WATER_PUMP_PIN, LOW);
 // lcd.setCursor ( 6, 1 );
 // lcd.print("W+");
  t.after(600000, pumpOff);
}

void pumpOff(){
  Serial.println("Pump Off");
  //lcd.setCursor ( 6, 1 );
 // lcd.print("W-");
  digitalWrite(WATER_PUMP_PIN, HIGH);
}

void digitalClockDisplay(){
  // digital clock display of the time
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
  
  // digital clock display of the time on LCD
  lcd.setCursor ( 0, 0 );
  lcd.print(hour() );
  lcd.print(":");
  lcd.print(minute());
  lcd.print(" ");  
    }

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
    if(digits < 10)
    Serial.print('0');
      Serial.print(digits);

  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float tf = ((t*9)/5) + 32;

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
     Serial.println("       ");
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    
    lcd.setCursor ( 10, 1 );
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor ( 10, 0 );    
    lcd.print(tf);
    lcd.print("F");
    }
  

   if (h < 55)    //If humidity is below 70%
    {
      digitalWrite(HUM_PIN, LOW);   //Run the fan
    //  digitalWrite(humLED, HIGH);
      Serial.println("Humidifing");
      lcd.setCursor ( 3, 1 );
      lcd.print("H+");
    }
    else if (h > 56) // add a little hysteresis to prevent rapid cycling due to noise
    {
       digitalWrite(HUM_PIN, HIGH);  //Stop fan when humidity 71% or higher
   //    digitalWrite(humLED, LOW);
       Serial.println("Humififier Off");
       lcd.setCursor ( 3, 1 );
      lcd.print("H-");
    }
  
}

void exhaustCheck(){
 
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   float tf = ((t*9)/5) + 32;
  
   if (h > 77 || tf > 82)    //If humidity is above 77% or if the temp is above 82
    {
      digitalWrite(EXHAUST_PIN, LOW);   //Run the fan
    //  digitalWrite(humLED, HIGH);
      Serial.println("Venting");
      //lcd.setCursor ( 3, 1 );  //save for later finetuning
      //lcd.print("H+");
    }
    else if (h < 75 || tf < 79) // add a little hysteresis to prevent rapid cycling due to noise
    {
       digitalWrite(EXHAUST_PIN, HIGH);  //Stop fan when humidity 71% or higher
   //    digitalWrite(humLED, LOW);
       Serial.println("venting Off");
      //lcd.setCursor ( 3, 1 );
      //lcd.print("H-");
    } 
}
  



