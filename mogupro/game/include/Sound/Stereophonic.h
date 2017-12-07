#pragma once
#include <Sound/Wav.h>
#include <Utility/cSingletonAble.h>
#include <memory>
#include <cinder/Vector.h>
#include <list>
#include <Utility/cUserPointer.hpp>
#include <Sound/Buffer.h>
#include <Sound/Source.h>
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
		explicit Stereophonic(T type, ci::vec3* listenerPos,
			ci::vec3* listenerDir, ci::vec3* sourcePos) :
			buffer(std::make_shared<Buffer>(type)),
			source(std::make_shared<Source>()), listenerPosition(listenerPos), listenerDirection(listenerDir), sourcePosition(sourcePos),
			end(false)
		{
			source->bindBuffer(*buffer);
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

		void direction(const float x, const float y, const float z) const
		{
			source->direction(x, y, z);
		}

		void soundPosition(const float x, const float y, const float z) const
		{
			source->soundPosition(x, y, z);
		}

		bool isPlaying() const 
		{
			return source->isPlaying();
		}

		float currentTime() const
		{
			return source->currentTime();
		}

		void update()
		{
			if (end == true)
				return;

			if (!listenerPosition || !listenerDirection || !sourcePosition)
			{
				end = true;
				stop();
				return;
			}

			position();
			direction();
			soundPosition();
		}

		void position(ci::vec3* listenerPos)
		{
			listenerPosition = listenerPos;
		}

		void position()
		{
			source->position(0.0f,0.0f,0.0f);
		}

		void direction(ci::vec3* listenerDir)
		{
			listenerDirection = listenerDir; 
			direction();
		}

		void direction()
		{
			source->direction(listenerDirection->x, listenerDirection->y, listenerDirection->z);
		}

		void soundPosition(ci::vec3* sourcePos)
		{
			sourcePosition = sourcePos;
			//!@LookMe : とりあえずPosition差し引きするだけ
			sourcePosition->x = sourcePosition->x - listenerPosition->x;
			sourcePosition->y = sourcePosition->y - listenerPosition->y;
			sourcePosition->z = sourcePosition->z - listenerPosition->z;
			soundPosition();
		}

		void soundPosition()
		{
			source->soundPosition(sourcePosition->x, sourcePosition->y, sourcePosition->z);
		}

		bool endPlay() const  { return end; }

		ci::vec3* listenerPosition;
		ci::vec3* listenerDirection;
		ci::vec3* sourcePosition;
		std::shared_ptr<Buffer> buffer;
		std::shared_ptr<Source> source;
		bool end;
	};

	// ディレイ
	// input_samples 加工する波形データ
	// attenuation   減衰率(0.0 ~ 1.0)
	// time          遅延時間(秒)
	// repeat        繰り返し回数
	//template<class T>
	static void delay(Wav& wav, float attenuation, float time, int repeat)
	{
		// 結果を格納する変数
		// TIPS:input_samplesと同じサイズをあらかじめ確保
		std::vector<ALshort> outputSamples(wav.alShortdata.size());

		// TODO:SAMPLING_RATEは元波形のサンプリングレートに併せて適時定義のこと
		//        波形のサンプリングレートが44.1kHzなら
		//        const int SAMPLING_RATE = 44100;
		//        などと定義
		float d = 44100 * time;

		for (size_t n = 0; n < wav.alShortdata.size(); ++n) {
			outputSamples[n] = wav.alShortdata[n];

			for (int i = 1; i <= repeat; ++i) {
				// 過去の波形の位置
				int m = int(n - i * d);

				if (m >= 0) {
					// 過去の音データをミックスする
					// 波形の減衰にはべき乗を使っている
					outputSamples[n] += std::pow(attenuation, float(i)) * wav.alShortdata[m];
				}
			}
		}

		wav.alShortdata = outputSamples;
	}

	static float sinc(float x) 
	{
		float y;

		if (x == 0.0) {
			y = 1.0;
		}
		else {
			y = sin(x) / x;
		}

		return y;
	}


	static void p(Wav& wav, float pitch)
	{
		std::vector<ALshort> output_samples(wav.alShortdata.size(), 0);
		for (int n = 0; n < output_samples.size(); ++n) 
			output_samples[n] = wav.alShortdata[n] * pitch;

		wav.alShortdata = output_samples;
		wav.setTotalTime(output_samples.size() * 2);
	}

	static void fastForwarding(Wav& wav,float rate) 
	{
		std::vector<ALshort> output_samples(wav.alShortdata.size() * rate + 1);

		int template_size = wav.chunkInfo.sampleRate * 0.01f;
		int pmin = wav.chunkInfo.sampleRate * 0.005f;
		int pmax = wav.chunkInfo.sampleRate * 0.02f;

		std::vector<int> x(template_size);
		std::vector<int> y(template_size);
		std::vector<int> r(pmax + 1);

		int offset0 = 0;
		int offset1 = 0;

		while (offset0 + pmax * 2 < wav.alShortdata.size()) {
			for (int n = 0; n < template_size; ++n) {
				// 本来のデータ
				x[n] = wav.alShortdata[offset0 + n];
			}

			float max_of_r = 0.0f;
			int p = pmin;
			for (int m = pmin; m <= pmax; ++m) {
				for (int n = 0; n < template_size; ++n) {
					// mサンプルずらした音データ
					y[n] = wav.alShortdata[offset0 + m + n];
				}

				r[m] = 0.0;
				for (int n = 0; n < template_size; ++n) {
					// 相関関数
					// -32768 ~ 32767 の範囲の値同士の掛け算を
					// 結果も-32768 ~ 32767の範囲に収まるようにしている
					r[m] += (x[n] * y[n]) / 32768;
				}
				if (r[m] > max_of_r) {
					max_of_r = r[m];      // 相関関数のピーク
					p = m;                // 音データの基本周期
				}
			}

			for (int n = 0; n < p; ++n) {
				// 単調減少の重みづけ
				output_samples[offset1 + n] = wav.alShortdata[offset0 + n] * (p - n) / p;
				// 単調増加の重みづけ
				output_samples[offset1 + n] += wav.alShortdata[offset0 + p + n] * n / p;
			}

			int q = int(p / (rate - 1.0) + 0.5);
			for (int n = p; n < q; ++n) {
				if (offset0 + p + n >= wav.alShortdata.size()) {
					break;
				}

				output_samples[offset1 + n] = wav.alShortdata[offset0 + p + n];
			}

			offset0 += p + q;
			offset1 += q;
		}

		wav.alShortdata = output_samples;
		wav.setTotalTime(output_samples.size() * 2);
	}

	static void slowForwarding(Wav& wav,float rate) {
		std::vector<ALshort> output_samples(wav.alShortdata.size() / rate + 1);

		int template_size = wav.chunkInfo.sampleRate * 0.01f;
		int pmin = wav.chunkInfo.sampleRate * 0.005f;
		int pmax = wav.chunkInfo.sampleRate * 0.02f;

		std::vector<int> x(template_size);
		std::vector<int> y(template_size);
		std::vector<int> r(pmax + 1);

		int offset0 = 0;
		int offset1 = 0;

		while (offset0 + pmax * 2 < wav.alShortdata.size()) {
			for (int n = 0; n < template_size; ++n) {
				// 本来のデータ
				x[n] = wav.alShortdata[offset0 + n];
			}

			float max_of_r = 0.0f;
			int p = pmin;
			for (int m = pmin; m <= pmax; ++m) {
				for (int n = 0; n < template_size; ++n) {
					// mサンプルずらした音データ
					y[n] = wav.alShortdata[offset0 + m + n];
				}

				r[m] = 0.0;
				for (int n = 0; n < template_size; ++n) {
					// 相関関数
					// -32768 ~ 32767 の範囲の値同士の掛け算を
					// 結果も-32768 ~ 32767の範囲に収まるようにしている
					r[m] += (x[n] * y[n]) / 32768;
				}
				if (r[m] > max_of_r) {
					max_of_r = r[m];      // 相関関数のピーク
					p = m;                // 音データの基本周期
				}
			}

			for (int n = 0; n < p; ++n) {
				output_samples[offset1 + n] = wav.alShortdata[offset0 + n];
			}
			for (int n = 0; n < p; ++n) {
				// 単調減少の重みづけ
				output_samples[offset1 + p + n] = wav.alShortdata[offset0 + p + n] * (p - n) / p;
				// 単調増加の重みづけ
				output_samples[offset1 + p + n] += wav.alShortdata[offset0 + n] * n / p;
			}

			int q = int(p * rate / (1.0 - rate) + 0.5);
			for (int n = p; n < q; ++n) {
				if (offset0 + n >=wav.alShortdata.size()) {
					break;
				}

				output_samples[offset1 + p + n] = wav.alShortdata[offset0 + n];
			}

			// offsetの更新
			offset0 += q;
			offset1 += p + q;
		}

		wav.alShortdata = output_samples;
		wav.setTotalTime(output_samples.size() * 2);
	}

	static void pitch(Wav& wav, float pitch)
	{
		std::vector<ALshort> output_samples(wav.alShortdata.size(), 0);

		int j = 36;

		for (int n = 0; n < output_samples.size(); ++n)
		{
			float t = pitch * n;
			int offset = int(t);
			for (int m = offset - j / 2; m <= offset + j / 2; ++m) 
			{
				if ((m >= 0) && (m < wav.alShortdata.size()))
					output_samples[n] += wav.alShortdata[m] * sinc((float)M_PI * (t - m));
			}
		}

		wav.alShortdata = output_samples;
		wav.setTotalTime(output_samples.size() * 2);
	}

	static void voiceChanger(Wav& wav, float depth, float rate)
	{
		std::vector<ALshort> output_samples;

		// TODO:元波形のサンプリングレートに併せてSAMPLING_RATEを適時定義すること
		for (int n = 0; n < wav.alShortdata.size(); ++n) {
			float a = depth * std::sin(2.0 * M_PI * rate * n / wav.chunkInfo.sampleRate);
			output_samples.push_back(a * wav.alShortdata[n]);
		}

		wav.alShortdata = output_samples;
	}

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
		void add(T type, ci::vec3* listenerPos,
			ci::vec3* listenerDir, ci::vec3* sourcePos)
		{
			stereophonicList.push_back(Stereophonic(type, listenerPos,listenerDir,sourcePos));
		}

		void update()
		{
			for (auto& itr = stereophonicList.begin(); itr != stereophonicList.end(); ++ itr)
			{
				itr->update();

				if (itr->endPlay() == false)
					continue;
				stereophonicList.erase(itr);
				continue;
			}
		}

		std::list<Stereophonic> stereophonicList;
	private:
		ALCdevice*  device;
		ALCcontext* context;
	};
}