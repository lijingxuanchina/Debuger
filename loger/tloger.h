#ifndef __TLOGER_H_
#define __TLOGER_H_

#include<thread>

namespace
{
const size_t MaxLogLength = MAX_PATH * 5;
}

class TLoger
{
public:
	TLoger();
	~TLoger();

public:
	template<typename... Args> 
	void logA(
		int pid,
		int tid,
		int line,
		const char* level,
		const char* file,
		const char* function,
		const char* format,
		Args... args);

private:
	void makeSureLogFileExist();
	const char* getTime(char* buffer);
	const char* getFileName(const char* file);

private:
	CString m_tlogPath;
};

template<typename... Args>
void TLoger::logA(
	int pid,
	int tid,
	int line,
	const char* level,
	const char* file,
	const char* function,
	const char* format,
	Args... args)
{
	makeSureLogFileExist();

	if (m_tlogPath.IsEmpty())
	{
		return;
	}

	FILE* tlogFile = nullptr;
	if (_wfopen_s(&tlogFile, m_tlogPath, L"a"))
	{
		return;
	}

	if (!tlogFile)
	{
		return;
	}

	auto strLength = snprintf(nullptr, 0, format, args...);
	char argBuffer[MaxLogLength] = { 0 };
	char logBuffer[MaxLogLength] = { 0 };
	char timeBuffer[MaxLogLength] = { 0 };
	snprintf(argBuffer, strLength + 1, format, args...);
	snprintf(logBuffer, MaxLogLength, "%s\t%s\t%d\t%d\t%s\t%d\t%s\t%s",
		getTime(timeBuffer), level, pid, tid, getFileName(file), line, function, argBuffer);

	fputws(CString(logBuffer) + "\n", tlogFile);

	fclose(tlogFile);
}

extern TLoger g_tloger;

#define __THREADID__ (*(uint32_t*)&std::this_thread::get_id())

#define LOGA_INFO(format, args) g_tloger.logA(_getpid(), __THREADID__,\
		__LINE__, "[INFO]", __FILE__, __FUNCTION__, format, args); 
#define LOGA_WARNING(format, args) g_tloger.logA(_getpid(), __THREADID__,\
		__LINE__, "[WARNING]", __FILE__, __FUNCTION__, format, args); 
#define LOGA_ERROR(format, args) g_tloger.logA(_getpid(), __THREADID__,\
		__LINE__, "[ERROR]", __FILE__, __FUNCTION__, format, args); 
#define LOGA(format) LOGA_INFO(format, "")

#endif // __TLOGER_H_