#include "CborDecoder.h"
#include "Arduino.h"
#include <stdlib.h>



CborInput::CborInput(void *data, int size) {
	this->data = (unsigned char *)data;
	this->size = size;
	this->offset = 0;
}

CborInput::~CborInput() {

}

bool CborInput::hasBytes(int count) {
	return size - offset >= count;
}

unsigned char CborInput::getByte() {
	return data[offset++];
}

unsigned short CborInput::getShort() {
	unsigned short value = ((unsigned short)data[offset] << 8) | ((unsigned short)data[offset + 1]);
	offset += 2;
	return value;
}

unsigned int CborInput::getInt() {
	unsigned int value = ((unsigned int)data[offset] << 24) | ((unsigned int)data[offset + 1] << 16) | ((unsigned int)data[offset + 2] << 8) | ((unsigned int)data[offset + 3]);
	offset += 4;
	return value;
}

unsigned long long CborInput::getLong() {
	unsigned long long value = ((unsigned long long)data[offset] << 56) | ((unsigned long long)data[offset+1] << 48) | ((unsigned long long)data[offset+2] << 40) | ((unsigned long long)data[offset+3] << 32) | ((unsigned long long)data[offset+4] << 24) | ((unsigned long long)data[offset+5] << 16) | ((unsigned long long)data[offset+6] << 8) | ((unsigned long long)data[offset+7]); 
	offset += 8;
	return value;
}

void CborInput::getBytes(void *to, int count) {
	memcpy(to, data + offset, count);
	offset += count;
}


CborReader::CborReader(CborInput &input) {
	this->input = &input;
	this->state = STATE_TYPE;
}

CborReader::CborReader(CborInput &input, CborListener &listener) {
	this->input = &input;
	this->listener = &listener;
	this->state = STATE_TYPE;
}

CborReader::~CborReader() {

}

void CborReader::SetListener(CborListener &listener) {
	this->listener = &listener;
}

