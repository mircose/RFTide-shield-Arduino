/* SendPacket EXAMPLE 

 RFTide Library EXAMPLE
 Example use SendPacket function
 Circuit: RFTide shield V1.0
 Sure activate NL function in serial monitor
 
 created 30 July 2013
 by Mirco Segatello
 
 Send a fixed length 8 byte packet to remote module
 */
 
// include library
#include <RFTide.h>
#include <SoftwareSerial.h>

// Define RFTide shield pin
byte TX  = 4;
byte RX  = 5;
byte EN  = 7;

// remote module to send a packet
byte remoteAddress = 2;

// Create object rftide
RFTide rftide(RX, TX, EN);

void setup() {
  Serial.begin(9600);
  Serial.println("RFTide SendPacket example");
  rftide.enable();
}

void loop()
{
  Serial.println();  
  Serial.println("Enter string to send");
  char txtMsg[200];
  readSerial(txtMsg);
  Serial.print("Sending: ");
  Serial.println(txtMsg);
  Serial.println();
  
  rftide.sendPacket(remoteAddress, txtMsg);
  
  //checks whether the message has been received
  if (rftide.error()==NOERROR) 
    Serial.println("Message received");
  else
    Serial.println("Message is not received!");   
}


//  Read input serial message
int readSerial(char result[])
{
  int i = 0;
  while(1)
  {
    while (Serial.available() > 0)
    {
      char inChar = Serial.read();
      if (inChar == '\n')
      {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if(inChar!='\r')
      {
        result[i] = inChar;
        i++;
      }
    }
  }
}

