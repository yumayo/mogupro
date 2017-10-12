#pragma once
#include <winsock2.h>

static bool exportInt(unsigned int& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(unsigned int)))
		return false;
	if (buffer == NULL)return false;
	unsigned int c = htonl(value);
	memcpy(buffer + offset, &c, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	return true;
}

static bool importInt(unsigned int& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(unsigned int)))
		return false;
	unsigned int c;
	memcpy(&c, buffer + offset, sizeof(unsigned int));
	value = ntohl(c);
	offset += sizeof(unsigned int);
	return true;
}

static bool exportShort(short& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(short)))
		return false;
	if (buffer == NULL)return false;
	short c = htons(value);
	memcpy(buffer + offset, &c, sizeof(short));
	offset += sizeof(short);
	return true;
}

static bool importShort(short& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(short)))
		return false;
	short c;
	memcpy(&c, buffer + offset, sizeof(short));
	value = ntohs(c);
	offset += sizeof(short);
	return true;
}

static bool exportChar(char& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(char)))
		return false;
	if (buffer == NULL)return false;
	memcpy(buffer + offset, &value, sizeof(char));
	offset += sizeof(char);
	return true;
}

static bool importStr(char& value, char* buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(char)))
		return false;
	memcpy(&value, buffer + offset, sizeof(char));
	offset += sizeof(char);
	return true;
}

static bool exportStr(char* value, char* buffer, int& offset, int totalSize, size_t valueSize)
{
	if (totalSize < int(offset + sizeof(valueSize)))
		return false;
	if (buffer == NULL)return false;
	memcpy(buffer + offset, value, sizeof(valueSize));
	offset += sizeof(valueSize);
	return true;
}

static bool importStr(char* value, char* buffer, int& offset, int totalSize, size_t valueSize)
{
	if (totalSize < int(offset + sizeof(valueSize)))
		return false;
	memcpy(value, buffer + offset, sizeof(valueSize));
	offset += sizeof(valueSize);
	return true;
}

//!@ LookMe : Float‚É‘Î‚·‚éhtonl,ntohl‚Í‚Å‚«‚È‚¢‚Ì‚Å‚Ç‚¤‚·‚é‚©
static bool exportFloat(const float& value, char* const buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(float)))
		return false;
	if (buffer == NULL)return false;
	memcpy(buffer + offset, &value, sizeof(float));
	offset += sizeof(float);
	return true;
}

static bool importFloat(float& value, const char* const buffer, int& offset, int totalSize)
{
	if (totalSize < int(offset + sizeof(float)))
		return false;
	float c;
	memcpy(&value, buffer + offset, sizeof(float));
	offset += sizeof(float);
	return true;
}