#include "CborEncoder.h"
#include "CborDecoder.h"

void setup() {

  Serial.begin(9600);
  //Serial.print("hola");
  test1();
}

void loop() {
  // put your main code here, to run repeatedly:

}



void test1() {
    CborStaticOutput output(32);
    CborWriter writer(output);


        writer.writeInt(124);
        writer.writeInt(125);
        writer.writeInt(126);
        writer.writeInt(157);
        writer.writeInt(157);
        writer.writeInt(157);
        writer.writeInt(157);
        
        writer.writeString("iI",2);
        writer.writeString("Vv",2);
        writer.writeString("Ll",2);
        writer.writeString("Ll",2);
        writer.writeString("Bb",2);
        writer.writeString("Cc",2);
        
    unsigned int sizeee = output.getSize();
    
    Serial.print("datalength:");
    Serial.println(sizeee);
    
    delay(1000);
    
    
    CborInput input(output.getData(), output.getSize());
    CborDebugListener listener;
    CborReader reader(input);
    reader.SetListener(listener);
    reader.Run();
}
