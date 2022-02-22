// PlayerSenseBody.inl
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
// PlayerSenseBody

inline int PlayerSenseBody::refereeTimer() const
{
	return m_refereeTimer;
}
inline ViewQuality PlayerSenseBody::viewQuality() const
{
	return m_viewQuality;
}
inline ViewWidth PlayerSenseBody::viewWidth() const
{
	return m_viewWidth;
}
inline double PlayerSenseBody::stamina() const
{
	return m_stamina;
}
inline double PlayerSenseBody::effort() const
{
	return m_effort;
}
inline double PlayerSenseBody::speed() const
{
	return m_speed;
}
inline Angle PlayerSenseBody::neckAngle() const
{
	ASSERT(m_neckAngle.valid());
	return m_neckAngle;
}
inline int PlayerSenseBody::kickCount() const
{
	return m_kickCount;
}
inline int PlayerSenseBody::dashCount() const
{
	return m_dashCount;
}
inline int PlayerSenseBody::turnCount() const
{
	return m_turnCount;
}
inline int PlayerSenseBody::sayCount() const
{
	return m_sayCount;
}
inline int PlayerSenseBody::turnNeckCount() const
{
	return m_turnNeckCount;
}

inline PlayerSenseBody::PlayerSenseBody()
{
	initialize();
}
inline void PlayerSenseBody::initialize()
{
#if LEARNING
	m_inspiration = false;
#endif
	m_refereeTimer = 0;
	m_viewQuality = VIEW_QUALITY_HIGH;
	m_viewWidth = VIEW_WIDTH_NORMAL;
	m_stamina = server().stamina_max();
	m_effort = 1.0;
	m_speed = 0.0;
	m_neckAngle = Angle(0);
	m_kickCount = 0;
	m_dashCount = 0;
	m_turnCount = 0;
	m_sayCount = 0;
	m_turnNeckCount = 0;
}

inline void PlayerSenseBody::initialize(const char* body, const char* teamName, Side teamSide)
{
	char quality[21], width[21];
	int length;
	double neckAngle;
	int n = sscanf(body, /*(*/"%d (view_mode %20s %20[^"/*(*/")])"
		" (stamina %lg %lg) (speed %lg) (head_angle %lg)"
		" (kick %d) (dash %d) (turn %d) (say %d) (turn_neck %d))"
		"%n",
		&m_refereeTimer, quality, width,
		&m_stamina, &m_effort, &m_speed, &neckAngle,
		&m_kickCount, &m_dashCount, &m_turnCount, &m_sayCount, &m_turnNeckCount,
		&length);
	if(neckAngle > 0.0)
		neckAngle += 0.5;
	else if(neckAngle < 0.0)
		neckAngle -= 0.5;
	m_neckAngle = Angle(neckAngle);
	n;	ASSERT(n == 12);
	switch(quality[0]) {
	case 'h':
		ASSERT(strcmp(quality, "high") == 0);
		m_viewQuality = VIEW_QUALITY_HIGH;
		break;
	case 'l':
		ASSERT(strcmp(quality, "low") == 0);
		m_viewQuality = VIEW_QUALITY_LOW;
		break;
	default:
		ASSERT(false);
	}
	ASSERT(strlen(width) >= 1);
	switch(width[1]) {
	case 'a':
		ASSERT(strcmp(width, "narrow") == 0);
		m_viewWidth = VIEW_WIDTH_NARROW;
		break;
	case 'o':
		ASSERT(strcmp(width, "normal") == 0);
		m_viewWidth = VIEW_WIDTH_NORMAL;
		break;
	case 'i':
		ASSERT(strcmp(width, "wide") == 0);
		m_viewWidth = VIEW_WIDTH_WIDE;
		break;
	default:
		ASSERT(false);
		m_viewWidth = VIEW_WIDTH_NORMAL;
	}

#if LEARNING
	m_inspiration = false;
	const char* p = &body[length];
	if(*p == '\0')
		return;
	static const char see[] = "(see ";
	ASSERT(strncmp(p, see, sizeof(see)-1) == 0);
	p += sizeof(see)-1;
	m_inspiration = true;
	m_inspirationSee.initialize(p, teamName, teamSide);
#else
	ASSERT(length == (int)strlen(body) || body[length] == '('/*)*/);
#endif

}

/////////////////////////////////////////////////////////////////////////////
#undef inline
