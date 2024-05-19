/***************************************************************************/
/* sourcefile for "hdc3020.h" module */
/***************************************************************************/
/*
Read functions for measurement values of the hdc3020 Sensor via I2C interface.

Copyright 2024 MW Technologies

Disclaimer:
This application example is non-binding and does not claim to be complete with regard
to configuration and equipment as well as all eventualities. The application example
is intended to provide assistance with the hdc3020 sensor module design-in and is provided "as is".
You yourself are responsible for the proper operation of the products described.
This application example does not release you from the obligation to handle the product safely
during application, installation, operation and maintenance. By using this application example,
you acknowledge that we cannot be held liable for any damage beyond the liability regulations
described.

We reserve the right to make changes to this application example at any time without notice.
In case of discrepancies between the suggestions in this application example and other MW Technologies
publications, such as catalogues, the content of the other documentation takes precedence.
We assume no liability for the information contained in this document.
*/

// Includes
//-----------------------------------------------------------------------------
#include "hdc3020I2c.h"
#include <Arduino.h>
#include "Wire.h"
#include "math.h"


enum Errorcode
{
    OKAY = 0,
    ERR_CKSUM = 1,
    CON_OK = 2,
    NO_SENSOR_ON_ADDR = 3,
    NO_SENSOR = 4,
    SEC_TO_HIGH = 5,
    CUR_NOT_IN_SPEC = 6,
    MEAS_RES_WRONG = 7
};


hdc3020I2c::hdc3020I2c(void)
{
}

hdc3020I2c::hdc3020I2c(unsigned char i2cAdress)
{
  address = i2cAdress;
}

uint8_t hdc3020I2c::singleShotTempHum(float &temperature, float &humidity, int Mode) // Mode : 0 = low noise, 1 , 2 , 3 = lowest power; 
{
  unsigned char i2cResponse[6] = {};
  unsigned char Command[2];
  Command[0] = {(HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_0 >> 8)};
  switch (Mode)
  {
    case 0:
      Command[1] = {(HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_0 & 0xFF)};
      break;
    case 1:
      Command[1] = {(HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_1 & 0xFF)};
      break;
    case 2:
      Command[1] = {(HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_2 & 0xFF)};
      break;
    case 3:
      Command[1] = {(HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_3 & 0xFF)};
      break;
    }
  wireWrite(Command, 1, true);
  delay(2);
  wireRead(i2cResponse, 6);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1) && i2cResponse[5] == calcCrc8(i2cResponse, 3, 4))
  {
    temperature = -45 + (175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1])/(65536 - 1));
    humidity = 100 * ((float)(i2cResponse[3]) * 256 + i2cResponse[4]) / (65536 - 1);
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::getPeriodicMeasurementTempHum(float &temperature, float &humidity)
{
  unsigned char i2cResponse[6] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_PERIODIC_MEASUREMENT >> 8), (HDC3020_COMMAND_READ_PERIODIC_MEASUREMENT & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 6);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1) && i2cResponse[5] == calcCrc8(i2cResponse, 3, 4))
  {
    temperature = -45 + (175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1])/(65536 - 1));
    humidity = 100 * ((float)(i2cResponse[3]) * 256 + i2cResponse[4]) / (65536 - 1);
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::getDewpoint(float temperature, float humidity ,float &dewpoint)
{
  if(temperature > 0)
  {
    dewpoint = 243.12 * ((17.62 * temperature) / (243.12 + temperature) + log(humidity / 100))/((17.62 * 243.12) / (243.12 + temperature) - log(humidity / 100));
  }
  else
  {
    dewpoint = 272.62 * ((((22.46*temperature) / (272.62 + temperature)) + log(humidity / 100)) / (((22.46 * 272.62) / (272.62 + temperature)) - log(humidity / 100)));
  }
}

