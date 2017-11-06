//#include <Sound/Stereophonic.h>
//namespace Sound
//{
//	void Stereophonic::read(const std::string& file)
//	{
//		wav.read(file);
//	}
//	void Stereophonic::bind()
//	{
//		device = alcOpenDevice(nullptr);
//		context = alcCreateContext(device, nullptr);
//		alcMakeContextCurrent(context);
//		// �o�b�t�@�̐���
//		alGenBuffers(1, &bufferId);
//		ALenum format = (wav.channel() == 1)
//			? AL_FORMAT_MONO16
//			: AL_FORMAT_STEREO16;
//
//		// �p�ӂ����T�C���g���o�b�t�@�ɃR�s�[
//		alBufferData(bufferId,
//			format,                       // ���m�����A�ʎq���r�b�g��:16
//			wav.data(),                   // ���j�APCM�`���f�[�^���i�[����Ă���ꏊ
//			wav.size(),                   // �T�C�Y(�o�C�g��)
//			wav.sampleRate());            // �T���v�����O���[�g
//										  // �\�[�X�̐���
//		alGenSources(1, &sourceId);
//
//		// �\�[�X�ɍĐ��������o�b�t�@�����蓖�Ă�
//		alSourcei(sourceId, AL_BUFFER, bufferId);
//	}
//
//	void Stereophonic::setListenerPos(const float& x, const float& y, const float& z)
//	{
//		ALfloat listenerPos[] = { x,y,z };
//		alListenerfv(AL_POSITION, listenerPos);
//	}
//
//	void Stereophonic::setListenerDirection(const float& x, const float& y, const float& z)
//	{
//		// up�x�N�g��
//		float upX = 0.0;
//		float upY = 1.0;
//		float upZ = 0.0;
//		ALfloat listenerOrientation[] = { x, y, z, upX, upY, upZ };
//		alListenerfv(AL_ORIENTATION, listenerOrientation);
//	}
//
//	void Stereophonic::setSourcePos(const float& x, const float& y, const float& z)
//	{
//		// �\�[�X�̈ʒu���ړ�
//		ALfloat sourcePos[] = { x,y,z };
//		alSourcefv(sourceId, AL_POSITION, sourcePos);
//	}
//
//	void Stereophonic::play()
//	{
//		alSourcePlay(sourceId);
//	}
//
//	void Stereophonic::stop()
//	{
//		alSourceStop(sourceId);
//	}
//
//	void Stereophonic::pause()
//	{
//		alSourcePause(sourceId);
//	}
//
//	void Stereophonic::gain(const float value)
//	{
//		alSourcef(sourceId, AL_GAIN, value);
//	}
//
//	void Stereophonic::pitch(const float value)
//	{
//		alSourcef(sourceId, AL_PITCH, value);
//	}
//
//	void Stereophonic::looping(const bool value)
//	{
//		alSourcei(sourceId, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
//	}
//
//	bool Stereophonic::isPlaying()
//	{
//		ALint state;
//		alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
//		return state == AL_PLAYING;
//	}
//
//	float Stereophonic::currentTime()
//	{
//		ALfloat currentTime;
//		alGetSourcef(sourceId, AL_SEC_OFFSET, &currentTime);
//		return currentTime;
//	}
//
//	void Stereophonic::clean()
//	{
//		// �\�[�X�̔j��
//		alDeleteSources(1, &sourceId);
//		// �o�b�t�@�̔j��
//		alDeleteBuffers(1, &bufferId);
//		// OpenAL�̌�n��
//		alcMakeContextCurrent(nullptr);
//		alcDestroyContext(context);
//		alcCloseDevice(device);
//	}
//
//}
