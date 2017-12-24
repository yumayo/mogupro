#pragma once
#include <Sound/Wav.h>
#include <Utility/cSingletonAble.h>
#include <memory>
#include <cinder/Vector.h>
#include <list>
#include <Utility/cUserPointer.hpp>
#include <Sound/Buffer.h>
#include <Sound/Source.h>
#include <map>
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
//! @date 2017-11-13
//! @author Taka Nomoto

namespace Sound
{
	class Stereophonic
	{
	public:
		Stereophonic()
		{

		}

		template<class T>
		explicit Stereophonic(T type,ci::vec3& pos) :
			buffer(std::make_shared<Buffer>(type)),
			source(std::make_shared<Source>()), sourcePos(&pos),
			end(false)
		{
			source->bindBuffer(*buffer);
			time = type.totalTime();
			position();
			direction();
			soundPosition();
			play();
		}

		~Stereophonic()
		{

		}

		void play()
		{
			source->play();
		}

		void stop() const
		{
			source->stop();
		}

		void pause() const
		{
			source->pause();
		}

		// 音量変更(value: 0.0f~)
		void gain(const float value) const
		{
			source->gain(value);
		}

		// 再生ピッチ変更(value: 0.0f~)
		void pitch(const float value) const 
		{
			source->pitch(value);
		}

		void looping(const bool value) const 
		{
			source->looping(value);
		}

		void position(const float x, const float y, const float z) const 
		{
			source->position(x, y, z);
		}
		void position()
		{
			source->position(0.0f, 0.0f, 0.0f);
		}

		void direction(const float x, const float y, const float z) const
		{
			source->direction(x, y, z);
		}

		void direction()
		{
			source->direction(0.0f,0.0f,1.0f);
		}

		void soundPosition(const float x, const float y, const float z) const
		{
			source->soundPosition(x, y, z);
		}

		void soundPosition()
		{
			source->soundPosition(10.0f,0.0f,0.0f);
		}

		bool isPlaying() const 
		{
			return source->isPlaying();
		}

		float currentTime() const
		{
			return source->currentTime();
		}

		ci::vec3* sourcePos;
		ci::vec3 currentSourcePos;

		bool endPlay() const  { return end; }
		std::shared_ptr<Buffer> buffer;
		std::shared_ptr<Source> source;
		bool end;
		float time;
	};

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
	//! @date 2017-11-13
	//! @author Taka Nomoto
	class StereophonicManager : public Utility::cSingletonAble<StereophonicManager>
	{
	public:
		void open();

		void close();

		template<class T>
		void add(T type,ci::vec3& sourcePos)
		{
			stereophonics.push_back(Stereophonic(type,sourcePos));
		}
		void update(float deltaTime);

		void clearList();

		void clear();


		std::list<Stereophonic> stereophonics;
	private:
		ALCdevice*  device;
		ALCcontext* context;
	};
}