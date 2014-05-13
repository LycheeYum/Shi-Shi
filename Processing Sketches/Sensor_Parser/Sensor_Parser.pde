/**
 * Data from multiple sensors / Processing 
 * by BARRAGAN <http://barraganstudio.com>
 * based on Tom Igoe's example from Making Things Talk book
 *
 * read serial data until a linefeed character is found
 * data are values comma separated. Split the data and convert it
 * into numbers in an array for firther use.
*/

import cosm.*;
import processing.serial.*;

DataOut feed;

String apiKey = "biqObGk2bQ2vXOTPiJP-ZDmD6CySAKxzUGdGaGNpdHlNTT0g";
String feedId = "128144";

Serial myPort;       
int linefeed = 10;   // Linefeed in ASCII
int numSensors = 5;  // we will be expecting for reading data from four sensors
int sensors[];       // array to read the 4 values


void setup() {
  size(200, 200);  
  // List all the available serial ports in the output pane. 
  // You will need to choose the port that the Wiring board is 
  // connected to from this list. The first port in the list is 
  // port #0 and the third port in the list is port #2. 
  println(Serial.list()); 
  
  myPort = new Serial(this, Serial.list()[1], 9600);
  // read bytes into a buffer until you get a linefeed (ASCII 10):
  myPort.bufferUntil(linefeed);
  
  feed = new DataOut(this, apiKey, feedId);  //intantiate feed
  feed.setVerbose(false);  //optional debug info
  
}

void draw() {
  

    // now do something with the values read sensors[0] .. sensors[3]

  }


void serialEvent(Serial myPort) {

  // read the serial buffer:
  String myString = myPort.readStringUntil(linefeed);

  // if you got any bytes other than the linefeed:
  if (myString != null) {

    myString = trim(myString);

    // split the string at the commas
    // and convert the sections into integers:





    sensors = int(split(myString, ','));

    // print out the values you got:
    
    for (int sensorNum = 0; sensorNum < sensors.length; sensorNum++) {
      print("Sensor " + sensorNum + ": " + sensors[sensorNum] + "\t");
    }
    
    // add a linefeed after all the sensor values are printed:
   println();
    feed.setStream("Humidity", sensors[0]); //send request (datastream id, new value)
    feed.setStream("Temperature", sensors[1]);
    feed.setStream("Humidifer", sensors[2]);
    feed.setStream("Light", sensors[3]);
    feed.setStream("vent", sensors[4]);
   
  }
}

