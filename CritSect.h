#pragma once

#include "Common.h"

#include <windows.h>
#define BF_PLATFORM_WINDOWS

NS_BF_BEGIN;

#ifdef BF_PLATFORM_WINDOWS
class CritSect
{
public:
	CRITICAL_SECTION mCritSect;
	int32 mLockCount;

public:
	CritSect()
	{
		InitializeCriticalSection(&mCritSect);
		mLockCount = 0;
	}

	~CritSect()
	{
		DeleteCriticalSection(&mCritSect);
		BF_ASSERT(mLockCount == 0);
	}

	bool TryLock()
	{
		bool locked = TryEnterCriticalSection(&mCritSect) != 0;
		if (locked)
			mLockCount++;
		return locked;
	}

	// This is a poor implementation.  Used only for debugging
	bool TryLock(int waitMS)
	{
		if (waitMS == 0)
			return TryLock();

		uint32 start = GetTickCount();
		while ((int)(GetTickCount() - start) < waitMS)
		{
			if (TryEnterCriticalSection(&mCritSect))
			{
				mLockCount++;
				return true;
			}
		}
		return false;
	}

	void Lock()
	{
		EnterCriticalSection(&mCritSect);
		mLockCount++;
	}

	void Unlock()
	{
		mLockCount--;
		LeaveCriticalSection(&mCritSect);
	}
};


class SyncEvent
{
public:
	HANDLE mEvent;

public:
	SyncEvent(bool manualReset = false, bool initialState = false)
	{
		mEvent = ::CreateEvent(NULL, manualReset, initialState, NULL);
	}

	~SyncEvent()
	{
		::CloseHandle(mEvent);
	}

	void Set()
	{
		::SetEvent(mEvent);
	}

	void Reset()
	{
		::ResetEvent(mEvent);
	}

	bool WaitFor(int timeoutMS = -1)
	{
		return ::WaitForSingleObject(mEvent, timeoutMS) == WAIT_OBJECT_0;
	}

	static int WaitForOne(SyncEvent& event0, SyncEvent& event1, int timeoutMS = -1)
	{
		HANDLE handles[] = { event0.mEvent, event1.mEvent };
		return ::WaitForMultipleObjects(2, handles, FALSE, (DWORD)timeoutMS);
	}
};

#else



#endif

class AutoCrit
{
public:
	CritSect* mCritSect;

public:
	AutoCrit(CritSect& critSect)
	{
		mCritSect = &critSect;
		mCritSect->Lock();
	}

	~AutoCrit()
	{
		mCritSect->Unlock();
	}
};

NS_BF_END;
