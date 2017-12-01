#pragma once

#include <Utility/cSingletonAble.h>
#include <memory>
#include "cSceneBase.h"
#include <Utility/cInput.h>

//! @file cSceneManager
//! @brief SceneØ‚è‘Ö‚¦‚ğŠÜ‚ßŒ»İ‚ÌScene‚Ì‚·‚×‚Ä‚ğŠÇ—‚·‚éƒNƒ‰ƒX‚Å‚·
//! @note 
//!      Change‚Åˆø”‚ÉSceneBase‚ğŒp³‚µ‚½Class‚ğ“n‚µ‚ÄƒV[ƒ“‚ğ•Ï‚¦‚Ü‚·
//!      ‚»‚±‚Å”»’f‚µ‚Ä‚¢‚Ü‚·
//! @date 2017-10-12
//! @author Taka Nomoto

class cSceneManager : public Utility::cSingletonAble<cSceneManager>
{
public:
	cSceneManager() = default;

	template<class scene,class... T>
	void change(T...args)
	{
		mSceneBase = std::make_shared<scene>(args...);
	};

	cSceneBase& now()
	{
		return *mSceneBase;
	};

	template<class scene, class... T>
	void shift(T...args)
	{
		if ( mSceneBase )
		{
			mSceneBase->shutDown( );
			ENV->flashInput( );
		}
		change<scene>(args...);
		mSceneBase->setup();
	}
private:

	std::shared_ptr<cSceneBase> mSceneBase;

};