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
    boost::optional<Packet::Event::cEvePlayers> getEvePlayers( );
    void ungetEvePlayers( Packet::Event::cEvePlayers&& data );
public:
    std::stack<Packet::Event::cEvePlayers> mEvePlayers;
public:
    boost::optional<Packet::Event::cEveGetJemPlayer> getEveGetJemPlayer( );
    void ungetEveGetJemPlayer( Packet::Event::cEveGetJemPlayer&& data );
public:
    std::stack<Packet::Event::cEveGetJemPlayer> mEveGetJemPlayer;
public:
    boost::optional<Packet::Event::cEveGetJemQuarry> getEveGetJemQuarry( );
    void ungetEveGetJemQuarry( Packet::Event::cEveGetJemQuarry&& data );
public:
    std::stack<Packet::Event::cEveGetJemQuarry> mEveGetJemQuarry;
public:
    boost::optional<Packet::Event::cEveBreakBlocks> getEveBreakBlocks( );
    void ungetEveBreakBlocks( Packet::Event::cEveBreakBlocks&& data );
public:
    std::stack<Packet::Event::cEveBreakBlocks> mEveBreakBlocks;
public:
    boost::optional<Packet::Event::cEveSetQuarry> getEveSetQuarry( );
    void ungetEveSetQuarry( Packet::Event::cEveSetQuarry&& data );
public:
    std::stack<Packet::Event::cEveSetQuarry> mEveSetQuarry;
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
