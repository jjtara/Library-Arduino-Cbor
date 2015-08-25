#include "CborEncoder.h"
#include "Arduino.h"
#include <stdlib.h>





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
		Serial.print("buffer overflow error");
	}
}

void CborStaticOutput::putBytes(const unsigned char *data, int size) {
	if(offset + size - 1 < capacity) {
		memcpy(buffer + offset, data, size);
		offset += size;
	} else {
		Serial.print("buffer overflow error");
	}
}



CborWriter::CborWriter(CborOutput &output) {
	this->output = &output;
}

CborWriter::~CborWriter() {

}

unsigned char *CborStaticOutput::getData() {
	return buffer;
}

unsigned int CborStaticOutput::getSize() {
	return offset;
}


CborDynamicOutput::CborDynamicOutput() {
    init(256);
}

CborDynamicOutput::CborDynamicOutput(unsigned int initalCapacity) {
    init(initalCapacity);
}

CborDynamicOutput::~CborDynamicOutput() {
    delete buffer;
}

void CborDynamicOutput::init(unsigned int initalCapacity) {
    this->capacity = initalCapacity;
    this->buffer = new unsigned char[initalCapacity];
    this->offset = 0;
}


unsigned char *CborDynamicOutput::getData() {
    return buffer;
}

unsigned int CborDynamicOutput::getSize() {
    return offset;
}

void CborDynamicOutput::putByte(unsigned char value) {
    if(offset < capacity) {
        buffer[offset++] = value;
    } else {
        capacity *= 2;
        buffer = (unsigned char *) realloc(buffer, capacity);
        buffer[offset++] = value;
    }
}

void CborDynamicOutput::putBytes(const unsigned char *data, int size) {
    while(offset + size > capacity) {
        capacity *= 2;
        buffer = (unsigned char *) realloc(buffer, capacity);
    }

    memcpy(buffer + offset, data, size);
    offset += size;
}



void CborWriter::writeTypeAndValue(int majorType, unsigned int value) {
	majorType <<= 5;
	if(value < 24) {
		output->putByte(majorType | value);
	} else if(value < 256) {
		output->putByte(majorType | 24);
		output->putByte(value);
	} else if(value < 65536) {
		output->putByte(majorType | 25);
		output->putByte(value >> 8);
		output->putByte(value);
	} else {
		output->putByte(majorType | 26);
		output->putByte(value >> 24);
		output->putByte(value >> 16);
		output->putByte(value >> 8);
		output->putByte(value);
	}
}

void CborWriter::writeTypeAndValue(int majorType, unsigned long long value) {
	majorType <<= 5;
	if(value < 24ULL) {
		output->putByte(majorType | value);
	} else if(value < 256ULL) {
		output->putByte(majorType | 24);
		output->putByte(value);
	} else if(value < 65536ULL) {
		output->putByte(majorType | 25);
		output->putByte(value >> 8);
	} else if(value < 4294967296ULL) {
		output->putByte(majorType | 26);
		output->putByte(value >> 24);
		output->putByte(value >> 16);
		output->putByte(value >> 8);
		output->putByte(value);
	} else {
		output->putByte(majorType | 27);
		output->putByte(value >> 56);
		output->putByte(value >> 48);
		output->putByte(value >> 40);
		output->putByte(value >> 32);
		output->putByte(value >> 24);
		output->putByte(value >> 16);
		output->putByte(value >> 8);
		output->putByte(value);
	}
}

void CborWriter::writeInt(unsigned int value) {
	writeTypeAndValue(0, value);
}

void CborWriter::writeInt(unsigned long long value) {
	writeTypeAndValue(0, value);
}

void CborWriter::writeInt(long long value) {
	if(value < 0) {
		writeTypeAndValue(1, (unsigned long long) -(value+1));
	} else {
		writeTypeAndValue(0, (unsigned long long) value);
	}
}

void CborWriter::writeInt(int value) {
	if(value < 0) {
		writeTypeAndValue(1, (unsigned int) -(value+1));
	} else {
		writeTypeAndValue(0, (unsigned int) value);
	}
}

void CborWriter::writeBytes(const unsigned char *data, unsigned int size) {
	writeTypeAndValue(2, size);
	output->putBytes(data, size);
}

void CborWriter::writeString(const char *data, unsigned int size) {
	writeTypeAndValue(3, size);
	output->putBytes((const unsigned char *)data, size);
}

void CborWriter::writeString(const String str) {
	writeTypeAndValue(3, (unsigned int)str.length());
	output->putBytes((const unsigned char *)str.c_str(), str.length());
}


void CborWriter::writeArray(int size) {
	writeTypeAndValue(4, (unsigned int)size);
}

void CborWriter::writeMap(int size) {
	writeTypeAndValue(5, (unsigned int)size);
}

void CborWriter::writeTag(const unsigned int tag) {
	writeTypeAndValue(6, tag);
}

void CborWriter::writeSpecial(int special) {
	writeTypeAndValue(7, (unsigned int)special);
}
