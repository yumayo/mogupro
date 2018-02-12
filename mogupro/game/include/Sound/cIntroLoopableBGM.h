#pragma once
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vector>
#include <memory>
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

	std::shared_ptr<int> fadeHandle;
public:
	cIntroLoopableBGM( );
	~cIntroLoopableBGM( );

	void create( char const* const rawData, size_t rawDataByte, float loopBeginSecond, float loopEndSecond );
	void play( ) const;
	void stop( ) const;
	void pause( ) const;
	void resume( ) const;
	void gain( float const value ) const;
	float getGain() const;
	void pitch( float const value ) const;
	float getPitch() const;
	bool isPlaying( ) const;
	void fadeout(float fadeSecond, float target);
	void fadein(float fadeSecond, float target);
	void update( float delta );
private:
	// 16bit stereo 44100Hz
	size_t calcOffset( float second );
};
}