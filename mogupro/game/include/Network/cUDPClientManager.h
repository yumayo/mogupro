#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDP.h>
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
namespace Network
{
namespace Packet
{
namespace Deliver
{
class cDliBreakBlocks;
}
}
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
public:
    cUDPClientManager( );
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
    bool isConnected( );
    void connect( cNetworkHandle const& handle );
    void update( );
private:
    void updateSend( );
    void updateRecv( );
private:
    void connection( );
    void ping( );
private:
    void sendDataBufferAdd( cNetworkHandle const& networkHandle, cPacketBuffer const& packetBuffer );
private:
    // 全てのプレイヤーの座標を行う。
    void allPlayersFormat( );
    // 全ての掘削機の設置イベントを行う。
    void allQuarrys( );
    // 全てのジェム採取イベントを行う。
    void allGems( );
    // 全てのブロック破壊イベントを行う。
    void allBreakBlocks( );
public:
    // ブロックを破壊したら呼んでください。
    void sendBreakBlock( cinder::vec3 const& position );
    // まとめてブロックを破壊したら呼んでください。
    void sendBreakBlocks( std::vector<cinder::vec3> const& positions );
    // 掘削機を設置したら呼んでください。
    void sendSetQuarry( cinder::vec3 const& position, ubyte1 drillType );
    // プレイヤーの位置情報をサーバーに送りますので呼んでください。
    void sendPlayerFormat( cinder::vec3 const& position, cinder::quat const& rotation );
    // プレイヤーがジェムを取得したら呼んでください。
    void sendGetGemPlayer( ubyte2 gemId );
    // 掘削機がジェムを取得したら呼んでください。
    void sendGetGemQuarry( ubyte1 drillId, ubyte2 gemId );
private:
    // 上記でたまったブロックを実際に送ります。
    void sendBreakBlocks( );
private:
    Packet::Deliver::cDliBreakBlocks* mBreakBlocksPecket = nullptr;
private:
    cUDP mSocket;
    std::map<cNetworkHandle, std::vector<char>> mSendDataBuffer;
    bool mIsConnected = false;
    cNetworkHandle mConnectServerHandle;
    float mCloseSecond;
};
}