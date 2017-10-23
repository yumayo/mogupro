#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <set>
#include <map>
#include <Node/node.h>
namespace Network
{
class cUDPServerManager : public Utility::cSingletonAble<cUDPServerManager>
{
public:
    cUDPServerManager( );
public:
    template <class Ty, Packet::PacketId packetId>
    void send( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        cPacketBuffer packetBuffer;
        packetBase->createPacket( packetBuffer );

        sendDataBufferAdd( networkHandle, packetBuffer );

        delete packetBase;
        packetBase = nullptr;
    }
public:
    void close( );
    void open( );
    void update( float delta );

    // ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª
    // ‚­ƒR:œc
private:
    void updateSend( );
    void updateRecv( );
private:
    void sendDataBufferAdd( cNetworkHandle const& networkHandle, cPacketBuffer const& packetBuffer );
private:
    void connection( );
    void ping( );
private:
    class cClientInfo
    {
    public:
        cClientInfo( );
    public:
        std::vector<char> buffer;
        float closeSecond;
        ubyte2 id;
    private:
        static ubyte2 idCount;
    };
    cUDP mSocket;
    std::map<cNetworkHandle, cClientInfo> mHandle;
    hardptr<Node::node> mRoot;
};
}