

#include "CborEncoder.h"


void setup() {

  Serial.begin(9600);
  
}

void loop() {
	
  Sender();
}



void Sender() {
    
    CborStaticOutput output(32);
    CborWriter writer(output);

    writer.writeInt(124);
    writer.writeString("Hola");

    unsigned int sizeee = output.getSize();
    unsigned char *databuf = new unsigned char [sizeee];
    databuf = output.getData();  
    Serial.write(databuf,sizeee);
    delay(1000);
}

