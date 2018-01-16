#include <Utility/cIdentifier.h>
#include <limits>
namespace Utility
{
cIdentifier::cIdentifier( )
	: min( std::numeric_limits<int>::min( ) )
	, max( std::numeric_limits<int>::max( ) )
{

}
cIdentifier::cIdentifier( const int min, const int max )
	: min( min )
	, max( max )
{
}
const int cIdentifier::createId( )
{
	for ( int i = min; i < max; ++i )
	{
		const auto handle = handles.find( i );
		if ( handle == handles.cend( ) )
		{
			return *handles.insert( i ).first;
		}
	}
	throw IDNotCreate( );
}
void cIdentifier::removeId( const int id )
{
	handles.erase( id );
}
cIdentifier::~cIdentifier( )
{

}
}
