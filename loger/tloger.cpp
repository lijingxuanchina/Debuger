#include "../stdafx.h"

#include "tloger.h"

namespace
{
const char* const Separator = "\\";
const char* const NewLine = "\n";
const char* const TLogerDir = "TLoger";
const char* const TLogerName = "tloger.log";
}

TLoger::TLoger()
{
	makeSureLogFileExist();
}

TLoger::~TLoger()
{
}

void TLoger::log(const CString& logInfo)
{
	makeSureLogFileExist();

	if (m_tlogPath.IsEmpty())
	{
		return;
	}

	FILE* tlogFile = nullptr;
	if (fopen_s(&tlogFile, m_tlogPath, "a"))
	{
		return;
	}

	if (EOF == fputs(logInfo + NewLine, tlogFile))
	{
		return;
	}
}

void TLoger::makeSureLogFileExist()
{
	char buffer[MAX_PATH] = { 0 };
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
