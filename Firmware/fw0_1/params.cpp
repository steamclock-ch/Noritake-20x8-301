 /*
Params management V0.1
STH 25.12.2020
*/

#include "Arduino.h"
#include "params.h"
#include "EEPROM.h"

// eeprom locations for wifi cred

const int _eevalidEEPROM = 1;

const int _eetimeFormat = 2;
const int _eezeroBlanking = 3;
const int _eetubeIntensity = 4;

const int _eeSecondOffHours = 5;
const int _eeSecondOffMinutes = 6;
const int _eeSecondOnHours = 7;
const int _eeSecondOnMinutes = 8;
const int _eeRebootCount = 9;
const int _eeWiFiCredValidity = 10;
const int _eeOTAReboot = 11;
const int _eeDSTZone = 12;
const int _eeTimeZone = 13;
const int _eeBlankingAllowed = 14;
const int _eeAuthenticate = 15;

const int _eeWiFiPSK = 350;
const int _eeWiFiSSID = 400;

const int _eeWebPassword = 450;
const int _eeWebLogin = 500;


String timeZoneName[] = { "None. DST settings used.",
                          "(GMT -12:00) Eniwetok",
                          "(GMT -11:00) Midway Island",
                          "(GMT -10:00) Hawaii",
                          "(GMT - 9:30) Taiohae",
                          "(GMT - 9:00) Alaska",
                          "(GMT - 8:00) Pacific Time (US & Canada)",
                          "(GMT - 7:00) Mountain Time (US & Canada)",
                          "(GMT - 6:00) Central Time (US & Canada)",
                          "(GMT - 5:00) Eastern Time (US & Canada)",
                          "(GMT - 4:30) Caracas",
                          "(GMT - 4:00) Atlantic Time (Canada)",
                          "(GMT - 3:30) Newfoundland",
                          "(GMT - 3:00) Brazil",
                          "(GMT - 2:00) Mid-Atlantic",
                          "(GMT - 1:00) Azores",
                          "(GMT + 0:00) Western Europe Time",
                          "(GMT + 1:00) Lausanne, Berne, Paris, Brussels",
                          "(GMT + 2:00) Kaliningrad",
                          "(GMT + 3:30) Tehran",
                          "(GMT + 4:00) Abu Dhabi",
                          "(GMT + 4:30) Kabul",
                          "(GMT + 5:00) Ekaterinburg",
                          "(GMT + 5:30) Bombay",
                          "(GMT + 5:45) Kathmandu",
                          "(GMT + 6:00) Almaty",
                          "(GMT + 6:30) Yangon",
                          "(GMT + 7:00) Bangkok",
                          "(GMT + 8:00) Beijing",
                          "(GMT + 8:45) Eucla",
                          "(GMT + 9:00) Tokyo",
                          "(GMT + 9:30) Adelaide",
                          "(GMT +10:00) Eastern Australia",
                          "(GMT +10:30) Lord Howe Island",
                          "(GMT +11:00) Magadan",
                          "(GMT +11:30) Norfolk Island",
                          "(GMT +12:00) Auckland",
                          "(GMT +12:45) Chatham Islands",
                          "(GMT +13:00) Apia",
                          "(GMT +14:00) Line Islands"
  };
                            
/*******************************************************************************/
Params::Params()
{
  EEPROM.begin(512);
  Serial.println("EEPROM start");
  if (readByte(_eevalidEEPROM)!= 42)
  { 
    Serial.println("Invalid EEPROM : Parameters back to Factory");
    factoryReset();
  }
  
  _timeFormat             = (EEPROM.read(_eetimeFormat)==h12)?h12:h24;
  _WiFiSSID               = readString(_eeWiFiSSID);
  _WiFiPSK                = readString(_eeWiFiPSK);
  _isZeroBlanked          = readBool(_eezeroBlanking)==1;
  _secondsBlankingAllowed = readBool(_eeBlankingAllowed);
  _luminosityMode         = EEPROM.read(_eetubeIntensity);
  _timeZone               = EEPROM.read(_eeTimeZone);
  _DSTZone                = EEPROM.read(_eeDSTZone);
  _timeServer             = "NULL";
  _timeOffsetIndex        = EEPROM.read(_eeTimeZone);
  _WiFiCredsValidity      = EEPROM.read(_eeWiFiCredValidity);
}



