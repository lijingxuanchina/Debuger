#include "../stdafx.h"

#include "tloger.h"

TLoger g_tloger;

namespace
{
const wchar_t* const Separator = L"\\";
const wchar_t* const NewLine = L"\n";
const wchar_t* const TLogerDir = L"TLoger";
const wchar_t* const TLogerName = L"tloger.log";
}

TLoger::TLoger()
{
	makeSureLogFileExist();
}

TLoger::~TLoger()
{
}

void TLoger::makeSureLogFileExist()
{
	wchar_t buffer[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(NULL, buffer, CSIDL_LOCAL_APPDATA, TRUE);

	CString appDataPath(buffer);
	CString tlogDirPath(appDataPath + Separator + TLogerDir);
	if (!::PathFileExists(tlogDirPath))
	{
		::CreateDirectory(tlogDirPath, NULL);
	}

	CString tlogPath(tlogDirPath + Separator + TLogerName);
	if (!::PathFileExists(tlogPath))
	{
		::CreateFile(tlogPath, GENERIC_READ | GENERIC_WRITE, 0,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	m_tlogPath = tlogPath;
}

const char* TLoger::getFileName(const char* file)
{
	auto index = strrchr(file, '\\');
	if (!index)
	{
		return "";
	}

	index = index + 1;
	return index;
}

const char* TLoger::getTime(char* buffer)
{
	SYSTEMTIME sysTime = { 0 };
	::GetLocalTime(&sysTime);
	snprintf(buffer, MAX_PATH, "%4d/%02d/%02d %02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);
	return buffer;
}
