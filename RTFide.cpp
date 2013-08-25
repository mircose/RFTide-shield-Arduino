/*
  RFTide.cpp - Library for Aurel RTFide modules.

  Created by Mirco Segatello, July 2013.
*/

#include "RFTide.h"

IOSTATUS InOutStatus = IOSTATUS();
NTSTATUS NetStatus = NTSTATUS();

/************ Constructor **********/

RFTide::RFTide(byte RX_pin, byte TX_pin, byte EN_pin)
{
	pinMode(EN_pin, OUTPUT);
	digitalWrite(EN_pin, LOW);
	_EN_pin = EN_pin;
	Status = ANSWER;
	softwareSerial = new SoftwareSerial(RX_pin, TX_pin);
	softwareSerial->begin(19200);
	//clear softwareSerial input buffer
	char c;
	while (softwareSerial->available() > 0) {
		c = softwareSerial->read();
	}
}



/************ High Level Function **********/

byte RFTide::status()
{
	return Status;
}

void RFTide::sendPacket(byte nodeAddress, char payload[])
{
	#ifdef DEBUG
  		Serial.println("CMD sendPacket");
		Serial.print("RFTide->send: ");
		Serial.println(payload);
	#endif

	outPacket[CommandLSB]=CMD_SNDPCK;
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=nodeAddress;
	//setZeroPayload();
	for (byte i=0; i<8; i++)
	{     
		outPacket[i+3]=payload[i];
	}
	sendPacket(outPacket);
	readAnswer();
}

int RFTide::version()
{
	//return val: 100 means library version 1.00
	return VERSION;
}

void RFTide::enable()
{
	digitalWrite(_EN_pin, HIGH);
	delay(200); // for warmup module
}

void RFTide::disable()
{
	digitalWrite(_EN_pin, LOW);
}


byte RFTide::readPacket(byte packet[])
{
	readAnswer(); //Try to read packet
	if (Status == RECPACKET)  { 
		packet[0] = inPacket[Byte0];
		packet[1] = inPacket[Byte1];
		packet[2] = inPacket[Byte2];
		packet[3] = inPacket[Byte3];
		packet[4] = inPacket[Byte4];
		packet[5] = inPacket[Byte5];
		packet[6] = inPacket[Byte6];
		packet[7] = inPacket[Byte7];	
		return inPacket[Address];
	}
	else
		return 255;	
}

void RFTide::remoteIoSet(byte nodeAddress, byte payload)
{
	if (payload>1) {
		Serial.print("BAD payload!");
	 	exit;
	}
	#ifdef DEBUG
  		Serial.println("CMD RemoteIoSet");
	#endif
	outPacket[CommandLSB]=CMD_RIOSET;
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=nodeAddress;
	setZeroPayload();     
	outPacket[Byte0]=payload;
	sendPacket(outPacket);
	readAnswer();
}

void RFTide::remoteIoReset(byte nodeAddress, byte payload)
{
	if (payload>1) {
		Serial.print("BAD payload!");
	 	exit;
	}
	#ifdef DEBUG
  		Serial.println("CMD RemoteIoReset");
	#endif
	outPacket[CommandLSB]=CMD_RIORST;
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=nodeAddress;
	setZeroPayload();     
	outPacket[Byte0]=payload;
	sendPacket(outPacket);
	readAnswer();
}

NTSTATUS RFTide::ping(byte nodeAddress)
{
	//ping remote module
	#ifdef DEBUG
  		Serial.println("CMD Ping");
	#endif
	outPacket[CommandLSB]=CMD_PING;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=nodeAddress; 
	setZeroPayload();     
	sendPacket(outPacket); 
	readAnswer();

	NetStatus.getAddress = inPacket[Address]; //address di chi ha risposto

	NetStatus.getNetworkID = 0;
	NetStatus.getFWversion = inPacket[Byte6]*10+inPacket[Byte5];
	NetStatus.getProfile = inPacket[Byte7];	

	return NetStatus;
}

void RFTide::resetLocalDevice()
{
	//reset locad device
	#ifdef DEBUG
  		Serial.println("CMD Reset Local Device");
	#endif
	outPacket[CommandLSB]=CMD_RESET;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=0x00; 
	setZeroPayload();     
	sendPacket(outPacket); 
	readAnswer();

}

void RFTide::progMessage(byte nodeAddress, byte nodeProfile)
{
	//program a new remote node
	#ifdef DEBUG
  		Serial.println("CMD Program Message");
	#endif
	outPacket[CommandLSB]=CMD_PRGMSG;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=nodeAddress; 
	setZeroPayload();     
	outPacket[Byte7]=nodeProfile;
	sendPacket(outPacket); 
	readAnswer();
}

void RFTide::localIoSet(byte payload)
{
	if (payload>1) {
		Serial.print("BAD payload!");
	 	exit;
	}
	#ifdef DEBUG
  		Serial.println("CMD LocalIoSet");
	#endif
	outPacket[CommandLSB]=CMD_LIOSET;
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=0;
	setZeroPayload();     
	outPacket[Byte0]=payload;
	sendPacket(outPacket);
	readAnswer();
}


void RFTide::localIoReset(byte payload)
{
	#ifdef DEBUG
  		Serial.println("CMD LocalIoReset");
	#endif
	outPacket[CommandLSB]=CMD_LIORST;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=0; 
	setZeroPayload();     
	outPacket[Byte0]=payload;
	sendPacket(outPacket); 
	readAnswer();
}

