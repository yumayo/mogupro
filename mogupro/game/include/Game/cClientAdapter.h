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
namespace Request
{
class cReqBreakBlocks;
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
    // 全てのプレイヤーの更新を行う。
    void recvAllPlayers( );
    // 全てのジェム採取イベントを行う。
    void recvAllGems( );
    // 全てのブロック破壊イベントを行う。
    void recvAllBreakBlocks( );
	// 全てのサブウェポンイベントを行う。
	void recvAllWeaponCapsules( );
	// 全ての大砲イベントを行う。
	void recvAllCannons( );
	//
	void recvAllGameInfo( );
public:
    // ブロックを破壊したら呼んでください。
    void sendBreakBlock( cinder::vec3 const& position, float radius, Network::ubyte1 type );
    // 掘削機を設置したら呼んでください。
    void sendSetQuarry( cinder::vec3 const& position );
    // プレイヤーの位置情報をサーバーに送りますので呼んでください。
    void sendPlayer( cinder::vec3 const& position, cinder::quat const& rotation );
    // プレイヤーがジェムを取得したら呼んでください。
    void sendGetGemPlayer( Network::ubyte2 gemId );
    // 掘削機がジェムを取得したら呼んでください。
    void sendGetGemQuarry( Network::ubyte2 objectId, Network::ubyte2 gemId );
	// ライトボムを投げる。
	void sendLightBomb( cinder::vec3 const& position, cinder::vec3 const& speed );
	// カプセルを投げる。
	void sendWeaponCapsule( cinder::vec3 const& position, cinder::vec3 const& speed, Network::ubyte1 type );
	// 誰かをキルしたら呼んでください。
	void sendKill( Network::ubyte1 enemyId );
	// 誰かにダメージを与えたら呼んでください。
	void sendDamage( Network::ubyte1 enemyId, float damage );
	//
	void sendRespawn( );
	//
	void sendAddCannonPower( Network::ubyte1 power, Network::ubyte1 playerOrQuarry );
	//
	void sendResult( );
	//
	void sendPlayerAttack( Network::ubyte1 playerId, Network::ubyte1 call );
private:
    // 上記でたまったブロックを実際に送ります。
    void sendBreakBlocks( );
private:
    Network::Packet::Request::cReqBreakBlocks* mBreakBlocksPecket = nullptr;
	Network::ubyte4 mPlayerSeq = 0U;
};
}
