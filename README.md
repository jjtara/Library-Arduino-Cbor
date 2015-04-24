#Library-Arduino-Cbor
Arduino Library to use Cbor encoder and decoder in Arduino BOARDS based in https://github.com/naphaso/cbor-cpp library.

I included several sketches to try the library basically:
 -cbortest: encode and decode in one board cbor message.
 -Cbor_Serialport_sender: Encode a Cbor package and send through serialport  
 -Cbor_Master_Reader: I2C Master reader a cbor package from a Slave Arduino (!!!! NOT WORKING YET)
 -Cbor_Slave_sender:  I2C Slave sender a cbor package to a Master Arduino



If you want to see how Encode and decode the library in one board, you might be want load in any arduino board the sketch included in the library called "cbortest"


 
###Install:
Just copy the folder Cbor in your sketchbook/libraries Folder

###Use:

Open Arduino IDE ( without it difficult :D)
File -> Examples -> Cbor -> cbortest
