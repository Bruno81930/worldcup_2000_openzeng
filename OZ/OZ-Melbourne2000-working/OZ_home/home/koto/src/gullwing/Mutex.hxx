// Mutex.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MUTEX_HXX__INCLUDED) // {
#define MUTEX_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Mutex

/*
* $B%^%k%A%9%l%C%I4D6-$G$N$_!"(BMutex $B$rDs6!$9$k!#(B
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
	* $B%*%V%8%'%/%H$r9=C[$9$k!#(B
	*/
	Mutex();
	Mutex(const Mutex&);
	
	/*
	* $B$3$N%a%s%P4X?t$r8F$S=P$7$?%9%l%C%I$K$3$N(B mutex $B$r!V%P%$%s%I!W$9$k!##1$D(B 
    * $B$N(Bmutex $B$O!"#1EY$K#1$D$N%9%l%C%I$H$7$+%P%$%s%I$5$l$J$$!#F10l$N%9%l%C%I$,(B
    * $BJ#?t2s$3$N%a%s%P4X?t$r8F$S=P$7$?>l9g!"%9%l%C%I$H(B mutex $B$OJ#?t2s%P%$%s%I(B 
    * $B$5$l$k!#(B
	*/
	void lock();
	/*
	* $B$3$N%a%s%P4X?t$r8F$S=P$7$?%9%l%C%I$H!"$3$N(B mutex $B$H$N!V%P%$%s%I!W$r2r=|(B 
    * $B$9$k!#J#?t2s%P%$%s%I$5$l$F$$$k>l9g!"$=$NFb$N#12sJ,$N%P%$%s%I$r2r=|$9$k!#(B
	*/
	void unlock();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Mutex.inl"
#endif

#endif // } !defined(MUTEX_HXX__INCLUDED)
