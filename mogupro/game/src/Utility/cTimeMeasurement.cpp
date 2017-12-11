#include <Utility/cTimeMeasurement.h>

cTimeMeasurement::cTimeMeasurement() 
	: mNow(std::chrono::steady_clock::now()), mPrev(mNow), mDeltaTime(0.0f), mTotalTime(0.0f)
{

}

void cTimeMeasurement::make()
{
	using namespace std::chrono;
	//現在時刻を_nowに代入
	mNow = steady_clock::now();
	//_nowから_prevで、今回の1フレームに掛かった時間を計算し、代入
	mDeltaTime = static_cast<float>(duration_cast<microseconds>(mNow - mPrev).count() * 0.000001f);
	mTotalTime += mDeltaTime;
	//_prevに_nowを入れて時間を同じにする
	mPrev = mNow;
	return;
}

float cTimeMeasurement::deltaTime()
{
	return mDeltaTime;
}

float cTimeMeasurement::totalTime()
{
	return mTotalTime;
}