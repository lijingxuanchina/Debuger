#include "stdafx.h"

#include "tlifespanhelper.h"

TLifeSpanHelper::TLifeSpanHelper()
{
}

TLifeSpanHelper::~TLifeSpanHelper()
{
	if (m_handle)
	{
		::CloseHandle(m_handle);
	}
	if (m_voidPoint)
	{
		::UnmapViewOfFile(m_voidPoint);
	}
}

void TLifeSpanHelper::setHandle(HANDLE handle)
{
	if (handle)
	{
		m_handle = handle;
	}
}

void TLifeSpanHelper::setVoidPoint(void* point)
{
	if (point)
	{
		m_voidPoint = point;
	}
}
