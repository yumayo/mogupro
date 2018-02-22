#pragma once
#include <Network/NetworkDefines.h>
#include <Network/cPacketQueue.h>
#include <Network/Packet/Event.hpp>
#include <Network/Packet/Request.hpp>
#include <Network/Packet/Response.hpp>
namespace Network
{
class cUDPManager
{
public:
	cUDPManager( ) { }
	~cUDPManager( ) { }
	static bool isConnectPacket( cPacketChunk const & packetChunk );
	void onReceive( cPacketChunk const & packetChunk );
public:
	PACKET_TEMPLATE_BEGIN
	cPacketQueue<Packet::Event::cEveString> EveString;
	cPacketQueue<Packet::Event::cEvePing> EvePing;
	cPacketQueue<Packet::Event::cEvePlayers> EvePlayers;
	cPacketQueue<Packet::Event::cEveGetJemPlayer> EveGetJemPlayer;
	cPacketQueue<Packet::Event::cEveGetJemQuarry> EveGetJemQuarry;
	cPacketQueue<Packet::Event::cEveBreakBlocks> EveBreakBlocks;
	cPacketQueue<Packet::Event::cEveSetQuarry> EveSetQuarry;
	cPacketQueue<Packet::Event::cEvePlayerRobJem> EvePlayerRobJem;
	cPacketQueue<Packet::Event::cEvePlayerDeath> EvePlayerDeath;
	cPacketQueue<Packet::Event::cEveRespawn> EveRespawn;
	cPacketQueue<Packet::Event::cEveAddCannonPower> EveAddCannonPower;
	cPacketQueue<Packet::Event::cEveLightBomb> EveLightBomb;
	cPacketQueue<Packet::Event::cEveWeaponCapsule> EveWeaponCapsule;
	cPacketQueue<Packet::Event::cEveTeamMember> EveTeamMember;
	cPacketQueue<Packet::Event::cEveDamage> EveDamage;
	cPacketQueue<Packet::Event::cEvePlayerAttack> EvePlayerAttack;
	cPacketQueue<Packet::Event::cEveFragmentGem> EveFragmentGem;
	cPacketQueue<Packet::Request::cReqString> ReqString;
	cPacketQueue<Packet::Request::cReqConnect> ReqConnect;
	cPacketQueue<Packet::Request::cReqPing> ReqPing;
	cPacketQueue<Packet::Request::cReqPlayer> ReqPlayer;
	cPacketQueue<Packet::Request::cReqBreakBlocks> ReqBreakBlocks;
	cPacketQueue<Packet::Request::cReqGetJemSeed> ReqGetJemSeed;
	cPacketQueue<Packet::Request::cReqGetJemPoint> ReqGetJemPoint;
	cPacketQueue<Packet::Request::cReqGetJemPlayer> ReqGetJemPlayer;
	cPacketQueue<Packet::Request::cReqGetJemQuarry> ReqGetJemQuarry;
	cPacketQueue<Packet::Request::cReqPlayerRobJem> ReqPlayerRobJem;
	cPacketQueue<Packet::Request::cReqSetQuarry> ReqSetQuarry;
	cPacketQueue<Packet::Request::cReqPlayerDeath> ReqPlayerDeath;
	cPacketQueue<Packet::Request::cReqRespawn> ReqRespawn;
	cPacketQueue<Packet::Request::cReqLightBomb> ReqLightBomb;
	cPacketQueue<Packet::Request::cReqDamage> ReqDamage;
	cPacketQueue<Packet::Request::cReqMakeRoom> ReqMakeRoom;
	cPacketQueue<Packet::Request::cReqInRoom> ReqInRoom;
	cPacketQueue<Packet::Request::cReqInRoomWatching> ReqInRoomWatching;
	cPacketQueue<Packet::Request::cReqWantTeamIn> ReqWantTeamIn;
	cPacketQueue<Packet::Request::cReqCheckBeginGame> ReqCheckBeginGame;
	cPacketQueue<Packet::Request::cReqCheckMember> ReqCheckMember;
	cPacketQueue<Packet::Request::cReqEndGamemainSetup> ReqEndGamemainSetup;
	cPacketQueue<Packet::Request::cReqEndStartTimer> ReqEndStartTimer;
	cPacketQueue<Packet::Request::cReqAddCannonPower> ReqAddCannonPower;
	cPacketQueue<Packet::Request::cReqWeaponCapsule> ReqWeaponCapsule;
	cPacketQueue<Packet::Request::cReqResult> ReqResult;
	cPacketQueue<Packet::Request::cReqPlayerAttack> ReqPlayerAttack;
	cPacketQueue<Packet::Request::cReqFragmentGem> ReqFragmentGem;
	cPacketQueue<Packet::Request::cReqGameEnd> ReqGameEnd;
	cPacketQueue<Packet::Response::cResString> ResString;
	cPacketQueue<Packet::Response::cResConnect> ResConnect;
	cPacketQueue<Packet::Response::cResGetJemSeed> ResGetJemSeed;
	cPacketQueue<Packet::Response::cResGetJemPoint> ResGetJemPoint;
	cPacketQueue<Packet::Response::cResMakeRoom> ResMakeRoom;
	cPacketQueue<Packet::Response::cResInRoom> ResInRoom;
	cPacketQueue<Packet::Response::cResWantTeamIn> ResWantTeamIn;
	cPacketQueue<Packet::Response::cResCheckBeginGame> ResCheckBeginGame;
	cPacketQueue<Packet::Response::cResCheckMember> ResCheckMember;
	cPacketQueue<Packet::Response::cResEndGamemainSetup> ResEndGamemainSetup;
	cPacketQueue<Packet::Response::cResSetGamestartTimer> ResSetGamestartTimer;
	cPacketQueue<Packet::Response::cResResult> ResResult;
	PACKET_TEMPLATE_END
};
}
