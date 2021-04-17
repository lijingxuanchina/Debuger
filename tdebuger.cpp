#include "stdafx.h"

#include "tdebuger.h"
#include "tlifespanhelper.h"

namespace
{
const int Max_Buffer_Size = MAX_PATH * 2;
}

TDebuger::TDebuger()
{
}

TDebuger::~TDebuger()
{
}

void TDebuger::startDebug(const PROCESS_INFORMATION& processInfo)
{
	for (DWORD endDisposition = DBG_CONTINUE; endDisposition != 0;)
	{
		DEBUG_EVENT debugEvent = { 0 };
		if (!::WaitForDebugEvent(&debugEvent, INFINITE))
		{
			LOGA_ERROR("wait for debug event failed, error code:%lu", ::GetLastError());
			OUTPUT("wait for debug event failed, error code:%lu", ::GetLastError());
			break;
		}
		endDisposition = processDebugEvent(debugEvent, processInfo);
		if (!endDisposition)
		{
			OUTPUT("exit process, exit code:%lu", debugEvent.u.ExitProcess.dwExitCode);
			LOGA("exit process, exit code:%lu", debugEvent.u.ExitProcess.dwExitCode);
			return;
		}
		if (!::ContinueDebugEvent(
			debugEvent.dwProcessId, debugEvent.dwThreadId, endDisposition))
		{
			LOGA_ERROR("continue debug event failed, error code:%lu", ::GetLastError());
			OUTPUT("continue debug event failed, error code:%lu", ::GetLastError());
			break;
		}
	}
}

DWORD TDebuger::processDebugEvent(
	const DEBUG_EVENT& debugEvent,
	const PROCESS_INFORMATION& processInfo)
{
	DWORD contionueStatus = DBG_CONTINUE;
	switch (debugEvent.dwDebugEventCode)
	{
	case EXCEPTION_DEBUG_EVENT:
		processExceptionDebugEvent(debugEvent, processInfo, contionueStatus);
		break;
	case CREATE_THREAD_DEBUG_EVENT:
		break;
	case CREATE_PROCESS_DEBUG_EVENT:
		processCreateProcessDebugEvent(debugEvent, processInfo);
		break;
	case EXIT_THREAD_DEBUG_EVENT:
		break;
	case EXIT_PROCESS_DEBUG_EVENT:
		processExitProcessDebugEvent(contionueStatus);
		break;
	case LOAD_DLL_DEBUG_EVENT:
		processLoadDllDebugEvent(debugEvent);
		break;
	case UNLOAD_DLL_DEBUG_EVENT:
		break;
	case OUTPUT_DEBUG_STRING_EVENT:
		processOutputDebugStringEvent(debugEvent, processInfo);
		break;
	case RIP_EVENT:
		break;
	default:
		break;
	}

	return contionueStatus;
}

void TDebuger::processOutputDebugStringEvent(
	const DEBUG_EVENT& debugEvent,
	const PROCESS_INFORMATION& processInfo)
{
	auto outputDebugStringInfo = debugEvent.u.DebugString;
	OUTPUT("%ws", readRemoteString(
		processInfo.hProcess,
		outputDebugStringInfo.lpDebugStringData,
		outputDebugStringInfo.nDebugStringLength,
		outputDebugStringInfo.fUnicode));
}

void TDebuger::processExceptionDebugEvent(
	const DEBUG_EVENT& debugEvent,
	const PROCESS_INFORMATION& processInfo,
	DWORD& continueStatus)
{
	// TODO lijingxuan 完成更多对于 exception event 的处理
	switch (debugEvent.u.Exception.ExceptionRecord.ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT:
	case EXCEPTION_SINGLE_STEP:
		continueStatus = DBG_CONTINUE;
	default:
		break;
	}

	continueStatus = DBG_EXCEPTION_NOT_HANDLED;
}

void TDebuger::processCreateProcessDebugEvent(
	const DEBUG_EVENT& debugEvent,
	const PROCESS_INFORMATION& processInfo)
{
	OUTPUT("Success create debug process!");
	OUTPUT("Pid:%lu", processInfo.dwProcessId);
}

void TDebuger::processExitProcessDebugEvent(DWORD& continueStatus)
{
	continueStatus = 0;
}

void TDebuger::processLoadDllDebugEvent(
	const DEBUG_EVENT& debugEvent)
{
	OUTPUT("Load dll: %ws", getLoadDllName(debugEvent));
}

