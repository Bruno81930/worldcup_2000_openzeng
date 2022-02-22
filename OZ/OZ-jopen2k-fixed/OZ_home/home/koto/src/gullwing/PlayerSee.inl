// PlayerSee.inl
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
// PlayerSee

inline Integer PlayerSeeObject::type() const
{
	return m_type;
}
inline Side PlayerSeeObject::team() const
{
	ASSERT(abs(m_type) == TYPE_PLAYER);
	return m_team;
}
inline Integer PlayerSeeObject::uniformNumber() const
{
	ASSERT(abs(m_type) == TYPE_PLAYER);
	return m_id;
}
inline Flag PlayerSeeObject::flagId() const
{
	ASSERT(abs(m_type) == TYPE_FLAG);
	if(!m_id.valid())
		return Flag::invalidValue();
	int result = m_id;
	ASSERT(0 <= result && result < FLAG_MAZ);
	return (Flag)(Flag_t)result;
}
inline Edge PlayerSeeObject::edge() const
{
	ASSERT(abs(m_type) == TYPE_LINE);
	int result = m_id;
	ASSERT(result == EDGE_TOP || result == EDGE_BOTTOM || result == EDGE_LEFT || result == EDGE_RIGHT);
	return (Edge)result;
}
inline Double PlayerSeeObject::distance() const
{
	return m_distance;
}
inline Angle PlayerSeeObject::direction() const
{
	return m_direction;
}
inline Double PlayerSeeObject::distanceChange() const
{
	return m_distanceChange;
}
inline Double PlayerSeeObject::directionChange() const
{
	return m_directionChange;
}
inline Angle PlayerSeeObject::bodyDirection() const
{
	return m_bodyDirection;
}
inline Angle PlayerSeeObject::faceDirection() const
{
	return m_faceDirection;
}

inline void PlayerSeeObject::initialize(const char* &cursor, const char* teamName, Side teamSide)
{
#if LEARNING
	m_inspirationTeam = Side::invalidValue();
	m_inspirationUniformNumber = -1;
#endif

	char name[256];
	double distance = -DBL_MAX;
	double direction = -DBL_MAX;
	double distanceChange = -DBL_MAX;
	double directionChange = -DBL_MAX;
	double bodyDirection = -DBL_MAX;
	double faceDirection = -DBL_MAX;
	char ch;
	int n = sscanf(cursor, "((%255[^"/*(*/")]) %lg %lg %lg %lg %lg %lg %c"/*)*/,
		name, &distance, &direction, &distanceChange, &directionChange,
		&bodyDirection, &faceDirection, &ch);
	n;
#define D(v) m_##v = (v == -DBL_MAX) ? Double::invalidValue() : Double(v);
#define A(v) m_##v = (v == -DBL_MAX) ? Angle::invalidValue() : Angle(v>0 ? v+0.5 : v<0 ? v-0.5 : v);
	D(distance);
	A(direction);
	D(distanceChange);
	D(directionChange);
	A(bodyDirection);
	A(faceDirection);
#undef A
#undef D
	ASSERT(m_distanceChange.valid() == m_directionChange.valid());
	ASSERT(m_bodyDirection.valid() == m_faceDirection.valid());
#ifndef NDEBUG
	if(!(n >= 2 && (n < 8 || ch == /*(*/')'))) {
		char buf[512];
		strncpy(buf, cursor, 511);
		buf[511] = '\0';
		report("bad input: %s\n", buf);
	}
#endif
	ASSERT(n >= 2);                      
	ASSERT(n < 8 || ch == /*(*/')');
	
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
	
	static const char player[] = "player";
	static const char flag[] = "flag ";
	static const char goal[] = "goal ";
	static const char line[] = "line ";
	const char* p = name;
	m_team = Side::invalidValue();
	m_id = Integer::invalidValue();
	switch(name[0]) {
	case 'b':
		ASSERT(strcmp(name, "ball") == 0);
		m_type = TYPE_BALL;
		break;
	case 'p':
		ASSERT(strncmp(name, player, sizeof(player)-1) == 0);
		p += sizeof(player)-1;
		m_type = TYPE_PLAYER;
		if(*p != /*(*/')') {
			char team[256];
			int uniformNumber;
			int c = -1;
			int n = sscanf(p, " %255s %d%n", team, &uniformNumber, &c);
			ASSERT(n < 2 || c == (int)strlen(p));
			if(n >= 1) {
				if(strcmp(team, teamName) == 0)
					m_team = SIDE_LEFT;
				else
					m_team = SIDE_RIGHT;
			}
			if(n >= 2)
				m_id = uniformNumber - 1;
		}
		break;
	case 'f':
		ASSERT(strncmp(name, flag, sizeof(flag)-1) == 0);
		p += sizeof(flag)-1;
		
		m_type = TYPE_FLAG;
		m_id = flagHelper.nameToFlag(p, teamSide).toPrimitive();
		break;
	case 'g':
		ASSERT(strncmp(name, goal, sizeof(goal)-1) == 0);
		p += sizeof(goal)-1;
		m_type = TYPE_FLAG;
		switch(*p++) {
		case 'l':
			m_id = (teamSide == SIDE_LEFT) ? GOAL_L : GOAL_R;
			break;
		case 'r':
			m_id = (teamSide == SIDE_LEFT) ? GOAL_R : GOAL_L;
			break;
		default:
			ASSERT(false);
		}
		ASSERT(*p == '\0');
		break;
	case 'l':
		ASSERT(strncmp(name, line, sizeof(line)-1) == 0);
		p += sizeof(line)-1;
		m_type = TYPE_LINE;
		switch(*p++) {
		case 'l':
			m_id = (teamSide == SIDE_LEFT) ? EDGE_LEFT : EDGE_RIGHT;
			break;
		case 'r':
			m_id = (teamSide == SIDE_LEFT) ? EDGE_RIGHT : EDGE_LEFT;
			break;
		case 't':
			m_id = (teamSide == SIDE_LEFT) ? EDGE_TOP : EDGE_BOTTOM;
			break;
		case 'b':
			m_id = (teamSide == SIDE_LEFT) ? EDGE_BOTTOM : EDGE_TOP;
			break;
		default:
			ASSERT(false);
		}
		ASSERT(*p == '\0');
		break;
	case 'B':
		ASSERT(strcmp(name, "Ball") == 0);
		m_type = -TYPE_BALL;
		break;
	case 'P':
		ASSERT(strcmp(name, "Player") == 0);
		m_type = -TYPE_PLAYER;
		break;
	case 'F':
		ASSERT(strcmp(name, "Flag") == 0);
		m_type = -TYPE_FLAG;
		break;
	case 'G':
		ASSERT(strcmp(name, "Goal") == 0);
		m_type = -TYPE_FLAG;
		break;
	case 'L':
		ASSERT(strcmp(name, "Line") == 0);
		m_type = -TYPE_LINE;
		break;
	default:
		ASSERT(false);
	}
}


