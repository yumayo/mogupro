#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
namespace Network
{
class cUDPManager : public Utility::cSingletonAble<cUDPManager>
{
public:
    cUDPManager( );
    cUDP server;
    cUDP client;
};
}