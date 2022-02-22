// Double.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DOUBLE_HXX__INCLUDED) // {
#define DOUBLE_HXX__INCLUDED

#include "Integer.hxx"

/////////////////////////////////////////////////////////////////////////////
// Double

/**
* 無効値を保持できる代わりに、 -DBL_MAX を保持できない double ライクなオブジェ
* クト。初期値は不定である。
*/
class Double {
private:
	double m_value;
public:
	/*dummy*/
	Double(const Double& master) { m_value = master.m_value; }	// gcc バグ対策
	/**
	* オブジェクトを構築するが、初期値は不定である。
	*/
	Double();
	/**
	* 初期値 v でオブジェクトを構築する。v が -DBL_MAX であってはならない。
	*/
	Double(double v);
	/**
	* 初期値 v でオブジェクトを構築する。v.valid() であること。
	*/
	Double(const Integer& v);
	/**
	* *this の保持する値を double 値で返す。this->valid() であること。
	*/
	operator double () const;
	/**
	* operator double () と同じ。
	*/
	double toPrimitive() const { return (double)*this; }
	
	/**
	* 無効値を保持するオブジェクトを得る。
	*/
	static Double invalidValue();
	/**
	* *this が無効値を保持している場合 false、そうでない場合 true を返す。
	*/
	bool valid() const;
	
	/**
	* 等しいかどうかを返す。
	* @return
	*		*this と x が同じ値を保持している場合 true、 そうでない場合 false 
	*		を返す。特に、両方が無効値の場合 true、片方だけが無効値の場合     
	*		false を返す。
	*/
	bool operator== (const Double& x) const;
	/**
	* !(*this == x) を返す。
	*/
	bool operator!= (const Double& x) const;
	/**
	* 比較する。左辺も右辺も無効値を保持してはいけない。以下同様である。
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
