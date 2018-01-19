#pragma once

#include <Utility/cSingletonAble.h>
#include <memory>
#include "cSceneBase.h"
#include <Utility/cInput.h>

//! @file cSceneManager
//! @brief Scene切り替えを含め現在のSceneのすべてを管理するクラスです
//! @note 
//!      Changeで引数にSceneBaseを継承したClassを渡してシーンを変えます
//!      そこで判断しています
//! @date 2017-10-12
//! @author Taka Nomoto

namespace Scene
{
class cSceneManager : public Utility::cSingletonAble<cSceneManager>
{
public:
	cSceneManager( );
	~cSceneManager( );

	void update( float delta )
	{
		for ( auto& scene : mSceneBases )
		{
			if ( scene )
			{
				scene->update( delta );
			}
		}
		mDontDestroyOnLoad->update( delta );
	}
	void draw( )
	{
		for ( auto& scene : mSceneBases )
		{
			if ( scene )
			{
				scene->draw( );
			}
		}
		mDontDestroyOnLoad->draw( );
	}
	void draw2D( )
	{
		for ( auto& scene : mSceneBases )
		{
			if ( scene )
			{
				scene->draw2D( );
			}
		}
		mDontDestroyOnLoad->draw2D( );
	}

	class SceneNotFound : public std::runtime_error
	{
	public:
		SceneNotFound( ) : std::runtime_error( "シーンが見つかりません。" )
		{
		}
	};
	template<class TyScene>
	TyScene& find( )
	{
		for ( auto& scene : mSceneBases )
		{
			if ( scene && ( scene->getName( ) == typeid(TyScene).name( ) ) )
			{
				return scene;
			}
		}
		throw SceneNotFound( );
	}

	cSceneBase& getDontDestroyOnLoad( );

	template<class TyScene, class... Args>
	void add( Args... args )
	{
		auto scene = std::make_shared<TyScene>( args... );
		mSceneBases.emplace_back( scene );
		scene->setup( );
	}

	template<class TyScene>
	void erase( )
	{
		auto it = mSceneBases.begin( );
		while ( it != mSceneBases.end( ) )
		{
			if ( ( *it )->getName( ) == typeid( TyScene ).name( ) )
			{
				(*it)->shutDown( );
				it = mSceneBases.erase( it );
			}
			else ++it;
		}
	}

	template<class TyScene, class... Args>
	void shift( Args... args )
	{
		for ( auto& scene : mSceneBases )
		{
			if ( scene )
			{
				scene->shutDown( );
			}
		}
		ENV->flashInput( );

		change<TyScene>( args... );

		for ( auto& scene : mSceneBases )
		{
			if ( scene )
			{
				scene->setup( );
			}
		}
	}
private:
	std::vector<std::shared_ptr<cSceneBase>> mSceneBases;
	std::shared_ptr<cSceneBase> mDontDestroyOnLoad;

	// setupとshutDownは必ず呼びたいためprivateに.
	template<class TyScene, class... Args>
	void change( Args...args )
	{
		auto it = mSceneBases.begin( );
		while ( it != mSceneBases.end( ) )
		{
			it = mSceneBases.erase( it );
		}
		mSceneBases.emplace_back( std::make_shared<TyScene>( args... ) );
	};
};
}