CString TDebuger::readRemoteString(
	HANDLE process, LPVOID address, WORD length, BOOL unicode)
{
	if (unicode)
	{
		WCHAR wbuffer[Max_Buffer_Size] = { 0 };
		if (!::ReadProcessMemory(
			process, address, &wbuffer, length * sizeof(WCHAR), NULL))
		{
			LOGA_ERROR("read process memory failed, error code:%lu", ::GetLastError());
			OUTPUT("read process memory failed, error code:%lu", ::GetLastError());
			return CString("can not read remote string");
		}

		return CString(wbuffer);
	}
	else
	{
		CHAR buffer[Max_Buffer_Size] = { 0 };
		if (!::ReadProcessMemory(
			process, address, &buffer, length, NULL))
		{
			LOGA_ERROR("read process memory failed, error code:%lu", ::GetLastError());
			OUTPUT("read process memory failed, error code:%lu", ::GetLastError());
			return CString("can not read remote string");
		}

		return CString(buffer);
	}
}

CString TDebuger::getLoadDllName(const DEBUG_EVENT& debugEvent)
{
	HANDLE fileHandle = debugEvent.u.LoadDll.hFile;
	DWORD fileSizeHigh = 0;
	DWORD fileSizeLow = ::GetFileSize(fileHandle, &fileSizeHigh);
	if (INVALID_FILE_SIZE == fileSizeLow)
	{
		CString errorInfo = "get load file size failed";
		LOGA_ERROR("%ws", errorInfo);
		OUTPUT("%ws", errorInfo);
		return errorInfo;
	}

	if (0 == fileSizeLow && 0 == fileSizeHigh)
	{
		CString errorInfo = "can not map a file with a length of zero";
		LOGA_ERROR("%ws", errorInfo);
		OUTPUT("%ws", errorInfo);
		return errorInfo;
	}

	HANDLE fileMap = ::CreateFileMappingW(fileHandle, NULL, PAGE_READONLY, 0, 1, NULL);
	if (!fileMap)
	{
		CString errorInfo;
		errorInfo.Format(L"create file map failed, error code:%lu", ::GetLastError());
		LOGA_ERROR("%ws", errorInfo);
		OUTPUT("%ws", errorInfo);
		return errorInfo;
	}

	TLifeSpanHelper tlifeSpanHelper;
	tlifeSpanHelper.setHandle(fileHandle);

	void* memory = ::MapViewOfFile(fileMap, FILE_MAP_READ, 0, 0, 1);
	if (!memory)
	{
		auto errorCode = ::GetLastError();
		CString errorInfo;
		errorInfo.Format(L"map view of file failed, error code:%lu", errorCode);
		LOGA_ERROR("%ws", errorInfo);
		OUTPUT("%ws", errorInfo);
		return errorInfo;
	}

	tlifeSpanHelper.setVoidPoint(memory);

	WCHAR fileNameBuffer[Max_Buffer_Size] = { 0 };
	if (!::GetMappedFileNameW(::GetCurrentProcess(), memory, fileNameBuffer, Max_Buffer_Size))
	{
		auto errorCode = ::GetLastError();
		CString errorInfo;
		errorInfo.Format(L"get mapped file name failed, error code:%lu", errorCode);
		LOGA_ERROR("%ws", errorInfo);
		OUTPUT("%ws", errorInfo);
		return errorInfo;
	}

	WCHAR tempBuffer[Max_Buffer_Size] = { 0 };
	if (!::GetLogicalDriveStringsW(Max_Buffer_Size, tempBuffer))
	{
		auto errorCode = ::GetLastError();
		CString errorInfo;
		errorInfo.Format(L"get logical drive strings failed, error code:%lu", errorCode);
		LOGA_ERROR("%ws", errorInfo);
		OUTPUT("%ws", errorInfo);
		return errorInfo;
	}

	WCHAR nameBuffer[Max_Buffer_Size] = { 0 };
	WCHAR driver[3] = TEXT(" :");
	bool isFound = false;
	WCHAR* pointToTempBuffer = tempBuffer;

	do
	{
		*driver = *pointToTempBuffer;
		if (!::QueryDosDeviceW(driver, nameBuffer, Max_Buffer_Size))
		{
			auto errorCode = ::GetLastError();
			CString errorInfo;
			errorInfo.Format(L"query dos device failed, error code:%lu", errorCode);
			LOGA_ERROR("%ws", errorInfo);
			OUTPUT("%ws", errorInfo);
			return errorInfo;
		}

		auto nameLength = _tcslen(nameBuffer);
		if (nameLength > Max_Buffer_Size)
		{
			return CString("get name length failed");
		}

		isFound = (0 == _tcsnicmp(fileNameBuffer, nameBuffer, nameLength)) &&
			(_T('\\') == *(fileNameBuffer + nameLength));
		if (!isFound)
		{
			return CString("can not find char");
		}

		WCHAR tempFile[Max_Buffer_Size] = { 0 };
		::StringCchPrintfW(tempFile, Max_Buffer_Size, TEXT("%ws%ws"), driver, fileNameBuffer + nameLength);
		::StringCchCopyNW(fileNameBuffer, Max_Buffer_Size, tempFile, _tcslen(tempFile));

		while (*pointToTempBuffer++);

	} while (!isFound && *pointToTempBuffer);

	return CString(fileNameBuffer);
}
