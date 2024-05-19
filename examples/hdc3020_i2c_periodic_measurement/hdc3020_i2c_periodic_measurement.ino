/*
Example script reading measurement values from the HDC3020 sensor via I2C interface.

Copyright 2024 MW Technologies

Disclaimer:
This application example is non-binding and does not claim to be complete with regard
to configuration and equipment as well as all eventualities. The application example
is intended to provide assistance with the HDC3020 sensor module design-in and is provided "as is".
You yourself are responsible for the proper operation of the products described.
This application example does not release you from the obligation to handle the product safely
during application, installation, operation and maintenance. By using this application example,
you acknowledge that we cannot be held liable for any damage beyond the liability regulations
described.
*/

#include <hdc3020I2c.h>
#include <Wire.h>
hdc3020I2c hdc(0x44);
#define REQUEST_INTERVAL_MS 800 //Time interval between measurements in ms
#define CSV_DELIMITER ','

float temperature, humidity, dewpoint, measurementTime;
unsigned char identification[8];
char errorString[200];
uint8_t errorcode;
int counter = 0;

void setup()
{
  Serial.begin(9600); //Start Serial communication
  Wire.begin();       //initialize I2C peripheral (SDA..A4, SCL..A5)
  delay(1000);
  hdc.startPeriodicMeasurement(1, 0); // measurementPerSeconds: 0 = 0.5 mps, 1 = 1mps, 2 = 2mps, 3 = 4mps, 4 = 10mps;  Mode : 0 = low noise, 1 , 2 , 3 = lowest power
  errorcode = hdc.readIdentification(identification);
  if (errorcode != 0)
  {
    hdc.getErrorString(errorcode, errorString);
    Serial.println(errorString);
  }
  else
  {
    Serial.print("Identificationnumber: ");
    for (int i = 0; i < 8; i++)
    {
      Serial.print(identification[i] < 16 ? "0" : "");
      Serial.print(identification[i], HEX);
    }
  }
  Serial.println("");
  delay(2000);
}

void loop()
{
  errorcode = hdc.getPeriodicMeasurementTempHum(temperature, humidity);
  if (errorcode != 0)
    {
      hdc.getErrorString(errorcode, errorString);
      Serial.println(errorString);
    }
    else
    {
      Serial.print(temperature);
      Serial.print(" °C");
      Serial.print(CSV_DELIMITER);
      Serial.print(humidity);
      Serial.print(" %RH");
      Serial.print(CSV_DELIMITER);
    }
    errorcode = hdc.getDewpoint(temperature, humidity, dewpoint);
    if (errorcode != 0)
    {
      hdc.getErrorString(errorcode, errorString);
      Serial.println(errorString);
    }
    else
    {
      Serial.print(dewpoint);
      Serial.println(" °C");
    }
  counter++; 
  if(counter == 100)
  {
    float max_temp = 0;
    float min_temp = 0;
    float max_hum = 0;
    float min_hum = 0;
    hdc.getPeriodicMeasurementMaxTemp(max_temp);
    hdc.getPeriodicMeasurementMinTemp(min_temp);
    hdc.getPeriodicMeasurementMaxHum(max_hum);
    hdc.getPeriodicMeasurementMinHum(min_hum);
    Serial.print(max_temp);
    Serial.print(min_temp);
    Serial.print(max_hum);
    Serial.println(min_hum);
    hdc.endPeriodicMeasurement();
  }
  delay(REQUEST_INTERVAL_MS); //time until the next loop start
}
