#pragma once

//! @file cSceneBase
//! @brief 全てのSceneに継承させる基盤クラスです
//! @note
//!       Scene切り替えは_isEnd で判断します
//! @date 2017-10-12
//! @author Taka Nomoto

class cSceneBase
{
public:
	//初期化
	virtual void setup() = 0;
	//Sceneの終了時のメモリ削除とか
	virtual void shutDown() = 0;

	virtual void update(float deltaTime) = 0;
	
	virtual void draw() = 0;

	virtual void drawFBO() = 0;

	virtual void draw2D() = 0;
	//サイズ変更時に呼びたいとき
	virtual void resize() = 0;
	//!@ Look Me : Scene切り替え時にtrueにしてください
	//もしもScene切り替え時のバグ出たらようで準備してます
	bool isEnd() { return mIsEnd; }
protected:

	bool mIsEnd;
};