/*******************************************************************************/
void Params::writeByte(int _address, byte _value)
{
  EEPROM.write(_address, _value);
  EEPROM.commit();
}

/*******************************************************************************/
byte Params::readByte(int address)
{
  int value;
  value = EEPROM.read(address);
  return value;
}

/*******************************************************************************/
    void Params::writeString(String strToSave, int eepromBase)
	{
		int i;
		for (i = 0; i < strToSave.length(); ++i)
		{
			EEPROM.write(eepromBase + i, strToSave[i]);
		}
		EEPROM.write(eepromBase + i, '|');
		EEPROM.commit();
	}

	/*******************************************************************************
	*
	* input : none
	* output:
	*******************************************************************************/
    String Params::readString(int eepromBase)
	{
		//Serial.println("\nReading From eeprom");
		String eeStr="";
		for (int i = 0; i < 33; ++i)
		{
			if (EEPROM.read(i + eepromBase) == '|') break;
			eeStr += char(EEPROM.read(i + eepromBase));
		}
		return eeStr;
	}

  /*******************************************************************************/
  void Params::writeBool(int _address, bool _value)
  {
    EEPROM.write(_address, (_value)?1:0);
    EEPROM.commit();
  }
  
  /*******************************************************************************/
  bool Params::readBool(int address)
  {
    return EEPROM.read(address)==1;
  }
  /*******************************************************************************/

  void Params::saveWebPassword(String SSID)
  {
    writeString(SSID, _eeWebPassword);
  }

  String Params::getwebPassword()
  {
    return readString(_eeWebPassword);
  }

  void Params::saveWebLogin(String SSID)
  {
    writeString(SSID, _eeWebLogin);
  }

  String Params::getwebLogin()
  {
    return readString(_eeWebLogin);
  }

  bool Params::getWebAuthentication()
  {
    return (readByte(_eeAuthenticate) == 42);
  }

  void Params::setWebAuthentication(bool validity)
  {
    writeByte(_eeAuthenticate, (validity) ? 42 : 0);
  }


	/*******************************************************************************
	*
	* input : none
	* output:
	*******************************************************************************/
    void Params::setWiFiCredentialsValidity(bool validity)
	{
    _WiFiCredsValidity = validity;
		writeBool(_eeWiFiCredValidity,_WiFiCredsValidity);
	}

	/*******************************************************************************
	*
	* input : none
	* output:
	*******************************************************************************/
    void Params::saveWiFiSSID(String SSID)
	{
    _WiFiSSID = SSID;
		writeString(_WiFiSSID, _eeWiFiSSID);
	}

	/*******************************************************************************
	*
	* input : none
	* output:
	*******************************************************************************/
    void Params::saveWiFiPSK(String PSK)
	{  
    _WiFiPSK = PSK;
		writeString(_WiFiPSK, _eeWiFiPSK);
	}