uint8_t hdc3020I2c::getPeriodicMeasurementMinTemp(float &temperature) // -45 - 130°C 
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_MINIMUM_MEASUREMENT_T >> 8), (HDC3020_COMMAND_READ_MINIMUM_MEASUREMENT_T & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    temperature = -45 + (175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1])/(65536 - 1));
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::getPeriodicMeasurementMaxTemp(float &temperature) // -45 - 130°C 
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_MAXIMUM_MEASUREMENT_T >> 8), (HDC3020_COMMAND_READ_MAXIMUM_MEASUREMENT_T & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    temperature = -45 + (175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1])/(65536 - 1));
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::getPeriodicMeasurementMinHum(float &humidity) // 0 - 100%
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_MINIMUM_MEASUREMENT_RH >> 8), (HDC3020_COMMAND_READ_MINIMUM_MEASUREMENT_RH & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    humidity = 100 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1]) / (65536 - 1);
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::getPeriodicMeasurementMaxHum(float &humidity) // 0 - 100%
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_MAXIMUM_MEASUREMENT_RH >> 8), (HDC3020_COMMAND_READ_MAXIMUM_MEASUREMENT_RH & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    humidity = 100 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1]) / (65536 - 1);
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::changeDefaultDevicePowerOn(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char Command[] = {(HDC3020_COMMAND_CHANGE_DEFAULT_POWER_ON >> 8), (HDC3020_COMMAND_CHANGE_DEFAULT_POWER_ON & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}

uint8_t hdc3020I2c::changeSetLowAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char Command[] = {(HDC3020_COMMAND_CHANGE_SET_LOW_ALERT >> 8), (HDC3020_COMMAND_CHANGE_SET_LOW_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}

uint8_t hdc3020I2c::changeClearLowAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char Command[] = {(HDC3020_COMMAND_CHANGE_CLEAR_LOW_ALERT >> 8), (HDC3020_COMMAND_CHANGE_CLEAR_LOW_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}

uint8_t hdc3020I2c::changeSetHighAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char Command[] = {(HDC3020_COMMAND_CHANGE_SET_HIGH_ALERT >> 8), (HDC3020_COMMAND_CHANGE_SET_HIGH_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}
uint8_t hdc3020I2c::changeClearHighAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char Command[] = {(HDC3020_COMMAND_CHANGE_CLEAR_HIGH_ALERT >> 8), (HDC3020_COMMAND_CHANGE_CLEAR_HIGH_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}

uint8_t hdc3020I2c::readSetLowAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_SET_LOW_ALERT >> 8), (HDC3020_COMMAND_READ_SET_LOW_ALERT & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::readClearLowAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_CLEAR_LOW_ALERT >> 8), (HDC3020_COMMAND_READ_CLEAR_LOW_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::readSetHighAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_SET_HIGH_ALERT >> 8), (HDC3020_COMMAND_READ_SET_HIGH_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::readClearHighAlert(unsigned char sendByte[2]) // more info in the datasheet on page 19
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_CLEAR_HIGH_ALERT >> 8), (HDC3020_COMMAND_READ_CLEAR_HIGH_ALERT & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

void hdc3020I2c::deactivateEnvironmentalTracking() 
{
 
  unsigned char Command[] = {(HDC3020_COMMAND_CHANGE_SET_LOW_ALERT >> 8), (HDC3020_COMMAND_CHANGE_SET_LOW_ALERT & 0xFF), 0xFF, 0xFF, 0xAC};
  wireWrite(Command, 4, true);
  unsigned char Command1[] = {(HDC3020_COMMAND_CHANGE_SET_HIGH_ALERT >> 8), (HDC3020_COMMAND_CHANGE_SET_HIGH_ALERT & 0xFF), 0x00, 0x00, 0x81};
  wireWrite(Command1, 4, true);
  return 0;
}

void hdc3020I2c::TransferThresholdsIntoNonVolatileMemory() 
{

  unsigned char Command[] = {(HDC3020_COMMAND_INTO_NON_VOLATILE_MEMORY >> 8), (HDC3020_COMMAND_INTO_NON_VOLATILE_MEMORY & 0xFF)};
  wireWrite(Command, 1, true);
  return 0;
}

uint8_t hdc3020I2c::changeOffsetValue(unsigned char sendByte[2]) 
{
  unsigned char Command[] = {(HDC3020_COMMAND_OFFSET_VALUE >> 8), (HDC3020_COMMAND_OFFSET_VALUE & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}

uint8_t hdc3020I2c::readOffsetValue(unsigned char sendByte[2]) 
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_OFFSET_VALUE >> 8), (HDC3020_COMMAND_OFFSET_VALUE & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::changeHeaterCurrent(unsigned char sendByte[2]) 
{
  unsigned char Command[] = {(HDC3020_COMMAND_HEATER_CONFIGURE >> 8), (HDC3020_COMMAND_HEATER_CONFIGURE & 0xFF), sendByte[0], sendByte[1], calcCrc8(sendByte, 0, 1)};
  wireWrite(Command, 4, true);
  return 0;
}

void hdc3020I2c::readHeaterCurrent(unsigned char sendByte[2])
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_HEATER_CONFIGURE >> 8), (HDC3020_COMMAND_HEATER_CONFIGURE & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

void hdc3020I2c::startPeriodicMeasurement(int measurementPerSeconds, int mode) // measurementPerSeconds: 0 = 0.5 mps, 1 = 1mps, 2 = 2mps, 3 = 4mps, 4 = 10mps;  Mode : 0 = low noise, 1 , 2 , 3 = lowest power; 
{
  unsigned char i2cResponse[6] = {};
  unsigned char Command[2];
  switch (measurementPerSeconds)
  {
  case 0:
    Command[0] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_0 >> 8)};
    switch (mode)
    {
      case 0:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_0 & 0xFF)};
        break;
      case 1:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_1 & 0xFF)};
        break;
      case 2:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_2 & 0xFF)};
        break;
      case 3:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_3 & 0xFF)};
        break;
    }
    break;
  case 1:
    Command[0] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_0 >> 8)};
    switch (mode)
    {
      case 0:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_0 & 0xFF)};
        break;
      case 1:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_1 & 0xFF)};
        break;
      case 2:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_2 & 0xFF)};
        break;
      case 3:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_3 & 0xFF)};
        break;
    }
    break;
  case 2:
    Command[0] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_0 >> 8)};
    switch (mode)
    {
      case 0:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_0 & 0xFF)};
        break;
      case 1:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_1 & 0xFF)};
        break;
      case 2:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_2 & 0xFF)};
        break;
      case 3:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_3 & 0xFF)};
        break;
    }
    break;
  case 3:
    Command[0] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_0 >> 8)};
    switch (mode)
    {
      case 0:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_0 & 0xFF)};
        break;
      case 1:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_1 & 0xFF)};
        break;
      case 2:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_2 & 0xFF)};
        break;
      case 3:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_3 & 0xFF)};
        break;
    }
    break;
  case 4:
    Command[0] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_0 >> 8)};
    switch (mode)
    {
      case 0:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_0 & 0xFF)};
        break;
      case 1:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_1 & 0xFF)};
        break;
      case 2:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_2 & 0xFF)};
        break;
      case 3:
        Command[1] = {(HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_3 & 0xFF)};
        break;
    }
    break;
  }
  wireWrite(Command, 1, false);
}

