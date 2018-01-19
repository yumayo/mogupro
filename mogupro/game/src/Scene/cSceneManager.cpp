#include <Scene/cSceneManager.h>
#include <Scene/Member/cDontDestroyOnLoad.h>
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
cSceneBase & cSceneManager::getDontDestroyOnLoad( )
{
	return *mDontDestroyOnLoad;
}
}
