#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Event.hpp>
namespace Network
{
class cEventManager : public Utility::cSingletonAble<cEventManager>
{
    // P=====BEGIN=====P
public:
    Packet::Event::cEveString getEveString( );
    void ungetEveString( Packet::Event::cEveString const& data );
private:
    std::stack<Packet::Event::cEveString> mEveString;
public:
    Packet::Event::cEveGetJem getEveGetJem( );
    void ungetEveGetJem( Packet::Event::cEveGetJem const& data );
private:
    std::stack<Packet::Event::cEveGetJem> mEveGetJem;
public:
    Packet::Event::cEvePlayerRobJem getEvePlayerRobJem( );
    void ungetEvePlayerRobJem( Packet::Event::cEvePlayerRobJem const& data );
private:
    std::stack<Packet::Event::cEvePlayerRobJem> mEvePlayerRobJem;
public:
    Packet::Event::cEvePlayerDeath getEvePlayerDeath( );
    void ungetEvePlayerDeath( Packet::Event::cEvePlayerDeath const& data );
private:
    std::stack<Packet::Event::cEvePlayerDeath> mEvePlayerDeath;
    // P=====END=====P
};
}
