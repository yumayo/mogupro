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
        , type( 0 )
    {

    }
    BlockFormat( cinder::vec3 const& position, float radius, ubyte1 type )
        : position( position )
        , radius( radius )
        , type(type)
    {

    }
    cinder::vec3 position;
    float radius;
    ubyte1 type;
};

struct PlayerFormat
{
    PlayerFormat( )
        : playerId( 0U )
        , position( cinder::vec3( 0.0F ) )
        , rotation( cinder::vec2( 0.0F ) )
    {

    }
    PlayerFormat( ubyte1 playerId, cinder::vec3 const& position, cinder::vec2 const& rotation )
        : playerId( playerId )
        , position( position )
        , rotation( rotation )
    {

    }
    ubyte1 playerId;
    cinder::vec3 position;
    cinder::vec2 rotation;
};
}
}
