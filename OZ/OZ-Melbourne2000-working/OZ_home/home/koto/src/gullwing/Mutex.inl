// Mutex.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// Mutex

#if OS_TYPE == WINDOWS && defined(_MT)

inline Mutex::~Mutex()
{
	::DeleteCriticalSection(&m_criticalSection);
	//::CloseHandle(m_hMutex);
	//m_hMutex = NULL;
}
inline Mutex::Mutex()
{
	::InitializeCriticalSection(&m_criticalSection);

	//m_hMutex = NULL;
	
	//SECURITY_ATTRIBUTES sa;
	//::ZeroMemory(&sa, sizeof(sa));
	//sa.nLength = sizeof(sa);
	
	//m_hMutex = ::CreateMutex(&sa, lock, "SoccerClientMutex");
	//ASSERT(m_hMutex);
}
inline void Mutex::lock()
{
	::EnterCriticalSection(&m_criticalSection);

	//DWORD dw = ::WaitForSingleObject(m_hMutex, INFINITE);
	//dw;	ASSERT(dw == WAIT_OBJECT_0);
}
inline void Mutex::unlock()
{
	::LeaveCriticalSection(&m_criticalSection);

	//::ReleaseMutex(m_hMutex);
}

#else

inline Mutex::~Mutex()
{
}
inline Mutex::Mutex()
{
}
inline void Mutex::lock()
{
}
inline void Mutex::unlock()
{
}

#endif

/////////////////////////////////////////////////////////////////////////////
#undef inline
