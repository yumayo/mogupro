#pragma once
#include <vector>
#include <Network/NetworkDefines.h>
namespace Network
{
class cReliable
{
public:
    cReliable( std::vector<char>&& buffer );
	bool isFinished( );
	std::vector<char> const& get( );
private:
	std::vector<char> mBuffer; // 生データ
	ubyte1 mCount; // 残送信回数
};
}