/*******************************************************************************/
void Params::saveWiFi(String newSSID, String newPSK)
{
	saveWiFiPSK(newPSK);
	saveWiFiSSID(newSSID);
	setWiFiCredentialsValidity(true);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
void Params::setTimeFormat(eTimeFormat timeformat)
{
  _timeFormat =  timeformat;
  writeByte(_eetimeFormat, _timeFormat);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
void Params::setLuminosityMode(byte mode)
{
	Serial.printf("\nW LuminosityMode %d\n", mode);
 if (mode <0) mode=0;
 if (mode >9) mode=9;
  _luminosityMode = mode;
	writeByte(_eetubeIntensity, _luminosityMode );
}

/* ************************************************************** */
//
//
/* ************************************************************** */
//bool Params::isZeroBlanked()
//{
//	return readByte(_eezeroBlanking);
//}

/* ************************************************************** */
//
//
/* ************************************************************** */
void Params::setZeroBlanking(bool isBlanked)
{
  _isZeroBlanked = isBlanked;
	writeByte(_eezeroBlanking,_isZeroBlanked);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
byte Params::getSecondsOffHours()
{
	return readByte(_eeSecondOffHours);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
byte Params::getSecondsOffMinutes()
{
	return readByte(_eeSecondOffMinutes);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
byte Params::getSecondsOnHours()
{
	return readByte(_eeSecondOnHours);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
byte Params::getSecondsOnMinutes()
{
	return readByte(_eeSecondOnMinutes);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
byte Params::getRebootCount()
{
	return readByte(_eeRebootCount);
}

/* ************************************************************** */
//
/* ************************************************************** */
void Params::setSecondsOffHours(byte offHour)
{
	writeByte(_eeSecondOffHours,offHour);
}

/* ************************************************************** */
//
/* ************************************************************** */
void Params::setSecondsOffMinutes(byte offMinute)
{
	writeByte(_eeSecondOffMinutes,offMinute);
}

/* ************************************************************** */
//
/* ************************************************************** */
void Params::setSecondsOnHours(byte onHour)
{
	writeByte(_eeSecondOnHours,onHour);
}

/* ************************************************************** */
//
/* ************************************************************** */
void Params::setSecondsOnMinutes(byte onMinute)
{
	writeByte(_eeSecondOnMinutes,onMinute);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
void Params::incRebootCount()
{
	int rebootCount = readByte(_eeRebootCount);
	rebootCount = rebootCount + 1;
	writeByte(_eeRebootCount,rebootCount);
}

/* ************************************************************** */
//
//
/* ************************************************************** */
void Params::resetRebootCount()
{
	writeByte(_eeRebootCount,0);
}

/* ************************************************************** */
//
/* ************************************************************** */
void Params::factoryReset()
{
	setWiFiCredentialsValidity(false);
	setTimeFormat(h24);
	setZeroBlanking(false);
	setSecondsBlankingAllowed(false);
	setLuminosityMode(0);
	setDSTZone(1);// DST = GMZ;
	setTimeOffsetIndex(0); // do time zone used

  setWebAuthentication(false);
  saveWebPassword("Steam");
  saveWebLogin("Clock");
  
	//below deprecated
	setSecondsOffHours(9);
	setSecondsOffMinutes(30);
	setSecondsOnHours(6);
	setSecondsOnMinutes(30);
	// end
  writeByte(_eevalidEEPROM,42);
  _validEEPROM=true;
}

/* ************************************************************** */
//
//
/* ************************************************************** */
void Params::setIsOTAReboot(bool OTAReboot)
{

}

/* ************************************************************** */
//
//
/* ************************************************************** */
bool Params::getIsOTAReboot()
{

}

// ------------------------------------------------------------------
void Params::setDSTZone(byte dstZone)
{
  _DSTZone =dstZone ;
	writeByte(_eeDSTZone, _DSTZone);

 
 if (_DSTZone!=0)
 {  
    Serial.println("by rule _timeOffsetIndex=;");
    _timeOffsetIndex=0;
    writeByte(_eeTimeZone, 0);
 }  
}


// ------------------------------------------------------------------
void Params::setTimeOffsetIndex(byte timeOffsetIndex)
{
  if (timeOffsetIndex<0  ) timeOffsetIndex = 0;
  if (timeOffsetIndex >39) timeOffsetIndex = 0;
  
  _timeOffsetIndex=timeOffsetIndex;
	writeByte(_eeTimeZone, timeOffsetIndex);

  if (_timeOffsetIndex!=0)
  {
     Serial.println("by rule _DSTZone=0");
    _DSTZone=0;
    writeByte(_eeDSTZone, 0);
  }
}


String Params::getTimezoneName(byte timeZone)
{
  switch (timeZone){
    case 0: return "None. DST settings used.";break;
    case 1: return "(GMT -12:00) Eniwetok";break;
    case 2: return "(GMT -11:00) Midway Island";break;
    case 3: return "(GMT -10:00) Hawaii";break;
    case 4: return "(GMT - 9:30) Taiohae";break;
    case 5: return "(GMT - 9:00) Alaska";break;
    case 6: return "(GMT - 8:00) Pacific Time (US & Canada)";break;
    case 7: return "(GMT - 7:00) Mountain Time (US & Canada)";break;
    case 8: return "(GMT - 6:00) Central Time (US & Canada)";break;
    case 9: return "(GMT - 5:00) Eastern Time (US & Canada)";break;
    case 10: return "(GMT - 4:30) Caracas";break;
    case 11: return "(GMT - 4:00) Atlantic Time (Canada)";break;
    case 12: return "(GMT - 3:30) Newfoundland";break;
    case 13: return "(GMT - 3:00) Brazil";break;
    case 14: return "(GMT - 2:00) Mid-Atlantic";break;
    case 15: return "(GMT - 1:00) Azores";break;
    case 16: return "(GMT + 0:00) Western Europe Time";break;
    case 17: return "(GMT + 1:00) Lausanne, Berne, Paris, Brussels";break;
    case 18: return  "(GMT + 2:00) Kaliningrad";break;
    case 19: return "(GMT + 3:30) Tehran";break;
    case 20: return "(GMT + 4:00) Abu Dhabi";break;
    case 21: return "(GMT + 4:30) Kabul";break;
    case 22: return  "(GMT + 5:00) Ekaterinburg";break;
    case 23: return  "(GMT + 5:30) Bombay";break;
    case 24: return  "(GMT + 5:45) Kathmandu";break;
    case 25: return  "(GMT + 6:00) Almaty";break;
    case 26: return   "(GMT + 6:30) Yangon";break;
    case 27: return   "(GMT + 7:00) Bangkok";break;
    case 28: return   "(GMT + 8:00) Beijing";break;
    case 29: return   "(GMT + 8:45) Eucla";break;
    case 30: return   "(GMT + 9:00) Tokyo";break;
    case 31: return   "(GMT + 9:30) Adelaide";break;
    case 32: return   "(GMT +10:00) Eastern Australia";break;
    case 33: return   "(GMT +10:30) Lord Howe Island";break;
    case 34: return   "(GMT +11:00) Magadan";break;
    case 35: return   "(GMT +11:30) Norfolk Island";break;
    case 36: return   "(GMT +12:00) Auckland";break;
    case 37: return   "(GMT +12:45) Chatham Islands";break;
    case 38: return   "(GMT +13:00) Apia";break;
    case 39: return   "(GMT +14:00) Line Islands";break;
    default : return "oob";  
  }
}

String Params::getDSTName(byte dst)
{
  switch (dst) {
    case 0: return "None, Timezone settings used.";break;
    case 1: return "GMT";break;
    case 2: return "Western Europe";break;
    case 3: return "Center Europe";break;
    case 4: return "Eastern Europe";break;
    case 5: return "US Pacific Standard";break;
    case 6: return "US Mountain Standard";break;
    case 7: return "US Central Standard Time";break;
    case 8: return "AU Eastern Standard Time";break;
    case 9: return "US Eastern Standard Time";break;
    default: return "NONE";
  }
}

String Params::getTubeIntensityLabel(byte pos)
{
  switch (pos){
    case 0 : return "Automatic";break;
    case 1 : return "Minimum Light";break;
    case 2 : return "2 on 9";break;
    case 3 : return "3 on 9";break;
    case 4 : return "4 on 9";break;
    case 5 : return "5 on 9";break;
    case 6 : return "6 on 9";break;
    case 7 : return "7 on 9";break;
    case 8 : return "8 on 9";break;
    case 9 : return "Full Light";break;
    default: return "OOB";
  }
}



void Params::setSecondsBlankingAllowed(bool status)
{
	_secondsBlankingAllowed = status;
	writeBool(_eeBlankingAllowed, _secondsBlankingAllowed);
}

void Params::displayParams()
{
  Serial.println ("\n******************************");
  Serial.println ((_WiFiCredsValidity)?"Wifi creds are valid": "Wifi creds are invalid");
  Serial.println ((_timeFormat==h12)?"Time Format is 12h":"Time Format is 24h");
  Serial.println ((_isZeroBlanked)? "Leading Zero is blanked": "Leading Zero is not blanked") ;
  Serial.println ((_secondsBlankingAllowed)?"Seconds can be blanked":"Seconds cannot be blanked");
  Serial.print   ("Luminosity mode is ");
  Serial.println (_luminosityMode);
  
  Serial.print  ("DST zone is #");
  Serial.print  (_DSTZone);
  Serial.print  (": ");
  Serial.println(getDSTName(_DSTZone));
  
  Serial.print  ("Time Offset Index is #");
  Serial.print  (_timeOffsetIndex);
  Serial.print  (": ");
  Serial.println(getTimezoneName(_timeOffsetIndex));
  
  Serial.println("******************************");
}
