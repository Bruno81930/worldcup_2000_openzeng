// basic.inl
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
// PlayModeHelper

inline const char* PlayModeHelper::toString(PlayMode playMode)
{
	ASSERT(0 <= playMode && playMode < COUNT_OF(m_playmodeStrings));
	return m_playmodeStrings[playMode];
}
inline int PlayModeHelper::toPlayMode(const char* string, Side teamSide)
{
	ASSERT(string);
	ASSERT(teamSide == SIDE_LEFT || teamSide == SIDE_RIGHT);
	for(int i=0; i<PLAYMODE_MAZ; i++) {
		if(strcmp(m_playmodeStrings[i], string) == 0) {
			ASSERT(0 <= i && i < PLAYMODE_MAZ);
			if(teamSide == SIDE_RIGHT && i <= PLAYMODE_LR_MAX)
				i ^= 1;
			return i;
		}
	}
	return -1;
}
inline bool PlayModeHelper::isPlayModeChanged(PlayMode prev, PlayMode next)
{
	if(prev == PLAYMODE_GOALIE_CATCH_L && next == PLAYMODE_FREE_KICK_L)
		return false;
	else if(prev == PLAYMODE_GOALIE_CATCH_R && next == PLAYMODE_FREE_KICK_R)
		return false;
	return true;
}
inline bool PlayModeHelper::isRefreeTimerProceeding(PlayMode playMode)
{
	switch(playMode) {
	default:
		ASSERT(false);
	case PLAYMODE_BEFORE_KICK_OFF:
	case PLAYMODE_GOAL_L:
	case PLAYMODE_GOAL_R:
	case PLAYMODE_OFFSIDE_L:
	case PLAYMODE_OFFSIDE_R:
	case PLAYMODE_TIME_OVER:
		return false;
	case PLAYMODE_PLAY_ON:
	case PLAYMODE_KICK_OFF_L:
	case PLAYMODE_KICK_OFF_R:
	case PLAYMODE_KICK_IN_L:
	case PLAYMODE_KICK_IN_R:
	case PLAYMODE_FREE_KICK_L:
	case PLAYMODE_FREE_KICK_R:
	case PLAYMODE_CORNER_KICK_L:
	case PLAYMODE_CORNER_KICK_R:
	case PLAYMODE_GOAL_KICK_L:
	case PLAYMODE_GOAL_KICK_R:
	case PLAYMODE_GOALIE_CATCH_L:
	case PLAYMODE_GOALIE_CATCH_R:
	case PLAYMODE_DROP_BALL:
		return true;
	}
}

/////////////////////////////////////////////////////////////////////////////
// RefreeMessageHelper

inline int RefreeMessageHelper::toRefreeMessage(const char* string, Side teamSide)
{
	int result = playModeHelper.toPlayMode(string, teamSide);
	if(result >= 0)
		return result;

	const char* const* pp = m_refreeMessageStrings;
	int index = 0;
	for(; *pp; pp++, index++) {
		if(strcmp(string, *pp) == 0)
			return index + REFREEMESSAGE_TIME_UP_WITHOUT_A_TEAM;
	}
	
	index = 0;
	for(pp++; *pp; pp++, index++) {
		if(strcmp(string, *pp) == 0) {
			int result = index + REFREEMESSAGE_FAUL_L;
			if(teamSide == SIDE_RIGHT)
				result ^= 1;
			return result;
		}
	}

	return REFREEMESSAGE_COACH;
}

/////////////////////////////////////////////////////////////////////////////
// FlagHelper

inline const Vector& FlagHelper::flagToPosition(int flag)
{
	ASSERT(0 <= flag && flag < FLAG_MAZ && FLAG_MAZ == COUNT_OF(m_flagToPosition));
	ASSERT(m_initialized);
	return m_flagToPosition[flag];
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
