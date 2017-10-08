#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/cUDPManager.h>
namespace Network
{
class cUDPServerManager : public Utility::cSingletonAble<cUDPServerManager>, public cUDPManager
{
public:
};
}