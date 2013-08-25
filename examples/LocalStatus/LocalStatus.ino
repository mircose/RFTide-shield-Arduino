/* localNetwork Status EXAMPLE 

 RFTide Library EXAMPLE
 Network status in local module
 Circuit: RFTide shield V1.0
 
 created 30 July 2013
 by Mirco Segatello
 
 */
 
// include library
#include <RFTide.h>
#include <SoftwareSerial.h>


// Define RFTide shield pin
byte TX  = 4;
byte RX  = 5;
byte EN  = 7;

// Create object rftide
RFTide rftide(RX, TX, EN);

void setup() {
  Serial.begin(9600);
  Serial.println("RFTide Local Network status");
  rftide.enable(); 
}

void loop() 
{   
  //mode 1
  rftide.localStatus();
  Serial.print("Address: ");
  Serial.println(rftide.getLocalAddress());
  Serial.print("NetworkID: ");
  Serial.println(rftide.getLocalNetworkID());
  Serial.print("FWversion: ");
  Serial.println(rftide.getLocalFWversion());
  Serial.print("Profile: ");
  Serial.println(rftide.getLocalProfile());
  Serial.println();  
  
  //mode 2
/*  
  NTSTATUS NetworkStatus = rftide.localStatus();  
  Serial.print ("Address: ");
  Serial.println(NetworkStatus.getAddress);
  Serial.print("NetworkID: ");
  Serial.println(NetworkStatus.getNetworkID);
  Serial.print("FWversione: ");
  Serial.println(NetworkStatus.getFWversion);
  Serial.print("Profile: ");
  Serial.println(NetworkStatus.getProfile);
  Serial.println();
*/
  
  //mode 3
/*  
  Serial.print("FWversion: ");
  Serial.println(rftide.localStatus().getFWversion);
*/ 
  
  delay(2000);
}
