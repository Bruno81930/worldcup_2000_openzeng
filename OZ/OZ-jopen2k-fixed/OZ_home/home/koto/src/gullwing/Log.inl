// Log.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// Log

inline Log::Log()
{
	m_fp = 0;

	showinfo_t zero = { 0 };
	m_showinfo = zero;
	m_showinfo.pmode = PM_BeforeKickOff;
	for(int i=0; i<=MAX_PLAYER*2; i++) {
		m_showinfo.pos[i].enable = htons(false);
	}
}
inline bool Log::open(char const* logName)
{
	if(m_fp)
		fclose(m_fp);
	m_fp = fopen(logName, "wb");
	if(!m_fp)
		return false;

	char buf[5];
	buf[0] = 'U';
	buf[1] = 'L';
	buf[2] = 'G';
	buf[3] = REC_VERSION_2;
	fwrite(buf, 4, 1, m_fp);
	fflush(m_fp);
	return true;
}

inline void Log::reset()
{
	for(int i=0; i<=MAX_PLAYER*2; i++) {
		m_showinfo.pos[i].enable = htons(false);
	}
}

inline void Log::set(int time)
{
	m_showinfo.time = htons((short)time);
}
inline void Log::set(int index, Side side, Integer uniformNumber, Vector position, Angle angle, bool kick)
{
	bool goalie = false;
	bool hand = false;

	if(!side.valid()) {
		side = SIDE_RIGHT;
		goalie = true;
	}
	if(!uniformNumber.valid())
		uniformNumber = 0;
	uniformNumber++;
	if(!position.valid())
		position = Vector(-server().PITCH_LENGTH()/2 + index * 1.0, -server().PITCH_WIDTH()/2 - server().PITCH_MARGIN());
	if(!angle.valid())
		angle = Angle(0);
	if(!angle.valid()) {
		angle = Angle(0);
		hand = true;
	}

	const double SHOWINFO_SCALE = 16.0;
	m_showinfo.pos[index].x = htons((short)(position.x() * SHOWINFO_SCALE));
	m_showinfo.pos[index].y = htons((short)(position.y() * SHOWINFO_SCALE));
	m_showinfo.pos[index].angle = htons((short)angle.degrees());
	m_showinfo.pos[index].side = htons((short)(side == SIDE_LEFT ? -1 : +1));
	m_showinfo.pos[index].unum = htons((short)uniformNumber);
	short mode = 0x0001;
	if(goalie)
		mode |= 0x0008;
	if(kick)
		mode |= 0x0002;
	if(hand)
		mode |= 0x0010;
	m_showinfo.pos[index].enable = htons(mode);
}

inline void Log::write()
{
	if(!m_fp)
		return;
	short mode = htons(1);
	fwrite(&mode, sizeof(mode), 1, m_fp);
	fwrite(&m_showinfo, sizeof(m_showinfo), 1, m_fp);
	reset();
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
