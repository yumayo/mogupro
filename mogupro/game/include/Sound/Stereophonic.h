//#pragma once
//#include <Sound/Wav.h>
////! @fn    Stereophonic
////! @brief 立体音響を再生するClassです
////! @note  使い方について
////!
////!                1.read 
////!                2.bind 
////!                3.setListenerPos
////!                4.setListenerDirection
////!                5.setSourcePos
////!                6.playとか
////!
////! @date 2017-11-06
////! @author Taka Nomoto
//
//namespace Sound
//{
//	class Stereophonic
//	{
//	public:
//		//AssetsのPath指定してください
//		//Wav読み込み
//		void read(const std::string& file);
//		//メインスレッドでデータのBindを行う
//		void bind();
//		//聞く位置の設定
//		void setListenerPos(const float& x, const float& y, const float& z);
//		//聞く向きの設定 (upベクトルはvec3(0,1,0))
//		void setListenerDirection(const float& x, const float& y, const float& z);
//		//音源の位置の設定
//		void setSourcePos(const float& x, const float& y, const float& z);
//		
//		void play();
//		
//		void stop();
//		
//		void pause();
//		
//		void gain(const float value);
//		
//		void pitch(const float value);
//		
//		void looping(const bool value);
//		
//		bool isPlaying();
//		
//		float currentTime();
//		
//		void clean();
//
//	private:
//		Wav wav;
//		ALCdevice*  device;
//		ALCcontext* context;
//
//		ALuint bufferId;
//		ALuint sourceId;
//	};
//}