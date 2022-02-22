// SayBuffer.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif
#include "Server.hxx"

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// SayBuffer

inline SayBuffer::~SayBuffer()
{
}

inline SayBuffer::SayBuffer()
{
	int i;
	for(i=0; i<COUNT_OF(m_charToInt); i++)
		m_charToInt[i] = -1;
	const char* p = server().SAY_MESSAGE_LETTERS();
	for(i=0; *p; p++, i++) {
		ASSERT(0 < *p && *p < COUNT_OF(m_charToInt));
		m_charToInt[*p] = i;
	}
	m_index = 0;
}

inline int SayBuffer::getBitsSize() const
{
	return m_buffer.size();
}
inline bool SayBuffer::cliff(int index, int bits) const
{
	ASSERT(index >= 0 && bits >= 0);
	return index + bits > (int)m_buffer.size();
}
inline int SayBuffer::getInt(int index, int bits) const
{
	ASSERT(index >= 0 && bits >= 0 && bits <= CHAR_BIT * sizeof(int));
	int result = 0;
	int b = 1;
	bits = min(bits, (int)m_buffer.size() - index);
	for(; bits>0; bits--) {
		result |= (m_buffer[index++] ? b : 0);
		b = b << 1;
	}
	return result;
}
inline long SayBuffer::getLong(int index, int bits) const
{
	ASSERT(index >= 0 && bits >= 0 && bits <= CHAR_BIT * sizeof(long));
	long result = 0;
	long b = 1;
	bits = min(bits, (int)m_buffer.size() - index);
	for(; bits>0; bits--) {
		result |= (m_buffer[index++] ? b : 0);
		b = b << 1;
	}
	return result;
}

inline void SayBuffer::clear()
{
	m_buffer.clear();
}

inline void SayBuffer::push(bool b)
{
	m_buffer.push_back(b);
}
inline void SayBuffer::push(short i)
{
	int size = CHAR_BIT * sizeof(i);
	push(i, size);
}
inline void SayBuffer::push(unsigned short i)
{
	int size = CHAR_BIT * sizeof(i);
	push(i, size);
}
inline void SayBuffer::push(int i)
{
	int size = CHAR_BIT * sizeof(i);
	push(i, size);
}
inline void SayBuffer::push(unsigned int i)
{
	int size = CHAR_BIT * sizeof(i);
	push(i, size);
}
inline void SayBuffer::push(long i)
{
	int size = CHAR_BIT * sizeof(i);
	push(i, size);
}
inline void SayBuffer::push(unsigned long i)
{
	int size = CHAR_BIT * sizeof(i);
	push(i, size);
}

inline bool SayBuffer::fromArray(const char* buffer, int time, int key)
{
	int size = strlen(buffer);
	int bits = server().sayBits();
	int mask = ~(~0 << bits);
	
	m_index = 0;

	// チェックサムをチェックする
	int checkSum = 0;
	if(size < 5)
		return false;

	int i;
	for(i=0; i<size-5; i++)
		checkSum += buffer[i];

	if(((checkSum + key) & mask) != m_charToInt[buffer[i++]])
		return false;
	if(((checkSum ^ key) & mask) != m_charToInt[buffer[i++]])
		return false;

	// 時刻をチェックする
	int t = m_charToInt[buffer[i+2]];
	t = (t << bits) | m_charToInt[buffer[i+1]];
	t = (t << bits) | m_charToInt[buffer[i]];
	if(t != time)
		return false;

	// データ本体
	clear();
	for(i=0; i<size-5; i++) {
		int value = m_charToInt[buffer[i]];
		push(value, bits);
	}
	
	return true;
}
inline void SayBuffer::toArray(char* buffer, int size, int time, int key) const
{
	size--;
	int index = 0;
	int bits = server().sayBits();
	int mask = ~(~0 << bits);

	int checkSum = 0;
	
	// データを出力する
	int i;
	for(i=0; i<getBitsSize(); i+=bits) {
		ASSERT(index < size);
		int value = getInt(i, bits);
		checkSum += buffer[index++] = server().SAY_MESSAGE_LETTERS()[value];
	}
	
	// チェックサムを出力する
	buffer[index++] = server().SAY_MESSAGE_LETTERS()[(checkSum + key) & mask];
	buffer[index++] = server().SAY_MESSAGE_LETTERS()[(checkSum ^ key) & mask];

	// 時刻を出力する
	for(i=0; i<3; i++) {
		buffer[index++] = server().SAY_MESSAGE_LETTERS()[time & mask];
		time >>= bits;
	}

	ASSERT(index < size);
	buffer[index] = '\0';
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
