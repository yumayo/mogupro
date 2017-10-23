#include <Game/Trans/cServerAdapter.h>
#include <Network/cUDPServerManager.h>
#include <Network/cUDPManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cFieldManager.h>
namespace Game
{
namespace Trans
{
cServerAdapter::cServerAdapter( )
{

}
cServerAdapter::~cServerAdapter( )
{

}
void cServerAdapter::sendPlayersPosition( )
{
    auto m = Network::cDeliverManager::getInstance( );
    while ( auto packet = m->getDliPlayer( ) )
    {
        auto id = mHandlePlayers[packet->mNetworkHandle];
        //Game::cPlayerManager::getInstance( )->setPlayerPosition( 
        //    id, 
        //    packet->mPosition, 
        //    packet->mRotation
        //);
    }
    //std::vector<cinder::vec3> const& playersPosition = Game::cPlayerManager::getInstance( )->getPlayersPosition( );
    //std::vector<cinder::quat> const& playersRotation = Game::cPlayerManager::getInstance( )->getPlayersRotation( );
    //for ( auto& players : mHandlePlayers )
    //{
    //    for(int i = 0; i <  )
    //    cUDPManager::getInstance( )->send( players.first,
    //                                       new );
    //}
}
void cServerAdapter::sendSetQuarry( )
{
}
void cServerAdapter::sendGetGemPlayer( )
{
}
void cServerAdapter::sendGetGemQuarry( )
{
}
void cServerAdapter::sendBreakBlocks( )
{
}
}
}
