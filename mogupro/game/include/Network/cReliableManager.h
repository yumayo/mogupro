#pragma once
#include <Network/cReliable.h>
#include <Network/cUDP.h>
#include <deque>
namespace Network
{
class cReliableManager
{
public:
    cReliableManager( );
    ~cReliableManager( );
	void append( std::vector<char>&& sendData );
	std::vector<char> update( );
private:
	std::deque<cReliable> mReliables;
};
}
