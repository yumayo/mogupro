#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <map>
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
    bool isNewDliString( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Deliver::cDliString> mDliString;
    std::map<ubyte4, double> mDliStringSequenceIds;
public:
    boost::optional<Packet::Deliver::cDliPing> getDliPing( );
    void ungetDliPing( Packet::Deliver::cDliPing&& data );
    bool isNewDliPing( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Deliver::cDliPing> mDliPing;
    std::map<ubyte4, double> mDliPingSequenceIds;
public:
    boost::optional<Packet::Deliver::cDliPlayer> getDliPlayer( );
    void ungetDliPlayer( Packet::Deliver::cDliPlayer&& data );
    bool isNewDliPlayer( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Deliver::cDliPlayer> mDliPlayer;
    std::map<ubyte4, double> mDliPlayerSequenceIds;
public:
    boost::optional<Packet::Deliver::cDliBreakBlocks> getDliBreakBlocks( );
    void ungetDliBreakBlocks( Packet::Deliver::cDliBreakBlocks&& data );
    bool isNewDliBreakBlocks( Packet::PacketHeader const& header );
private:
    std::stack<Packet::Deliver::cDliBreakBlocks> mDliBreakBlocks;
    std::map<ubyte4, double> mDliBreakBlocksSequenceIds;
    // P=====END=====P
};
}