NTSTATUS RFTide::localStatus()
{
	#ifdef DEBUG
  		Serial.println("CMD LocalStatus");
	#endif
	
	outPacket[CommandLSB]=CMD_LSTATS;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=0;      
	setZeroPayload();
	sendPacket(outPacket); 	
	readAnswer();

	NetStatus.getAddress = inPacket[Byte0];
	NetStatus.getNetworkID = inPacket[Byte1]+inPacket[Byte2]<<8+inPacket[Byte3]<<16+inPacket[Byte4]<<24;
	NetStatus.getFWversion = inPacket[Byte6]*10+inPacket[Byte5];
	NetStatus.getProfile = inPacket[Byte7];	

	return NetStatus;

}

byte RFTide::getLocalAddress()
{
	return NetStatus.getAddress;
}

unsigned long RFTide::getLocalNetworkID()
{
	return NetStatus.getNetworkID;
}

int RFTide::getLocalFWversion()
{
	return NetStatus.getFWversion;
}

byte RFTide::getLocalProfile()
{
	return NetStatus.getProfile;
}

IOSTATUS RFTide::remoteIoRead(byte nodeAddress)
{
	#ifdef DEBUG
  		Serial.println("CMD LocalIoRead");
	#endif
	
	outPacket[CommandLSB]=CMD_RIORAD;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=nodeAddress;      
	setZeroPayload();
	sendPacket(outPacket); 	
	readAnswer();

	InOutStatus.getGPIN0  = inPacket[Byte0];
	InOutStatus.getGPIN1  = inPacket[Byte1];
	InOutStatus.getGPOUT0 = inPacket[Byte2];
	InOutStatus.getGPOUT1 = inPacket[Byte3];
	InOutStatus.getADC0   = inPacket[Byte4]+inPacket[Byte5]<<8;
	InOutStatus.getADC1   = inPacket[Byte6]+inPacket[Byte7]<<8;	

	return InOutStatus;
}


IOSTATUS RFTide::localIoRead()
{
	#ifdef DEBUG
  		Serial.println("CMD LocalIoRead");
	#endif
	
	outPacket[CommandLSB]=CMD_LIORAD;   
	outPacket[CommandMSB]=0x00;   
	outPacket[Address]=0;      
	setZeroPayload();
	sendPacket(outPacket); 	
	readAnswer();

	InOutStatus.getGPIN0  = inPacket[Byte0];
	InOutStatus.getGPIN1  = inPacket[Byte1];
	InOutStatus.getGPOUT0 = inPacket[Byte2];
	InOutStatus.getGPOUT1 = inPacket[Byte3];
	InOutStatus.getADC0   = inPacket[Byte4]+inPacket[Byte5]<<8;
	InOutStatus.getADC1   = inPacket[Byte6]+inPacket[Byte7]<<8;	

	return InOutStatus;
}

byte RFTide::getGPIN0()
{
	return InOutStatus.getGPIN0;
}

byte RFTide::getGPIN1()
{
	return InOutStatus.getGPIN1;
}

byte RFTide::getGPOUT0()
{
	return InOutStatus.getGPOUT0;
}

byte RFTide::getGPOUT1()
{
	return InOutStatus.getGPOUT1;
}

/************ low level data pushing commands **********/


void RFTide::setZeroPayload()
{
	// set null payload
	for (byte i=Byte0; i<=Byte7; i++) {
      	outPacket[i]=0;
	}
}

void RFTide::sendPacket(byte outByte[]) 
{
	// send 11 byte fixed size packet
	for (byte i=0; i<11; i++) {
      	softwareSerial->write(outByte[i]);
	}
}

void RFTide::readAnswer() 
{
	unsigned long time = millis();
	// wait for answer

	while ((millis() - time < TIMEOUTANSWER) &&  (softwareSerial->available() < 11) ) {
	}

	if ((softwareSerial->available())==0) {
		// empty buffer NO ANSWER
		#ifdef DEBUG
			Serial.println("NO HANSWER!");
		#endif
		Status = NOANSWER;
		for (byte i=0; i<11; i++) {
			inPacket[i] = 0;
		}
	} 
	else if ((softwareSerial->available())<11) {
		// packet non complete size < 11 byte
		Status = BRKANSWER;
	}
	else if ((softwareSerial->available())==11) {
		// packet OK read 11 byte fixed size
		#ifdef DEBUG
  			Serial.print("HANSWER (bin): ");
		#endif
		for (byte i=0; i<11; i++) {
     			inPacket[i] = softwareSerial->read();
			#ifdef DEBUG
				Serial.print(inPacket[i], BIN);
				Serial.print(".");
			#endif
		}
		#ifdef DEBUG
  			Serial.println();
		#endif
		if (inPacket[CommandLSB]==CMD_ANSMSG) {
			Status = ANSWER; //is answer
		}
		else if (inPacket[CommandLSB]==CMD_SNDPCK) {
			Status = RECPACKET;
		}
		else {
			Status = BADANSWER;
		}
	}
	else {
		// buffer over size >11 byte
		#ifdef DEBUG
			Serial.print("BAD BUFFER!");
		#endif
		Status = BADANSWER;
		// clear buffer
		while (softwareSerial->available() > 0)  {
			char c = char(softwareSerial->read());
			Serial.print(c, BIN);
			Serial.print(".");
		}
		for (byte i=0; i<11; i++) {
     			inPacket[i] = 0;
		}
	}
}