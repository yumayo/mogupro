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
		for (auto& m : stereophonicList)
		{
			if (m.second.time < 0)
				continue;
			ci::vec3 pos = CAMERA->getPos();
			m.second.position(0,0,0);
			ci::vec3 direction = CAMERA->getCameraLook();
			direction = glm::normalize(direction);
			m.second.direction(direction.x, direction.y, direction.z);
			if (m.second.sourcePos != NULL)
				m.second.currentSourcePos = *m.second.sourcePos;
			ci::vec3 soundPos = m.second.currentSourcePos - pos;
			//soundPos = glm::normalize(soundPos);
			m.second.soundPosition(soundPos.x / 10.0f, soundPos.y / 10.0f, soundPos.z / 10.0f);

			m.second.time -= deltaTime;
		}

		clearList();
	}

	void StereophonicManager::clearList()
	{
		for (auto it = stereophonicList.begin(); it != stereophonicList.end();)
		{
			if (it->second.time < 0)
			{
				it->second.stop();
				it->second.unbind();
				it = stereophonicList.erase(it);
				continue;
			}
			++it;
		}
	}

	void StereophonicManager::clear()
	{
		stereophonicList.clear();
	}
}
