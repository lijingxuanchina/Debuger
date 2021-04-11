/*
* 作者：	李京轩
* 邮箱：	847720711@qq.com
* 日期：	2021/04/08 0:21
*/

#include "stdafx.h"

int main(int argc, char *argv[])
{
	// 通过命令行参数启动调试进程
	// 用户态下调试器启动进程
	STARTUPINFO startupinfo = { 0 };
	startupinfo.cb = sizeof(startupinfo);
	PROCESS_INFORMATION processInfo = { 0 };
	if (!::CreateProcess(
		NULL, argv[0], NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL,
		&startupinfo, &processInfo))
	{
		DWORD errorCode = ::GetLastError();
	}

	// 调试结束释放进程以及线程句柄
	::CloseHandle(processInfo.hThread);
	::CloseHandle(processInfo.hProcess);

	return 0;
}