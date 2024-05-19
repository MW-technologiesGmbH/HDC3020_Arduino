/*
Example script reading measurement values from the hdc3020 sensor via I2C interface.

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
#define REQUEST_INTERVAL_MS 2000 //Time interval between measurements in ms
#define CSV_DELIMITER ','

float temperature = 0, humidity = 0, dewpoint = 0;
unsigned char identification[6];
char errorString[200];
uint8_t errorcode;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
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
  Serial.print("temperature");
  Serial.print(CSV_DELIMITER);
  Serial.print(" relative humidity");
  Serial.print(CSV_DELIMITER);
  Serial.println(" dewpoint");
  delay(1000);
}

void loop()
{
  errorcode = hdc.singleShotTempHum(temperature, humidity,1);
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
    Serial.print(" °C");
    Serial.println(CSV_DELIMITER);
  }
  delay(REQUEST_INTERVAL_MS);
}
