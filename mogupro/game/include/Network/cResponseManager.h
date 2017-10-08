#pragma once
#include <Utility/cSingletonAble.h>
#include <stack>
#include <Network/Packet/Response.hpp>
namespace Network
{
class cResponseManager : public Utility::cSingletonAble<cResponseManager>
{
public:
    Packet::Response::cResString&& getResString( );
    void ungetResString( Packet::Response::cResString const& data );
    std::stack<Packet::Response::cResString> mResString;
};
}