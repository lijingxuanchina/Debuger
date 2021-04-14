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
	// TODO lijingxuan ������������¼�
	// ����ҪΪ exception �¼�
	void processOutputDebugStringEvent(
		const DEBUG_EVENT& debugEvent,
		const PROCESS_INFORMATION& processInfo);

private:
	CString readRemoteString(HANDLE process, LPVOID address, WORD length, BOOL unicode);
};

#endif // __TDEBUGER_H_
