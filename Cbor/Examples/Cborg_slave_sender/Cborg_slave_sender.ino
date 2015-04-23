#include "CborEncoder.h"
#include <Wire.h>


  CborStaticOutput output(32);
  CborWriter writer(output);

int i;


void setup() {
    Wire.begin(2);                // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
 Serial.begin(9600);
// writting();
}

void loop() {

  writting();
  delay(3000);
  
}

void writting()
{

  writer.writeString("hello");

  unsigned char *data = output.getData();
  int i = output.getSize();
  
  Serial.print(*data);
  Serial.print(i);


}


// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  
  writer.writeTag(123);
  writer.writeArray(3);
  //writer.writeString("hello");
  writer.writeString("xun");

  unsigned char *data = output.getData();
  int i = output.getSize();
  
  //Serial.print(*data);
  //Serial.print(i);

  Wire.write(*data); // respond with message of 6 bytes
  // as expected by master
}
