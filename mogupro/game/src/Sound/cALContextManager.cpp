#include <Sound/cALContextManager.h>
namespace Sound
{
cALContextManager::cALContextManager( )
{
	device = alcOpenDevice( nullptr );
	context = alcCreateContext( device, nullptr );
	alcMakeContextCurrent( context );
}
cALContextManager::~cALContextManager( )
{
	alcMakeContextCurrent( nullptr );
	alcDestroyContext( context );
	alcCloseDevice( device );
}
}
