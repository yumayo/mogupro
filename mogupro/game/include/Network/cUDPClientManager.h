#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <Node/node.h>
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
	 void connectMatchingServer( );
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
    void sendDataBufferAdd( cPacketBuffer const& packetBuffer );
private:
    cUDP mSocket;
    std::vector<char> mSendDataBuffer;
    cNetworkHandle mConnectServerHandle;
    hardptr<Node::node> mRoot;

    // サーバーとの接続が維持されているのかを保証します。
    // 5秒以上応答がない場合は切断します。
    float mCloseSecond;

    // サーバーに接続できたかを保証します。
    // 5秒以上応答がない場合は切断します。
    float mConnectSecond;

	 bool mFoundMatchingServer;
	 std::function<void( std::string const& )> mOnFoundMatchingServer;
};
}