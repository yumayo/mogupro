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
    // P=====END=====P
};
}