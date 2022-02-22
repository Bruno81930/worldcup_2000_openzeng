// CoachSee.inl
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
// CoachSee

inline bool CoachSeeObject::valid() const
{
	return m_valid;
}
inline ObjectType CoachSeeObject::type() const
{
	ASSERT(m_valid);
	return m_type;
}
inline Side CoachSeeObject::team() const
{
	ASSERT(m_valid);
	ASSERT(m_team.valid());
	return m_team;
}
inline int CoachSeeObject::uniformNumber() const
{
	ASSERT(m_valid);
	ASSERT(m_uniformNumber.valid());
	return m_uniformNumber;
}
inline Vector CoachSeeObject::position() const
{
	ASSERT(m_valid);
	return m_position;
}
inline Vector CoachSeeObject::velocity() const
{
	ASSERT(m_valid);
	return m_velocity;
}
inline Angle CoachSeeObject::bodyDirection() const
{
	ASSERT(m_valid);
	return m_bodyDirection;
}
inline Angle CoachSeeObject::neckAngle() const
{
	ASSERT(m_valid);
	return m_neckAngle;
}

inline CoachSeeObject::CoachSeeObject()
{
	m_valid = false;
}
inline void CoachSeeObject::initialize()
{
	m_valid = false;
}
inline void CoachSeeObject::initialize(const char* &cursor, const char* teamName, Side teamSide)
{
	m_valid = true;

	char name[256];
	double p1 = -DBL_MAX;
	double p2 = -DBL_MAX;
	double p3 = -DBL_MAX;
	double p4 = -DBL_MAX;
	double p5 = -DBL_MAX;
	double p6 = -DBL_MAX;
	char ch;
	int n = sscanf(cursor, "((%255[^"/*(*/")]) %lg %lg %lg %lg %lg %lg %c"/*)*/,
		name, &p1, &p2, &p3, &p4, &p5, &p6, &ch);
	n;
	ASSERT(p1 != -DBL_MAX && p2 != -DBL_MAX);
	if(teamSide == SIDE_LEFT)
		m_position = Vector(p1, p2);
	else
		m_position = Vector(-p1, -p2);
	if(p3 == -DBL_MAX) {
		m_velocity = Vector::invalidValue();
		m_bodyDirection = Angle::invalidValue();
		m_neckAngle = Angle::invalidValue();
	} else if(p5 == -DBL_MAX) {
		ASSERT(p4 != -DBL_MAX);
		if(teamSide == SIDE_LEFT)
			m_velocity = Vector(p3, p4);
		else
			m_velocity = Vector(-p3, -p4);
	} else {
		ASSERT(p6 != -DBL_MAX);
		if(p3 > 0.0)
			p3 += 0.5;
		else if(p3 < 0.0)
			p3 -= 0.5;
		if(teamSide == SIDE_LEFT) {
			m_velocity = Vector(p5, p6);
			m_bodyDirection = Angle(p3);
			m_neckAngle = Angle(p4);
		} else {
			m_velocity = Vector(-p5, -p6);
			m_bodyDirection = Angle(p3 + 180);
			m_neckAngle = Angle(p4 + 180);
		}
	}

	int count = 0;
	for(;;) {
		ASSERT(*cursor != '\0');
		if(*cursor++ == /*(*/')') {
			if(++count >= 2)
				break;
		}
	}
	while(isspace(*cursor))
		cursor++;
	
	static const char player[] = "player ";
	static const char goal[] = "goal ";
	const char* p = name;
	m_team = Side::invalidValue();
	m_uniformNumber = Integer::invalidValue();
	int c, uniformNumber;
	switch(name[0]) {
	case 'b':
		ASSERT(strcmp(name, "ball") == 0);
		m_type = TYPE_BALL;
		break;
	case 'p':
		ASSERT(strncmp(name, player, sizeof(player)-1) == 0);
		p += sizeof(player)-1;
		m_type = TYPE_PLAYER;
		char team[256];
		c = -1;
		n = sscanf(p, "%255s %d%n", team, &uniformNumber, &c);
		ASSERT(n == 2 && c == (int)strlen(p));
		if(strcmp(team, teamName) == 0)
			m_team = SIDE_LEFT;
		else
			m_team = SIDE_RIGHT;
		m_uniformNumber = uniformNumber - 1;
		break;
	case 'g':
		ASSERT(strncmp(name, goal, sizeof(goal)-1) == 0);
		p += sizeof(goal)-1;
		m_type = TYPE_FLAG;
		/*switch(*p++) {
		case 'l':
			m_id = (teamSide == SIDE_LEFT) ? GOAL_L : GOAL_R;
			break;
		case 'r':
			m_id = (teamSide == SIDE_LEFT) ? GOAL_R : GOAL_L;
			break;
		default:
			ASSERT(false);
		}
		ASSERT(*p == '\0');*/
		break;
	default:
		ASSERT(false);
	}
}


inline const CoachSeeObject& CoachSee::ball() const
{
	return m_ball;
}
inline const CoachSeeObject& CoachSee::player(Side team, int uniformNumber) const
{
	ASSERT(team == SIDE_LEFT ||team == SIDE_RIGHT);
	ASSERT(0 <= uniformNumber && uniformNumber < MAX_PLAYER);
	int index = (team == SIDE_LEFT) ? 0 : 1;
	return m_players[index][uniformNumber];
}

inline CoachSee::CoachSee()
{
	m_refreeTimer = -1;
}

inline void CoachSee::initialize(const char* body, const char* teamName, Side teamSide)
{
	ASSERT(teamName);
	ASSERT(teamSide == SIDE_LEFT || teamSide == SIDE_RIGHT);

	int refereeTimer;
	int c = -1;
	int n = sscanf(body, "%d %n", &refereeTimer, &c);
	n;	ASSERT(n == 1 && c != -1);

	m_refreeTimer = refereeTimer;
	for(int i=0; i<COUNT_OF(m_players); i++) {
		for(int j=0; j<COUNT_OF(m_players[i]); j++)
			m_players[i][j].initialize();
	}
	const char* cursor = body + c;
	for(int count = 0; *cursor != /*(*/')'; count++) {
		switch(count) {
		case 0:
		case 1:
			m_goals[count].initialize(cursor, teamName, teamSide);
			break;
		case 2:
			m_ball.initialize(cursor, teamName, teamSide);
			break;
		default:
			{
				CoachSeeObject tmp;
				tmp.initialize(cursor, teamName, teamSide);
				int index = (tmp.team() == SIDE_LEFT) ? 0 : 1;
				m_players[index][tmp.uniformNumber()] = tmp;
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
