#include "stdafx.h"

#include "TDebuger.h"

using std::cout;
using std::endl;
using std::wcout;

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
	OUTPUT("success create debug process!");
	OUTPUT("pid:%lu", processInfo.dwProcessId);
}

void TDebuger::processExitProcessDebugEvent(DWORD& continueStatus)
{
	continueStatus = 0;
}

CString TDebuger::readRemoteString(
	HANDLE process, LPVOID address, WORD length, BOOL unicode)
{
	if (unicode)
	{
		WCHAR wbuffer[MAX_PATH] = { 0 };
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
		CHAR buffer[MAX_PATH] = { 0 };
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
