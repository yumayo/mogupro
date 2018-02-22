#pragma once

#include <Utility/cSingletonAble.h>
#include <memory>
#include "cSceneBase.h"
#include <Utility/cInput.h>
#include <algorithm>

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
private:
	class SceneRemoveSelf : public std::runtime_error
	{
	public:
		SceneRemoveSelf( ) : std::runtime_error( "シーンを追加しました。" )
		{
		}
	};
	class SceneAllDeleted : public std::runtime_error
	{
	public:
		SceneAllDeleted( ) : std::runtime_error( "シーンを削除しました。" )
		{
		}
	};
public:
	class SceneNotFound : public std::runtime_error
	{
	public:
		SceneNotFound( ) : std::runtime_error( "シーンが見つかりません。" )
		{
		}
	};
	void update(float delta);
	void draw( )
	{
		for ( auto& scene : mSceneBases )
		{
			scene->draw( );
		}
		mDontDestroyOnLoad->draw( );
	}
	void draw2D( )
	{
		for ( auto& scene : mSceneBases )
		{
			scene->draw2D( );
		}
		mDontDestroyOnLoad->draw2D( );
	}

	template<class TyScene>
	TyScene& find( )
	{
		for ( auto& scene : mSceneBases )
		{
			if ( scene->getName( ) == typeid(TyScene).name( ) )
			{
				return scene;
			}
		}
		throw SceneNotFound( );
	}

	std::vector<std::string> getSceneNames()
	{
		std::vector<std::string> ret;
		for (auto& scene : mSceneBases)
		{
			ret.emplace_back(scene->getName());
		}
		return ret;
	}

	template<class TyScene>
	bool isCurrentScene()
	{
		for (auto& scene : mSceneBases)
		{
			if (scene->getName() == typeid(TyScene).name())
			{
				return true;
			}
		}
		return false;
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
		if ( mSceneBases.empty( ) ) return;
		auto erase_itr = std::find_if( mSceneBases.begin(), mSceneBases.end(), [ this ] ( std::shared_ptr<cSceneBase>& scene )
		{
			return scene->getName() == typeid( TyScene ).name( );
		} );
		if ( erase_itr == mSceneBases.end( ) ) return;
		auto index = std::distance( mSceneBases.begin( ), erase_itr );
		mSceneBases[index]->shutDown( );
		if ( iteration )
		{
			if ( index == iterator ) { mSceneBases.erase( mSceneBases.begin( ) + index ); iterator--; throw SceneRemoveSelf( ); }
			else { mSceneBases.erase( mSceneBases.begin( ) + index ); if ( index < iterator ) iterator--; }
		}
		else
		{
			mSceneBases.erase( mSceneBases.begin( ) + index );
		}
	}

	template<class TyScene, class... Args>
	void shift( Args... args )
	{
		// 全てのシーンのシャットダウン
		for ( auto& scene : mSceneBases )
		{
			scene->shutDown( );
		}

		// 入力を遮断
		ENV->flashInput( );

		// 全てのシーンを削除
		auto it = mSceneBases.begin( );
		while ( it != mSceneBases.end( ) )
		{
			it = mSceneBases.erase( it );
		}

		// 新しいシーンを作成
		mSceneBases.emplace_back( std::make_shared<TyScene>( args... ) );

		// 全てのシーンのセットアップ
		for ( auto& scene : mSceneBases )
		{
			scene->setup( );
		}

		throw SceneAllDeleted( );
	}
private:
	std::vector<std::shared_ptr<cSceneBase>> mSceneBases;
	std::shared_ptr<cSceneBase> mDontDestroyOnLoad;
	int iterator = 0;
	bool iteration = false;
};
}