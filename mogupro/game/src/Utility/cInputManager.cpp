#include <Utility/cInputManager.h>
#include <Utility/cInput.h>
namespace Utility
{
cInputManager::cInputManager( )
{
	registerButton( "any", [ ]
	{
		cInputAll::getInstance( )->pushKey(  );
		return false;
	} );
}
void cInputManager::registerButton( std::string const & key, std::function<bool( )> method )
{
	mInputs.insert( std::make_pair( key, method ) );
}
bool cInputManager::getButton( std::string const& key )
{
	auto itr = mInputs.find( key );
	if ( itr != mInputs.end( ) )
	{
		if ( itr->second )
		{
			return itr->second( );
		}
	}
	return false;
}
}
