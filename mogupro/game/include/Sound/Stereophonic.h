#pragma once
#include <Sound/Wav.h>
#include <Utility/cSingletonAble.h>
#include <memory>
#include <cinder/Vector.h>
#include <vector>
#include <Utility/cUserPointer.hpp>
//! @fn    Stereophonic
//! @brief 立体音響を再生するClassです
//! @note  使い方について
//!
//!                1.read 
//!                2.bind 
//!                3.setListenerPos
//!                4.setListenerDirection
//!                5.setSourcePos
//!                6.playとか
//!
//! @date 2017-11-06
//! @author Taka Nomoto

namespace Sound
{
	class Stereophonic
	{
	public:
		Stereophonic() {}
		Stereophonic(const std::string& file, std::shared_ptr<ci::vec3> listenerPos,
			std::shared_ptr<ci::vec3> listenerDir, std::shared_ptr<ci::vec3> sourcePos)
		{
			read(file);
			//bindWav();
			bind();
			setListenerPos(listenerPos);
			setListenerDirection(listenerDir);
			setSourcePos(sourcePos);
			play();
		}

		//AssetsのPath指定してください
		//Wav読み込み
		void read(const std::string& file);
		//wavのDataをvector<ALshort> dataに保存します
		void bindWav();
		//メインスレッドでデータのBindを行う
		void bind();
		//聞く位置の設定
		void setListenerPos(std::shared_ptr<ci::vec3>);
		void setListenerPos();
		//聞く向きの設定 (upベクトルはvec3(0,1,0))
		void setListenerDirection(std::shared_ptr<ci::vec3>);
		void setListenerDirection();
		//音源の位置の設定
		void setSourcePos(std::shared_ptr<ci::vec3>);
		void setSourcePos();
		void play();

		void stop();

		void pause();

		void gain(const float value);

		void pitch(const float value);

		void looping(const bool value);

		bool isPlaying();

		float currentTime();

		void update();

		void clean();

		Wav wav;
		std::vector<ALshort> data;
	private:
		Utility::softptr<ci::vec3> listenerPosition;
		Utility::softptr<ci::vec3> listenerDirection;
		Utility::softptr<ci::vec3> sourcePosition;

		ALuint bufferId;
		ALuint sourceId;
	};

	// ディレイ
	// input_samples 加工する波形データ
	// attenuation   減衰率(0.0 ~ 1.0)
	// time          遅延時間(秒)
	// repeat        繰り返し回数
	void delay(Stereophonic& stereophonic, float attenuation, float time, int repeat);


	//! @fn    StereophonicManager
	//! @brief 立体音響再生をまとめるClassです
	//! @note  使い方について
	//!
	//!                1.read 
	//!                2.bind 
	//!                3.setListenerPos
	//!                4.setListenerDirection
	//!                5.setSourcePos
	//!                6.playとか
	//!
	//! @date 2017-11-09
	//! @author Taka Nomoto
	class StereophonicManager : public Utility::cSingletonAble<StereophonicManager>
	{
	public:
		void open();

		void close();

		void add(const std::string& file, std::shared_ptr<ci::vec3> listenerPos,
			std::shared_ptr<ci::vec3> listenerDir, std::shared_ptr<ci::vec3> sourcePos)
		{
			stereophonicList.push_back(Stereophonic(file, listenerPos,listenerDir,sourcePos));
		}

		void update()
		{
			for (int i = 0; i < stereophonicList.size(); ++i)
			{
				stereophonicList[i].update();
			}
		}

	private:
		ALCdevice*  device;
		ALCcontext* context;
		std::vector<Stereophonic> stereophonicList;
	};
}