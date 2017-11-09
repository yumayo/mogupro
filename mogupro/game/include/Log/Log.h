#pragma once

#include <fstream>
#include <map>
#include <Utility/cSingletonAble.h>

namespace Log
{
	class cLog
	{
	public:
		cLog(){}

		cLog(const std::string& filename)
		{
			open(filename);
		}

		void open(const std::string& filename);

		void close();

		void writeLog(std::string str);

		std::ofstream fst;

	};
	class cLogManager : public Utility::cSingletonAble<cLogManager>
	{
	public:
		void add(const std::string filename);

		void close();

		void writeLog(const std::string& filename, const std::string& str);

	private:
		std::map<std::string, cLog> logs;
	};

}
