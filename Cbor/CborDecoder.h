#ifndef CBORDE_H
#define CBORDE_H

#include "Arduino.h"

#define INT_MAX 2
#define INT_MIN 2


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

class CborInput {
	
public:
	CborInput(void *data, int size);
	~CborInput();

	bool hasBytes(int count);
	unsigned char getByte();
	unsigned short getShort();
	unsigned int getInt();
	unsigned long long getLong();
	void getBytes(void *to, int count);
private:
	unsigned char *data;
	int size;
	int offset;
};




class CborListener {
public:
	virtual void OnInteger(int value) = 0;
	virtual void OnBytes(unsigned char *data, int size) = 0;
	virtual void OnString(String &str) = 0;
	virtual void OnArray(int size) = 0;
	virtual void OnMap(int size) = 0;
	virtual void OnTag(unsigned int tag) = 0;
	virtual void OnSpecial(unsigned int code) = 0;
	virtual void OnError(const char *error) = 0;
    virtual void OnExtraInteger(unsigned long long value, int sign) {}
    virtual void OnExtraTag(unsigned long long tag) {}
    virtual void OnExtraSpecial(unsigned long long tag) {}
};

class CborDebugListener : public CborListener {
public:
	virtual void OnInteger(int value);
	virtual void OnBytes(unsigned char *data, int size);
	virtual void OnString(String &str);
	virtual void OnArray(int size);
	virtual void OnMap(int size);
	virtual void OnTag(unsigned int tag);
	virtual void OnSpecial(unsigned int code);
	virtual void OnError(const char *error);

    virtual void OnExtraInteger(unsigned long long value, int sign);
    virtual void OnExtraTag(unsigned long long tag);
    virtual void OnExtraSpecial(unsigned long long tag);
};

class CborReader {
public:
	CborReader(CborInput &input);
	CborReader(CborInput &input, CborListener &listener);
	~CborReader();
	void Run();
	void SetListener(CborListener &listener);
	void GetCborData(String &Cborpackage);
private:
	CborListener *listener;
	CborInput *input;
	CborReaderState state;
	int currentLength;
};


class CborExampleListener : public CborListener {
  public:
    void OnInteger(int value);
    void OnBytes(unsigned char *data, int size);
    void OnString(String &str);
    void OnArray(int size);
    void OnMap(int size) ;
    void OnTag(unsigned int tag);
    void OnSpecial(int code);
    void OnError(const char *error);
  };



#endif
