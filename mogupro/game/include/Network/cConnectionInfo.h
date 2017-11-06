#pragma once
#include <vector>
#include <Network/NetworkDefines.h>
namespace Network
{
class cConnectionInfo
{
public:
	cConnectionInfo( ubyte1 idCount );
public:
	std::vector<char> buffer;
	float closeSecond;
	ubyte1 id;
};
}
