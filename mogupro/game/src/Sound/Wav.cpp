#include <Sound/Wav.h>
namespace Sound
{
	unsigned int Wav::getValue(const char* p, const unsigned int num)
	{
		unsigned int value = 0;

		for (unsigned int i = 0; i < num; ++i, ++p)
			value += (static_cast<unsigned char>(*p) << (i * 8));

		return value;
	}
	//!@TODO : 2byteのTagに対応していない
	bool Wav::searchChunk(std::ifstream& fstr, const char* chunk)
	{
		fstr.seekg(WAV_HEADER_SIZE, fstr.beg);

		while (1)
		{
			//Tag検索
			char tag[4];
			fstr.read(tag, 4);
			if (!std::strncmp(tag, chunk, 4))
				return true;

			//次のチャンクまでに詰まっているものを飛ばす
			char data[4];
			fstr.read(data, 4);
			unsigned int chunkSize = getValue(data, 4);
			fstr.seekg(chunkSize, fstr.cur);

			if (fstr.eof())
				break;
		}
		return false;
	}

	bool Wav::read(const std::string& file)
	{
		std::ifstream fstr(file, std::ios::binary);
		if (!fstr)
		{
			std::cout << "Can't open: " << file << std::endl;
			return false;
		}

		char header[WAV_HEADER_SIZE];

		fstr.read(header, WAV_HEADER_SIZE);
		if (std::strncmp(&header[0], "RIFF", 4))
		{
			std::cout << "This file isn't RIFF format." << std::endl;
			return false;
		}

		if (std::strncmp(&header[8], "WAVE", 4))
		{
			std::cout << "This file isn't WAVE format." << std::endl;
			return false;
		}

		//!@ LookMe : fmt の文字位置が一定じゃない場合があるらしい
		char data[4];
		fstr.read(data, 4);
		if (std::strncmp(&data[0], "fmt ", 4))
		{
			std::cout << "No chunk 'fmt'." << std::endl;
			return false;
		}

		fstr.read(data, 4);
		unsigned int chunkSize = getValue(data, 4);
		std::vector<char> chunk(chunkSize);
		fstr.read(&chunk[0], chunkSize);
		chunkInfo.id = getValue(&chunk[0], 2);
		if (chunkInfo.id != 1) {
			// IDが1以外は扱わない
			std::cout << "Wav's id error :  " << chunkInfo.id << " : " << file << std::endl;
			return false;
		}

		chunkInfo.ch = getValue(&chunk[2], 2);
		chunkInfo.sampleRate = getValue(&chunk[4], 4);
		chunkInfo.bit = getValue(&chunk[14], 2);
		if (chunkInfo.bit != 16) {
			// 量子化ビット数が16以外は扱わない
			std::cout << "Wav's bit error :  " << chunkInfo.bit << " : " << file << std::endl;
			return false;
		}

		if (searchChunk(fstr, "data") == false)
		{
			std::cout << "No chank 'data'." << std::endl;
			return false;
		}
		fstr.read(data, 4);
		chunkInfo.size = getValue(data, 4);
		time = chunkInfo.size / chunkInfo.ch / 2.0f / chunkInfo.sampleRate;

		wavData.resize(chunkInfo.size);
		fstr.read(&wavData[0], chunkInfo.size);
		return true;
	}

	void Wav::bindAlShortData()
	{
		const char* t = data();
		// キャスト
		const ALshort* sample = (const ALshort*)t;
		// データサイズ
		int sampleSize = size() / 2;
		// 配列の開始位置と終了位置から作成
		alShortdata = std::vector<ALshort>(&sample[0], &sample[sampleSize]);
	}
}
