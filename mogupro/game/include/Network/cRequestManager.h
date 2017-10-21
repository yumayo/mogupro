#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Request.hpp>
namespace Network
{
class cRequestManager : public Utility::cSingletonAble<cRequestManager>
{
    // P=====BEGIN=====P
public:
    Packet::Request::cReqString getReqString( );
    void ungetReqString( Packet::Request::cReqString const& data );
    std::stack<Packet::Request::cReqString> mReqString;
public:
    Packet::Request::cReqPlayer getReqPlayer( );
    void ungetReqPlayer( Packet::Request::cReqPlayer const& data );
    std::stack<Packet::Request::cReqPlayer> mReqPlayer;
public:
    Packet::Request::cReqGetJemSeed getReqGetJemSeed( );
    void ungetReqGetJemSeed( Packet::Request::cReqGetJemSeed const& data );
    std::stack<Packet::Request::cReqGetJemSeed> mReqGetJemSeed;
public:
    Packet::Request::cReqGetJemPoint getReqGetJemPoint( );
    void ungetReqGetJemPoint( Packet::Request::cReqGetJemPoint const& data );
    std::stack<Packet::Request::cReqGetJemPoint> mReqGetJemPoint;
public:
    Packet::Request::cReqCheckGetJem getReqCheckGetJem( );
    void ungetReqCheckGetJem( Packet::Request::cReqCheckGetJem const& data );
    std::stack<Packet::Request::cReqCheckGetJem> mReqCheckGetJem;
public:
    Packet::Request::cReqCheckPlayerRobJem getReqCheckPlayerRobJem( );
    void ungetReqCheckPlayerRobJem( Packet::Request::cReqCheckPlayerRobJem const& data );
    std::stack<Packet::Request::cReqCheckPlayerRobJem> mReqCheckPlayerRobJem;
public:
    Packet::Request::cReqCheckBreakBlocks getReqCheckBreakBlocks( );
    void ungetReqCheckBreakBlocks( Packet::Request::cReqCheckBreakBlocks const& data );
    std::stack<Packet::Request::cReqCheckBreakBlocks> mReqCheckBreakBlocks;
public:
    Packet::Request::cReqCheckSetQuarry getReqCheckSetQuarry( );
    void ungetReqCheckSetQuarry( Packet::Request::cReqCheckSetQuarry const& data );
    std::stack<Packet::Request::cReqCheckSetQuarry> mReqCheckSetQuarry;
public:
    Packet::Request::cReqCheckPlayerDeath getReqCheckPlayerDeath( );
    void ungetReqCheckPlayerDeath( Packet::Request::cReqCheckPlayerDeath const& data );
    std::stack<Packet::Request::cReqCheckPlayerDeath> mReqCheckPlayerDeath;
public:
    Packet::Request::cReqMakeRoom getReqMakeRoom( );
    void ungetReqMakeRoom( Packet::Request::cReqMakeRoom const& data );
    std::stack<Packet::Request::cReqMakeRoom> mReqMakeRoom;
public:
    Packet::Request::cReqInRoom getReqInRoom( );
    void ungetReqInRoom( Packet::Request::cReqInRoom const& data );
    std::stack<Packet::Request::cReqInRoom> mReqInRoom;
public:
    Packet::Request::cReqWantTeamIn getReqWantTeamIn( );
    void ungetReqWantTeamIn( Packet::Request::cReqWantTeamIn const& data );
    std::stack<Packet::Request::cReqWantTeamIn> mReqWantTeamIn;
public:
    Packet::Request::cReqCheckBeginGame getReqCheckBeginGame( );
    void ungetReqCheckBeginGame( Packet::Request::cReqCheckBeginGame const& data );
    std::stack<Packet::Request::cReqCheckBeginGame> mReqCheckBeginGame;
public:
    Packet::Request::cReqCheckMember getReqCheckMember( );
    void ungetReqCheckMember( Packet::Request::cReqCheckMember const& data );
    std::stack<Packet::Request::cReqCheckMember> mReqCheckMember;
    // P=====END=====P
};
}
