#include "stdafx.h"

#include "tdebuger.h"
#include "toutputhelper.h"

TOutPutHelper g_toutputHelper;

TOutPutHelper::TOutPutHelper()
{
	// 首先尝试附加到父进程控制台
	// 如果无法附加则自主创建
	if (!::AttachConsole(ATTACH_PARENT_PROCESS))
	{
		if (!::AllocConsole())
		{
			LOGA_ERROR("failed get console");
			m_isSuccessGetConsole = false;
			return;
		}
	}

	m_consoleHnadle = ::GetStdHandle(STD_OUTPUT_HANDLE);
}

TOutPutHelper::~TOutPutHelper()
{
	if (m_isSuccessGetConsole)
	{
		::FreeConsole();
	}
}
