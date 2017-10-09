#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayerRobJem : public cPacketBase<cEvePlayerRobJem, PacketId::EVE_PLAYER_ROB_JEM>
{
public:
    cEvePlayerRobJem( );
    cEvePlayerRobJem( std::string const& playerName );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    // íDÇ¡ÇΩëäéËÇÃñºëOÅB
    // ÇªÇÃÇ§ÇøIDÇ∆Ç©Ç…Ç»ÇËÇªÇ§ÅB
    std::string mPlayerName;
};
}
}
}