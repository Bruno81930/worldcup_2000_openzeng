// Angle.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ANGLE_HXX__INCLUDED) // {
#define ANGLE_HXX__INCLUDED

#include "Double.hxx"

/////////////////////////////////////////////////////////////////////////////
// Angle

/**
* 方向（角の大きさではない。角の大きさは double で表す）。オブジェクトは「無効
* 値」を表す場合がある。オブジェクトは有効な方向を表すか、無効値を表すかのいず
* れかである。
*/
class Angle
{
private:
	/**
	* 無効値の場合 Double::invalidValue()、そうでない場合角度（度単位）で -180
	* より大きく、180 以下の値。
	*/
	Double m_degrees;
public:
	/** オブジェクトを構築するが、初期値は不定である。*/
	Angle();
	/**
	* 特定の角で初期化する。
	* @param	degrees
	*				初期化する角（度単位）。
	*/
	explicit Angle(Double degrees);
	/**
	* 特定の角で初期化された Angle オブジェクトを返す。
	* @param	radians
	*				初期化する角（ラジアン単位）。
	*/
	static Angle fromRadians(double radians);
	
	/** 無効値を表す Angle オブジェクトを返す。*/
	static Angle invalidValue();
	/** this が無効値を表すかどうかを返す。*/
	bool valid() const;
	
	/**
	* this が表す角を度単位で返す。戻り値は -180〜180 の範囲にある。this->
	* valid() で無ければならない。
	*/
	double degrees() const;
	/**
	* this が表す角をラジアン単位で返す。戻り値は -PI〜PI の範囲にある。this->
	* valid() で無ければならない。
	*/
	double radians() const;
	
	/** 両辺が無効値か、角が等しい？ */
	bool operator== (Angle x) const;
	bool operator!= (Angle x) const;
	/*bool operator< (Angle x) const;
	bool operator<= (Angle x) const;
	bool operator> (Angle x) const;
	bool operator>= (Angle x) const;*/
	
	/** *this を返す。this->valid() で無ければならない。*/
	const Angle& operator+ () const;
	/** Angle(-this->degrees()) を返す。this->valid() で無ければならない。*/
	Angle operator- () const;
	/** Angle(this->degrees() + x.degrees()) を返す。this->valid() で無ければならない。*/
	Angle operator+ (Angle x) const;
	/** Angle(this->degrees() - x.degrees()) を返す。this->valid() で無ければならない。*/
	Angle operator- (Angle x) const;
	/*Angle operator* (double x) const;
	Angle operator/ (double x) const;*/
	
	Angle& operator+= (Angle x);
	Angle& operator-= (Angle x);
	/*Angle& operator*= (double x);
	Angle& operator/= (double x);*/
};

#ifndef NDEBUG
/** Angle(abs(this->degrees())) を返す。*/
Angle abs(Angle angle);
#else
/** Angle(abs(this->degrees())) を返す。*/
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
