// PlayerHear.inl
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
// PlayerHear

inline int PlayerHear::refereeTimer() const
{
	return m_refereeTimer;
}
inline SenderType PlayerHear::senderType() const
{
	return m_senderType;
}
inline Angle PlayerHear::direction() const
{
	ASSERT(m_senderType == SENDER_DIRECTION);
	return m_direction;
}
inline int PlayerHear::refereeMessage() const
{
	ASSERT(m_senderType == SENDER_REFEREE);
	return m_refereeMessage;
}
inline int PlayerHear::score() const
{
	ASSERT(m_senderType == SENDER_REFEREE);
	ASSERT(m_refereeMessage == PLAYMODE_GOAL_L || m_refereeMessage == PLAYMODE_GOAL_R);
	return m_extra;
}
inline const char* PlayerHear::message() const
{
	return m_message;
}


inline PlayerHear::PlayerHear()
{
	m_message = new char[server().say_msg_size()];
	initialize();
}
inline PlayerHear::~PlayerHear()
{
	delete[] m_message;
}
inline void PlayerHear::initialize()
{
	m_refereeTimer = 0;
	m_senderType = SENDER_DIRECTION;
	m_direction = Angle::invalidValue();
	m_refereeMessage = -1;
	m_extra = 0;
}
inline void PlayerHear::initialize(const char* body, Side teamSide)
{
	const char* p = body;
	
	// 時刻を得る
	int refereeTimer;
	int c = -1;
	int n = sscanf(body, "%d %n", &refereeTimer, &c);
	n;	ASSERT(n == 1 && c != -1);
	m_refereeTimer = refereeTimer;
	p += c;
	
	// 送信者文字列を得る
	const int senderMaxSize = 256;
	char sender[senderMaxSize+1];
	char* q = sender;
	while(*p != ' ') {
		ASSERT(*p != '\0');
		ASSERT(q < sender + senderMaxSize);
		*q++ = *p++;
	}
	*q = '\0';
	while(*p == ' ')
		p++;
	
	// 送信者文字列を解析する。
	switch(sender[0]) {
	case 'r':
		ASSERT(strcmp(sender, "referee") == 0);
		m_senderType = SENDER_REFEREE;
		m_direction = Angle::invalidValue();
		break;
	case 's':
		ASSERT(strcmp(sender, "self") == 0);
		m_senderType = SENDER_SELF;
		m_direction = Angle::invalidValue();
		break;
	case 'o':
		ASSERT(strlen(sender) > 13);
		switch(sender[13]) {
		case 'l':
			break;
		case 'r':
			break;
		}
		m_senderType = SENDER_COACH;
		m_direction = Angle::invalidValue();
		break;
	default:
		{
			double degrees;
			int c = -1;
			int n = sscanf(sender, "%lg%n", &degrees, &c);
			n;	ASSERT(n == 1 && c == (int)strlen(sender));
			m_senderType = SENDER_DIRECTION;
			if(degrees > 0.0)
				degrees += 0.5;
			else if(degrees < 0.0)
				degrees -= 0.5;
			m_direction = Angle(degrees);
		}
		break;
	}
	
	// メッセージを得る
	strcpy(m_message, p);
	q = m_message + strlen(m_message) - 1;
	ASSERT(*q == /*(*/')');
	*q = '\0';
	
	// メッセージを解析する
	if(m_senderType == SENDER_REFEREE) {
		m_refereeMessage = refreeMessageHelper.toRefreeMessage(m_message, teamSide);
		if(m_refereeMessage == REFREEMESSAGE_COACH) {
			const char goalL[] = "goal_l_";
			const char goalR[] = "goal_r_";
			if(strncmp(m_message, goalL, sizeof(goalL)-1) == 0) {
				m_refereeMessage = (teamSide == SIDE_LEFT) ? PLAYMODE_GOAL_L : PLAYMODE_GOAL_R;
				m_extra = atoi(m_message + sizeof(goalL)-1);
			} else if(strncmp(m_message, goalR, sizeof(goalR)-1) == 0) {
				m_refereeMessage = (teamSide == SIDE_LEFT) ? PLAYMODE_GOAL_R : PLAYMODE_GOAL_L;
				m_extra = atoi(m_message + sizeof(goalR)-1);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
