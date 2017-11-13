#pragma once
#define NOMINMAX
#pragma comment(lib,"OpenAL32.lib")
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <Sound/Buffer.h>
namespace Sound
{
	class Source
	{
	public:
		Source();

		~Source();

		Source(const Source&) = delete;
		Source& operator=(const Source&) = delete;

		ALuint id() const;
		void bindBuffer(const Buffer& buffer) const;
		void unbindBuffer() const;
		void play() const;
		void stop() const;
		void pause() const;
		void gain(const float value) const;
		void pitch(const float value) const;
		void looping(const bool value) const;
		void position(const float x, const float y, const float z) const;
		void direction(const float x,const float y,const float z);
		void soundPosition(const float x, const float y, const float z);
		bool isPlaying() const;
		float currentTime() const;

	private:
		ALuint id_;
	};

}
