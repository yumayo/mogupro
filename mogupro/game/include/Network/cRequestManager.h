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
    boost::optional<Packet::Request::cReqGetJemPlayer> getReqGetJemPlayer( );
    void ungetReqGetJemPlayer( Packet::Request::cReqGetJemPlayer&& data );
    bool isNewReqGetJemPlayer( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqGetJemPlayer> mReqGetJemPlayer;
    std::map<ubyte4, double> mReqGetJemPlayerSequenceIds;
public:
    boost::optional<Packet::Request::cReqGetJemQuarry> getReqGetJemQuarry( );
    void ungetReqGetJemQuarry( Packet::Request::cReqGetJemQuarry&& data );
    bool isNewReqGetJemQuarry( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqGetJemQuarry> mReqGetJemQuarry;
    std::map<ubyte4, double> mReqGetJemQuarrySequenceIds;
public:
    boost::optional<Packet::Request::cReqPlayerRobJem> getReqPlayerRobJem( );
    void ungetReqPlayerRobJem( Packet::Request::cReqPlayerRobJem&& data );
    bool isNewReqPlayerRobJem( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqPlayerRobJem> mReqPlayerRobJem;
    std::map<ubyte4, double> mReqPlayerRobJemSequenceIds;
public:
    boost::optional<Packet::Request::cReqSetQuarry> getReqSetQuarry( );
    void ungetReqSetQuarry( Packet::Request::cReqSetQuarry&& data );
    bool isNewReqSetQuarry( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqSetQuarry> mReqSetQuarry;
    std::map<ubyte4, double> mReqSetQuarrySequenceIds;
public:
    boost::optional<Packet::Request::cReqPlayerDeath> getReqPlayerDeath( );
    void ungetReqPlayerDeath( Packet::Request::cReqPlayerDeath&& data );
    bool isNewReqPlayerDeath( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqPlayerDeath> mReqPlayerDeath;
    std::map<ubyte4, double> mReqPlayerDeathSequenceIds;
public:
    boost::optional<Packet::Request::cReqRespawn> getReqRespawn( );
    void ungetReqRespawn( Packet::Request::cReqRespawn&& data );
    bool isNewReqRespawn( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqRespawn> mReqRespawn;
    std::map<ubyte4, double> mReqRespawnSequenceIds;
public:
    boost::optional<Packet::Request::cReqLightBomb> getReqLightBomb( );
    void ungetReqLightBomb( Packet::Request::cReqLightBomb&& data );
    bool isNewReqLightBomb( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqLightBomb> mReqLightBomb;
    std::map<ubyte4, double> mReqLightBombSequenceIds;
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
    boost::optional<Packet::Request::cReqResult> getReqResult( );
    void ungetReqResult( Packet::Request::cReqResult&& data );
    bool isNewReqResult( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqResult> mReqResult;
    std::map<ubyte4, double> mReqResultSequenceIds;
public:
    boost::optional<Packet::Request::cReqPlayerAttack> getReqPlayerAttack( );
    void ungetReqPlayerAttack( Packet::Request::cReqPlayerAttack&& data );
    bool isNewReqPlayerAttack( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqPlayerAttack> mReqPlayerAttack;
    std::map<ubyte4, double> mReqPlayerAttackSequenceIds;
public:
    boost::optional<Packet::Request::cReqFragmentGem> getReqFragmentGem( );
    void ungetReqFragmentGem( Packet::Request::cReqFragmentGem&& data );
    bool isNewReqFragmentGem( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Request::cReqFragmentGem> mReqFragmentGem;
    std::map<ubyte4, double> mReqFragmentGemSequenceIds;
    // P=====END=====P
};
}
