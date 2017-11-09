#include <Sound/Stereophonic.h>
namespace Sound
{
	void Stereophonic::read(const std::string& file)
	{
		wav.read(file);
	}

	void Stereophonic::bindWav()
	{
		const char* wavData = wav.data();
		// キャスト
		const ALshort* sample = (const ALshort*)wavData;
		// データサイズ
		int sampleSize = wav.size() / 2;
		// 配列の開始位置と終了位置から作成
		data = std::vector<ALshort>(&sample[0],&sample[sampleSize]);
	}

	void Stereophonic::bind()
	{
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

	void Stereophonic::setListenerPos(std::shared_ptr<ci::vec3> listenerPos)
	{
		listenerPosition = listenerPos;
		alListenerfv(AL_POSITION, reinterpret_cast<float*>(&listenerPosition));
	}
	void Stereophonic::setListenerPos()
	{
		float listenerP[] = { listenerPosition->x,listenerPosition->y,listenerPosition->z };
		alListenerfv(AL_POSITION, listenerP);
	}

	void Stereophonic::setListenerDirection(std::shared_ptr<ci::vec3> listerDir)
	{
		listenerDirection = listerDir;
		// upベクトル
		float upX = 0.0;
		float upY = 1.0;
		float upZ = 0.0;
		ALfloat listenerOrientation[] = { listenerDirection->x, listenerDirection->y, listenerDirection->z, upX, upY, upZ };
		alListenerfv(AL_ORIENTATION, listenerOrientation);
	}
	void Stereophonic::setListenerDirection()
	{
		float upX = 0.0;
		float upY = 1.0;
		float upZ = 0.0;
		ALfloat listenerOrientation[] = { listenerDirection->x, listenerDirection->y, listenerDirection->z, upX, upY, upZ };
		alListenerfv(AL_ORIENTATION, listenerOrientation);
	}

	void Stereophonic::setSourcePos(std::shared_ptr<ci::vec3> sourcePos)
	{
		sourcePosition = sourcePos;
		// ソースの位置を移動
		alSourcefv(sourceId, AL_POSITION, reinterpret_cast<float*>(&sourcePosition));
	}
	void Stereophonic::setSourcePos()
	{
		float sourceP[] = { sourcePosition->x,sourcePosition->y,sourcePosition->z };
		alSourcefv(sourceId, AL_POSITION, sourceP);
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

	void Stereophonic::update()
	{
		if (!sourcePosition)
		{
			stop();
			return;
		}

		setListenerPos();
		setListenerDirection();
		setSourcePos();
		
	}
	void Stereophonic::clean()
	{
		// ソースの破棄
		alDeleteSources(1, &sourceId);
		// バッファの破棄
		alDeleteBuffers(1, &bufferId);
	}


	void delay(Stereophonic& stereophonic, float attenuation, float time, int repeat)
	{
		// 結果を格納する変数
		// TIPS:input_samplesと同じサイズをあらかじめ確保
		std::vector<ALshort> outputSamples(stereophonic.data.size());

		// TODO:SAMPLING_RATEは元波形のサンプリングレートに併せて適時定義のこと
		//        波形のサンプリングレートが44.1kHzなら
		//        const int SAMPLING_RATE = 44100;
		//        などと定義
		float d = 44100 * time;

		for (size_t n = 0; n < stereophonic.data.size(); ++n) {
			outputSamples[n] = stereophonic.data[n];

			for (int i = 1; i <= repeat; ++i) {
				// 過去の波形の位置
				int m = int(n - i * d);

				if (m >= 0) {
					// 過去の音データをミックスする
					// 波形の減衰にはべき乗を使っている
					outputSamples[n] += std::pow(attenuation, float(i)) * stereophonic.data[m];
				}
			}
		}
		stereophonic.data = outputSamples;
	}


	void StereophonicManager::open()
	{
		device = alcOpenDevice(nullptr);
		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
	}

	void StereophonicManager::close()
	{
		for (int i = 0; i < stereophonicList.size(); ++i)
			stereophonicList[i].clean();

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

}
