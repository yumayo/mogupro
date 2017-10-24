#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Deliver.hpp>
#include <boost/optional.hpp>
namespace Network
{
class cDeliverManager : public Utility::cSingletonAble<cDeliverManager>
{
    // P=====BEGIN=====P
public:
    boost::optional<Packet::Deliver::cDliString> getDliString( );
    void ungetDliString( Packet::Deliver::cDliString&& data );
public:
    std::stack<Packet::Deliver::cDliString> mDliString;
public:
    boost::optional<Packet::Deliver::cDliPing> getDliPing( );
    void ungetDliPing( Packet::Deliver::cDliPing&& data );
public:
    std::stack<Packet::Deliver::cDliPing> mDliPing;
public:
    boost::optional<Packet::Deliver::cDliPlayer> getDliPlayer( );
    void ungetDliPlayer( Packet::Deliver::cDliPlayer&& data );
public:
    std::stack<Packet::Deliver::cDliPlayer> mDliPlayer;
public:
    boost::optional<Packet::Deliver::cDliBreakBlocks> getDliBreakBlocks( );
    void ungetDliBreakBlocks( Packet::Deliver::cDliBreakBlocks&& data );
public:
    std::stack<Packet::Deliver::cDliBreakBlocks> mDliBreakBlocks;
    // P=====END=====P
};
}