void hdc3020I2c::endPeriodicMeasurement(void)
{
  unsigned char Command[] = {(HDC3020_COMMAND_END_PERIODIC_MEASUREMENT >> 8), (HDC3020_COMMAND_END_PERIODIC_MEASUREMENT & 0xFF)};
  wireWrite(Command, 1, true);
}

void hdc3020I2c::heaterOn(void)
{
  unsigned char Command[] = {(HDC3020_COMMAND_HEATER_ON >> 8), (HDC3020_COMMAND_HEATER_ON & 0xFF)};
  wireWrite(Command, 1, true);
}

void hdc3020I2c::heaterOff(void)
{
  unsigned char Command[] = {(HDC3020_COMMAND_HEATER_OFF >> 8), (HDC3020_COMMAND_HEATER_OFF & 0xFF)};
  wireWrite(Command, 1, true);
}

uint8_t hdc3020I2c::readIdentification(unsigned char identification[])
{
  unsigned char i2cResponse1[3] = {};
  unsigned char i2cResponse2[3] = {};
  unsigned char i2cResponse3[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_MANUFACTOR_ID_0 >> 8), (HDC3020_COMMAND_READ_MANUFACTOR_ID_0 & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse1, 3);
  unsigned char Command1[] = {(HDC3020_COMMAND_READ_MANUFACTOR_ID_1 >> 8), (HDC3020_COMMAND_READ_MANUFACTOR_ID_1 & 0xFF)};
  wireWrite(Command1, 1, false);
  wireRead(i2cResponse2, 3);
  unsigned char Command2[] = {(HDC3020_COMMAND_READ_MANUFACTOR_ID_2 >> 8), (HDC3020_COMMAND_READ_MANUFACTOR_ID_2 & 0xFF)};
  wireWrite(Command2, 1, false);
  wireRead(i2cResponse3, 3);
  if (i2cResponse1[2] == calcCrc8(i2cResponse1, 0, 1) && i2cResponse2[2] == calcCrc8(i2cResponse2, 0, 1) && i2cResponse3[2] == calcCrc8(i2cResponse3, 0, 1))
  {
    identification[0] = i2cResponse1[0];
    identification[1] = i2cResponse1[1];
    identification[2] = i2cResponse2[0];
    identification[3] = i2cResponse2[1];
    identification[4] = i2cResponse3[0];
    identification[5] = i2cResponse3[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

void hdc3020I2c::reset(void)
{
  unsigned char Command[] = {(HDC3020_COMMAND_SOFT_RESET >> 8), (HDC3020_COMMAND_SOFT_RESET & 0xFF)};
  wireWrite(Command, 1, true);
}

uint8_t hdc3020I2c::HeaterOnOff(bool &conHeaterOnOff)
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_STATUS_REGISTER >> 8), (HDC3020_COMMAND_READ_STATUS_REGISTER & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    i2cResponse[0] = i2cResponse[0] << 2;
    conHeaterOnOff = i2cResponse[0] >> 7;
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t hdc3020I2c::readStatusRegister(unsigned char sendByte[2])
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HDC3020_COMMAND_READ_STATUS_REGISTER >> 8), (HDC3020_COMMAND_READ_STATUS_REGISTER & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    sendByte[0] = i2cResponse[0];
    sendByte[1] = i2cResponse[1];
    return 0;
  }
  else
  {
    return 1;
  }
}

void hdc3020I2c::clearStatusregister(void)
{
  unsigned char Command[] = {(HDC3020_COMMAND_CLEAR_STATUS_REGISTER >> 8), (HDC3020_COMMAND_CLEAR_STATUS_REGISTER & 0xFF)};
  wireWrite(Command, 1, true);
}

void hdc3020I2c::wireWrite(unsigned char buf[], int to, bool stopmessage)
{
  Wire.beginTransmission(address);
  for (int i = 0; i <= to; i++)
  {
    Wire.write(buf[i]);
  }
  Wire.endTransmission(stopmessage);
}

void hdc3020I2c::wireRead(unsigned char buf[], uint8_t to)
{
  int i = 0;
  Wire.requestFrom(address, to);
  while (Wire.available())
  {
    buf[i++] = Wire.read();
  }
}

unsigned char hdc3020I2c::calcCrc8(unsigned char buf[], unsigned char from, unsigned char to)
{
  unsigned char crcVal = CRC8_ONEWIRE_START;
  unsigned char i = 0;
  unsigned char j = 0;
  for (i = from; i <= to; i++)
  {
    int curVal = buf[i];
    for (j = 0; j < 8; j++)
    {
      if (((crcVal ^ curVal) & 0x80) != 0) //If MSBs are not equal
      {
        crcVal = ((crcVal << 1) ^ CRC8_ONEWIRE_POLY);
      }
      else
      {
        crcVal = (crcVal << 1);
      }
      curVal = curVal << 1;
    }
  }
  return crcVal;
}

void hdc3020I2c::getErrorString(uint8_t Status, char errorString[])
{
  memset(errorString, '\0', sizeof(errorString));
  switch (Status)
  {
  case OKAY:
    strcpy(errorString, "Success");
    break;
  case ERR_CKSUM:
    strcpy(errorString, "Checksum error");
    break;
  default:
    strcpy(errorString, "unknown error");
    break;
  }
}