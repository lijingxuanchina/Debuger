#ifndef __TDEBUGER_H_
#define __TDEBUGER_H_

class TDebuger
{
public:
	TDebuger();
	~TDebuger();
	void startDebug(const PROCESS_INFORMATION& processInfo);

private:
	DWORD processDebugEvent(
		const DEBUG_EVENT& debugEvent,
		const PROCESS_INFORMATION& processInfo);
	void processOutputDebugStringEvent(
		const DEBUG_EVENT& debugEvent,
		const PROCESS_INFORMATION& processInfo);
	void processExceptionDebugEvent(
		const DEBUG_EVENT& debugEvent,
		const PROCESS_INFORMATION& processInfo,
		DWORD& continueStatus);
	void processCreateProcessDebugEvent(
		const DEBUG_EVENT& debugEvent,
		const PROCESS_INFORMATION& processInfo);
	void processExitProcessDebugEvent(
		DWORD& continueStatus);
	void processLoadDllDebugEvent(
		const DEBUG_EVENT& debugEvent);

private:
	CString readRemoteString(HANDLE process, LPVOID address, WORD length, BOOL unicode);
};

#endif // __TDEBUGER_H_