inline int PlayerSee::refereeTimer() const
{
	return m_refereeTimer;
}
inline const PlayerSeeObject* PlayerSee::ball() const
{
	return m_pBall;
}
inline const PlayerSeeObject* const* PlayerSee::players() const
{
	return m_players;
}
inline const PlayerSeeObject* const* PlayerSee::flags() const
{
	return m_flags;
}
inline const PlayerSeeObject* const* PlayerSee::lines() const
{
	return m_lines;
}

inline PlayerSee::PlayerSee()
{
	initialize();
}
inline void PlayerSee::initialize()
{
#if LEARNING
	m_inspiration = 0;
#endif
	m_refereeTimer = 0;
	m_pBall = 0;
	m_players[0] = 0;
	m_flags[0] = 0;
	m_lines[0] = 0;
}
inline void PlayerSee::initialize(const char* body, const char* teamName, Side teamSide)
{
	ASSERT(teamName);
	ASSERT(teamSide == SIDE_LEFT || teamSide == SIDE_RIGHT);
	m_pBall = 0;
	int players = 0;
	int flags = 0;
	int lines  = 0;
	int count = 0;
	
	int refereeTimer;
	int c = -1;
	int n = sscanf(body, "%d %n", &refereeTimer, &c);
	n;	ASSERT(n == 1 && c != -1);
	m_refereeTimer = refereeTimer;
	const char* cursor = body + c;
	while(*cursor != /*(*/')') {
		ASSERT(count < COUNT_OF(m_seeObjects));
		m_seeObjects[count].initialize(cursor, teamName, teamSide);
		PlayerSeeObject* o = &m_seeObjects[count++];
		switch(abs(o->type())) {
		case TYPE_BALL:
			ASSERT(!m_pBall);
			m_pBall = o;
			break;
		case TYPE_PLAYER:
			ASSERT(players < COUNT_OF(m_players));
			m_players[players++] = o;
			break;
		case TYPE_FLAG:
			ASSERT(flags < COUNT_OF(m_flags));
			m_flags[flags++] = o;
			break;
		case TYPE_LINE:
			ASSERT(lines < COUNT_OF(m_lines));
			m_lines[lines++] = o;
			break;
		default:
			ASSERT(false);
		}
	}
	ASSERT(*cursor == /*(*/')');
	cursor++;
#if LEARNING
	m_inspiration = false;
	if(*cursor++ == '('/*)*/) {
		m_inspiration = true;
		char inspiration[] = "inspiration ";
		if(strncmp(cursor, inspiration, sizeof(inspiration)-1) == 0) {
			cursor += sizeof(inspiration)-1;
			int i = 0;
			for(;; i++) {
				while(isspace(*cursor))
					cursor++;
				if(*cursor == /*(*/')')
					break;
				
				static const char player[] = "(player ";
				if(strncmp(cursor, player, sizeof(player)-1) == 0) {
					cursor += sizeof(player)-1;
					char team[256];
					int uniformNumber;
					int n = sscanf(cursor, "%255s %d", team, &uniformNumber);
					ASSERT(n == 2);
					if(strcmp(team, teamName) == 0)
						m_seeObjects[count].m_inspirationTeam = SIDE_LEFT;
					else
						m_seeObjects[count].m_inspirationTeam = SIDE_RIGHT;
					m_seeObjects[count].m_inspirationUniformNumber = uniformNumber-1;
				}
				while(*cursor++ != /*(*/')')
					;
			}
		}
		static const char see[] = ")(see ";
		ASSERT(strncmp(cursor, see, sizeof(see)-1) == 0);
		cursor += sizeof(see)-1;
		m_inspirationSee.initialize(cursor, teamName, teamSide);
	}
#else
	//ASSERT(strcmp(cursor, /*(*/")") == 0);
#endif
	
	ASSERT(players < COUNT_OF(m_players));
	m_players[players] = 0;
	ASSERT(flags < COUNT_OF(m_flags));
	m_flags[flags] = 0;
	ASSERT(lines < COUNT_OF(m_lines));
	m_lines[lines] = 0;
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
