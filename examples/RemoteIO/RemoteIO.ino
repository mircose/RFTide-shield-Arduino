/* RemoteIO EXAMPLE 

 RFTide Library EXAMPLE
 GPIO pin control in Remote module
 Circuit: RFTide shield V1.0
 
 created 30 July 2013
 by Mirco Segatello
 */
 
// include the RFTide library
#include <RFTide.h>
#include <SoftwareSerial.h>

// Define RFTide shield pin
byte TX  = 4;
byte RX  = 5;
byte EN  = 7;

// Remote module address
byte remoteAddress = 4;

// Create object rftide
RFTide rftide(RX, TX, EN);

void setup() {
  Serial.begin(9600);
  Serial.println("RFTide REMOTE IO TEST");
  rftide.enable(); 
}

void loop() { 
  // Set remote GPOUT0 pin
  rftide.remoteIoSet(remoteAddress, GPOUT0);
  delay(1000);
  // Reset remote GPOUT0 pin
  rftide.remoteIoReset(remoteAddress, GPOUT0);
  delay(1000);

  // Read Remote IO

  IOSTATUS InOutStatus = rftide.remoteIoRead(remoteAddress);  
  Serial.print("GPIN0=");
  Serial.print(InOutStatus.getGPIN0);
  Serial.print(" GPIN1=");
  Serial.print(InOutStatus.getGPIN1);
  Serial.print(" GPOUT0=");
  Serial.print(InOutStatus.getGPOUT0);
  Serial.print(" GPOUT1=");
  Serial.println(InOutStatus.getGPOUT1); 
  
  
  // read all GPIO pin and report only GPIN0
  Serial.println(rftide.remoteIoRead(remoteAddress).getGPIN0);
  
  //read all GPIO pin and print only GPIN0
  rftide.remoteIoRead(remoteAddress);  
  Serial.println(rftide.getGPIN0()); 
}
