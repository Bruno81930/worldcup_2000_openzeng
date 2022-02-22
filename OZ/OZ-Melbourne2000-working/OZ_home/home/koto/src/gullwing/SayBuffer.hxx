// SayBuffer.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SAYBUFFER_HXX__INCLUDED) // {
#define SAYBUFFER_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// SayBuffer

class SayBuffer
{
private:
	SayBuffer(const SayBuffer&);
	SayBuffer& operator= (const SayBuffer&);
	bool operator== (const SayBuffer&) const;
	
private:
	int m_charToInt[128];
	std::vector<bool> m_buffer;
	int m_index;
	
protected:
	
public:
	~SayBuffer();
	SayBuffer();
	
	int getBitsSize() const;
	
	bool fromArray(const char* buffer, int time, int key);	// 他人のデータなら false
	bool cliff(int index, int bits = 1) const;
	int getInt(int index, int bits = CHAR_BIT * sizeof(int)) const;
	long getLong(int index, int bits = CHAR_BIT * sizeof(long)) const;
	template<class T> void get(T& data) {
		get(data, CHAR_BIT * sizeof(T));
	}
	template<class T> void get(T& data, int bits) {
		if(cliff(m_index, bits)) {
			data = 0;
		} else {
			data = (T)getLong(m_index, bits);
			m_index += bits;
		}
	}
	
	void clear();
	void push(bool b);
	void push(short i);
	void push(unsigned short i);
	void push(int i);
	void push(unsigned int i);
	void push(long i);
	void push(unsigned long i);
	template<class T> void push(T i, int bits)
	{
		for(; bits; bits--) {
			if(i & 1)
				push(true);
			else
				push(false);
			i = (T)(i >> 1);
		}
	}
	void toArray(char* buffer, int size, int time, int key) const;
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "SayBuffer.inl"
#endif

#endif // } !defined(SAYBUFFER_HXX__INCLUDED)
