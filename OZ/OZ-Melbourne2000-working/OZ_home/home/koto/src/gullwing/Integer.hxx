// Integer.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(INTEGER_HXX__INCLUDED) // {
#define INTEGER_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Integer

/**
* $BL58zCM$rJ];}$G$-$kBe$o$j$K!"(B INT_MIN $B$rJ];}$G$-$J$$(B int $B%i%$%/$J%*%V%8%'%/%H!#(B
* $B=i4|CM$OITDj$G$"$k!#(B
*/
class Integer {
private:
	int m_value;
public:
	/*dummy*/
	/**
	* $B%*%V%8%'%/%H$r9=C[$9$k$,!"=i4|CM$OITDj$G$"$k!#(B
	*/
	Integer();
	/**
	* $B=i4|CM(B v $B$G%*%V%8%'%/%H$r9=C[$9$k!#(Bv $B$,(B INT_MIN $B$G$"$C$F$O$J$i$J$$!#(B
	*/
	Integer(int v);
	/**
	* *this $B$NJ];}$9$kCM$r(B int $BCM$GJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	operator int () const;
	/**
	* operator int () $B$HF1$8!#(B
	*/
	int toPrimitive() const { return (int)*this; }
	
	/**
	* $BL58zCM$rJ];}$9$k%*%V%8%'%/%H$rF@$k!#(B
	*/
	static Integer invalidValue();
	/**
	* *this $B$,L58zCM$rJ];}$7$F$$$k>l9g(B false$B!"$=$&$G$J$$>l9g(B true $B$rJV$9!#(B
	*/
	bool valid() const;
	
	/**
	* $BEy$7$$$+$I$&$+$rJV$9!#(B
	* @return
	*		*this $B$H(B x $B$,F1$8CM$rJ];}$7$F$$$k>l9g(B true$B!"(B $B$=$&$G$J$$>l9g(B false 
	*		$B$rJV$9!#FC$K!"N>J}$,L58zCM$N>l9g(B true$B!"JRJ}$@$1$,L58zCM$N>l9g(B     
	*		false $B$rJV$9!#(B
	*/
	bool operator== (const Integer& x) const;
	/**
	* !(*this == x) $B$rJV$9!#(B
	*/
	bool operator!= (const Integer& x) const;
	/**
	* $BHf3S$9$k!#:8JU$b1&JU$bL58zCM$rJ];}$7$F$O$$$1$J$$!#0J2<F1MM$G$"$k!#(B
	*/
	bool operator< (const Integer& x) const;
	bool operator<= (const Integer& x) const;
	bool operator> (const Integer& x) const;
	bool operator>= (const Integer& x) const;
	
	Integer operator- () const;
	Integer operator+ () const;
	Integer operator+ (const Integer& x) const;
	Integer operator- (const Integer& x) const;
	Integer operator* (const Integer& x) const;
	Integer operator/ (const Integer& x) const;
	
	Integer& operator++ ();
	Integer& operator-- ();
	Integer operator++ (int);
	Integer operator-- (int);
	Integer& operator+= (const Integer& x);
	Integer& operator-= (const Integer& x);
	Integer& operator*= (const Integer& x);
	Integer& operator/= (const Integer& x);
};

#ifndef NDEBUG
Integer abs(Integer v);
#else
inline Integer abs(Integer v);
#endif

bool operator== (const Integer& x, int y);
bool operator!= (const Integer& x, int y);
bool operator< (const Integer& x, int y);
bool operator<= (const Integer& x, int y);
bool operator> (const Integer& x, int y);
bool operator>= (const Integer& x, int y);

bool operator== (int x, const Integer& y);
bool operator!= (int x, const Integer& y);
bool operator< (int x, const Integer& y);
bool operator<= (int x, const Integer& y);
bool operator> (int x, const Integer& y);
bool operator>= (int x, const Integer& y);

Integer operator+ (const Integer& x, int y);
Integer operator- (const Integer& x, int y);
Integer operator* (const Integer& x, int y);
Integer operator/ (const Integer& x, int y);

Integer operator+ (int x, const Integer& y);
Integer operator- (int x, const Integer& y);
Integer operator* (int x, const Integer& y);
Integer operator/ (int x, const Integer& y);

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Integer.inl"
#endif

#endif // } !defined(INTEGER_HXX__INCLUDED)
