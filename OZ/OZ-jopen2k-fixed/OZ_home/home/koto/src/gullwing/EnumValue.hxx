// EnumValue.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ENUMVALUE_HXX__INCLUDED) // {
#define ENUMVALUE_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// EnumValue

/**
* $BL58zCM$rJ];}$G$-$k(B enum $B%i%$%/$J%*%V%8%'%/%H!#(B
* @param	E
*		$B4pK\$K$J$k(B enum $B7?!#(B
* @param	invalid
*		$BL58zCM$rI=$9CM!#(BE $B$K4^$^$l$J$$CM$G$J$1$l$P$J$i$J$$!#(B
*
*/
template <typename E, int invalid>
class EnumValue {
private:
	int m_value;
public:
	/*dummy*/
	/**
	* $B%*%V%8%'%/%H$r9=C[$9$k$,!"=i4|CM$OITDj$G$"$k!#(B
	*/
	EnumValue() {
#ifndef NDEBUG
		m_value = invalid;
#endif
	}
	/**
	* $B=i4|CM(B v $B$G%*%V%8%'%/%H$r9=C[$9$k!#(Bv == invalid $B$G$"$C$F$O$J$i$J$$!#(B
	*/
	EnumValue(E v) {
		ASSERT(v != invalid);
		m_value = v;
	}
	/**
	* *this $B$NJ];}$9$kCM$r(B E $B7?$NCM$GJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	operator E () const {
		ASSERT(valid());
		return (E)m_value;
	}
	/**
	* *this $B$NJ];}$9$kCM$r(B E $B7?$NCM$GJV$9!#(Bthis->valid() $B$G$"$k$3$H!#(B
	*/
	E toPrimitive() const {
		return (E)*this;
	}
	
	/**
	* $BL58zCM$rJ];}$9$k%*%V%8%'%/%H$rF@$k!#(B
	*/
	static EnumValue invalidValue() {
		EnumValue result;
		result.m_value = invalid;
		return result;
	}
	/**
	* *this $B$,L58zCM$rJ];}$7$F$$$k>l9g(B false$B!"$=$&$G$J$$>l9g(B true $B$rJV$9!#(B
	*/
	bool valid() const {
		return m_value != invalid;
	}
	
	/**
	* $BEy$7$$$+$I$&$+$rJV$9!#(B
	* @return
	*		*this $B$H(B x $B$,F1$8CM$rJ];}$7$F$$$k>l9g(B true$B!"(B $B$=$&$G$J$$>l9g(B false 
	*		$B$rJV$9!#FC$K!"N>J}$,L58zCM$N>l9g(B true$B!"JRJ}$@$1$,L58zCM$N>l9g(B     
	*		false $B$rJV$9!#(B
	*/
	bool operator== (EnumValue x) const {
		return m_value == x.m_value;
	}
	/**
	* !(*this == x) $B$rJV$9!#(B
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
