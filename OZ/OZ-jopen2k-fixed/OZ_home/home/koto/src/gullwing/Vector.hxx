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
* ベクトル。初期値は不定である。無効値を保持できる。
*/
class Vector
{
private:
	Double m_x;
	Double m_y;
public:
	/*dummy*/
	/**
	* オブジェクトを構築するが、初期値は不定である。
	*/
	Vector();
	/**
	* オブジェクトをベクトル (x, y) で初期化する。x と y は共に valid() である
    * こと。
	*/
	Vector(const Double& x, const Double& y);
	/**
	* オブジェクトを極座標で表された座標 (r, theta) で初期化する。r と theta  
    * は共に valid() であること。
	*/
	Vector(const Double& r, Angle theta);
	
	static Vector invalidValue();
	bool valid() const;
	
	/**
	* ベクトルの x 座標を返す。this->valid() であること。
	*/
	double x() const;
	/**
	* ベクトルの y 座標を返す。this->valid() であること。
	*/
	double y() const;
	/**
	* ベクトルの極座標上での角度座標を返す。this->valid() であること。
	*/
	Angle angle() const;
	/**
	* ベクトルの差す座標の、原点からの距離を返す。this->valid() であること。
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
