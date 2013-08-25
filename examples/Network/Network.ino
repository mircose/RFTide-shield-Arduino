/* Network create EXAMPLE 

 RFTide Library EXAMPLE
 Learn new module and scan Network
 Circuit: RFTide shield V1.0
 
 created 30 July 2013
 by Mirco Segatello
 
  To add a module to the network:
    press the LEARN button on module to associate,
    wait for LED steady on,
    send address to assign to the remote module (0-5)
    the LED on remote module go off. 
 
  For list of the modules in the network send a 'p' char 
  (the local module do not response)
 */
 
// include library
#include <RFTide.h>
#include <SoftwareSerial.h>

// Define RFTide shield pin
byte TX  = 4;
byte RX  = 5;
byte EN  = 7;
byte c;
// Create object rftide
RFTide rftide(RX, TX, EN);

void setup() {
  Serial.begin(9600);
  Serial.println("RFTide make a network example");
  Serial.println("To add a module to the network send a remote address (0-5)");
  Serial.println("For list of the modules in the network send a 'p' char");
  Serial.println();
  rftide.enable();
}

void loop() { 
  
  if (Serial.available())
  {
    c = Serial.read();
    if (c=='p') {
      Serial.println("Elenco moduli in rete:");
      for (int i=0; i<6; i++) {
          
          NTSTATUS Populate = rftide.ping(i); 
         // if (Populate.getFWversion>0) {
          if (rftide.error()==NOERROR) { 
            Serial.print("AddrSend");
            Serial.print(i);
            Serial.print(" AddrRemote:"); 
            Serial.print(Populate.getAddress);
            Serial.print(" FWversion:");
            Serial.print(Populate.getFWversion);
            Serial.print(" Profile: ");
            Serial.println(Populate.getProfile);   
            }
        }
      Serial.println();
    }  
    if (('0'<=c) && (c<='5')) {
      Serial.print("Learn module with Address=");      
      Serial.println(char(c));
      rftide.progMessage(c-48, 0);
      Serial.println();      
      }
   } 
}
