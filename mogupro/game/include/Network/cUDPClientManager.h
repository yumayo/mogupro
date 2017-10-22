#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDPManager.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <Network/Packet/Deliver/cDliBreakBlocks.h>
namespace Network
{
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
public:
    cUDPClientManager( );
public:
    void update( );
private:
    // 全てのプレイヤーの座標を行う。
    void allPlayersPosition( );
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
    void sendPlayerPosition( cinder::vec3 const& position, cinder::quat const& rotation );
    // プレイヤーがジェムを取得したら呼んでください。
    void sendGetGemPlayer( ubyte2 gemId );
    // 掘削機がジェムを取得したら呼んでください。
    void sendGetGemQuarry( ubyte1 drillId, ubyte2 gemId );
private:
    // 上記でたまったブロックを実際に送ります。
    void sendBreakBlocks( );
private:
    cNetworkHandle mConnectServerHandle;
    Packet::Deliver::cDliBreakBlocks* mBreakBlocksPecket = nullptr;
};
}