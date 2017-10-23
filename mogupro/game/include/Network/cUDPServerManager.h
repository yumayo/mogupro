#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <set>
#include <map>
namespace Network
{
class cUDPServerManager : public Utility::cSingletonAble<cUDPServerManager>
{
private:
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
    void update( );
private:
    void updateSend( );
    void updateRecv( );
private:
    void sendDataBufferAdd( cNetworkHandle const& networkHandle, cPacketBuffer const& packetBuffer );
private:
    void sendPlayersPosition( );
    void sendSetQuarry( );
    void sendGetGemPlayer( );
    void sendGetGemQuarry( );
    void sendBreakBlocks( );
private:
    void connection( );
    void ping( );
private:
    cUDP mSocket;
    std::map<cNetworkHandle, std::vector<char>> mSendDataBuffer;
    std::map<cNetworkHandle, float> mHandle;
private:
    std::map<cNetworkHandle, ubyte1> mHandlePlayers;
    std::set<ubyte2> mQuarrys;
    std::set<ubyte2> mGems;
};
}