#include <SD.h>
#include <SPI.h>
#include <math.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h"

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
  Serial.begin(9600);
 
  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz
  myIMU.begin(); //attach IMU
  myIMU.enableRotationVector(10); //Send data update every 10ms
  myIMU.enableAccelerometer(10);
  //myIMU.enableGyro(10); Gyro makes the rotation vector update VERY SLOWLY

  //Set up SD card
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  //Find file to use
  int counter = 1;
  while (SD.exists(String(counter) + ".csv")) {
    ++counter;
  }
  fileName = String(counter) + ".csv";
  Serial.println("Writing to file: " + fileName);

  //Write file header
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (!dataFile) {
    Serial.println("error opening file");
  } else {
    dataFile.println("Time (ms),yaw (degrees),pitch (degrees),roll (degrees),accel X (m/s^2),accel Y (m/s^2),accel Z (m/s^2)");
    dataFile.close();
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
    float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();
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

    Serial.println(dataString);

    File dataFile = SD.open(fileName, FILE_WRITE);
    if (!dataFile) {
      Serial.println("error opening file");
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
