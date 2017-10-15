#pragma once
#include <cinder/audio/Context.h>
#include <cinder/audio/NodeEffects.h>
#include <cinder/audio/SamplePlayerNode.h>
namespace Sound
{
class cBGM
{
public:
    cBGM( std::string const& assetsSePath );
    ~cBGM( );
    void play( );
    void stop( );
    bool isPlaying( );
    bool isLooping( );
    void setLooping( const bool islooping );
    void setGain( const float gainSize );
private:
    cinder::audio::BufferPlayerNodeRef mBufferPlayerRef;
    cinder::audio::GainNodeRef mGainRef;
};
}
