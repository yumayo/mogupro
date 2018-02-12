#include <Sound/cIntroLoopableBGM.h>
#include <cinder/app/App.h>
#include <Utility/cScheduler.h>
namespace Sound
{
cIntroLoopableBGM::cIntroLoopableBGM( )
{
	alGenSources( 1, &source );
}
cIntroLoopableBGM::~cIntroLoopableBGM( )
{
	if ( source != 0 )
	{
		stop( );
		alDeleteSources( 1, &source );
	}
	if ( introId != 0 )
	{
		alDeleteBuffers( 1, &introId );
	}
	if ( mainId != 0 )
	{
		alDeleteBuffers( 1, &mainId );
	}
}
void cIntroLoopableBGM::create( char const* const rawData, size_t rawDataByte, float loopBeginSecond, float loopEndSecond )
{
	alGenBuffers( 1, &introId );
	alBufferData
	( 
		introId,
		AL_FORMAT_STEREO16,
		rawData,
		calcOffset( loopBeginSecond ),
		44100
	);
	alGenBuffers( 1, &mainId );
	alBufferData
	(
		mainId,
		AL_FORMAT_STEREO16,
		rawData + calcOffset( loopBeginSecond ),
		calcOffset( loopEndSecond ) - calcOffset( loopBeginSecond ),
		44100
	);

	cinder::app::console( ) << "Sound: " << rawDataByte << std::endl;
	cinder::app::console( ) << "Sound: " << calcOffset( loopBeginSecond ) << std::endl;
	cinder::app::console( ) << "Sound: " << calcOffset( loopEndSecond ) << std::endl;
	cinder::app::console( ) << "Sound: " << calcOffset( loopEndSecond ) - calcOffset( loopBeginSecond ) << std::endl;

	this->loopBeginSecond = loopBeginSecond;
}
void cIntroLoopableBGM::play( ) const
{
	alSourceQueueBuffers( source, 1, &introId );
	alSourceQueueBuffers( source, 1, &mainId );
	alSourcePlay( source );
	time = 0.0F;
	introDeleted = false;
}
void cIntroLoopableBGM::stop( ) const
{
	alSourceStop( source );
}
void cIntroLoopableBGM::pause( ) const
{
	alSourcePause( source );
}
float cIntroLoopableBGM::getPitch() const
{
	ALfloat value = 0.0F;
	alGetSourcef(source, AL_PITCH, &value);
	return value;
}
void cIntroLoopableBGM::fadeout(float fadeSecond, float target)
{
	auto decrement = (getGain() - target) / fadeSecond;
	fadeHandle = Utility::cScheduler::getInstance()->applyLimitUpdate(fadeSecond, [this, decrement](float delta) { gain(getGain() - delta * decrement); }, [this] { stop(); });
}
void cIntroLoopableBGM::fadein(float fadeSecond, float target)
{
	auto increment = (target) / fadeSecond;
	gain( 0.0F );
	fadeHandle = Utility::cScheduler::getInstance()->applyLimitUpdate(fadeSecond, [this, increment](float delta) { gain(getGain() + delta * increment); });
}
void cIntroLoopableBGM::resume() const
{
	alSourcePlay( source );
}
void cIntroLoopableBGM::gain( float const value ) const
{
	alSourcef( source, AL_GAIN, value );
}
float cIntroLoopableBGM::getGain() const
{
	ALfloat value = 0.0F;
	alGetSourcef( source, AL_GAIN, &value );
	return value;
}
void cIntroLoopableBGM::pitch( float const value ) const
{
	alSourcef( source, AL_PITCH, value );
}
bool cIntroLoopableBGM::isPlaying( ) const
{
	ALint state;
	alGetSourcei( source, AL_SOURCE_STATE, &state );
	return state == AL_PLAYING;
}
void cIntroLoopableBGM::update( float delta )
{
	if ( !introDeleted )
	{
		if ( loopBeginSecond + 1.0F < time )
		{
			alSourceUnqueueBuffers( source, 1, &introId );
			auto e = alGetError( );
			if ( e == AL_INVALID_VALUE )
			{
				cinder::app::console( ) << "AL_INVALID_VALUE" << std::endl;
			}
			else if ( e == AL_INVALID_NAME )
			{
				cinder::app::console( ) << "AL_INVALID_NAME" << std::endl;
			}
			else if ( e == AL_INVALID_OPERATION )
			{
				cinder::app::console( ) << "AL_INVALID_OPERATION" << std::endl;
			}
			alSourcei( source, AL_LOOPING, AL_TRUE );
			introDeleted = true;
		}
	}

	if ( isPlaying( ) ) time += delta;
}
size_t cIntroLoopableBGM::calcOffset( float second )
{
	size_t bit8_offset = second * 44100;
	size_t byte16_offset = bit8_offset * 2;
	size_t toStereo = 2;
	return byte16_offset * toStereo;
}
}