void CborReader::Run() {
    unsigned int temp;
	while(1) {
		if(state == STATE_TYPE) {
			if(input->hasBytes(1)) {
				unsigned char type = input->getByte();
				unsigned char majorType = type >> 5;
				unsigned char minorType = type & 31;

				switch(majorType) {
					case 0: // positive integer
						if(minorType < 24) {
							listener->OnInteger(minorType);
						} else if(minorType == 24) { // 1 byte
							currentLength = 1;
							state = STATE_PINT;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_PINT;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_PINT;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_PINT;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid integer type");
						}
						break;
					case 1: // negative integer
						if(minorType < 24) {
							listener->OnInteger(-minorType);
						} else if(minorType == 24) { // 1 byte
							currentLength = 1;
							state = STATE_NINT;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_NINT;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_NINT;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_NINT;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid integer type");
						}
						break;
					case 2: // bytes
						if(minorType < 24) {
							state = STATE_BYTES_DATA;
							currentLength = minorType;
						} else if(minorType == 24) {
							state = STATE_BYTES_SIZE;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_BYTES_SIZE;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_BYTES_SIZE;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_BYTES_SIZE;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid bytes type");
						}
						break;
					case 3: // string
						if(minorType < 24) {
							state = STATE_STRING_DATA;
							currentLength = minorType;
						} else if(minorType == 24) {
							state = STATE_STRING_SIZE;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_STRING_SIZE;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_STRING_SIZE;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_STRING_SIZE;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid string type");
						}
						break;
					case 4: // array
						if(minorType < 24) {
							listener->OnArray(minorType);
						} else if(minorType == 24) {
							state = STATE_ARRAY;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_ARRAY;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_ARRAY;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_ARRAY;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid array type");
						}
						break;
					case 5: // map
						if(minorType < 24) {
							listener->OnMap(minorType);
						} else if(minorType == 24) {
							state = STATE_MAP;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_MAP;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_MAP;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_MAP;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid array type");
						}
						break;
					case 6: // tag
						if(minorType < 24) {
							listener->OnTag(minorType);
						} else if(minorType == 24) {
							state = STATE_TAG;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_TAG;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_TAG;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_TAG;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid tag type");
						}
						break;
					case 7: // special
						if(minorType < 24) {
							listener->OnSpecial(minorType);
						} else if(minorType == 24) {
							state = STATE_SPECIAL;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_SPECIAL;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_SPECIAL;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_SPECIAL;
						} else {
                            state = STATE_ERROR;
							listener->OnError("invalid special type");
						}
						break;
				}
			} else break;
		} else if(state == STATE_PINT) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						listener->OnInteger(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						listener->OnInteger(input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
                        temp = input->getInt();
                        if(temp <= INT_MAX) {
                            listener->OnInteger(temp);
                        } else {
                            listener->OnExtraInteger(temp, 1);
                        }
						state = STATE_TYPE;
						break;
					case 8:
                        listener->OnExtraInteger(input->getLong(), 1);
						state = STATE_TYPE;
						break;
				}
			} else break;
		} else if(state == STATE_NINT) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						listener->OnInteger(-(int)input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						listener->OnInteger(-(int)input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						temp = input->getInt();
                        if(temp <= INT_MAX) {
                            listener->OnInteger(-(int) temp);
                        } else if(temp == 2147483648u) {
                            listener->OnInteger(INT_MIN);
                        } else {
                            listener->OnExtraInteger(temp, -1);
                        }
						state = STATE_TYPE;
						break;
					case 8:
						listener->OnExtraInteger(input->getLong(), -1);
						break;
				}
			} else break;
		} else if(state == STATE_BYTES_SIZE) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						currentLength = input->getByte();
						state = STATE_BYTES_DATA;
						break;
					case 2:
						currentLength = input->getShort();
						state = STATE_BYTES_DATA;
						break;
					case 4:
						currentLength = input->getInt();
						state = STATE_BYTES_DATA;
						break;
					case 8:
                        state = STATE_ERROR;
                        listener->OnError("extra long bytes");
						break;
				}
			} else break;
		} else if(state == STATE_BYTES_DATA) {
			if(input->hasBytes(currentLength)) {
				unsigned char *data = new unsigned char[currentLength];
				input->getBytes(data, currentLength);
				state = STATE_TYPE;
				listener->OnBytes(data, currentLength);
			} else break;
		} else if(state == STATE_STRING_SIZE) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						currentLength = input->getByte();
						state = STATE_STRING_DATA;
						break;
					case 2:
						currentLength = input->getShort();
						state = STATE_STRING_DATA;
						break;
					case 4:
						currentLength = input->getInt();
						state = STATE_STRING_DATA;
						break;
					case 8:
                        state = STATE_ERROR;
                        listener->OnError("extra long array");
                        break;
				}
			} else break;
		} else if(state == STATE_STRING_DATA) {
			if(input->hasBytes(currentLength)) {
				unsigned char *data = new unsigned char[currentLength];
				input->getBytes(data, currentLength);
				state = STATE_TYPE;
				String str(*data);
				listener->OnString(str);
			} else break;
		} else if(state == STATE_ARRAY) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						listener->OnArray(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						listener->OnArray(currentLength = input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						listener->OnArray(input->getInt());
						state = STATE_TYPE;
						break;
                    case 8:
                        state = STATE_ERROR;
                        listener->OnError("extra long array");
						break;
				}
			} else break;
		} else if(state == STATE_MAP) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						listener->OnMap(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						listener->OnMap(currentLength = input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						listener->OnMap(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
                        state = STATE_ERROR;
                        listener->OnError("extra long map");
						break;
				}
			} else break;
		} else if(state == STATE_TAG) {
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						listener->OnTag(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						listener->OnTag(input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						listener->OnTag(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						listener->OnExtraTag(input->getLong());
						state = STATE_TYPE;
						break;
				}
			} else break;
		} else if(state == STATE_SPECIAL) {
            if (input->hasBytes(currentLength)) {
                switch (currentLength) {
                    case 1:
                        listener->OnSpecial(input->getByte());
                        state = STATE_TYPE;
                        break;
                    case 2:
                        listener->OnSpecial(input->getShort());
                        state = STATE_TYPE;
                        break;
                    case 4:
                        listener->OnSpecial(input->getInt());
                        state = STATE_TYPE;
                        break;
                    case 8:
                        listener->OnExtraSpecial(input->getLong());
                        state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(state == STATE_ERROR) {
            break;
		} else {
			Serial.print("UNKNOWN STATE");
		}
	}
}



  void CborExampleListener::OnInteger(int value) {
    printf("integer: %d\n", value);
  }

  void CborExampleListener::OnBytes(unsigned char *data, int size) {
    printf("bytes with size: %d", size);
  }

  void CborExampleListener::OnString(String &str) {
    printf("string: '%.*s'\n", (int)str.size(), str.c_str());
  }

  void CborExampleListener::OnArray(int size) {
    printf("array: %d\n", size);
  }

  void CborExampleListener::OnMap(int size) {
    printf("map: %d\n", size);
  }

  void CborExampleListener::OnTag(unsigned int tag) {
    printf("tag: %d\n", tag);
  }

  void CborExampleListener::OnSpecial(int code) {
    printf("special: %d\n", code);
  }

  void CborExampleListener::OnError(const char *error) {
    printf("error: %s\n", error);
  }

