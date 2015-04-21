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




#ifndef CBOREN_H
#define CBOREN_H

#include "Arduino.h"

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


class CborDynamicOutput : public CborOutput {
public:
    CborDynamicOutput();
    CborDynamicOutput(unsigned int initalCapacity);
    ~CborDynamicOutput();


    virtual unsigned char *getData();
    virtual unsigned int getSize();
    virtual void putByte(unsigned char value);
    virtual void putBytes(const unsigned char *data, int size);
private:
    void init(unsigned int initalCapacity);
    unsigned char *buffer;
    unsigned int capacity;
    unsigned int offset;
};

class CborWriter {
public:
	CborWriter(CborOutput &output);
	~CborWriter();

	void writeInt(int value);
	void writeInt(long long value);
	void writeInt(unsigned int value);
	void writeInt(unsigned long long value);
	void writeBytes(const unsigned char *data, unsigned int size);
	void writeString(const char *data, unsigned int size);
	void writeString(const String str);
	void writeArray(int size);
	void writeMap(int size);
	void writeTag(const unsigned int tag);
	void writeSpecial(int special);
private:
	void writeTypeAndValue(int majorType, unsigned int value);
	void writeTypeAndValue(int majorType, unsigned long long value);
	CborOutput *output;
};

class CborSerializable {
public:
	virtual void Serialize(CborWriter &writer) = 0;
};
#endif
