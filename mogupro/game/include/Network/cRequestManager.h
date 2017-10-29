#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
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
public:
    std::stack<Packet::Request::cReqString> mReqString;
public:
    boost::optional<Packet::Request::cReqConnect> getReqConnect( );
    void ungetReqConnect( Packet::Request::cReqConnect&& data );
public:
    std::stack<Packet::Request::cReqConnect> mReqConnect;
public:
    boost::optional<Packet::Request::cReqGetJemSeed> getReqGetJemSeed( );
    void ungetReqGetJemSeed( Packet::Request::cReqGetJemSeed&& data );
public:
    std::stack<Packet::Request::cReqGetJemSeed> mReqGetJemSeed;
public:
    boost::optional<Packet::Request::cReqGetJemPoint> getReqGetJemPoint( );
    void ungetReqGetJemPoint( Packet::Request::cReqGetJemPoint&& data );
public:
    std::stack<Packet::Request::cReqGetJemPoint> mReqGetJemPoint;
public:
    boost::optional<Packet::Request::cReqCheckGetJemPlayer> getReqCheckGetJemPlayer( );
    void ungetReqCheckGetJemPlayer( Packet::Request::cReqCheckGetJemPlayer&& data );
public:
    std::stack<Packet::Request::cReqCheckGetJemPlayer> mReqCheckGetJemPlayer;
public:
    boost::optional<Packet::Request::cReqCheckGetJemQuarry> getReqCheckGetJemQuarry( );
    void ungetReqCheckGetJemQuarry( Packet::Request::cReqCheckGetJemQuarry&& data );
public:
    std::stack<Packet::Request::cReqCheckGetJemQuarry> mReqCheckGetJemQuarry;
public:
    boost::optional<Packet::Request::cReqCheckPlayerRobJem> getReqCheckPlayerRobJem( );
    void ungetReqCheckPlayerRobJem( Packet::Request::cReqCheckPlayerRobJem&& data );
public:
    std::stack<Packet::Request::cReqCheckPlayerRobJem> mReqCheckPlayerRobJem;
public:
    boost::optional<Packet::Request::cReqCheckSetQuarry> getReqCheckSetQuarry( );
    void ungetReqCheckSetQuarry( Packet::Request::cReqCheckSetQuarry&& data );
public:
    std::stack<Packet::Request::cReqCheckSetQuarry> mReqCheckSetQuarry;
public:
    boost::optional<Packet::Request::cReqCheckPlayerDeath> getReqCheckPlayerDeath( );
    void ungetReqCheckPlayerDeath( Packet::Request::cReqCheckPlayerDeath&& data );
public:
    std::stack<Packet::Request::cReqCheckPlayerDeath> mReqCheckPlayerDeath;
public:
    boost::optional<Packet::Request::cReqMakeRoom> getReqMakeRoom( );
    void ungetReqMakeRoom( Packet::Request::cReqMakeRoom&& data );
public:
    std::stack<Packet::Request::cReqMakeRoom> mReqMakeRoom;
public:
    boost::optional<Packet::Request::cReqInRoom> getReqInRoom( );
    void ungetReqInRoom( Packet::Request::cReqInRoom&& data );
public:
    std::stack<Packet::Request::cReqInRoom> mReqInRoom;
public:
    boost::optional<Packet::Request::cReqWantTeamIn> getReqWantTeamIn( );
    void ungetReqWantTeamIn( Packet::Request::cReqWantTeamIn&& data );
public:
    std::stack<Packet::Request::cReqWantTeamIn> mReqWantTeamIn;
public:
    boost::optional<Packet::Request::cReqCheckBeginGame> getReqCheckBeginGame( );
    void ungetReqCheckBeginGame( Packet::Request::cReqCheckBeginGame&& data );
public:
    std::stack<Packet::Request::cReqCheckBeginGame> mReqCheckBeginGame;
public:
    boost::optional<Packet::Request::cReqCheckMember> getReqCheckMember( );
    void ungetReqCheckMember( Packet::Request::cReqCheckMember&& data );
public:
    std::stack<Packet::Request::cReqCheckMember> mReqCheckMember;
public:
    boost::optional<Packet::Request::cReqEndGamemainSetup> getReqEndGamemainSetup( );
    void ungetReqEndGamemainSetup( Packet::Request::cReqEndGamemainSetup&& data );
public:
    std::stack<Packet::Request::cReqEndGamemainSetup> mReqEndGamemainSetup;
    // P=====END=====P
};
}
