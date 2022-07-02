#ifndef _NORITAKE20X8301_h
#define _NORITAKE20X8301_h

	#if defined(ARDUINO) && ARDUINO >= 100
	  #include "Arduino.h"
	#else
	  #include "WProgram.h"
	#endif
#include "Wire.h"




	class Noritake20X8301
	{
    
		protected:
      static const char bankA =0x12;
      static const char bankB =0x13;

      static const uint8_t ON  =1;
      static const uint8_t OFF =0;

      static const uint8_t _nervous =0;
      static const uint8_t _speedy =20;
      static const uint8_t _smooth =20;

      unsigned char VFDAdd[20] = {0b11000000,0b11000010,0b11000001,                            0b11000011,
                            0b11001000,0b11001010,                            0b11001001,                            0b11001011,
                            0b11000100,0b11000110,                            0b11000101,                            0b11000111,
                            0b11001100,0b11001110,                            0b11001101,                            0b11001111,
                            0b11100000,0b11100010,                            0b11100001,                            0b11100011};
                            
       unsigned char VFDPix[8] = {0b00000010,0b00000001,0b00001000,0b00000100,0b00100000,0b00010000,0b10000000,0b01000000 };
       unsigned char numbers[10][2][5]{
                                {/*0*/ { 0,0,0,0,0} ,{VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]+VFDPix[5],VFDPix[0]+VFDPix[4]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[2]+VFDPix[6],VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]+VFDPix[5]} },
                                {/*1*/ { 0,0,0,0,0} ,{VFDPix[2]+VFDPix[6],VFDPix[1]+VFDPix[6],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]+VFDPix[5]+VFDPix[6],VFDPix[6],VFDPix[6]} },
                                {/*2*/ { 0,0,0,0,0} ,{VFDPix[1]+VFDPix[6],VFDPix[0]+VFDPix[5]+VFDPix[6],VFDPix[0]+VFDPix[4]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[1]+VFDPix[2]+VFDPix[6] } },
                                {/*3*/ { 0,0,0,0,0} ,{VFDPix[1]+VFDPix[5],VFDPix[0]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[1]+VFDPix[2]+VFDPix[4]+VFDPix[5]} },
                                {/*4*/ { 0,0,0,0,0} ,{VFDPix[3]+VFDPix[4],VFDPix[2]+VFDPix[4],VFDPix[1]+VFDPix[4],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]+VFDPix[5]+VFDPix[6],VFDPix[4] } },//4
                                {/*5*/ { 0,0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[5],VFDPix[0]+VFDPix[2]+VFDPix[6],VFDPix[0]+VFDPix[2]+VFDPix[6],VFDPix[0]+VFDPix[2]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[4]+VFDPix[5]} },//5
                                {/*6*/ { 0,0,0,0,0} ,{VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]+VFDPix[5],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[1]+VFDPix[4]+VFDPix[5]} },//6
                                {/*7*/ { 0,0,0,0,0} ,{VFDPix[0],VFDPix[0]+VFDPix[4]+VFDPix[5]+VFDPix[6],VFDPix[0]+VFDPix[3],VFDPix[0]+VFDPix[2],VFDPix[0]+VFDPix[1]} },//7
                                {/*8*/ { 0,0,0,0,0} ,{VFDPix[1]+VFDPix[2]+VFDPix[4]+VFDPix[5],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[1]+VFDPix[2]+VFDPix[4]+VFDPix[5]} },//8
                                {/*9*/ { 0,0,0,0,0} ,{VFDPix[1]+VFDPix[2]+VFDPix[5],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[0]+VFDPix[3]+VFDPix[6],VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]+VFDPix[5]} },//9
                              };


       unsigned char smallNumbers[10][2][4]{
                                     {/*0*/{0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4],VFDPix[0]+VFDPix[4],VFDPix[0]+VFDPix[4],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },
                                     {/*1*/ {0,0,0,0} ,{0,0,VFDPix[1],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },//1
                                     {/*2*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[2]+VFDPix[3]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[4]} },//2
                                     {/*3*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },//3
                                     {/*4*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2],VFDPix[2],VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4],VFDPix[2]} },//4
                                     {/*5*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },//5
                                     {/*6*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },//6
                                     {/*7*/ {0,0,0,0} ,{VFDPix[0],VFDPix[0],VFDPix[0],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },//7
                                     {/*8*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]} },//8
                                     {/*9*/ {0,0,0,0} ,{VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[2]+VFDPix[4],VFDPix[0]+VFDPix[1]+VFDPix[2]+VFDPix[3]+VFDPix[4]} }
                                    
                                    };
			
			static const uint8_t mainExtenderAdd = 0x20;

      
			void extenderAsOutput(unsigned char extenderAdd);
			void writePulse();
      void displayDigit(uint8_t address, uint8_t data);
		  //void displaySign(unsigned char color);
      //void digitZ(unsigned char number, unsigned char offset, unsigned char shift,unsigned char color);
      //void digitS(unsigned char number,unsigned char color);
      
      
      
		public:
      static const uint8_t RED   =16;
      static const uint8_t BLUE  =0;
      static const uint8_t WHITE =1;
      static const uint8_t CLEAR =2;

      void tubeTest();
      void clear();
      void pattern();
      void scan();
			void setup();
      void semicolon(unsigned char color);
      
      void digitS(unsigned char pos, unsigned char number,unsigned char color, unsigned char displayDelay);
	};

#endif  
