#include <Network/cUDPManager.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
bool cUDPManager::isConnectPacket( cPacketChunk const & packetChunk )
{
	cNetworkHandle const& networkHandle = packetChunk.networkHandle;
	cPacketBuffer const& packetBuffer = packetChunk.packetBuffer;
	ubyte2 const& transferredBytes = packetBuffer.transferredBytes;
	cBuffer const& buffer = packetBuffer.buffer;

	ubyte2 headerOffset = 0U;
	do
	{
		Packet::PacketHeader packetHeader;
		char const* bufferData = buffer.data( ) + headerOffset;
		memcpy( &packetHeader, bufferData, sizeof( Packet::PacketHeader ) );
		ubyte2 bufferSize = packetHeader.mPacketByte;
		switch ( packetHeader.mPacketId )
		{
		case Network::Packet::PacketId::REQ_CONNECT:
		{
			return true;
			break;
		}
		default:
			break;
		}
		headerOffset += packetHeader.mPacketByte;
		if ( headerOffset > transferredBytes )
		{
			throw std::exception( "Ç‚ÇŒÇ¢ÅB" );
		}

	} while ( headerOffset != transferredBytes );

	return false;
}
void cUDPManager::onReceive( cPacketChunk const & packetChunk )
{
	cNetworkHandle const& networkHandle = packetChunk.networkHandle;
	cPacketBuffer const& packetBuffer = packetChunk.packetBuffer;
	ubyte2 const& transferredBytes = packetBuffer.transferredBytes;
	cBuffer const& buffer = packetBuffer.buffer;

	ubyte2 headerOffset = 0U;
	do
	{
		Packet::PacketHeader packetHeader;
		char const* bufferData = buffer.data( ) + headerOffset;
		memcpy( &packetHeader, bufferData, sizeof( Packet::PacketHeader ) );
		ubyte2 bufferSize = packetHeader.mPacketByte;
		switch ( packetHeader.mPacketId )
		{
			PACKET_TEMPLATE_BEGIN
		case Network::Packet::PacketId::EVE_STRING:
		{
			if ( EveString.isNew( packetHeader ) )
			{
				Packet::Event::cEveString data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveString.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_PING:
		{
			if ( EvePing.isNew( packetHeader ) )
			{
				Packet::Event::cEvePing data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EvePing.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_PLAYERS:
		{
			if ( EvePlayers.isNew( packetHeader ) )
			{
				Packet::Event::cEvePlayers data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EvePlayers.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_GET_JEM_PLAYER:
		{
			if ( EveGetJemPlayer.isNew( packetHeader ) )
			{
				Packet::Event::cEveGetJemPlayer data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveGetJemPlayer.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_GET_JEM_QUARRY:
		{
			if ( EveGetJemQuarry.isNew( packetHeader ) )
			{
				Packet::Event::cEveGetJemQuarry data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveGetJemQuarry.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_BREAK_BLOCKS:
		{
			if ( EveBreakBlocks.isNew( packetHeader ) )
			{
				Packet::Event::cEveBreakBlocks data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveBreakBlocks.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_SET_QUARRY:
		{
			if ( EveSetQuarry.isNew( packetHeader ) )
			{
				Packet::Event::cEveSetQuarry data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveSetQuarry.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_PLAYER_ROB_JEM:
		{
			if ( EvePlayerRobJem.isNew( packetHeader ) )
			{
				Packet::Event::cEvePlayerRobJem data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EvePlayerRobJem.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_PLAYER_DEATH:
		{
			if ( EvePlayerDeath.isNew( packetHeader ) )
			{
				Packet::Event::cEvePlayerDeath data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EvePlayerDeath.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_RESPAWN:
		{
			if ( EveRespawn.isNew( packetHeader ) )
			{
				Packet::Event::cEveRespawn data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveRespawn.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_ADD_CANNON_POWER:
		{
			if ( EveAddCannonPower.isNew( packetHeader ) )
			{
				Packet::Event::cEveAddCannonPower data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveAddCannonPower.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_LIGHT_BOMB:
		{
			if ( EveLightBomb.isNew( packetHeader ) )
			{
				Packet::Event::cEveLightBomb data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveLightBomb.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_WEAPON_CAPSULE:
		{
			if ( EveWeaponCapsule.isNew( packetHeader ) )
			{
				Packet::Event::cEveWeaponCapsule data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveWeaponCapsule.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_TEAM_MEMBER:
		{
			if ( EveTeamMember.isNew( packetHeader ) )
			{
				Packet::Event::cEveTeamMember data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveTeamMember.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_DAMAGE:
		{
			if ( EveDamage.isNew( packetHeader ) )
			{
				Packet::Event::cEveDamage data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveDamage.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_PLAYER_ATTACK:
		{
			if ( EvePlayerAttack.isNew( packetHeader ) )
			{
				Packet::Event::cEvePlayerAttack data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EvePlayerAttack.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::EVE_FRAGMENT_GEM:
		{
			if ( EveFragmentGem.isNew( packetHeader ) )
			{
				Packet::Event::cEveFragmentGem data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				EveFragmentGem.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_STRING:
		{
			if ( ReqString.isNew( packetHeader ) )
			{
				Packet::Request::cReqString data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqString.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_CONNECT:
		{
			if ( ReqConnect.isNew( packetHeader ) )
			{
				Packet::Request::cReqConnect data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqConnect.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_PING:
		{
			if ( ReqPing.isNew( packetHeader ) )
			{
				Packet::Request::cReqPing data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqPing.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_PLAYER:
		{
			if ( ReqPlayer.isNew( packetHeader ) )
			{
				Packet::Request::cReqPlayer data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqPlayer.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_BREAK_BLOCKS:
		{
			if ( ReqBreakBlocks.isNew( packetHeader ) )
			{
				Packet::Request::cReqBreakBlocks data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqBreakBlocks.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_GET_JEM_SEED:
		{
			if ( ReqGetJemSeed.isNew( packetHeader ) )
			{
				Packet::Request::cReqGetJemSeed data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqGetJemSeed.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_GET_JEM_POINT:
		{
			if ( ReqGetJemPoint.isNew( packetHeader ) )
			{
				Packet::Request::cReqGetJemPoint data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqGetJemPoint.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_GET_JEM_PLAYER:
		{
			if ( ReqGetJemPlayer.isNew( packetHeader ) )
			{
				Packet::Request::cReqGetJemPlayer data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqGetJemPlayer.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_GET_JEM_QUARRY:
		{
			if ( ReqGetJemQuarry.isNew( packetHeader ) )
			{
				Packet::Request::cReqGetJemQuarry data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqGetJemQuarry.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_PLAYER_ROB_JEM:
		{
			if ( ReqPlayerRobJem.isNew( packetHeader ) )
			{
				Packet::Request::cReqPlayerRobJem data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqPlayerRobJem.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_SET_QUARRY:
		{
			if ( ReqSetQuarry.isNew( packetHeader ) )
			{
				Packet::Request::cReqSetQuarry data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqSetQuarry.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_PLAYER_DEATH:
		{
			if ( ReqPlayerDeath.isNew( packetHeader ) )
			{
				Packet::Request::cReqPlayerDeath data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqPlayerDeath.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_RESPAWN:
		{
			if ( ReqRespawn.isNew( packetHeader ) )
			{
				Packet::Request::cReqRespawn data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqRespawn.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_LIGHT_BOMB:
		{
			if ( ReqLightBomb.isNew( packetHeader ) )
			{
				Packet::Request::cReqLightBomb data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqLightBomb.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_DAMAGE:
		{
			if ( ReqDamage.isNew( packetHeader ) )
			{
				Packet::Request::cReqDamage data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqDamage.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_MAKE_ROOM:
		{
			if ( ReqMakeRoom.isNew( packetHeader ) )
			{
				Packet::Request::cReqMakeRoom data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqMakeRoom.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_IN_ROOM:
		{
			if ( ReqInRoom.isNew( packetHeader ) )
			{
				Packet::Request::cReqInRoom data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqInRoom.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_IN_ROOM_WATCHING:
		{
			if ( ReqInRoomWatching.isNew( packetHeader ) )
			{
				Packet::Request::cReqInRoomWatching data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqInRoomWatching.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_WANT_TEAM_IN:
		{
			if ( ReqWantTeamIn.isNew( packetHeader ) )
			{
				Packet::Request::cReqWantTeamIn data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqWantTeamIn.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_CHECK_BEGIN_GAME:
		{
			if ( ReqCheckBeginGame.isNew( packetHeader ) )
			{
				Packet::Request::cReqCheckBeginGame data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqCheckBeginGame.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_CHECK_MEMBER:
		{
			if ( ReqCheckMember.isNew( packetHeader ) )
			{
				Packet::Request::cReqCheckMember data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqCheckMember.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_END_GAMEMAIN_SETUP:
		{
			if ( ReqEndGamemainSetup.isNew( packetHeader ) )
			{
				Packet::Request::cReqEndGamemainSetup data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqEndGamemainSetup.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_END_START_TIMER:
		{
			if ( ReqEndStartTimer.isNew( packetHeader ) )
			{
				Packet::Request::cReqEndStartTimer data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqEndStartTimer.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_ADD_CANNON_POWER:
		{
			if ( ReqAddCannonPower.isNew( packetHeader ) )
			{
				Packet::Request::cReqAddCannonPower data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqAddCannonPower.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_WEAPON_CAPSULE:
		{
			if ( ReqWeaponCapsule.isNew( packetHeader ) )
			{
				Packet::Request::cReqWeaponCapsule data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqWeaponCapsule.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_RESULT:
		{
			if ( ReqResult.isNew( packetHeader ) )
			{
				Packet::Request::cReqResult data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqResult.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_PLAYER_ATTACK:
		{
			if ( ReqPlayerAttack.isNew( packetHeader ) )
			{
				Packet::Request::cReqPlayerAttack data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqPlayerAttack.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_FRAGMENT_GEM:
		{
			if ( ReqFragmentGem.isNew( packetHeader ) )
			{
				Packet::Request::cReqFragmentGem data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqFragmentGem.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::REQ_GAME_END:
		{
			if ( ReqGameEnd.isNew( packetHeader ) )
			{
				Packet::Request::cReqGameEnd data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ReqGameEnd.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_STRING:
		{
			if ( ResString.isNew( packetHeader ) )
			{
				Packet::Response::cResString data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResString.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_CONNECT:
		{
			if ( ResConnect.isNew( packetHeader ) )
			{
				Packet::Response::cResConnect data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResConnect.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_GET_JEM_SEED:
		{
			if ( ResGetJemSeed.isNew( packetHeader ) )
			{
				Packet::Response::cResGetJemSeed data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResGetJemSeed.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_GET_JEM_POINT:
		{
			if ( ResGetJemPoint.isNew( packetHeader ) )
			{
				Packet::Response::cResGetJemPoint data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResGetJemPoint.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_MAKE_ROOM:
		{
			if ( ResMakeRoom.isNew( packetHeader ) )
			{
				Packet::Response::cResMakeRoom data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResMakeRoom.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_IN_ROOM:
		{
			if ( ResInRoom.isNew( packetHeader ) )
			{
				Packet::Response::cResInRoom data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResInRoom.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_WANT_TEAM_IN:
		{
			if ( ResWantTeamIn.isNew( packetHeader ) )
			{
				Packet::Response::cResWantTeamIn data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResWantTeamIn.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_CHECK_BEGIN_GAME:
		{
			if ( ResCheckBeginGame.isNew( packetHeader ) )
			{
				Packet::Response::cResCheckBeginGame data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResCheckBeginGame.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_CHECK_MEMBER:
		{
			if ( ResCheckMember.isNew( packetHeader ) )
			{
				Packet::Response::cResCheckMember data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResCheckMember.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_END_GAMEMAIN_SETUP:
		{
			if ( ResEndGamemainSetup.isNew( packetHeader ) )
			{
				Packet::Response::cResEndGamemainSetup data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResEndGamemainSetup.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_SET_GAMESTART_TIMER:
		{
			if ( ResSetGamestartTimer.isNew( packetHeader ) )
			{
				Packet::Response::cResSetGamestartTimer data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResSetGamestartTimer.unget( std::move( data ) );
			}
			break;
		}
		case Network::Packet::PacketId::RES_RESULT:
		{
			if ( ResResult.isNew( packetHeader ) )
			{
				Packet::Response::cResResult data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				ResResult.unget( std::move( data ) );
			}
			break;
		}
		PACKET_TEMPLATE_END
		default:
			break;
		}
		headerOffset += packetHeader.mPacketByte;
		if ( headerOffset > transferredBytes )
		{
			throw std::exception( "Ç‚ÇŒÇ¢ÅB" );
		}
	} while ( headerOffset != transferredBytes );
}
}
