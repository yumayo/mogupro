#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDPManager.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
namespace Network
{
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
public:
    void setAllPlayerPosition( );
private:
    void setPlayerPosition( int playerId, cinder::vec3 const & position, cinder::quat const& rotation );
};
}