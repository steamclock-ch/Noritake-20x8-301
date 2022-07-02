/*
Params management V0.1
STH 25.12.2020
*/

#ifndef _N614_EEPROM_h
#define _N614_EEPROM_h

#include "Arduino.h"
#include <EEPROM.h>

  enum eTimeFormat{h12,h24};


class Params
{
   
  private:
    void    writeByte(int _address, byte _value);
    byte    readByte(int address);
    void    writeString(String strToSave, int eepromBase);
    void    writeBool(int _address, bool _value);
    bool    readBool(int address);
    String  readString(int eepromBase);
    int     getWiFiCredentialsValidity();
    
    eTimeFormat     _timeFormat;
    String  _WiFiSSID;
    String  _WiFiPSK;
    bool    _isZeroBlanked;
    bool    _secondsBlankingAllowed;
    byte     _luminosityMode;
    byte     _timeZone;
    byte    _DSTZone;
    String _timeServer;
    byte    _timeOffsetIndex;
    bool   _validEEPROM;
    bool   _WiFiCredsValidity;


  public:
    Params(); 

    const String&      WiFiSSID = _WiFiSSID;      
    const String&      WiFiPSK = _WiFiPSK;
    const bool&        isZeroBlanked = _isZeroBlanked;
    const bool&        secondsBlankingAllowed = _secondsBlankingAllowed;
    const byte&        luminosityMode=_luminosityMode; 
    const byte&        timeZone=_timeZone;
    const byte&        DSTZone=_DSTZone;
    const String&      timeServer = _timeServer;
    const byte&        timeOffsetIndex = _timeOffsetIndex;
    const eTimeFormat& timeFormat = _timeFormat;
    const bool&        WiFiCredsValidity= _WiFiCredsValidity;
     
    
    void   begin();
    void   displayParams();
    String getTubeIntensityLabel(byte pos);
    bool   WiFiCredValidity();
    void   setWiFiCredentialsValidity(bool validity);
    void   saveWiFiSSID(String SSID);
    void   saveWiFiPSK(String PSK);

  
    void   saveWebPassword(String SSID);
    String getwebPassword();
    void   saveWebLogin(String SSID);
    String getwebLogin();
    bool   getWebAuthentication();
    void   setWebAuthentication(bool validity);
      

    void saveWiFi(String newSSID, String newPSK);
    void setTimeFormat(eTimeFormat timeformat);
    void setLuminosityMode(byte mode);

    //bool isZeroBlanked();
    void setZeroBlanking(bool isBlanked);
    byte getSecondsOffHours();
    byte getSecondsOffMinutes();
    byte getSecondsOnHours();
    byte getSecondsOnMinutes();
    byte getRebootCount();
    void setSecondsOffHours(byte offHour);
    void setSecondsOffMinutes(byte offMinute);
    void setSecondsOnHours(byte onHour);
    void setSecondsOnMinutes(byte onMinute);
    void incRebootCount();
    void resetRebootCount();
    void factoryReset();
    void setIsOTAReboot(bool OTAReboot);
    bool getIsOTAReboot();
    void setDSTZone(byte dstZone);
    
    String getDSTName(byte dst);
    void setTimeOffsetIndex(byte timeOffsetIndex);
    byte  getTimeOfsetIndex();
    String getTimezoneName(byte timeZone);
    void setSecondsBlankingAllowed(bool status);
    void setup();
};

#endif
