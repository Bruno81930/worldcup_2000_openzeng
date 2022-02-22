// Vector.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(VECTOR_HXX__INCLUDED) // {
#define VECTOR_HXX__INCLUDED

#include "Double.hxx"
#include "Angle.hxx"

/////////////////////////////////////////////////////////////////////////////
// Vector

/**
* $B%Y%/%H%k!#=i4|CM$OITDj$G$"$k!#L58zCM$rJ];}$G$-$k!#(B
*/
class Vector
{
private:
	Double m_x;
	Double m_y;
public:
	/*dummy*/
	/**
	* $B%*%V%8%'%/%H$r9=C[$9$k$,!"=i4|CM$OITDj$G$"$k!#(B
	*/
	Vector();
	/**
	* $B%*%V%8%'%/%H$r%Y%/%H%k(B (x, y) $B$G=i4|2=$9$k!#(Bx $B$H(B y $B$O6&$K(B valid() $B$G$"$k(B
    * $B$3$H!#(B
	*/
	Vector(const Double& x, const Double& y);
	/**
	* $B%*%V%8%'%/%H$r6K:BI8$GI=$5$l$?:BI8(B (r, theta) $B$G=i4|2=$9$k!#(Br $B$H(B theta  
    * $B$O6&$K(B valid() $B$G$"$k$3$H!#(B
	*/
	Vector(const Double& r, Angle theta);
	
	static Vector invalidValue();
	bool valid() const;
	
	/**
	* $B%Y%/%H%k$N(B x $B:BI8$rJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	double x() const;
	/**
	* $B%Y%/%H%k$N(B y $B:BI8$rJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	double y() const;
	/**
	* $B%Y%/%H%k$N6K:BI8>e$G$N3QEY:BI8$rJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	Angle angle() const;
	/**
	* $B%Y%/%H%k$N:9$9:BI8$N!"86E@$+$i$N5wN%$rJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	double norm() const;
	
	bool operator== (Vector x) const;
	bool operator!= (Vector x) const;
	
	Vector operator- () const;
	Vector operator+ () const;
	Vector operator+ (Vector x) const;
	Vector operator- (Vector x) const;
	Vector operator* (double x) const;
	Vector operator/ (double x) const;
	
	Vector& operator+= (Vector x);
	Vector& operator-= (Vector x);
	Vector& operator*= (double x);
	Vector& operator/= (double x);
	Vector& normalize(double x);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Vector.inl"
#endif

#endif // } !defined(VECTOR_HXX__INCLUDED)
