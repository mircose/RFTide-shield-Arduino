/* localIO EXAMPLE 

 RFTide Library EXAMPLE
 GPIO pin control in local module
 Circuit: RFTide shield V1.0
 
 created 30 July 2013
 by Mirco Segatello
 
 */
 
// include the RFTide library
#include <RFTide.h>
#include <SoftwareSerial.h>

// Define RFTide shield pin
byte TX  = 3;
byte RX  = 2;
byte EN  = 10;

// Create object rftide
RFTide rftide(RX, TX, EN);

void setup() {
  Serial.begin(9600);
  Serial.println("RFTide LOCALIO TEST");
  rftide.enable();
}

void loop() { 
  // Set local GPOU0T0 pin
  rftide.localIoSet(GPOUT0);
  delay(1000);
  // Reset local GPOU0T0 pin
  rftide.localIoReset(GPOUT0);
  delay(1000);
  
  // Read local all IO

  IOSTATUS InOutStatus = rftide.localIoRead();  
  Serial.print("GPIN0=");
  Serial.print(InOutStatus.getGPIN0);
  Serial.print(" GPIN1=");
  Serial.print(InOutStatus.getGPIN1);
  Serial.print(" GPOUT0=");
  Serial.print(InOutStatus.getGPOUT0);
  Serial.print(" GPOUT1=");
  Serial.println(InOutStatus.getGPOUT1); 
  
  // Alternative read local all IO (report only GPIN0 value)
  Serial.print("GPIN0=");
  Serial.println(rftide.localIoRead().getGPIN0);
  
  // Alternative read local all IO (report only GPIN0 value)
  Serial.print("GPIN0=");
  rftide.localIoRead();  
  Serial.println(rftide.getGPIN0()); 
}
