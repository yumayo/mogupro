#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/cReliableManager.h>
#include <Node/node.h>
namespace Network
{
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
public:
    cUDPClientManager( );
public:
    template <class Ty, Packet::PacketId packetId>
    void send( Packet::cPacketBase<Ty, packetId>* packetBase, bool reliable = false )
    {
        if ( packetBase == nullptr ) return;

		cPacketBuffer packetBuffer;
		packetBase->createPacket( packetBuffer, static_cast<Packet::PacketHeader::State>( reliable << 0 ) );
		sendDataBufferAdd( packetBuffer, reliable );

        delete packetBase;
        packetBase = nullptr;
    }
public:
    void close( );
    void open( );
    bool isConnected( );
    void connect( std::string const& ipAddress );
    void connectOfflineServer( );
    void update( float delta );

    // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
    // くコ:彡
private:
    void updateSend( );
    void updateRecv( );
private:
    void connection( );
    void ping( );
private:
    void sendDataBufferAdd( cPacketBuffer const& packetBuffer, bool reliable );
private:
    cUDP mSocket;
    std::vector<char> mSendDataBuffer;
    cNetworkHandle mConnectServerHandle;
	bool mIsConnected;

	cReliableManager mReliableManager;

    hardptr<Node::node> mRoot;

	ubyte4 mSequenceId;

    // サーバーとの接続が維持されているのかを保証します。
    // 5秒以上応答がない場合は切断します。
    float mCloseSecond;

    // サーバーに接続できたかを保証します。
    // 5秒以上応答がない場合は切断します。
    float mConnectSecond;
};
}