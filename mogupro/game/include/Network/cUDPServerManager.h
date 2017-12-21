#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/cUDPManager.h>
#include <Network/cReliableManager.h>
#include <Network/cConnectionInfo.h>
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
    void sendUnsafe( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase, bool reliable = false )
    {
        cPacketBuffer packetBuffer;
        packetBase->createPacket( packetBuffer, static_cast<Packet::PacketHeader::State>( reliable << 0 ) );
        sendDataBufferAdd( networkHandle, packetBuffer, reliable );
    }
public:
    template <class Ty, Packet::PacketId packetId>
    void send( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase, bool reliable = false )
    {
        if ( packetBase == nullptr ) return;

        sendUnsafe( networkHandle, packetBase, reliable );

        delete packetBase;
        packetBase = nullptr;
    }
    template <class Ty, Packet::PacketId packetId>
    void broadcast( Packet::cPacketBase<Ty, packetId>* packetBase, bool reliable = false )
    {
        if ( packetBase == nullptr ) return;

        for ( auto& handle : mInfo )
        {
            sendUnsafe( handle.first, packetBase, reliable );
        }

        delete packetBase;
        packetBase = nullptr;
    }
    template <class Ty, Packet::PacketId packetId>
    void broadcastOthers( cNetworkHandle const& networkHandle, Packet::cPacketBase<Ty, packetId>* packetBase, bool reliable = false )
    {
        if ( packetBase == nullptr ) return;

        for ( auto& handle : mInfo )
        {
            if ( networkHandle == handle.first ) continue;

            sendUnsafe( handle.first, packetBase, reliable );
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
	float const& getServerTime( );
	std::vector<cUDPManager*> getUDPManager( );

    // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
    // くコ:彡
private:
    void updateSend( );
    void updateRecv( );
private:
    void sendDataBufferAdd( cNetworkHandle const& networkHandle, cPacketBuffer const& packetBuffer, bool reliable );
private:
    void connection( );
    void ping( );
private:
    cUDP mSocket;
	std::map<cNetworkHandle, cConnectionInfo> mInfo;
	std::map<cNetworkHandle, cUDPManager> mPacket;
    hardptr<Node::node> mRoot;
    bool mIsAccept;
    ubyte1 mIdCount;

	ubyte4 mSequenceId;

	float mServerTime;
};
}