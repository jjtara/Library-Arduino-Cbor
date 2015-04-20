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


#include "Arduino.h"
#include "Cbor.h"
#include <stdlib.h>

#include "log.h"

using namespace std;



CborStaticOutput::CborStaticOutput(unsigned int capacity) {
	this->capacity = capacity;
	this->buffer = new unsigned char[capacity];
	this->offset = 0;
}

CborStaticOutput::~CborStaticOutput() {
	delete buffer;
}

void CborStaticOutput::putByte(unsigned char value) {
	if(offset < capacity) {
		buffer[offset++] = value;
	} else {
		logger("buffer overflow error");
	}
}

void CborStaticOutput::putBytes(const unsigned char *data, int size) {
	if(offset + size - 1 < capacity) {
		memcpy(buffer + offset, data, size);
		offset += size;
	} else {
		logger("buffer overflow error");	
	}
}

