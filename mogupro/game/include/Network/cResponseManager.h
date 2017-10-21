#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Response.hpp>
namespace Network
{
class cResponseManager : public Utility::cSingletonAble<cResponseManager>
{
    // P=====BEGIN=====P
public:
    Packet::Response::cResString getResString( );
    void ungetResString( Packet::Response::cResString const& data );
    std::stack<Packet::Response::cResString> mResString;
public:
    Packet::Response::cResPlayer getResPlayer( );
    void ungetResPlayer( Packet::Response::cResPlayer const& data );
    std::stack<Packet::Response::cResPlayer> mResPlayer;
public:
    Packet::Response::cResGetJemSeed getResGetJemSeed( );
    void ungetResGetJemSeed( Packet::Response::cResGetJemSeed const& data );
    std::stack<Packet::Response::cResGetJemSeed> mResGetJemSeed;
public:
    Packet::Response::cResGetJemPoint getResGetJemPoint( );
    void ungetResGetJemPoint( Packet::Response::cResGetJemPoint const& data );
    std::stack<Packet::Response::cResGetJemPoint> mResGetJemPoint;
public:
    Packet::Response::cResCheckGetJem getResCheckGetJem( );
    void ungetResCheckGetJem( Packet::Response::cResCheckGetJem const& data );
    std::stack<Packet::Response::cResCheckGetJem> mResCheckGetJem;
public:
    Packet::Response::cResCheckPlayerRobJem getResCheckPlayerRobJem( );
    void ungetResCheckPlayerRobJem( Packet::Response::cResCheckPlayerRobJem const& data );
    std::stack<Packet::Response::cResCheckPlayerRobJem> mResCheckPlayerRobJem;
public:
    Packet::Response::cResCheckBreakBlocks getResCheckBreakBlocks( );
    void ungetResCheckBreakBlocks( Packet::Response::cResCheckBreakBlocks const& data );
    std::stack<Packet::Response::cResCheckBreakBlocks> mResCheckBreakBlocks;
public:
    Packet::Response::cResCheckSetQuarry getResCheckSetQuarry( );
    void ungetResCheckSetQuarry( Packet::Response::cResCheckSetQuarry const& data );
    std::stack<Packet::Response::cResCheckSetQuarry> mResCheckSetQuarry;
public:
    Packet::Response::cResCheckPlayerDeath getResCheckPlayerDeath( );
    void ungetResCheckPlayerDeath( Packet::Response::cResCheckPlayerDeath const& data );
    std::stack<Packet::Response::cResCheckPlayerDeath> mResCheckPlayerDeath;
public:
    Packet::Response::cResMakeRoom getResMakeRoom( );
    void ungetResMakeRoom( Packet::Response::cResMakeRoom const& data );
    std::stack<Packet::Response::cResMakeRoom> mResMakeRoom;
public:
    Packet::Response::cResInRoom getResInRoom( );
    void ungetResInRoom( Packet::Response::cResInRoom const& data );
    std::stack<Packet::Response::cResInRoom> mResInRoom;
public:
    Packet::Response::cResWantTeamIn getResWantTeamIn( );
    void ungetResWantTeamIn( Packet::Response::cResWantTeamIn const& data );
    std::stack<Packet::Response::cResWantTeamIn> mResWantTeamIn;
public:
    Packet::Response::cResCheckBeginGame getResCheckBeginGame( );
    void ungetResCheckBeginGame( Packet::Response::cResCheckBeginGame const& data );
    std::stack<Packet::Response::cResCheckBeginGame> mResCheckBeginGame;
public:
    Packet::Response::cResCheckMember getResCheckMember( );
    void ungetResCheckMember( Packet::Response::cResCheckMember const& data );
    std::stack<Packet::Response::cResCheckMember> mResCheckMember;
    // P=====END=====P
};
}
