#include <Scene/cSceneManager.h>
#include <Scene/Member/cDontDestroyOnLoad.h>
#include <Scene/Member/cTitle.h>
namespace Scene
{
cSceneManager::cSceneManager( )
{
	mDontDestroyOnLoad = std::make_shared<Member::cDontDestroyOnLoad>( );
}
cSceneManager::~cSceneManager( )
{
	for ( auto& scene : mSceneBases )
	{
		scene->shutDown( );
	}
	mDontDestroyOnLoad->shutDown( );
}
void cSceneManager::update(float delta)
{
	try
	{
		if (ENV->isPadPress(4) && ENV->isPadPress(5) && ENV->isPadPress(6) && ENV->isPadPress(7))
		{
			shift<Scene::Member::cTitle>();
		}
	}
	catch (SceneAllDeleted const&)
	{
		// nothing
	}

	try
	{
		iteration = true;
		for (iterator = 0; iterator < mSceneBases.size(); ++iterator)
		{
			try
			{
				mSceneBases[iterator]->update(delta);
			}
			catch (SceneRemoveSelf const&)
			{
				// nothing
			}
		}
		iteration = false;
		mDontDestroyOnLoad->update(delta);
	}
	catch (SceneAllDeleted const&)
	{
		// nothing
	}
}
cSceneBase & cSceneManager::getDontDestroyOnLoad( )
{
	return *mDontDestroyOnLoad;
}
}
