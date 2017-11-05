#include <Network/cReliableManager.h>
namespace Network
{
cReliableManager::cReliableManager( )
{

}
cReliableManager::~cReliableManager( )
{

}
void cReliableManager::append( std::vector<char>&& sendData )
{
	mReliables.emplace_back( std::move( sendData ) );
}
std::vector<char> cReliableManager::update( )
{
	// 規定回数送ったデータは削除されます。
	auto itr = mReliables.begin( );
	while ( itr != mReliables.end( ) )
	{
		if ( !itr->isFinished( ) ) break;
		else
		{
			itr = mReliables.erase( itr );
		}
	}
	// 詰められて上位のクラスに出荷されます。
	std::vector<char> sendData;
	for ( auto& o : mReliables )
	{
		auto reliableData = o.get( );
		std::copy( reliableData.begin( ), reliableData.end( ), std::back_inserter( sendData ) );
	}
	return sendData;
}
}
