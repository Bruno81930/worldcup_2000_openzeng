// Double.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DOUBLE_HXX__INCLUDED) // {
#define DOUBLE_HXX__INCLUDED

#include "Integer.hxx"

/////////////////////////////////////////////////////////////////////////////
// Double

/**
* $BL58zCM$rJ];}$G$-$kBe$o$j$K!"(B -DBL_MAX $B$rJ];}$G$-$J$$(B double $B%i%$%/$J%*%V%8%'(B
* $B%/%H!#=i4|CM$OITDj$G$"$k!#(B
*/
class Double {
private:
	double m_value;
public:
	/*dummy*/
	Double(const Double& master) { m_value = master.m_value; }	// gcc $B%P%0BP:v(B
	/**
	* $B%*%V%8%'%/%H$r9=C[$9$k$,!"=i4|CM$OITDj$G$"$k!#(B
	*/
	Double();
	/**
	* $B=i4|CM(B v $B$G%*%V%8%'%/%H$r9=C[$9$k!#(Bv $B$,(B -DBL_MAX $B$G$"$C$F$O$J$i$J$$!#(B
	*/
	Double(double v);
	/**
	* $B=i4|CM(B v $B$G%*%V%8%'%/%H$r9=C[$9$k!#(Bv.valid() $B$G$"$k$3$H!#(B
	*/
	Double(const Integer& v);
	/**
	* *this $B$NJ];}$9$kCM$r(B double $BCM$GJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	operator double () const;
	/**
	* operator double () $B$HF1$8!#(B
	*/
	double toPrimitive() const { return (double)*this; }
	
	/**
	* $BL58zCM$rJ];}$9$k%*%V%8%'%/%H$rF@$k!#(B
	*/
	static Double invalidValue();
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
	bool operator== (const Double& x) const;
	/**
	* !(*this == x) $B$rJV$9!#(B
	*/
	bool operator!= (const Double& x) const;
	/**
	* $BHf3S$9$k!#:8JU$b1&JU$bL58zCM$rJ];}$7$F$O$$$1$J$$!#0J2<F1MM$G$"$k!#(B
	*/
	bool operator< (const Double& x) const;
	bool operator<= (const Double& x) const;
	bool operator> (const Double& x) const;
	bool operator>= (const Double& x) const;
	
	Double operator- () const;
	Double operator+ () const;
	Double operator+ (const Double& x) const;
	Double operator- (const Double& x) const;
	Double operator* (const Double& x) const;
	Double operator/ (const Double& x) const;
	
	Double& operator+= (const Double& x);
	Double& operator-= (const Double& x);
	Double& operator*= (const Double& x);
	Double& operator/= (const Double& x);
};

#ifndef NDEBUG
Double abs(Double v);
#else
inline Double abs(Double v);
#endif

bool operator== (const Double& x, double y);
bool operator!= (const Double& x, double y);
bool operator< (const Double& x, double y);
bool operator<= (const Double& x, double y);
bool operator> (const Double& x, double y);
bool operator>= (const Double& x, double y);

bool operator== (double x, const Double& y);
bool operator!= (double x, const Double& y);
bool operator< (double x, const Double& y);
bool operator<= (double x, const Double& y);
bool operator> (double x, const Double& y);
bool operator>= (double x, const Double& y);

Double operator+ (const Double& x, double y);
Double operator- (const Double& x, double y);
Double operator* (const Double& x, double y);
Double operator/ (const Double& x, double y);

Double operator+ (double x, const Double& y);
Double operator- (double x, const Double& y);
Double operator* (double x, const Double& y);
Double operator/ (double x, const Double& y);

bool operator== (const Double& x, int y);
bool operator!= (const Double& x, int y);
bool operator< (const Double& x, int y);
bool operator<= (const Double& x, int y);
bool operator> (const Double& x, int y);
bool operator>= (const Double& x, int y);

bool operator== (int x, const Double& y);
bool operator!= (int x, const Double& y);
bool operator< (int x, const Double& y);
bool operator<= (int x, const Double& y);
bool operator> (int x, const Double& y);
bool operator>= (int x, const Double& y);

Double operator+ (const Double& x, int y);
Double operator- (const Double& x, int y);
Double operator* (const Double& x, int y);
Double operator/ (const Double& x, int y);

Double operator+ (int x, const Double& y);
Double operator- (int x, const Double& y);
Double operator* (int x, const Double& y);
Double operator/ (int x, const Double& y);

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Double.inl"
#endif

#endif // } !defined(DOUBLE_HXX__INCLUDED)
