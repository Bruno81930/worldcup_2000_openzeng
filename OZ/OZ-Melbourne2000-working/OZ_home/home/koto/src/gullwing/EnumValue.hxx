// EnumValue.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ENUMVALUE_HXX__INCLUDED) // {
#define ENUMVALUE_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// EnumValue

/**
* 無効値を保持できる enum ライクなオブジェクト。
* @param	E
*		基本になる enum 型。
* @param	invalid
*		無効値を表す値。E に含まれない値でなければならない。
*
*/
template <typename E, int invalid>
class EnumValue {
private:
	int m_value;
public:
	/*dummy*/
	/**
	* オブジェクトを構築するが、初期値は不定である。
	*/
	EnumValue() {
#ifndef NDEBUG
		m_value = invalid;
#endif
	}
	/**
	* 初期値 v でオブジェクトを構築する。v == invalid であってはならない。
	*/
	EnumValue(E v) {
		ASSERT(v != invalid);
		m_value = v;
	}
	/**
	* *this の保持する値を E 型の値で返す。this->valid() であること。
	*/
	operator E () const {
		ASSERT(valid());
		return (E)m_value;
	}
	/**
	* *this の保持する値を E 型の値で返す。this->valid() であること。
	*/
	E toPrimitive() const {
		return (E)*this;
	}
	
	/**
	* 無効値を保持するオブジェクトを得る。
	*/
	static EnumValue invalidValue() {
		EnumValue result;
		result.m_value = invalid;
		return result;
	}
	/**
	* *this が無効値を保持している場合 false、そうでない場合 true を返す。
	*/
	bool valid() const {
		return m_value != invalid;
	}
	
	/**
	* 等しいかどうかを返す。
	* @return
	*		*this と x が同じ値を保持している場合 true、 そうでない場合 false 
	*		を返す。特に、両方が無効値の場合 true、片方だけが無効値の場合     
	*		false を返す。
	*/
	bool operator== (EnumValue x) const {
		return m_value == x.m_value;
	}
	/**
	* !(*this == x) を返す。
	*/
	bool operator!= (EnumValue x) const {
		return !(m_value == x.m_value);
	}
	
	bool operator== (E x) const {
		return *this == EnumValue(x);
	}
	bool operator!= (E x) const {
		return *this != EnumValue(x);
	}
};

template <typename E, int invalid>
inline bool operator == (E e, EnumValue<E, invalid> x) {
	return EnumValue<E, invalid>(e) == x;
}
template <typename E, int invalid>
inline bool operator != (E e, EnumValue<E, invalid> x) {
	return EnumValue<E, invalid>(e) != x;
}

/////////////////////////////////////////////////////////////////////////////

#endif // } !defined(ENUMVALUE_HXX__INCLUDED)
