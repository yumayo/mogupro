#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
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
public:
    std::stack<Packet::Event::cEveString> mEveString;
public:
    boost::optional<Packet::Event::cEveGetJem> getEveGetJem( );
    void ungetEveGetJem( Packet::Event::cEveGetJem&& data );
public:
    std::stack<Packet::Event::cEveGetJem> mEveGetJem;
public:
    boost::optional<Packet::Event::cEveBreakBlocks> getEveBreakBlocks( );
    void ungetEveBreakBlocks( Packet::Event::cEveBreakBlocks&& data );
public:
    std::stack<Packet::Event::cEveBreakBlocks> mEveBreakBlocks;
public:
    boost::optional<Packet::Event::cEvePlayerRobJem> getEvePlayerRobJem( );
    void ungetEvePlayerRobJem( Packet::Event::cEvePlayerRobJem&& data );
public:
    std::stack<Packet::Event::cEvePlayerRobJem> mEvePlayerRobJem;
public:
    boost::optional<Packet::Event::cEvePlayerDeath> getEvePlayerDeath( );
    void ungetEvePlayerDeath( Packet::Event::cEvePlayerDeath&& data );
public:
    std::stack<Packet::Event::cEvePlayerDeath> mEvePlayerDeath;
    // P=====END=====P
};
}
