#include <Sound/cBGM.h>
#include <cinder/app/App.h>
#include <Utility/cScheduler.h>
namespace Sound
{
using namespace cinder;
cBGM::cBGM( std::string const& assetsSePath )
{
    auto ctx = audio::master( );
    ctx->enable( );
    mGainRef = ctx->makeNode( new audio::GainNode( 1.0F ) );
    mBufferPlayerRef = ctx->makeNode( new audio::BufferPlayerNode( ) );
    mBufferPlayerRef->loadBuffer( audio::load( app::loadAsset( assetsSePath ) ) );
    mBufferPlayerRef >> mGainRef >> ctx->getOutput( );
};
cBGM::~cBGM( )
{
    stop( );
}
void cBGM::play( )
{
    mBufferPlayerRef->start( );
}
void cBGM::stop( )
{
    mBufferPlayerRef->stop( );
}
bool cBGM::isPlaying( )
{
    return mBufferPlayerRef->isEnabled( );
}
bool cBGM::isLooping( )
{
    return mBufferPlayerRef->isLoopEnabled( );
}
void cBGM::setLooping( const bool islooping )
{
    mBufferPlayerRef->setLoopEnabled( islooping );
}
void cBGM::setGain( const float gain )
{
    mGainRef->setValue( gain );
}
float cBGM::getGain()
{
	return mGainRef->getValue();
}
void cBGM::fadeout(const float fadeSecond, const float target)
{
	auto decrement = ( getGain() - target ) / fadeSecond;
	cinder::app::console() << decrement << std::endl;
	fadeHandle = Utility::cScheduler::getInstance()->applyLimitUpdate(fadeSecond, [this, decrement](float delta) { setGain(getGain() - delta * decrement); }, [this]() { stop(); });
}
void cBGM::fadein(const float fadeSecond, const float target)
{
	auto increment = ( target ) / fadeSecond;
	setGain( 0.0F );
	fadeHandle = Utility::cScheduler::getInstance()->applyLimitUpdate(fadeSecond, [this, increment](float delta){ setGain(getGain() + delta * increment); });
}
}
