#include "Noritake20X8_301.h"
#include "Wire.h"



void Noritake20X8301::extenderAsOutput(unsigned char extenderAdd)
{
  Wire.beginTransmission(extenderAdd);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission();
  
  Wire.beginTransmission(extenderAdd);
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set entire PORT B to output
  Wire.endTransmission();
}

void Noritake20X8301::writePulse()
{
  delay(1);
  Wire.beginTransmission(0x38);
  //               ESRW 
  //              DNYSR
  Wire.write(0b00011000); // dataport
  Wire.endTransmission(); 
  delay(1); 
  Wire.beginTransmission(0x38);
  //               ESRW 
  //              DNYSR
  Wire.write(0b00011001); // dataport
  Wire.endTransmission(); 
}

void Noritake20X8301::displayDigit(uint8_t address, uint8_t data){
  unsigned char valueA,valueB;
  
  Wire.beginTransmission(0x20);
  Wire.write(bankA); // address bank A
  Wire.write(data); //SETA
  Wire.endTransmission();
  
  Wire.beginTransmission(0x20);
  Wire.write(bankB); // address bank b
  Wire.write(address); 
  Wire.endTransmission(); 
  
  writePulse();
}

void Noritake20X8301::digitS(unsigned char pos, unsigned char number,unsigned char color, unsigned char displayDelay)
{
  unsigned char R,B,W;

  B = (color == BLUE)?1:0;
  R = (color == RED )?1:0;
  if (color==WHITE) {B=1;R=1;} ;

  char offset=0;
  if (pos>1){
    offset=1;  
  }
 
  for (char i=0; i<4;i++)
  {
    displayDigit(VFDAdd[i+(pos*4)+pos*1+offset]+BLUE, smallNumbers[number][B][i]); 
    displayDigit(VFDAdd[i+(pos*4)+pos*1+offset]+RED , smallNumbers[number][R][i]); 
    delay(displayDelay);
  };
  
}

void Noritake20X8301::semicolon(unsigned char color)
{

  if (color==RED || color==WHITE){
    displayDigit(VFDAdd[9]+RED, VFDPix[2]+VFDPix[4]); 
    displayDigit(VFDAdd[10]+RED, VFDPix[2]+VFDPix[4]); 
  }  
  
  if (color==BLUE || color==WHITE){
    displayDigit(VFDAdd[9]+BLUE, VFDPix[2]+VFDPix[4]); 
    displayDigit(VFDAdd[10]+BLUE, VFDPix[2]+VFDPix[4]); 
  }

  if (color==CLEAR)  
  {
      displayDigit(VFDAdd[9]+BLUE, 0x00); 
      displayDigit(VFDAdd[9]+RED , 0x00); 
      displayDigit(VFDAdd[10]+BLUE, 0x00); 
      displayDigit(VFDAdd[10]+RED , 0x00); 
  }
  
}

void Noritake20X8301::clear()
{
  int i;
 
  for (i=0;i<20;i++)
  {
    displayDigit(VFDAdd[i]+RED,  0x00 );
    displayDigit(VFDAdd[i]+BLUE, 0x00 );
  }
}


/*Noritake20X8301::Noritake20X8301()
{
  //extenderAsOutput(mainExtenderAdd);  
  //clearVFD(); 
}
*/
void Noritake20X8301::pattern()
{
  int i;
 
  for (i=0;i<20;i++)
  {
    displayDigit(VFDAdd[i]+RED,  0x01);
    displayDigit(VFDAdd[i]+BLUE, 0x02);
  }
}

void Noritake20X8301::setup()
{
  extenderAsOutput(mainExtenderAdd);  
  clear(); 
}



void Noritake20X8301::tubeTest()
{  
  int dly = 5;
  clear();
  for (int hh=0;hh<10;hh++)
  {   
    digitS(0,hh,WHITE,0);
    digitS(1,hh,WHITE,0);
    digitS(2,hh,WHITE,0);
    digitS(3,hh,WHITE,0);
    delay(dly);
    digitS(0,hh,RED,0);
    digitS(1,hh,RED,0);
    digitS(2,hh,RED,0);
    digitS(3,hh,RED,0);
    delay(dly);
    digitS(0,hh,BLUE,0);
    digitS(1,hh,BLUE,0);
    digitS(2,hh,BLUE,0);
    digitS(3,hh,BLUE,0);
    delay(dly);
  }
  clear();
}

void Noritake20X8301::scan()
{
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
 
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
}
