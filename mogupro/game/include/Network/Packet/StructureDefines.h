#pragma once
#include <Network/NetworkDefines.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
namespace Network
{
namespace Packet
{
struct BlockFormat
{
    BlockFormat( )
        : position( cinder::vec3( 0 ) )
        , radius( 0.0F )
    {

    }
    BlockFormat( cinder::vec3 const& position, float radius )
        : position( position )
        , radius( radius )
    {

    }
    cinder::vec3 position;
    float radius;
};

struct PlayerFormat
{
    PlayerFormat( )
        : playerId( 0U )
        , position( cinder::vec3( 0.0F ) )
        , rotation( cinder::quat( ) )
    {

    }
    PlayerFormat( ubyte1 playerId, cinder::vec3 const& position, cinder::quat const& rotation )
        : playerId( playerId )
        , position( position )
        , rotation( rotation )
    {

    }
    ubyte1 playerId;
    cinder::vec3 position;
    cinder::quat rotation;
};
}
}
