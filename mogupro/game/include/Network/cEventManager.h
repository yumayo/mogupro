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
    Packet::Event::cEveString&& getEveString( );
    void ungetEveString( Packet::Event::cEveString const& data );
private:
    std::stack<Packet::Event::cEveString> mEveString;
    // P=====END=====P
};
}