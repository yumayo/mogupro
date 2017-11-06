#pragma once
#define NOMINMAX
#pragma comment(lib,"OpenAL32.lib")
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <cmath>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

//Wav Header の詳細
//0 ~ 3  : "RIFF" 文字
//4 ~ 7  : "総ファイルサイズ -8 (byte)"
//8 ~ 11 : "WAVE" 文字
#define WAV_HEADER_SIZE (12)
//Wav Chunk の詳細
//0  ~ 3  : "fmt " 文字　
//4  ~ 7  : フォーマットサイズ　デフォルト値16 (下のやつが全部で16byte)
//8  ~ 9  : フォーマットコード　非圧縮のＰＣＭフォーマットは1
//10 ~ 11 : チャンネル数　モノラルは1、ステレオは2
//12 ~ 15 : サンプリングレート　44.1kHzの場合なら44100
//16 ~ 19 : バイト／秒　１秒間の録音に必要なバイト数
//20 ~ 21 : ブロック境界　ステレオ16bitなら、16bit*2 = 32bit = 4byte
//22 ~ 23 : ビット／サンプル　１サンプルに必要なビット数
#define WAV_CHUNK_SIZE  (24)

//! @fn    Wav
//! @brief WavFile
//! @note 
//!        Wav再生をするためのClassです。
//!        WavClassのみで再生はできません。
//! @date 2017-11-06
//! @author Taka Nomoto

namespace Sound
{
	class Wav
	{
	public:
		bool read(const std::string& file);
		unsigned int getValue(const char* p, const unsigned int num);
		//!@TODO : 2byteのTagに対応していない
		bool searchChunk(std::ifstream& fstr, const char* chunk);
		//チャンネル数を返す
		unsigned int  Wav::channel() const { return chunkInfo.ch; }
		//データがステレオならtrueを返す
		bool Wav::isStereo() const { return chunkInfo.ch == 2; }
		// サンプリングレートを返す
		unsigned int  Wav::sampleRate() const { return chunkInfo.sampleRate; }
		// データサイズ(バイト数)を返す
		unsigned int  Wav::size() const { return chunkInfo.size; }
		// 再生時間(秒)を返す
		float Wav::totalTime() const { return time; }
		// 波形データを返す
		const char* Wav::data() const { return &wavData[0]; }

		struct Chunk
		{
			unsigned int id;
			unsigned int ch;
			unsigned int sampleRate;
			unsigned int bit;
			unsigned int size;
		};

		std::vector<char> wavData;
		Chunk chunkInfo;
		float time;
	};
}
