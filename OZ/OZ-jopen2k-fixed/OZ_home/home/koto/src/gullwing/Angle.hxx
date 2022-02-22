// Angle.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ANGLE_HXX__INCLUDED) // {
#define ANGLE_HXX__INCLUDED

#include "Double.hxx"

/////////////////////////////////////////////////////////////////////////////
// Angle

/**
* $BJ}8~!J3Q$NBg$-$5$G$O$J$$!#3Q$NBg$-$5$O(B double $B$GI=$9!K!#%*%V%8%'%/%H$O!VL58z(B
* $BCM!W$rI=$9>l9g$,$"$k!#%*%V%8%'%/%H$OM-8z$JJ}8~$rI=$9$+!"L58zCM$rI=$9$+$N$$$:(B
* $B$l$+$G$"$k!#(B
*/
class Angle
{
private:
	/**
	* $BL58zCM$N>l9g(B Double::invalidValue()$B!"$=$&$G$J$$>l9g3QEY!JEYC10L!K$G(B -180
	* $B$h$jBg$-$/!"(B180 $B0J2<$NCM!#(B
	*/
	Double m_degrees;
public:
	/** $B%*%V%8%'%/%H$r9=C[$9$k$,!"=i4|CM$OITDj$G$"$k!#(B*/
	Angle();
	/**
	* $BFCDj$N3Q$G=i4|2=$9$k!#(B
	* @param	degrees
	*				$B=i4|2=$9$k3Q!JEYC10L!K!#(B
	*/
	explicit Angle(Double degrees);
	/**
	* $BFCDj$N3Q$G=i4|2=$5$l$?(B Angle $B%*%V%8%'%/%H$rJV$9!#(B
	* @param	radians
	*				$B=i4|2=$9$k3Q!J%i%8%"%sC10L!K!#(B
	*/
	static Angle fromRadians(double radians);
	
	/** $BL58zCM$rI=$9(B Angle $B%*%V%8%'%/%H$rJV$9!#(B*/
	static Angle invalidValue();
	/** this $B$,L58zCM$rI=$9$+$I$&$+$rJV$9!#(B*/
	bool valid() const;
	
	/**
	* this $B$,I=$93Q$rEYC10L$GJV$9!#La$jCM$O(B -180$B!A(B180 $B$NHO0O$K$"$k!#(Bthis->
	* valid() $B$GL5$1$l$P$J$i$J$$!#(B
	*/
	double degrees() const;
	/**
	* this $B$,I=$93Q$r%i%8%"%sC10L$GJV$9!#La$jCM$O(B -PI$B!A(BPI $B$NHO0O$K$"$k!#(Bthis->
	* valid() $B$GL5$1$l$P$J$i$J$$!#(B
	*/
	double radians() const;
	
	/** $BN>JU$,L58zCM$+!"3Q$,Ey$7$$!)(B */
	bool operator== (Angle x) const;
	bool operator!= (Angle x) const;
	/*bool operator< (Angle x) const;
	bool operator<= (Angle x) const;
	bool operator> (Angle x) const;
	bool operator>= (Angle x) const;*/
	
	/** *this $B$rJV$9!#(Bthis->valid() $B$GL5$1$l$P$J$i$J$$!#(B*/
	const Angle& operator+ () const;
	/** Angle(-this->degrees()) $B$rJV$9!#(Bthis->valid() $B$GL5$1$l$P$J$i$J$$!#(B*/
	Angle operator- () const;
	/** Angle(this->degrees() + x.degrees()) $B$rJV$9!#(Bthis->valid() $B$GL5$1$l$P$J$i$J$$!#(B*/
	Angle operator+ (Angle x) const;
	/** Angle(this->degrees() - x.degrees()) $B$rJV$9!#(Bthis->valid() $B$GL5$1$l$P$J$i$J$$!#(B*/
	Angle operator- (Angle x) const;
	/*Angle operator* (double x) const;
	Angle operator/ (double x) const;*/
	
	Angle& operator+= (Angle x);
	Angle& operator-= (Angle x);
	/*Angle& operator*= (double x);
	Angle& operator/= (double x);*/
};

#ifndef NDEBUG
/** Angle(abs(this->degrees())) $B$rJV$9!#(B*/
Angle abs(Angle angle);
#else
/** Angle(abs(this->degrees())) $B$rJV$9!#(B*/
inline Angle abs(Angle angle);
#endif

double sin(Angle theta);
double cos(Angle theta);
double tan(Angle theta);

#undef PI
const double PI = 3.14159265358979323846;

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Angle.inl"
#endif

#endif // } !defined(ANGLE_HXX__INCLUDED)
