#pragma once
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vector>
namespace Sound
{
class cIntroLoopableBGM
{
	ALuint source = 0;
	ALuint introId = 0;
	ALuint mainId = 0;

	float loopBeginSecond = 0.0F;
	mutable float time = 0.0F;
	mutable bool introDeleted = false;
public:
	cIntroLoopableBGM( );
	~cIntroLoopableBGM( );

	void create( char const* const rawData, size_t rawDataByte, float loopBeginSecond, float loopEndSecond );
	void play( ) const;
	void stop( ) const;
	void pause( ) const;
	void gain( float const value ) const;
	void pitch( float const value ) const;
	bool isPlaying( ) const;
	void update( float delta );
private:
	// 16bit stereo 44100Hz
	size_t calcOffset( float second );
};
}