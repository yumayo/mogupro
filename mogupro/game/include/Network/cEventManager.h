#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <map>
#include <Network/Packet/Event.hpp>
#include <boost/optional.hpp>
namespace Network
{
class cEventManager : public Utility::cSingletonAble<cEventManager>
{
    // P=====BEGIN=====P
public:
    boost::optional<Packet::Event::cEveString> getEveString( );
    void ungetEveString( Packet::Event::cEveString&& data );
    bool isNewEveString( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEveString> mEveString;
    std::map<ubyte4, double> mEveStringSequenceIds;
public:
    boost::optional<Packet::Event::cEvePing> getEvePing( );
    void ungetEvePing( Packet::Event::cEvePing&& data );
    bool isNewEvePing( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEvePing> mEvePing;
    std::map<ubyte4, double> mEvePingSequenceIds;
public:
    boost::optional<Packet::Event::cEvePlayers> getEvePlayers( );
    void ungetEvePlayers( Packet::Event::cEvePlayers&& data );
    bool isNewEvePlayers( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEvePlayers> mEvePlayers;
    std::map<ubyte4, double> mEvePlayersSequenceIds;
public:
    boost::optional<Packet::Event::cEveGetJemPlayer> getEveGetJemPlayer( );
    void ungetEveGetJemPlayer( Packet::Event::cEveGetJemPlayer&& data );
    bool isNewEveGetJemPlayer( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEveGetJemPlayer> mEveGetJemPlayer;
    std::map<ubyte4, double> mEveGetJemPlayerSequenceIds;
public:
    boost::optional<Packet::Event::cEveGetJemQuarry> getEveGetJemQuarry( );
    void ungetEveGetJemQuarry( Packet::Event::cEveGetJemQuarry&& data );
    bool isNewEveGetJemQuarry( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEveGetJemQuarry> mEveGetJemQuarry;
    std::map<ubyte4, double> mEveGetJemQuarrySequenceIds;
public:
    boost::optional<Packet::Event::cEveBreakBlocks> getEveBreakBlocks( );
    void ungetEveBreakBlocks( Packet::Event::cEveBreakBlocks&& data );
    bool isNewEveBreakBlocks( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEveBreakBlocks> mEveBreakBlocks;
    std::map<ubyte4, double> mEveBreakBlocksSequenceIds;
public:
    boost::optional<Packet::Event::cEveSetQuarry> getEveSetQuarry( );
    void ungetEveSetQuarry( Packet::Event::cEveSetQuarry&& data );
    bool isNewEveSetQuarry( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEveSetQuarry> mEveSetQuarry;
    std::map<ubyte4, double> mEveSetQuarrySequenceIds;
public:
    boost::optional<Packet::Event::cEvePlayerRobJem> getEvePlayerRobJem( );
    void ungetEvePlayerRobJem( Packet::Event::cEvePlayerRobJem&& data );
    bool isNewEvePlayerRobJem( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEvePlayerRobJem> mEvePlayerRobJem;
    std::map<ubyte4, double> mEvePlayerRobJemSequenceIds;
public:
    boost::optional<Packet::Event::cEvePlayerDeath> getEvePlayerDeath( );
    void ungetEvePlayerDeath( Packet::Event::cEvePlayerDeath&& data );
    bool isNewEvePlayerDeath( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEvePlayerDeath> mEvePlayerDeath;
    std::map<ubyte4, double> mEvePlayerDeathSequenceIds;
public:
    boost::optional<Packet::Event::cEveTeamMember> getEveTeamMember( );
    void ungetEveTeamMember( Packet::Event::cEveTeamMember&& data );
    bool isNewEveTeamMember( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEveTeamMember> mEveTeamMember;
    std::map<ubyte4, double> mEveTeamMemberSequenceIds;
public:
    boost::optional<Packet::Event::cEvePlayersRespawn> getEvePlayersRespawn( );
    void ungetEvePlayersRespawn( Packet::Event::cEvePlayersRespawn&& data );
    bool isNewEvePlayersRespawn( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Event::cEvePlayersRespawn> mEvePlayersRespawn;
    std::map<ubyte4, double> mEvePlayersRespawnSequenceIds;
    // P=====END=====P
};
}
