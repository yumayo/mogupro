#include <Sound/Stereophonic.h>
#include <Game/cPlayerManager.h>
#include <CameraManager/cCameraManager.h>
namespace Sound
{
	void StereophonicManager::open()
	{
	}

	void StereophonicManager::close()
	{
	}

	void StereophonicManager::update(float deltaTime)
	{
		for (auto& m : stereophonics)
		{
			if (m.time < 0)
				continue;
			ci::vec3 pos = CAMERA->getPos();
			m.position(0,0,0);
			ci::vec3 direction = CAMERA->getCameraLook();
			direction = glm::normalize(direction);
			m.direction(direction.x, direction.y, direction.z);
			if (m.sourcePos != NULL)
				m.currentSourcePos = *m.sourcePos;
			ci::vec3 soundPos = m.currentSourcePos - pos;
			soundPos = glm::normalize(soundPos);
			m.soundPosition(soundPos.x, soundPos.y, soundPos.z);

			m.time -= deltaTime;
		}

		clearList();
	}

	void StereophonicManager::clearList()
	{
		for (auto it = stereophonics.begin(); it != stereophonics.end();)
		{
			if (it->time < 0)
			{
				it->stop();
				it->unbind();
				it = stereophonics.erase(it);
				continue;
			}
			++it;
		}
	}

	void StereophonicManager::clear()
	{
		stereophonics.clear();
	}
}
