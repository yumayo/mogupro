#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
class cUDPManager
{
public:
    virtual ~cUDPManager( ) { }
    template <class Ty, Packet::PacketId packetId>
    void send( std::string ipadress, int port, Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        auto&& packetRaw = packetBase->createPacket( );
        ubyte2& byte = std::get<0>( packetRaw );
        cBuffer& data = std::get<1>( packetRaw );
        socket.write( ipadress, port, byte, data.data( ) );

        delete packetBase;
        packetBase = nullptr;
    }
    void close( );
    void open( );
    void open( int port );
public:
    virtual void update( );
private:
    void onReceive( cPacketRaw const & raw );
protected:
    cUDP socket;
};
}