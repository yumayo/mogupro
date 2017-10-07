#include <Network/cUDPManager.h>
namespace Network
{
cUDPManager::cUDPManager( )
    : cSingletonAble( )
    , server( 25565 )
{
}
}