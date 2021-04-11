#include "../stdafx.h"

#include "tloger.h"

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

void TLoger::log(const CString& logInfo)
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

	fputws(logInfo + NewLine, tlogFile);

	fclose(tlogFile);
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
