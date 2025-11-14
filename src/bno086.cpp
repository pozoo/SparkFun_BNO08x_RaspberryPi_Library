/*
  Using the BNO08x IMU

  This example shows how to communicate with the sensor over SPI.

  It requires a few more connections than the qwiic cable.
  In addition to your usual SPI lines (CS/PICO/POCI/SCK),
  This also requires INT and RST. These are crucial for timing when talking to
  this sensor.

  It outputs the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Originally written by Nathan Seidle @ SparkFun Electronics, December 28th, 2017

  Adjusted by Pete Lewis @ SparkFun Electronics, June 2023 to incorporate the
  CEVA Sensor Hub Driver, found here:
  https://github.com/ceva-dsp/sh2

  Also, utilizing code from the Adafruit BNO08x Arduino Library by Bryan Siepert
  for Adafruit Industries. Found here:
  https://github.com/adafruit/Adafruit_BNO08x

  Also, utilizing I2C and SPI read/write functions and code from the Adafruit
  BusIO library found here:
  https://github.com/adafruit/Adafruit_BusIO

  Hardware Connections:
  IoT RedBoard --> BNO08x
  D5  --> CS
  PICO --> SI
  POCI --> SO
  SCK  --> SCK
  A4  --> INT
  A5  --> RST
  3V3  --> 3V3
  GND  --> GND

  BNO08x "mode" pins set for SPI:
  PSO --> 3V3
  PS1 --> 3V3

  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22857
*/

//#include <Wire.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <cmath>

#include "SparkFun_BNO08x_Raspi_Library.h"  // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BNO08x

BNO08x myIMU;

// For SPI, we need some extra pins defined:
// Note, these can be other GPIO if you like.
#define BNO08X_CS   17 // GPIO17 (Pin 11)
#define BNO08X_INT  27 // GPIO27 (Pin 13)
#define BNO08X_RST  22 // GPIO22 (Pin 15)

// Forward declaration
void setReports();

void setup() {
  printf("BNO08x Read setup\n ");

  myIMU.enableDebugging(false); 
  
  if (myIMU.beginSPI(BNO08X_INT, BNO08X_RST) == false) {
    printf("BNO08x not detected. Check your jumpers and the hookup guide. Exiting. \n");
    std::exit(EXIT_FAILURE);
  }
  printf("BNO08x found!\n");

  if (myIMU.wasReset()) {
    printf("sensor was reset 1\n");
  }

  setReports();

  printf("Reading events\n");
  usleep(1000000);
}

// Here is where you define the sensor outputs you want to receive
void setReports() {
  printf("Setting desired report\n");
  if (myIMU.enableRotationVector(10) == true) {
    printf("Rotation vector enabled\n");
    printf("Output in form i, j, k, real, accuracy\n");
  } else {
    printf("Could not enable rotation vector\n");
  }
  usleep(100000); // This delay allows enough time for the BNO086 to accept the new 
              // configuration and clear its reset status
  
  if (myIMU.wasReset()) {
    printf("sensor was reset 2\n");
  }
}

void loop() {
  myIMU.getSensorEvent();
  usleep(10000);

  if (myIMU.wasReset()) {
    printf("sensor was reset\n");
    setReports();
  }

  // Has a new event come in on the Sensor Hub Bus?
  if (myIMU.getSensorEvent() == true) {

    // is it the correct sensor data we want?
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_ROTATION_VECTOR) {

      if (false) {
      float quatI = myIMU.getQuatI();
      float quatJ = myIMU.getQuatJ();
      float quatK = myIMU.getQuatK();
      float quatReal = myIMU.getQuatReal();
      float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();

      printf("%.2f,%.2f,%.2f,%.2f,%.2f\n",
             quatI, quatJ, quatK, quatReal, quatRadianAccuracy);
      }

      float roll = (myIMU.getRoll()) * 180.0 / M_PI; // Convert roll to degrees
      float pitch = (myIMU.getPitch()) * 180.0 / M_PI; // Convert pitch to degrees
      float yaw = (myIMU.getYaw()) * 180.0 / M_PI; // Convert yaw / heading to degrees

      printf("Roll: %.1f, Pitch: %.1f, Yaw: %.1f\n",
             roll, pitch, yaw);
    }
  }
}

int main(int argc, char const *argv[])
{
  setup();
  for (int i = 0; i < 100; i++) {
    loop();
  } 
  myIMU.closeSPI();
  return 0;
}

