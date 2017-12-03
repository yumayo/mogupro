#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <map>
#include <Network/Packet/Request.hpp>
#include <boost/optional.hpp>
namespace Network
{
class cRequestManager : public Utility::cSingletonAble<cRequestManager>
{
    // P=====BEGIN=====P
public:
    boost::optional<Packet::Request::cReqString> getReqString( );
    void ungetReqString( Packet::Request::cReqString&& data );
    bool isNewReqString( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqString> mReqString;
    std::map<ubyte4, double> mReqStringSequenceIds;
public:
    boost::optional<Packet::Request::cReqConnect> getReqConnect( );
    void ungetReqConnect( Packet::Request::cReqConnect&& data );
    bool isNewReqConnect( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqConnect> mReqConnect;
    std::map<ubyte4, double> mReqConnectSequenceIds;
public:
    boost::optional<Packet::Request::cReqGetJemSeed> getReqGetJemSeed( );
    void ungetReqGetJemSeed( Packet::Request::cReqGetJemSeed&& data );
    bool isNewReqGetJemSeed( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqGetJemSeed> mReqGetJemSeed;
    std::map<ubyte4, double> mReqGetJemSeedSequenceIds;
public:
    boost::optional<Packet::Request::cReqGetJemPoint> getReqGetJemPoint( );
    void ungetReqGetJemPoint( Packet::Request::cReqGetJemPoint&& data );
    bool isNewReqGetJemPoint( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqGetJemPoint> mReqGetJemPoint;
    std::map<ubyte4, double> mReqGetJemPointSequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckGetJemPlayer> getReqCheckGetJemPlayer( );
    void ungetReqCheckGetJemPlayer( Packet::Request::cReqCheckGetJemPlayer&& data );
    bool isNewReqCheckGetJemPlayer( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckGetJemPlayer> mReqCheckGetJemPlayer;
    std::map<ubyte4, double> mReqCheckGetJemPlayerSequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckGetJemQuarry> getReqCheckGetJemQuarry( );
    void ungetReqCheckGetJemQuarry( Packet::Request::cReqCheckGetJemQuarry&& data );
    bool isNewReqCheckGetJemQuarry( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckGetJemQuarry> mReqCheckGetJemQuarry;
    std::map<ubyte4, double> mReqCheckGetJemQuarrySequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckPlayerRobJem> getReqCheckPlayerRobJem( );
    void ungetReqCheckPlayerRobJem( Packet::Request::cReqCheckPlayerRobJem&& data );
    bool isNewReqCheckPlayerRobJem( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckPlayerRobJem> mReqCheckPlayerRobJem;
    std::map<ubyte4, double> mReqCheckPlayerRobJemSequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckSetQuarry> getReqCheckSetQuarry( );
    void ungetReqCheckSetQuarry( Packet::Request::cReqCheckSetQuarry&& data );
    bool isNewReqCheckSetQuarry( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckSetQuarry> mReqCheckSetQuarry;
    std::map<ubyte4, double> mReqCheckSetQuarrySequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckPlayerDeath> getReqCheckPlayerDeath( );
    void ungetReqCheckPlayerDeath( Packet::Request::cReqCheckPlayerDeath&& data );
    bool isNewReqCheckPlayerDeath( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckPlayerDeath> mReqCheckPlayerDeath;
    std::map<ubyte4, double> mReqCheckPlayerDeathSequenceIds;
public:
    boost::optional<Packet::Request::cReqRespawn> getReqRespawn( );
    void ungetReqRespawn( Packet::Request::cReqRespawn&& data );
    bool isNewReqRespawn( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqRespawn> mReqRespawn;
    std::map<ubyte4, double> mReqRespawnSequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckLightBomb> getReqCheckLightBomb( );
    void ungetReqCheckLightBomb( Packet::Request::cReqCheckLightBomb&& data );
    bool isNewReqCheckLightBomb( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckLightBomb> mReqCheckLightBomb;
    std::map<ubyte4, double> mReqCheckLightBombSequenceIds;
public:
    boost::optional<Packet::Request::cReqDamage> getReqDamage( );
    void ungetReqDamage( Packet::Request::cReqDamage&& data );
    bool isNewReqDamage( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqDamage> mReqDamage;
    std::map<ubyte4, double> mReqDamageSequenceIds;
public:
    boost::optional<Packet::Request::cReqMakeRoom> getReqMakeRoom( );
    void ungetReqMakeRoom( Packet::Request::cReqMakeRoom&& data );
    bool isNewReqMakeRoom( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqMakeRoom> mReqMakeRoom;
    std::map<ubyte4, double> mReqMakeRoomSequenceIds;
public:
    boost::optional<Packet::Request::cReqInRoom> getReqInRoom( );
    void ungetReqInRoom( Packet::Request::cReqInRoom&& data );
    bool isNewReqInRoom( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqInRoom> mReqInRoom;
    std::map<ubyte4, double> mReqInRoomSequenceIds;
public:
    boost::optional<Packet::Request::cReqWantTeamIn> getReqWantTeamIn( );
    void ungetReqWantTeamIn( Packet::Request::cReqWantTeamIn&& data );
    bool isNewReqWantTeamIn( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqWantTeamIn> mReqWantTeamIn;
    std::map<ubyte4, double> mReqWantTeamInSequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckBeginGame> getReqCheckBeginGame( );
    void ungetReqCheckBeginGame( Packet::Request::cReqCheckBeginGame&& data );
    bool isNewReqCheckBeginGame( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckBeginGame> mReqCheckBeginGame;
    std::map<ubyte4, double> mReqCheckBeginGameSequenceIds;
public:
    boost::optional<Packet::Request::cReqCheckMember> getReqCheckMember( );
    void ungetReqCheckMember( Packet::Request::cReqCheckMember&& data );
    bool isNewReqCheckMember( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCheckMember> mReqCheckMember;
    std::map<ubyte4, double> mReqCheckMemberSequenceIds;
public:
    boost::optional<Packet::Request::cReqEndGamemainSetup> getReqEndGamemainSetup( );
    void ungetReqEndGamemainSetup( Packet::Request::cReqEndGamemainSetup&& data );
    bool isNewReqEndGamemainSetup( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqEndGamemainSetup> mReqEndGamemainSetup;
    std::map<ubyte4, double> mReqEndGamemainSetupSequenceIds;
public:
    boost::optional<Packet::Request::cReqEndStartTimer> getReqEndStartTimer( );
    void ungetReqEndStartTimer( Packet::Request::cReqEndStartTimer&& data );
    bool isNewReqEndStartTimer( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqEndStartTimer> mReqEndStartTimer;
    std::map<ubyte4, double> mReqEndStartTimerSequenceIds;
public:
    boost::optional<Packet::Request::cReqAddCannonPower> getReqAddCannonPower( );
    void ungetReqAddCannonPower( Packet::Request::cReqAddCannonPower&& data );
    bool isNewReqAddCannonPower( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqAddCannonPower> mReqAddCannonPower;
    std::map<ubyte4, double> mReqAddCannonPowerSequenceIds;
public:
    boost::optional<Packet::Request::cReqCannonPower> getReqCannonPower( );
    void ungetReqCannonPower( Packet::Request::cReqCannonPower&& data );
    bool isNewReqCannonPower( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqCannonPower> mReqCannonPower;
    std::map<ubyte4, double> mReqCannonPowerSequenceIds;
public:
    boost::optional<Packet::Request::cReqResult> getReqResult( );
    void ungetReqResult( Packet::Request::cReqResult&& data );
    bool isNewReqResult( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqResult> mReqResult;
    std::map<ubyte4, double> mReqResultSequenceIds;
    // P=====END=====P
};
}
