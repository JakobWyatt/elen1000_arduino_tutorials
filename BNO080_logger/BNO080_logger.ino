#include <SD.h>
#include <SPI.h>
#include <math.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h"

#define DEBUG

//create IMU
BNO080 myIMU;
const int chipSelect = 53;
String fileName = "";

// create quat & eular struct to make data easier
struct Euler{ float yaw; float pitch; float roll; };
struct Quat{ float i; float j; float k; float real; };

Quat myQuat; 
Euler eul;

void setup()
{
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
 
  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz
  myIMU.begin(); //attach IMU
  myIMU.enableRotationVector(10); //Send data update every 10ms
  myIMU.enableAccelerometer(10);
  while (!initSDCard()) {
    delay(1000);
  }
}


void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable())
  {
    //Get IMU Data
    myQuat.i = myIMU.getQuatI();
    myQuat.j = myIMU.getQuatJ();
    myQuat.k = myIMU.getQuatK();
    myQuat.real = myIMU.getQuatReal();
    eul = getAngles(myQuat);

    float aX = myIMU.getAccelX();
    float aY = myIMU.getAccelY();
    float aZ = myIMU.getAccelZ();
    
    String dataString = "";
    dataString += String(millis());
    dataString += ",";
    dataString += String(eul.yaw);
    dataString += ",";
    dataString += String(eul.pitch);
    dataString += ",";
    dataString += String(eul.roll);
    dataString += ",";
    dataString += String(aX);
    dataString += ",";
    dataString += String(aY);
    dataString += ",";
    dataString += String(aZ);

    log(dataString);

    File dataFile = SD.open(fileName, FILE_WRITE);
    if (!dataFile) {
      log("error opening file");
      while (!initSDCard()) {
        delay(1000);
      }
    } else {
      dataFile.println(dataString);
      dataFile.close();
    }
  }
}


// Return the Euler angle structure from a Quaternion structure 
Euler getAngles(Quat q){

  Euler ret_val;
  float x; float y;

  /* YAW */ 
  x = 2 * ((q.i * q.j) + (q.real * q.k)); 
  y = square(q.real) - square(q.k) - square(q.j) + square(q.i); 
  ret_val.yaw = degrees(atan2(y, x));

  /* PITCH */ 
  ret_val.pitch = degrees(asin(-2 * (q.i * q.k - q.j * q.real)));

  /* ROLL */ 
  x = 2 * ((q.j * q.k) + (q.i * q.real)); 
  y = square(q.real) + square(q.k) - square(q.j) - square(q.i); 
  ret_val.roll = degrees(atan2(y , x));

  return ret_val;
}

bool initSDCard() {
  //Set up SD card
  log("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    log("Card failed, or not present");
    return false;
  }
  log("card initialized.");

  //Find file to use
  int counter = 1;
  while (SD.exists(String(counter) + ".csv")) {
    ++counter;
  }
  fileName = String(counter) + ".csv";
  log("Writing to file: " + fileName);

  //Write file header
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (!dataFile) {
    log("error opening file");
    return false;
  }
  dataFile.println("Time (ms),yaw (degrees),pitch (degrees),roll (degrees),accel X (m/s^2),accel Y (m/s^2),accel Z (m/s^2)");
  dataFile.close();

  return true;
}

void log(String s) {
  #ifdef DEBUG
  Serial.println(s);
  #endif
}
