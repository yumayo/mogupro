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
private:
    template <class Ty, Packet::PacketId packetId>
    void sendUnsafe( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        cPacketBuffer packetBuffer;
        packetBase->createPacket( packetBuffer );
        sendDataBufferAdd( networkHandle, packetBuffer );
    }
public:
    template <class Ty, Packet::PacketId packetId>
    void send( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        sendUnsafe( networkHandle, packetBase );

        delete packetBase;
        packetBase = nullptr;
    }
    template <class Ty, Packet::PacketId packetId>
    void broadcast( Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        for ( auto& handle : mHandle )
        {
            sendUnsafe( handle.first, packetBase );
        }

        delete packetBase;
        packetBase = nullptr;
    }
    template <class Ty, Packet::PacketId packetId>
    void broadcastOthers( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase )
    {
        if ( packetBase == nullptr ) return;

        for ( auto& handle : mHandle )
        {
            if ( networkHandle == handle.first ) continue;

            sendUnsafe( handle.first, packetBase );
        }

        delete packetBase;
        packetBase = nullptr;
    }
public:
    // サーバーを閉じる。
    void close( );
    // サーバーを開く。
    void open( );
    // 新規ユーザーの接続を認めないようにする。
    void closeAccepter( );
    // 新規ユーザーの接続を認めるようにする。
    void openAccepter( );
    void update( float delta );
    ubyte1 getPlayerId( cNetworkHandle const& handle );

    // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
    // くコ:彡
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
        ubyte1 id;
    private:
        static ubyte1 idCount;
    };
    cUDP mSocket;
    std::map<cNetworkHandle, cClientInfo> mHandle;
    hardptr<Node::node> mRoot;
    bool mIsAccept;
};
}