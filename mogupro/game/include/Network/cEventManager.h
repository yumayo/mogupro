#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Event.hpp>
namespace Network
{
class cEventManager : public Utility::cSingletonAble<cEventManager>
{
public:
    Packet::Event::cEveString&& getEveString( );
    void ungetEveString( Packet::Event::cEveString const& data );
    std::stack<Packet::Event::cEveString> mEveString;
};
}