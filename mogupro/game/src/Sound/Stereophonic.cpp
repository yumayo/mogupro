#include <Sound/Stereophonic.h>
#include <Game/cPlayerManager.h>
#include <CameraManager/cCameraManager.h>
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
		for (auto& m : stereophonics)
		{
			ci::vec3 pos = Game::cPlayerManager::getInstance()->getActivePlayer()->getReferencePos();
			m.position(0,0,0);
			ci::vec3 direction = CAMERA->getCameraLook();
			direction = glm::normalize(direction);
			m.direction(direction.x, direction.y, direction.z);
			if (m.sourcePos != NULL)
			{
				m.currentSourcePos = *m.sourcePos;
			}
			ci::vec3 soundPos = m.currentSourcePos - pos;
			soundPos = glm::normalize(soundPos);
			m.soundPosition(soundPos.x, soundPos.y, soundPos.z);
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
