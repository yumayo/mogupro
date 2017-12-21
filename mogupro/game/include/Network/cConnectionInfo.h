#pragma once
#include <vector>
#include <Network/NetworkDefines.h>
#include <Network/cReliableManager.h>
namespace Network
{
class cConnectionInfo
{
public:
	explicit cConnectionInfo( ubyte1 idCount );
public:
	cReliableManager reliableManager;
	std::vector<char> buffer;
	float closeSecond;
	ubyte1 id;
};
}
