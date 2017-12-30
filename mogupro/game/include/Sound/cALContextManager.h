#pragma once
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <Utility/cSingletonAble.h>
namespace Sound
{
class cALContextManager : public Utility::cSingletonAble<cALContextManager>
{
public:
    cALContextManager( );
    ~cALContextManager( );
private:
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
};
}
