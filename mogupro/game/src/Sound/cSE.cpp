#include <Sound/cSE.h>
#include <cinder/app/App.h>
namespace Sound
{
using namespace cinder;
cSE::cSE( std::string const& assetsSePath )
{
    auto ctx = audio::master( );
    ctx->enable( );
    mGainRef = ctx->makeNode( new audio::GainNode( 1.0F ) );
    mBufferPlayerRef = ctx->makeNode( new audio::BufferPlayerNode( ) );
    mBufferPlayerRef->loadBuffer( audio::load( app::loadAsset( assetsSePath ) ) );
    mBufferPlayerRef >> mGainRef >> ctx->getOutput( );
};
cSE::~cSE( )
{
    stop( );
}
void cSE::play( )
{
    mBufferPlayerRef->start( );
}
void cSE::stop( )
{
    mBufferPlayerRef->stop( );
}
bool cSE::isPlaying( )
{
    return mBufferPlayerRef->isEnabled( );
}
bool cSE::isLooping( )
{
    return mBufferPlayerRef->isLoopEnabled( );
}
void cSE::setLooping( const bool islooping )
{
    mBufferPlayerRef->setLoopEnabled( islooping );
}
void cSE::setGain( const float gain )
{
    mGainRef->setValue( gain );
}
;
}
