#include <Network/cReliable.h>
namespace Network
{
cReliable::cReliable( std::vector<char>&& buffer )
	: mBuffer( std::move( buffer ) )
	, mCount( RELIABLE_COUNT )
{

}
bool cReliable::isFinished( )
{
	return mCount == 0U;
}
std::vector<char> const & cReliable::get( )
{
	mCount--;
	return mBuffer;
}
}
