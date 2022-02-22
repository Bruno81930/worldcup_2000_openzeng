// Mutex.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MUTEX_HXX__INCLUDED) // {
#define MUTEX_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Mutex

/*
* マルチスレッド環境でのみ、Mutex を提供する。
*/
class Mutex
{
private:
#if OS_TYPE == WINDOWS && defined(_MT)
	CRITICAL_SECTION m_criticalSection;
	//HANDLE m_hMutex;
#endif
	
protected:
	
public:
	~Mutex();
	/**
	* オブジェクトを構築する。
	*/
	Mutex();
	Mutex(const Mutex&);
	
	/*
	* このメンバ関数を呼び出したスレッドにこの mutex を「バインド」する。１つ 
    * のmutex は、１度に１つのスレッドとしかバインドされない。同一のスレッドが
    * 複数回このメンバ関数を呼び出した場合、スレッドと mutex は複数回バインド 
    * される。
	*/
	void lock();
	/*
	* このメンバ関数を呼び出したスレッドと、この mutex との「バインド」を解除 
    * する。複数回バインドされている場合、その内の１回分のバインドを解除する。
	*/
	void unlock();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Mutex.inl"
#endif

#endif // } !defined(MUTEX_HXX__INCLUDED)
