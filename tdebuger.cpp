#include "stdafx.h"

#include "TDebuger.h"

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
			break;
		}
		endDisposition = processDebugEvent(debugEvent, processInfo);
		if (!::ContinueDebugEvent(
			debugEvent.dwProcessId, debugEvent.dwThreadId, endDisposition))
		{
			LOGA_ERROR("continue debug event failed, error code:%lu", ::GetLastError());
			break;
		}
	}
}

DWORD TDebuger::processDebugEvent(
	const DEBUG_EVENT& debugEvent,
	const PROCESS_INFORMATION& processInfo)
{
	switch (debugEvent.dwDebugEventCode)
	{
	case EXCEPTION_DEBUG_EVENT:
		break;
	case CREATE_THREAD_DEBUG_EVENT:
		break;
	case CREATE_PROCESS_DEBUG_EVENT:
		break;
	case EXIT_THREAD_DEBUG_EVENT:
		break;
	case EXIT_PROCESS_DEBUG_EVENT:
		break;
	case LOAD_DLL_DEBUG_EVENT:
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

	return DBG_CONTINUE;
}

void TDebuger::processOutputDebugStringEvent(
	const DEBUG_EVENT& debugEvent,
	const PROCESS_INFORMATION& processInfo)
{
	auto outputDebugStringInfo = debugEvent.u.DebugString;
	std::wcout << _T("OutputDebugStringEvent\nMessage:\t");
	std::wcout << readRemoteString(
		processInfo.hProcess,
		outputDebugStringInfo.lpDebugStringData,
		outputDebugStringInfo.nDebugStringLength,
		outputDebugStringInfo.fUnicode) << std::endl;
}

CString TDebuger::readRemoteString(
	HANDLE process, LPVOID address, WORD length, BOOL unicode)
{
	// TODO lijingxuan 完成这个功能
	return CString();
}
