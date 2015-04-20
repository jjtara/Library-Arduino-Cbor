/*
   Copyright 2014-2015 Stanislav Ovsyannikov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.
*/
#ifndef CBOR_H
#define CBOR_H




#include "Arduino.h"

#define INT_MAX 4
#define INT_MIN 4


typedef enum {
	STATE_TYPE,
	STATE_PINT,	
    STATE_NINT,
    STATE_BYTES_SIZE,
    STATE_BYTES_DATA,
    STATE_STRING_SIZE,
    STATE_STRING_DATA,
    STATE_ARRAY,
    STATE_MAP,
    STATE_TAG,
    STATE_SPECIAL,
    STATE_ERROR
} CborReaderState;




class CborOutput {
public:
    virtual unsigned char *getData() = 0;
    virtual unsigned int getSize() = 0;
    virtual void putByte(unsigned char value) = 0;
    virtual void putBytes(const unsigned char *data, int size) = 0;
};

class CborStaticOutput : public CborOutput {
public:
	CborStaticOutput(unsigned int capacity);
	~CborStaticOutput();
	virtual unsigned char *getData();
	virtual unsigned int getSize();
	virtual void putByte(unsigned char value);
	virtual void putBytes(const unsigned char *data, int size);
private:
	unsigned char *buffer;
	unsigned int capacity;
	unsigned int offset;
};




#endif


