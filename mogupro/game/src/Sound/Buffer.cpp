#include <Sound/Buffer.h>
namespace Sound
{
	Buffer::Buffer() : time(0.0f)
	{
		alGenBuffers(1, &id_);
	}


	Buffer::~Buffer()
	{
		alDeleteBuffers(1, &id_);
	}

	ALuint Buffer::id() const { return id_; }
}
