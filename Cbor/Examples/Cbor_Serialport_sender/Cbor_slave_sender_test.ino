#include "CborEncoder.h"


void setup() {
  Serial.begin(9600);
}

void loop() {

  Serialportsender();
  delay(5000);
  
}

void Serialportsender() {
    CborStaticOutput output(32);
    CborWriter writer(output);    
    writer.writeString("I");
    unsigned int sizepkg = output.getSize();
    unsigned char *ddata = output.getData();
    Serial.print(*ddata);

}

