/*
 *	Header:
 *	File: playmode.c
 *	Author: Yusuke Yamauchi
 *	Date: 1999/06/12
 *	EndHeader:
 */

#include <stdio.h>
#include <string.h>

#include "libsclient.h"


Bool estimate_current_pm(HearInfo *hinf, PlayMode *plmode)
{
	char buf[7] ;

	if (hinf->sender != ST_Referee)
		return FALSE ;

	if(!strcmp(hinf->message, "before_kick_off"))
		*plmode = PM_BeforeKickOff ;
	else if(!strcmp(hinf->message, "play_on"))
		*plmode = PM_PlayOn ;
	else if(!strcmp(hinf->message, "time_over"))
		*plmode = PM_TimeOver ;
	else if(!strcmp(hinf->message, "kick_off_l"))
		*plmode = PM_KickOff_L ;
	else if(!strcmp(hinf->message, "kick_off_r"))
		*plmode = PM_KickOff_R ;
	else if(!strcmp(hinf->message, "kick_in_l"))
		*plmode = PM_KickIn_L ;
	else if(!strcmp(hinf->message, "kick_in_r"))
		*plmode = PM_KickIn_R ;
	else if(!strcmp(hinf->message, "free_kick_l"))
		*plmode = PM_FreeKick_L ;
	else if(!strcmp(hinf->message, "free_kick_r"))
		*plmode = PM_FreeKick_R ;
	else if(!strcmp(hinf->message, "corner_kick_l"))
		*plmode = PM_CornerKick_L ;
	else if(!strcmp(hinf->message, "corner_kick_r"))
		*plmode = PM_CornerKick_R ;
	else if(!strcmp(hinf->message, "goal_kick_l"))
		*plmode = PM_GoalKick_L ;
	else if(!strcmp(hinf->message, "goal_kick_r"))
		*plmode = PM_GoalKick_R ;
	else {
		strncpy(buf, hinf->message, 6) ;
		buf[6] = '\0' ;

		if(!strcmp(buf, "goal_l"))
			*plmode = PM_Goal_L ;
		else if(!strcmp(buf, "goal_r"))
			*plmode = PM_Goal_R ;
		else
			return FALSE ;
	}

	return TRUE ;
}

