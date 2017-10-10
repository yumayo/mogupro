#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
class cUDPManager : public Utility::cSingletonAble<cUDPManager>
{
public:
    template <class Ty, Packet::PacketId packetId>
    void send( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        auto&& raw = packetBase->createPacket( );
        sendDataBufferAdd( networkHandle, raw );

        delete packetBase;
        packetBase = nullptr;
    }
    void close( );
    void open( );
    void open( int port );
public:
    void update( );
private:
    void onReceive( cPacketChunk const & packetChunk );
    void sendDataBufferAdd( cNetworkHandle const& networkHandle, cPacketBuffer const& packetBuffer );
protected:
    std::map<cNetworkHandle, std::vector<char>> mSendDataBuffer;
    cUDP mSocket;
};
}