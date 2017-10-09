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
    Packet::Response::cResString&& getResString( );
    void ungetResString( Packet::Response::cResString const& data );
private:
    std::stack<Packet::Response::cResString> mResString;
    // P=====END=====P
};
}