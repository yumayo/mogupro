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
private:
    std::stack<Packet::Request::cReqString> mReqString;
public:
    Packet::Request::cReqPlayer getReqPlayer( );
    void ungetReqPlayer( Packet::Request::cReqPlayer const& data );
private:
    std::stack<Packet::Request::cReqPlayer> mReqPlayer;
public:
    Packet::Request::cReqGetJemSeed getReqGetJemSeed( );
    void ungetReqGetJemSeed( Packet::Request::cReqGetJemSeed const& data );
private:
    std::stack<Packet::Request::cReqGetJemSeed> mReqGetJemSeed;
public:
    Packet::Request::cReqGetJemPoint getReqGetJemPoint( );
    void ungetReqGetJemPoint( Packet::Request::cReqGetJemPoint const& data );
private:
    std::stack<Packet::Request::cReqGetJemPoint> mReqGetJemPoint;
public:
    Packet::Request::cReqCheckGetJem getReqCheckGetJem( );
    void ungetReqCheckGetJem( Packet::Request::cReqCheckGetJem const& data );
private:
    std::stack<Packet::Request::cReqCheckGetJem> mReqCheckGetJem;
public:
    Packet::Request::cReqCheckPlayerRobJem getReqCheckPlayerRobJem( );
    void ungetReqCheckPlayerRobJem( Packet::Request::cReqCheckPlayerRobJem const& data );
private:
    std::stack<Packet::Request::cReqCheckPlayerRobJem> mReqCheckPlayerRobJem;
public:
    Packet::Request::cReqCheckBrakeBlocks getReqCheckBrakeBlocks( );
    void ungetReqCheckBrakeBlocks( Packet::Request::cReqCheckBrakeBlocks const& data );
private:
    std::stack<Packet::Request::cReqCheckBrakeBlocks> mReqCheckBrakeBlocks;
public:
    Packet::Request::cReqCheckSetQuarry getReqCheckSetQuarry( );
    void ungetReqCheckSetQuarry( Packet::Request::cReqCheckSetQuarry const& data );
private:
    std::stack<Packet::Request::cReqCheckSetQuarry> mReqCheckSetQuarry;
public:
    Packet::Request::cReqCheckPlayerDeath getReqCheckPlayerDeath( );
    void ungetReqCheckPlayerDeath( Packet::Request::cReqCheckPlayerDeath const& data );
private:
    std::stack<Packet::Request::cReqCheckPlayerDeath> mReqCheckPlayerDeath;
    // P=====END=====P
};
}
