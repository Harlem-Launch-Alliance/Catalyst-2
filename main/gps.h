// Test code for Ultimate GPS Using Hardware Serial.
// Need to remove the GPS.lastNMEA() and see if it still works, still get compilation error

#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial2

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

void setupGPS()
{
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

gpsReading getGPS()
{
  int t1 = micros();
  while(micros - t1 < 5000){
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPS.newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      c = GPS.lastNMEA(); //this gives an error but doesn't seem to be an issue
      if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        return; // we can fail to parse a sentence in which case we should just wait for another
    }
  }

  gpsReading sample;

  if(GPS.fix){
    sample.latitude = GPS.latitudeDegrees;
    sample.longitude = GPS.longitudeDegrees;
  }
  sample.time = millis();
  return sample;
}
