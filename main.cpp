/*
* 作者：	李京轩
* 邮箱：	847720711@qq.com
* 日期：	2021/04/08 0:21
*/

#include "stdafx.h"

#include "resultinfo.h"
#include "tdebuger.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	LOGA("start debug");
	// 通过命令行参数启动调试进程
	// 用户态下调试器启动进程
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	PROCESS_INFORMATION processInfo = { 0 };
	if (!::CreateProcessW(
		NULL, lpCmdLine, NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL,
		&startupInfo, &processInfo))
	{
		LOGA_ERROR("create process failed, error code: %lu", ::GetLastError());
		return Result_CreateProcessFailed;
	}

	TDebuger tdebuger;
	tdebuger.startDebug(processInfo);

	// 调试结束释放进程以及线程句柄
	::CloseHandle(processInfo.hThread);
	::CloseHandle(processInfo.hProcess);

	return Result_Success;
}