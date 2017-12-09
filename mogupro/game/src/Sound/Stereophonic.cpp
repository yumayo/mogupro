#include <Sound/Stereophonic.h>
#include <Game/cPlayerManager.h>
namespace Sound
{
	void StereophonicManager::open()
	{
		device = alcOpenDevice(nullptr);
		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
	}

	void StereophonicManager::close()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void StereophonicManager::update()
	{
		for(auto& m : stereophonics)
		{
			ci::vec3 pos =  Game::cPlayerManager::getInstance()->getActivePlayer()->getReferencePos();
			m.position(pos.x, pos.y, pos.z);
			
			ci::vec3 direction = Game::cPlayerManager::getInstance()->getActivePlayer()->getReferencePlayerVec();
			m.direction(direction.x, direction.y, direction.z);
			m.soundPosition(m.sourcePos->x,m.sourcePos->y,m.sourcePos->z);
		}
	}

	void StereophonicManager::deleteMap(ci::vec3& sourcePos)
	{
		for (auto it = stereophonics.begin(); it != stereophonics.end(); ++it)
		{
			if (it->sourcePos != &sourcePos)continue;
			stereophonics.erase(it);
		}
	}

	void StereophonicManager::clear()
	{
		stereophonics.clear();
	}
}
