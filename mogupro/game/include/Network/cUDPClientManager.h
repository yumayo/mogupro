#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
namespace Network
{
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
public:
    cUDPClientManager( );
public:
    template <class Ty, Packet::PacketId packetId>
    void send( Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        cPacketBuffer packetBuffer;
        packetBase->createPacket( packetBuffer );

        sendDataBufferAdd( packetBuffer );

        delete packetBase;
        packetBase = nullptr;
    }
public:
    void close( );
    void open( );
    bool isConnected( );
    void connect( cNetworkHandle const& handle );
    void update( );

    // ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª
    // ‚­ƒR:œc
private:
    void updateSend( );
    void updateRecv( );
private:
    void connection( );
    void ping( );
private:
    void sendDataBufferAdd( cPacketBuffer const& packetBuffer );
private:
    cUDP mSocket;
    std::vector<char> mSendDataBuffer;
    cNetworkHandle mConnectServerHandle;
    float mCloseSecond;
};
}