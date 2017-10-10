#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDPManager.h>
namespace Network
{
class cUDPClientManager : public Utility::cSingletonAble<cUDPClientManager>
{
};
}