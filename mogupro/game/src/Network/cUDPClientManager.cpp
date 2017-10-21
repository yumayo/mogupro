#include <Network/cUDPClientManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
namespace Network
{
void cUDPClientManager::setAllPlayerPosition( )
{
    auto m = Network::cResponseManager::getInstance( );
    while ( auto packet = m->getResPlayer( ) )
    {
    }
}
void cUDPClientManager::setPlayerPosition( int playerId, cinder::vec3 const & position, cinder::quat const& rotation )
{
    // Game::cPlayerManager::getInstance( )->setPlayerPosition( playerId, position, rotation );
}
}