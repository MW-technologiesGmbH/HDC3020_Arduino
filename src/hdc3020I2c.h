/***************************************************************************/
/* headerfile for "hdc3020.cpp" module */
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

#ifndef hdc3020I2c_H
#define hdc3020I2c_H

#include "Arduino.h"
#include "Wire.h"

// Defines
//-----------------------------------------------------------------------------
#define CRC8_ONEWIRE_POLY                            0x31
#define CRC8_ONEWIRE_START                           0xFF
#define HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_0      0x2400
#define HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_1      0x240B
#define HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_2      0x2416
#define HDC3020_COMMAND_READ_SINGLE_SHOT_MODE_3      0x24FF
#define HDC3020_COMMAND_READ_PERIODIC_MEASUREMENT    0xE000
#define HDC3020_COMMAND_READ_MINIMUM_MEASUREMENT_T   0xE002
#define HDC3020_COMMAND_READ_MAXIMUM_MEASUREMENT_T   0xE003
#define HDC3020_COMMAND_READ_MINIMUM_MEASUREMENT_RH  0xE004
#define HDC3020_COMMAND_READ_MAXIMUM_MEASUREMENT_RH  0xE005
#define HDC3020_COMMAND_CLEAR_STATUS_REGISTER        0x3041
#define HDC3020_COMMAND_READ_STATUS_REGISTER         0xF32D
#define HDC3020_COMMAND_READ_REGISTER_2              0xF352
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT   0x201E
#define HDC3020_COMMAND_END_PERIODIC_MEASUREMENT     0x3093
#define HDC3020_COMMAND_SOFT_RESET                   0x30A2
#define HDC3020_COMMAND_HEATER_CONFIGURE             0x306E
#define HDC3020_COMMAND_HEATER_ON                    0x306D
#define HDC3020_COMMAND_HEATER_OFF                   0x3066
#define HDC3020_COMMAND_READ_MANUFACTOR_ID           0x3781
#define HDC3020_COMMAND_READ_MANUFACTOR_ID_0         0x3683
#define HDC3020_COMMAND_READ_MANUFACTOR_ID_1         0x3684
#define HDC3020_COMMAND_READ_MANUFACTOR_ID_2         0x3685
#define HDC3020_COMMAND_CHANGE_SET_LOW_ALERT         0x6100
#define HDC3020_COMMAND_CHANGE_CLEAR_LOW_ALERT       0x610B
#define HDC3020_COMMAND_CHANGE_SET_HIGH_ALERT        0x611D
#define HDC3020_COMMAND_CHANGE_CLEAR_HIGH_ALERT      0x6116
#define HDC3020_COMMAND_READ_SET_LOW_ALERT           0xE102
#define HDC3020_COMMAND_READ_CLEAR_LOW_ALERT         0xE109
#define HDC3020_COMMAND_READ_SET_HIGH_ALERT          0xE11F
#define HDC3020_COMMAND_READ_CLEAR_HIGH_ALERT        0xE114
#define HDC3020_COMMAND_INTO_NON_VOLATILE_MEMORY     0x6155
#define HDC3020_COMMAND_OFFSET_VALUE                 0xA004
#define HDC3020_COMMAND_CHANGE_DEFAULT_POWER_ON      0x61BB
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_0   0x2032
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_1   0x2024
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_2   0x202F
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MODE_3   0x20FF
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_0     0x2130
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_1     0x2126
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_2     0x212D
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_1_MODE_3     0x21FF
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_0     0x2236
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_1     0x2220
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_2     0x222B
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_2_MODE_3     0x22FF  
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_0     0x2334
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_1     0x2322
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_2     0x2329
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_4_MODE_3     0x23FF
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_0    0x2737
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_1    0x2721
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_2    0x272A
#define HDC3020_COMMAND_START_PERIODIC_MEASUREMENT_10_MODE_3    0x27FF

// declaration of functions
//-----------------------------------------------------------------------------

class hdc3020I2c
{
public:
    hdc3020I2c(void);
    hdc3020I2c(unsigned char i2cAdress);
    uint8_t singleShotTempHum(float &temperature, float &humidity, int Mode);
    void startPeriodicMeasurement(int measurementPerSeconds, int mode);
    void endPeriodicMeasurement(void);
    uint8_t getPeriodicMeasurementTempHum(float &temperature, float &humidity);
    uint8_t getDewpoint(float temperature, float humidity ,float &dewpoint);
    uint8_t getPeriodicMeasurementMinTemp(float &temperature);
    uint8_t getPeriodicMeasurementMaxTemp(float &temperature);
    uint8_t getPeriodicMeasurementMinHum(float &humidity);
    uint8_t getPeriodicMeasurementMaxHum(float &humidity);
    uint8_t changeDefaultDevicePowerOn(unsigned char sendByte[2]);
    uint8_t changeSetLowAlert(unsigned char sendByte[2]);
    uint8_t changeClearLowAlert(unsigned char sendByte[2]);
    uint8_t changeSetHighAlert(unsigned char sendByte[2]);
    uint8_t changeClearHighAlert(unsigned char sendByte[2]);
    uint8_t readSetLowAlert(unsigned char sendByte[2]);
    uint8_t readClearLowAlert(unsigned char sendByte[2]);
    uint8_t readSetHighAlert(unsigned char sendByte[2]);
    uint8_t readClearHighAlert(unsigned char sendByte[2]);
    void deactivateEnvironmentalTracking();
    void TransferThresholdsIntoNonVolatileMemory();
    uint8_t changeOffsetValue(unsigned char sendByte[2]);
    uint8_t readOffsetValue(unsigned char sendByte[2]);
    uint8_t changeHeaterCurrent(unsigned char sendByte[2]);
    void readHeaterCurrent(unsigned char sendByte[2]);
    void heaterOn(void);
    void heaterOff(void);
    uint8_t readIdentification(unsigned char identification[]);
    uint8_t HeaterOnOff(bool &conHeaterOnOff);
    uint8_t readStatusRegister(unsigned char sendByte[2]);
    void clearStatusregister(void);
    void reset(void);
    unsigned char address = 0x44;
    void wireWrite(unsigned char buf[], int to, bool stopmessage);
    void wireRead(unsigned char buf[], uint8_t to);
    unsigned char calcCrc8(unsigned char buf[], unsigned char from, unsigned char to);
    void getErrorString(uint8_t Status, char errorString[]);
};

#endif