#include <Sound/Stereophonic.h>
namespace Sound
{
	void Stereophonic::read(const std::string& file)
	{
		wav.read(file);
	}
	void Stereophonic::bind()
	{
		device = alcOpenDevice(nullptr);
		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
		// バッファの生成
		alGenBuffers(1, &bufferId);
		ALenum format = (wav.channel() == 1)
			? AL_FORMAT_MONO16
			: AL_FORMAT_STEREO16;

		// 用意したサイン波をバッファにコピー
		alBufferData(bufferId,
			format,                       // モノラル、量子化ビット数:16
			wav.data(),                   // リニアPCM形式データが格納されている場所
			wav.size(),                   // サイズ(バイト数)
			wav.sampleRate());            // サンプリングレート
										  // ソースの生成
		alGenSources(1, &sourceId);

		// ソースに再生したいバッファを割り当てる
		alSourcei(sourceId, AL_BUFFER, bufferId);
	}

	void Stereophonic::setListenerPos(const float& x, const float& y, const float& z)
	{
		ALfloat listenerPos[] = { x,y,z };
		alListenerfv(AL_POSITION, listenerPos);
	}

	void Stereophonic::setListenerDirection(const float& x, const float& y, const float& z)
	{
		// upベクトル
		float upX = 0.0;
		float upY = 1.0;
		float upZ = 0.0;
		ALfloat listenerOrientation[] = { x, y, z, upX, upY, upZ };
		alListenerfv(AL_ORIENTATION, listenerOrientation);
	}

	void Stereophonic::setSourcePos(const float& x, const float& y, const float& z)
	{
		// ソースの位置を移動
		ALfloat sourcePos[] = { x,y,z };
		alSourcefv(sourceId, AL_POSITION, sourcePos);
	}

	void Stereophonic::play()
	{
		alSourcePlay(sourceId);
	}

	void Stereophonic::stop()
	{
		alSourceStop(sourceId);
	}

	void Stereophonic::pause()
	{
		alSourcePause(sourceId);
	}

	void Stereophonic::gain(const float value)
	{
		alSourcef(sourceId, AL_GAIN, value);
	}

	void Stereophonic::pitch(const float value)
	{
		alSourcef(sourceId, AL_PITCH, value);
	}

	void Stereophonic::looping(const bool value)
	{
		alSourcei(sourceId, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
	}

	bool Stereophonic::isPlaying()
	{
		ALint state;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	float Stereophonic::currentTime()
	{
		ALfloat currentTime;
		alGetSourcef(sourceId, AL_SEC_OFFSET, &currentTime);
		return currentTime;
	}

	void Stereophonic::clean()
	{
		// ソースの破棄
		alDeleteSources(1, &sourceId);
		// バッファの破棄
		alDeleteBuffers(1, &bufferId);
		// OpenALの後始末
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

}
