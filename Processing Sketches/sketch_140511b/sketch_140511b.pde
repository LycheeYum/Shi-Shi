// import libraries
import processing.serial.*;
import eeml.*;
// create a DataOut stream
DataOut dOut;
// establish serial communications with the Arduino
Serial port;
// retreive data from Arduino
String serialData;
// start the program
void setup()
{
  // set the size of the window
  size(60,60);
  // set the Arduino port
  // change COM13 to the correct port
  port = new Serial(this, "COM4",9600);
  // set the frame rate
  frameRate(300);
  // white background
  background(255);
  
  // output data to COSM
  // change the url and insert the API key given to you accordingly
  dOut = new DataOut(this, "https://api.cosm.com/v2/feeds/123456.xml", "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  
  // create data streams and add tags
  dOut.addData(0,"Potentiometer,Arduino,Processing");
  dOut.addData(1, "Random");
}

// draw the program
void draw()
{
  // white background
  background(255);
}
// when the computer receives data from the Arduino
void serialEvent(Serial port)
{
  // get status messages from COSM
  serialData = port.readStringUntil(13);
  
  // if Arduino sent data...
  if(serialData!= null)
  {
    // data is not delimited, but this removes the return character
    float value[] = float(split(serialData, (char)13));
    // show what data was recieved
    println("Data: "+value[0]);
    // we are now ready to send data to COSM
    println("ready to POST: ");
    // update the datastream
    dOut.update(0, value[0]);
    dOut.update(1, random(1000));
    // recieve response from COSM
    int response = dOut.updatePachube();
    // display response code
    // 200 means received properly
    println("Response: "+response);
  }
}
