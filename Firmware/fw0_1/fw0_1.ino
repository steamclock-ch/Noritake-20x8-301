#include <FS.h>

#include <DNSServer.h>
DNSServer dnsServer;


#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <TimeLib.h>
#include <Timezone.h>    // https://github.com/JChristensen/Timezone
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <U8x8lib.h>
#include <Wire.h>

#include "params.h"

// NTP Servers:
static const char ntpServerName[] = "ch.pool.ntp.org";

const int timeZone = 1;     // Central European Time


Params params;
bool actAsPortal;

U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   

enum _epage {homePage,dstPage,timezonePage,timeformatPage,zeroblankingPage,tubeintensityPage,timeserverPage,wifiPage};
_epage currentPage;


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

AsyncWebServer server(80);

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);
time_t prevDisplay = 0; // when the digital clock was displayed

int WiFiNetworkCount;



/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
#include "Noritake20X8_301.h"
Noritake20X8301 vfd;
static const char ClockType[] = "SteamClock N&deg;160 W";

//*********************************************************
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
 
  Serial.begin(115200);
  delay(250);
  Serial.println("");
  Serial.println(ESP.getResetReason());
  Serial.print("Starting ");
  Serial.println(ClockType);
  
  Wire.begin(4, 5);
  setupOLED();

  params.displayParams();
  actAsPortal=setupWiFi();  
  setupSPIFFS();
  setupWebServer(actAsPortal);
  setupTime();

  Serial.println("Setup");
  vfd.setup();
 
  vfd.tubeTest();
   
}


//*********************************************************
void digitalClockDisplay()
{  
  byte rh=19;
  byte rm=15;
  byte rs=00;
  static byte blink;

  time_t local = now();
    
  Serial.printf("Local %02d:%02d:%02d   ", hour(), minute(), second());
 
  local = getTime(params.DSTZone, params.timeOffsetIndex , local);

  rh = hour(local);
  rm= minute(local);
  rs = second(local);
  
  if ((params.timeFormat==h12) && (rh>12)){
    rh = rh-12;
  }
    
  byte ss;
  byte s;
  ss = rs / 10;
  s = rs % 10; 

  byte mm;
  byte m;
  mm = rm / 10;
  m = rm % 10; 
  
  byte hh;
  byte h;
  hh = rh / 10;
  h = rh % 10; 
   
  if (blink==0){
    vfd.semicolon(vfd.BLUE);
    blink = 1;  
    }else{
      
      vfd.semicolon(vfd.CLEAR);
      blink =0;
    }
    
  if (params.isZeroBlanked && hh==0)  
    vfd.digitS(0,hh,vfd.CLEAR,50);
  else 
    vfd.digitS(0,hh,vfd.RED,50);
  
  vfd.digitS(1,h,vfd.RED,50);
  vfd.digitS(2,mm,vfd.RED,50);
  vfd.digitS(3,m,vfd.RED,50);

  char time[9];

  sprintf( time, " %02hu:%02hhu:%02hhu ", rh, rm,rs );
  if (params.isZeroBlanked && rh<10)
  {
    time[4]=' ';
  }

  u8x8.drawString(0,2,time);

 /* if (params.DSTZone>0)
  {
    Serial.print(params.getDSTName(params.DSTZone));
  }
  
  if (params.timeOffsetIndex>0)
  {
    Serial.print(params.getTimezoneName(params.timeOffsetIndex));
  }


  Serial.print(" Local time is ");
  Serial.print(time);
  Serial.print(" ");
  
  Serial.print(day(local));
  Serial.print(".");
  Serial.print(month(local));
  Serial.print(".");
  Serial.print(year(local));
  Serial.println(); 
*/

}

//*********************************************************
void loop()
{
  static char syncWindow =0;
  static int syncInterval=10;

  if (actAsPortal)
  {
    dnsServer.processNextRequest();  
  }
  else
  {
    if (now() != prevDisplay){
      if (timeStatus() != timeNotSet){
          prevDisplay = now();
          digitalClockDisplay();       
      } else {
        u8x8.drawString(0,3,"Time not set 10s");//C-L
        
        if (syncInterval!=10){  
          setSyncInterval(10);//60s*60m*6h 
          syncInterval=10;
          Serial.println("Sync interval set to 10s");
        }
      }
    
      if (timeStatus()==timeNeedsSync){
        u8x8.drawString(0,3,"Sync needed (10s)");//C-L
        syncWindow ++;
        if (syncWindow > 10){
          if(syncInterval!=10){
            Serial.println("Sync interval set to 10s");
            setSyncInterval(10);
            syncInterval=10;
          }
        }    
      }
      
      if (timeStatus()==timeSet)    {
        u8x8.drawString(0,3,"Sync done (6h)");//C-L
        syncWindow=0;
        if(syncInterval!=21600){
          setSyncInterval(21600);//60s*60m*6h 
          syncInterval=21600;
          Serial.println("Sync interval set to 6h");
        }
        
      }
    }
  }
   



}
