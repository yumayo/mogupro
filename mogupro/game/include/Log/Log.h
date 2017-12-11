#pragma once

#include <fstream>
#include <map>
#include <Utility/cSingletonAble.h>

namespace Log
{
	//! @fn   cLog
	//! @brief Logの吐き出し処理を行います
	//! @note 
	//!       
	//! @date 2017-11-09
	//! @author Taka Nomoto

	class cLog
	{
	public:
		cLog(){}

		cLog(const std::string& filename)
		{
			open(filename);
		}

		//Fileを開きます
		//Fileがない時は、自動的に生成されます
		void open(const std::string& filename);
		
		//File閉じます
		//閉じなくても大丈夫だけどちゃんと閉じるように
		void close();

		//Logの吐き出し
		void writeLog(std::string str);

		std::ofstream fst;

	};
	//! @fn   cLogManager
	//! @brief Logの管理を行います
	//! @note 
	//!       使い方 : 
	//!        ex)
	//!             add("test")
	//!              ->Key -> test でLogを作成
	//!                test.logがLog/日付/の場所に生成されます
	//!
	//!             write("test","書き込み")
	//!              -> test.logに　"書き込み"が吐き出されます
	//!           
	//! @date 2017-11-09
	//! @author Taka Nomoto
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
