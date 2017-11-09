#include <Log/Log.h>
#include <cinder/app/App.h>
#include <time.h>
#include <direct.h>
namespace Log
{
	void cLog::open(const std::string& filename)
	{
		fst.open(filename, std::ios::out | std::ios::app);

		if (!fst.is_open())
		{
			int a = 0;
		}
	}

	void cLog::close()
	{
		fst.close();
	}

	void cLog::writeLog(std::string str)
	{
		fst << str << std::endl;
	}


	void cLogManager::add(const std::string filename)
	{
		//ŽžŠÔŽæ“¾
		time_t timer;
		struct tm *t_st;
		time(&timer);
		t_st = localtime(&timer);

		std::string dirPath = ci::app::getAssetPath("").string() + "\\Log\\" + std::to_string(t_st->tm_year + 1900)
			+ "-" + std::to_string(t_st->tm_mon + 1) + "-" + std::to_string(t_st->tm_mday);
		std::string filePath = dirPath + "\\" + filename + ".log";
		_mkdir(dirPath.c_str());
		logs.insert(std::make_pair(filename, cLog(filePath)));
	}

	void cLogManager::close()
	{
		for (auto itr = logs.begin(); itr != logs.end(); ++itr)
			itr->second.close();
	}

	void cLogManager::writeLog(const std::string& filename, const std::string& str)
	{
		auto itr = logs.find(filename);
		if (itr == logs.end())return;
		itr->second.writeLog(str);
	}
}
