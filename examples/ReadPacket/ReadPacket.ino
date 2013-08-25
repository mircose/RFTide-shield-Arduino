/* ReadPacket EXAMPLE 

 RFTide Library EXAMPLE
 Example use ReadPacket function
 Circuit: RFTide shield V1.0
 
 created 30 July 2013
 by Mirco Segatello
 
 Read fixed length 8 byte packet received from remote module
 */
 
// include library
#include <RFTide.h>
#include <SoftwareSerial.h>

// Define RFTide shield pin
byte TX  = 4;
byte RX  = 5;
byte EN  = 7;

byte packet[8]; //for data read

// Create object rftide
RFTide rftide(RX, TX, EN);

void setup() {
  Serial.begin(9600);
  Serial.println("RFTide ReadPacket example...");
  rftide.enable();
}

void loop()
{
  
  byte remoteAddressFromReceive = rftide.readPacket(packet);
  
  // alternative: if (remoteAddressFromReceive<255)
  if (rftide.status()==RECPACKET)
  {  
    Serial.print("Received: ");
    for (int i=0; i<8; i++) { 
      Serial.print(packet[i], HEX); 
      Serial.print("."); 
    }
    Serial.print("  from address: ");  
    Serial.println(remoteAddressFromReceive);
  } 
}

