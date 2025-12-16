#pragma once
#include "GenTL.h"
#include "cxp_standard.h"
#include "windows.h"
using namespace GenTL;

static inline bool8_t gentl_is_success(GC_ERROR status)
{
	return (GC_ERR_SUCCESS == status);
}

//自动锁
class CCapCritical
{
public:
	CCapCritical()
	{
		InitializeCriticalSection(&m_cs);
	}

	virtual ~CCapCritical()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	CRITICAL_SECTION m_cs;
};

class CCapAutoLock//自动锁
{
public:
	CCapCritical* m_csec;
	CCapAutoLock(CCapCritical* critsec)
	{
		m_csec = critsec;
		m_csec->Lock();
	}
	virtual ~CCapAutoLock()
	{
		m_csec->Unlock();
	}
};