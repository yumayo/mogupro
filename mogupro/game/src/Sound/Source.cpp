#include <Sound/Source.h>
namespace Sound
{
	Source::Source()
	{
		alGenSources(1, &id_);
	}

	Source::~Source()
	{
		unbindBuffer();
		alDeleteSources(1, &id_);
	}

	ALuint Source::id() const { return id_; }

	void Source::bindBuffer(const Buffer& buffer) const
	{
		alSourcei(id_, AL_BUFFER, buffer.id());
		int a = 0;
	}

	void Source::unbindBuffer() const 
	{
		alSourcei(id_, AL_BUFFER, 0);
	}

	void Source::play() const
	{
		alSourcePlay(id_);
	}

	void Source::stop() const
	{
		alSourceStop(id_);
	}

	void Source::pause() const
	{
		alSourcePause(id_);
	}

	void Source::gain(const float value) const
	{
		alSourcef(id_, AL_GAIN, value);
	}

	void Source::pitch(const float value) const
	{
		alSourcef(id_, AL_PITCH, value);
	}

	// ループのON/OFF
	// value: trueでループON、falseでループOFF
	void Source::looping(const bool value) const 
	{
		alSourcei(id_, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
	}

	void Source::position(const float x, const float y, const float z) const
	{
		ALfloat sourcePos[] = { x, y, z };
		alSourcefv(id_, AL_POSITION, sourcePos);
	}
	void Source::direction(const float x, const float y, const float z)
	{
		// upベクトル
		float upX = 0.0;
		float upY = 1.0;
		float upZ = 0.0;
		ALfloat listenerOrientation[] = { x, y, z, upX, upY, upZ };
		alListenerfv(AL_ORIENTATION, listenerOrientation);
	}
	
	void Source::soundPosition(const float x, const float y, const float z)
	{
		ALfloat soundPos[] = { x ,y ,z };

		alSourcefv(id_, AL_POSITION, soundPos);
	}

	bool Source::isPlaying() const
	{
		ALint state;
		alGetSourcei(id_, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	float Source::currentTime() const
	{
		ALfloat currentTimeSec;
		alGetSourcef(id_, AL_SEC_OFFSET, &currentTimeSec);
		return currentTimeSec;
	}
}
