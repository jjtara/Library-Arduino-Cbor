#include "CborEncoder.h"
#include <Wire.h>





void setup() {
    Wire.begin(2);                // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
 Serial.begin(9600);
// writting();
}

void loop() {

  test1();
  delay(10000);
  
}

void test1() {
    CborStaticOutput output(32);
    CborWriter writer(output);
    writer.writeInt(124);      
    //writer.writeString("I");

        
    unsigned int sizeee = output.getSize();
    unsigned char *ddata = output.getData();
    //Serial.print("datalength:");
    //Serial.println(sizeee);
    Serial.print(*ddata);
}
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{


  //unsigned char *data = output.getData();
  //int i = output.getSize();
  
  //Serial.print(*data);
  //Serial.print(i);

  //Wire.write(*data); // respond with message of 6 bytes
  // as expected by master
}
