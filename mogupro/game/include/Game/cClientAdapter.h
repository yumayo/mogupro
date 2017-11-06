#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/NetworkDefines.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <vector>
#include <Game/Field/cBreakBlockType.h>
namespace Network
{
namespace Packet
{
namespace Deliver
{
class cDliBreakBlocks;
}
}
}
namespace Game
{
class cClientAdapter : public Utility::cSingletonAble<cClientAdapter>
{
public:
    cClientAdapter( );
    ~cClientAdapter( );
public:
    void update( );
private:
    // 全てのプレイヤーの座標を行う。
    void recvAllPlayers( );
    // 全ての掘削機の設置イベントを行う。
    void recvAllQuarrys( );
    // 全てのジェム採取イベントを行う。
    void recvAllGems( );
    // 全てのブロック破壊イベントを行う。
    void recvAllBreakBlocks( );
public:
    // ブロックを破壊したら呼んでください。
    void sendBreakBlock( cinder::vec3 const& position, float radius, Network::ubyte1 type );
    // 掘削機を設置したら呼んでください。
    void sendSetQuarry( cinder::vec3 const& position, Network::ubyte1 drillType );
    // プレイヤーの位置情報をサーバーに送りますので呼んでください。
    void sendPlayer( cinder::vec3 const& position, cinder::quat const& rotation );
    // プレイヤーがジェムを取得したら呼んでください。
    void sendGetGemPlayer( Network::ubyte2 gemId );
    // 掘削機がジェムを取得したら呼んでください。
    void sendGetGemQuarry( Network::ubyte2 drillId, Network::ubyte2 gemId );
private:
    // 上記でたまったブロックを実際に送ります。
    void sendBreakBlocks( );
private:
    Network::Packet::Deliver::cDliBreakBlocks* mBreakBlocksPecket = nullptr;
};
}
