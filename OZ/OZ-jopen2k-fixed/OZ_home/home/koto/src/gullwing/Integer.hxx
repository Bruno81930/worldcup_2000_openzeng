// Integer.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(INTEGER_HXX__INCLUDED) // {
#define INTEGER_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Integer

/**
* 無効値を保持できる代わりに、 INT_MIN を保持できない int ライクなオブジェクト。
* 初期値は不定である。
*/
class Integer {
private:
	int m_value;
public:
	/*dummy*/
	/**
	* オブジェクトを構築するが、初期値は不定である。
	*/
	Integer();
	/**
	* 初期値 v でオブジェクトを構築する。v が INT_MIN であってはならない。
	*/
	Integer(int v);
	/**
	* *this の保持する値を int 値で返す。this->valid() であること。
	*/
	operator int () const;
	/**
	* operator int () と同じ。
	*/
	int toPrimitive() const { return (int)*this; }
	
	/**
	* 無効値を保持するオブジェクトを得る。
	*/
	static Integer invalidValue();
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
	bool operator== (const Integer& x) const;
	/**
	* !(*this == x) を返す。
	*/
	bool operator!= (const Integer& x) const;
	/**
	* 比較する。左辺も右辺も無効値を保持してはいけない。以下同様である。
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
