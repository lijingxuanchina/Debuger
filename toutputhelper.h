#ifndef __TOUTPUTHELPER_H_
#define __TOUTPUTHELPER_H_

namespace
{
const size_t MaxOutPutLength = MAX_PATH * 5;
}

class TOutPutHelper
{
public:
	TOutPutHelper();
	~TOutPutHelper();
	template<typename... Args>
	void output(const char* info, Args... args);

private:
	bool m_isSuccessGetConsole = false;
	HANDLE m_consoleHnadle = nullptr;
};

template<typename... Args>
void TOutPutHelper::output(const char* info, Args... args)
{
	if (!m_isSuccessGetConsole)
	{
		return;
	}

	auto strLength = snprintf(nullptr, 0, info, args...);
	char argBuffer[MaxOutPutLength] = { 0 };
	char infoBuffer[MaxOutPutLength] = { 0 };
	snprintf(argBuffer, strLength + 1, info, args...);
	snprintf(infoBuffer, MaxOutPutLength, "%s\n", argBuffer);
	CString outputInfo(infoBuffer);
	::WriteConsole(m_consoleHnadle, outputInfo, outputInfo.GetLength(), NULL, NULL);
}

extern TOutPutHelper g_toutputHelper;

#define OUTPUT(info, ...) g_toutputHelper.output(info, __VA_ARGS__);

#endif // __TOUTPUTHELPER_H_
