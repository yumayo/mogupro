#pragma once
#include <Utility/cSingletonAble.h>
#include <iostream>
#include <chrono>

//! @file TimeMeasurement
//! @brief ゲーム内の時間管理をするクラスです
//! @note 
//!      フレーム毎にズレなく動かしたい時にget()をかけてください
//!      ゲーム開始からの時間が欲しい場合は、totalTime()を貰ってください
//! @date 2017-10-12
//! @author Taka Nomoto

class cTimeMeasurement : public Utility::cSingletonAble<cTimeMeasurement>
{
public:

	cTimeMeasurement();

	//_totalTimeと_deltaTime生成
	//updateで必ず読んでください
	void make();

	//_deltaTime取得
	//_deltaTime => 1Frameに掛かった時間
	float deltaTime();

	//exe起動からの総合時間
	float totalTime();

private:

	//コピーコンストラクタ禁止
	cTimeMeasurement(const cTimeMeasurement&) = delete;
	cTimeMeasurement& operater(const cTimeMeasurement&) = delete;

	std::chrono::steady_clock::time_point mNow;

	std::chrono::steady_clock::time_point mPrev;

	float mDeltaTime;

	float mTotalTime;
};