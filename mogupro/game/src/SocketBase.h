#pragma once
#include <winsock2.h>

//!@ file   cSocketBase
//!@ brief  Socket‚ÌŠî”Õ‚Å‚·
//!@ note   Œp³‚ğ•K‚¸s‚Á‚Ä‰º‚³‚¢
class cSocketBase
{
public:
	//!@ LookMe : Œp³æ‚Å“à•”ˆ—‚ğ‘‚¢‚Ä‚­‚¾‚³‚¢
	virtual void import() = 0;
	virtual void export() = 0;

	char* buff;
	int offset;
};

bool exportInt(unsigned int& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(unsigned int)))
		return false;
	if (buffer == NULL)return false;
	unsigned int c = htonl(value);
	memcpy(buffer + offset, &c, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	return true;
}

bool importInt(unsigned int& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(unsigned int)))
		return false;
	unsigned int c;
	memcpy(&c, buffer + offset, sizeof(unsigned int));
	value = ntohl(c);
	offset += sizeof(unsigned int);
	return true;
}

bool exportChar(char* value, char* buffer, int& offset, int totalSize, size_t valueSize)
{
	if (totalSize < int(offset + sizeof(valueSize)))
		return false;
	if (buffer == NULL)return false;
	unsigned int c = htonl((*(int *)value));
	memcpy(buffer + offset, &c, sizeof(valueSize));
	offset += sizeof(valueSize);
	return true;
}

bool importChar(char* value, char* buffer, int& offset, int totalSize, size_t valueSize)
{
	if (totalSize < int(offset + sizeof(valueSize)))
		return false;
	float c;
	memcpy(&c, buffer + offset, sizeof(valueSize));
	value =(char*)ntohl(c);
	offset += sizeof(valueSize);
	return true;
}

bool exportFloat(float& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(float)))
		return false;
	if (buffer == NULL)return false;
	unsigned int c = htonl(value);
	memcpy(buffer + offset, &c, sizeof(float));
	offset += sizeof(float);
	return true;
}

bool importIFloat(float& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(float)))
		return false;
	float c;
	memcpy(&c, buffer + offset, sizeof(float));
	value = ntohl(c);
	offset += sizeof(float);
	return true;
}