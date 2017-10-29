#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Response.hpp>
#include <boost/optional.hpp>
namespace Network
{
class cResponseManager : public Utility::cSingletonAble<cResponseManager>
{
    // P=====BEGIN=====P
public:
    boost::optional<Packet::Response::cResString> getResString( );
    void ungetResString( Packet::Response::cResString&& data );
public:
    std::stack<Packet::Response::cResString> mResString;
public:
    boost::optional<Packet::Response::cResConnect> getResConnect( );
    void ungetResConnect( Packet::Response::cResConnect&& data );
public:
    std::stack<Packet::Response::cResConnect> mResConnect;
public:
    boost::optional<Packet::Response::cResGetJemSeed> getResGetJemSeed( );
    void ungetResGetJemSeed( Packet::Response::cResGetJemSeed&& data );
public:
    std::stack<Packet::Response::cResGetJemSeed> mResGetJemSeed;
public:
    boost::optional<Packet::Response::cResGetJemPoint> getResGetJemPoint( );
    void ungetResGetJemPoint( Packet::Response::cResGetJemPoint&& data );
public:
    std::stack<Packet::Response::cResGetJemPoint> mResGetJemPoint;
public:
    boost::optional<Packet::Response::cResCheckGetJemPlayer> getResCheckGetJemPlayer( );
    void ungetResCheckGetJemPlayer( Packet::Response::cResCheckGetJemPlayer&& data );
public:
    std::stack<Packet::Response::cResCheckGetJemPlayer> mResCheckGetJemPlayer;
public:
    boost::optional<Packet::Response::cResCheckGetJemQuarry> getResCheckGetJemQuarry( );
    void ungetResCheckGetJemQuarry( Packet::Response::cResCheckGetJemQuarry&& data );
public:
    std::stack<Packet::Response::cResCheckGetJemQuarry> mResCheckGetJemQuarry;
public:
    boost::optional<Packet::Response::cResCheckPlayerRobJem> getResCheckPlayerRobJem( );
    void ungetResCheckPlayerRobJem( Packet::Response::cResCheckPlayerRobJem&& data );
public:
    std::stack<Packet::Response::cResCheckPlayerRobJem> mResCheckPlayerRobJem;
public:
    boost::optional<Packet::Response::cResCheckSetQuarry> getResCheckSetQuarry( );
    void ungetResCheckSetQuarry( Packet::Response::cResCheckSetQuarry&& data );
public:
    std::stack<Packet::Response::cResCheckSetQuarry> mResCheckSetQuarry;
public:
    boost::optional<Packet::Response::cResCheckPlayerDeath> getResCheckPlayerDeath( );
    void ungetResCheckPlayerDeath( Packet::Response::cResCheckPlayerDeath&& data );
public:
    std::stack<Packet::Response::cResCheckPlayerDeath> mResCheckPlayerDeath;
public:
    boost::optional<Packet::Response::cResMakeRoom> getResMakeRoom( );
    void ungetResMakeRoom( Packet::Response::cResMakeRoom&& data );
public:
    std::stack<Packet::Response::cResMakeRoom> mResMakeRoom;
public:
    boost::optional<Packet::Response::cResInRoom> getResInRoom( );
    void ungetResInRoom( Packet::Response::cResInRoom&& data );
public:
    std::stack<Packet::Response::cResInRoom> mResInRoom;
public:
    boost::optional<Packet::Response::cResWantTeamIn> getResWantTeamIn( );
    void ungetResWantTeamIn( Packet::Response::cResWantTeamIn&& data );
public:
    std::stack<Packet::Response::cResWantTeamIn> mResWantTeamIn;
public:
    boost::optional<Packet::Response::cResCheckBeginGame> getResCheckBeginGame( );
    void ungetResCheckBeginGame( Packet::Response::cResCheckBeginGame&& data );
public:
    std::stack<Packet::Response::cResCheckBeginGame> mResCheckBeginGame;
public:
    boost::optional<Packet::Response::cResCheckMember> getResCheckMember( );
    void ungetResCheckMember( Packet::Response::cResCheckMember&& data );
public:
    std::stack<Packet::Response::cResCheckMember> mResCheckMember;
public:
    boost::optional<Packet::Response::cResEndGamemainSetup> getResEndGamemainSetup( );
    void ungetResEndGamemainSetup( Packet::Response::cResEndGamemainSetup&& data );
public:
    std::stack<Packet::Response::cResEndGamemainSetup> mResEndGamemainSetup;
    // P=====END=====P
};
}
