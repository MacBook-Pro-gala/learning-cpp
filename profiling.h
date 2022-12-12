#define _CRT_SECURE_NO_WARNINGS

#ifdef __cplusplus

#define MAX_TIMES_CIJI 10000
#include <chrono>
#include <iomanip>
#include <fstream>
#include <vector>

class ciji_profiling
{
	inline std::string getTime(const char* formate)
	{
		time_t timep;
		static char tmp[64];
		time(&timep);
		strftime(tmp, sizeof(tmp), formate, localtime(&timep));

		return tmp;
	}

public:
	class timeInfo
	{
		std::chrono::system_clock::time_point start;
		std::chrono::duration<double, std::milli> duration{0};

	public:
		long long counter = 0;

	public:
		void startTimer()
		{
			switch (counter / MAX_TIMES_CIJI) {
			case 0:
				start = std::chrono::system_clock::now();
				break;
			}
		}
		void endTimer()
		{
			switch (counter / MAX_TIMES_CIJI) {
			case 0:
				duration += std::chrono::system_clock::now() - start;
				break;
			}
			counter += 1;
		}
		auto getDuration()
		{
			return duration.count();
		}
	};
	typedef std::pair<const char*, ciji_profiling::timeInfo> moduleTimeInfoPair;

private:
	std::ofstream log;
	timeInfo mainProgramTimer;
	std::vector<moduleTimeInfoPair*> moduleTime;

public:
	ciji_profiling()
	{
		log.open(getTime("profile.log"), std::ofstream::app);
		if (!log.is_open())
			throw("log file create failed!");
		mainProgramTimer.startTimer();
	}

	void endTimer(moduleTimeInfoPair* timer) {
		timer->second.endTimer();
	}
	void startTimer(moduleTimeInfoPair* timer) {
		switch ((long long)(timer->first) % 2){
		case 0:
			moduleTime.push_back(timer), timer->first++;
		}
		timer->second.startTimer();
	}

	~ciji_profiling()
	{
		mainProgramTimer.endTimer();

		log << "性能探查日志                                                作者:吃稽\n" <<
			"生成时间                                                   " << getTime("[%m-%d %H:%M:%S]\n") <<
			"=========================================================================\n" <<
			"代码块名称          |执行的次数          |一万次耗时(ms)      |预估总耗时(ms)\n";

		for (auto iter = moduleTime.begin(); iter != moduleTime.end(); iter++)log <<
			std::setw(18) << std::setiosflags(std::ios::left) << --(*iter)->first << "|" <<
			std::setw(18) << std::setiosflags(std::ios::left) << (*iter)->second.counter << "|" <<
			std::setw(18) << std::setiosflags(std::ios::left) << (*iter)->second.getDuration() << "|" <<
			std::setw(18) << std::setiosflags(std::ios::left) <<
			((*iter)->second.counter > MAX_TIMES_CIJI ?
				(*iter)->second.counter * (*iter)->second.getDuration() / MAX_TIMES_CIJI :
				(*iter)->second.getDuration())
			<< std::endl;

		log << "=========================================================================\n";
		log << "测试人员：吃稽/Weijun Zhang/Zixin Xie \n\n\n\n\n";
		log.close();
	}

}CijiProfiling;

#define START_TIMER(moduleName)\
static ciji_profiling::moduleTimeInfoPair moduleName##_timeInfo_ciji{#moduleName,ciji_profiling::timeInfo{}};\
CijiProfiling.startTimer(&moduleName##_timeInfo_ciji);
#define END_TIMER(moduleName)CijiProfiling.endTimer(&moduleName##_timeInfo_ciji);

#elif
#error 性能探查需要在C++环境下编译
#endif