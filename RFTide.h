/*
  RFTide.cpp - Library for Aurel RTFide modules.
  Created by Mirco Segatello, July 2013.
  Released under the Creative Commons Attribution-Share
  http://www.creativecommons.org/licenses/by-sa/3.0/
*/

#ifndef RFTide_h
#define RFTide_h

/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
*/

#include "Arduino.h"
#include "SoftwareSerial.h"

// use this line to enable debug message
//#define DEBUG

//define version
#define VERSION   100 // library version X.YY (e.g. 1.00)

//define status value
#define ANSWER	   0	//receive correctly answer 
#define NOANSWER   1	//no answer received
#define BADANSWER  2	//bad data on receive buffer
#define BRKANSWER  3	//incomplete data on receive buffer
#define RECPACKET  4	//receive packet

//define RFTide pin
#define GPIN0  0
#define GPIN1  1
#define GPOUT0 0
#define GPOUT1 1

//define RFTide OUT PACKET
#define CommandLSB 0
#define CommandMSB 1
#define Address 2
#define Byte0 3
#define Byte1 4
#define Byte2 5
#define Byte3 6
#define Byte4 7
#define Byte5 8
#define Byte6 9
#define Byte7 10

// Define RFTide command
#define CMD_SNDPCK 0x00
#define CMD_LSTATS 0x01
#define CMD_PING   0x02
#define CMD_PRGMSG 0x03
#define CMD_ANSMSG 0x04
#define CMD_RESET  0x07
#define CMD_RIOSET 0x10
#define CMD_RIORST 0x11
#define CMD_RIORAD 0x12
#define CMD_RVCCRD 0x13
#define CMD_LIOSET 0x18
#define CMD_LIORST 0x19
#define CMD_LIORAD 0x1A
#define CMD_LVCCRD 0x1B

// Define profile
#define none 0
#define motor 1
#define PWM 2
#define metering 3
#define repeater 0xFF

// time for request answer
#define TIMEOUTANSWER 500


struct IOSTATUS{
	byte getGPIN0;
	byte getGPIN1;
	byte getGPOUT0;
	byte getGPOUT1;
	int getADC0;
	int getADC1;
};

struct NTSTATUS{
	byte getAddress;
	unsigned long getNetworkID;
	int getFWversion;
	byte getProfile;
};

class RFTide {

  public:

	RFTide(byte RX_pin, byte TX_pin, byte EN_pin);	

	void localIoSet(byte payload);
	void localIoReset(byte payload);
	IOSTATUS localIoRead();
	NTSTATUS localStatus();	
	byte getLocalAddress();
	int getLocalFWversion();
	byte getLocalProfile();
	void resetLocalDevice();
	unsigned long getLocalNetworkID();


	void remoteIoSet(byte nodeAddress, byte payload);
	void remoteIoReset(byte nodeAddress, byte payload);
	IOSTATUS remoteIoRead(byte nodeAddress);
	void progMessage(byte nodeAddress, byte nodeProfile);
	NTSTATUS ping(byte nodeAddress);

	byte getGPIN0();
	byte getGPIN1();
	byte getGPOUT0();
	byte getGPOUT1();

	int version();
	void enable();
	void disable();
	void sendPacket(byte nodeAddress, char payload[]);
	byte readPacket(byte inbyte[]);
	byte status();

  private:

	SoftwareSerial * softwareSerial;
	void setZeroPayload();
	void sendPacket(byte outByte[]);
	void readAnswer();
	byte outPacket[11];
	byte inPacket[11];
	byte _EN_pin;
	byte Status;
};

#endif
