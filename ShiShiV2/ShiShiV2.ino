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
#include "RTClib.h"
#include <TimeLord.h>
#include <Time.h>  
#include <TimeAlarms.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include "Timer.h"
#include "DHT.h"
#include <LCD.h>
#include <LiquidCrystal_I2C.h>


Timer t;




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
 
 

// Time variables
TimeLord LightTimeLord;
byte   lightOnHr, lightOnMin, lightOffHr, lightOffMin;


//Pins that are used to switch on components
int WATER_PUMP_PIN = 2; //unneeded with Waterfarm
int LIGHT_RELAY_PIN = 3;
int HUM_PIN = 4;
int EXHAUST_PIN = 5;

//LED Indocator Pins
//int waterLED = 6;
//int lightLED = 7;
//int humLED = 8;
//int led8 = 9;


//Serial LCD setup
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
  
// Switch on the LCD backlight
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
  //   Serial.println("RTC has set the system time");      
     
// TimeLord Stuff  - Do not Touch
  LightTimeLord.TimeZone(TIMEZONE * 60);
  LightTimeLord.Position(LATITUDE, LONGITUDE);
  
//This is the call that sets the sunrise and sunset times
  calculateAlarmTimes(); 
  

  
//water Pump stuff - unsused with WaterFarm
//Alarm.timerRepeat(10800, pumpOn); //Cycle the water pump once every 3 hours

//Alarms for sunrise & set  w/examples
Alarm.alarmRepeat(lightOnHr,lightOnMin,0,lightOn);  // Sunrise
Alarm.alarmRepeat(lightOffHr,lightOffMin,0,lightOff); // Sunset
Alarm.timerRepeat(12,0,0,calculateAlarmTimes); // Get new rise and set after midnight

//Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
//Alarm.timerOnce(10, OnceOnly);             // called once after 10 seconds 




dht.begin();

}
