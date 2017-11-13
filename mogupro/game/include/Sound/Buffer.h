#pragma once
#define NOMINMAX
#pragma comment(lib,"OpenAL32.lib")
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
namespace Sound
{
	class Buffer
	{
	public:
		Buffer();
		~Buffer();
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		template<class T>
		inline explicit Buffer(T type)
		{
			alGenBuffers(1, &id_);
			time = type.totalTime();

			int a = type.alDataSize();

			alBufferData(id_,
				type.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
				type.alData(),
				type.alDataSize(),
				type.sampleRate());
		}

		ALuint id() const;

	private:
		ALuint id_;

		float time;
	};
}
