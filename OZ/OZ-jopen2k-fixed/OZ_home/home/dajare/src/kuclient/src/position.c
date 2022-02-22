/*
 *	Header:
 *	File: position.c
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *	Changed by Yusuke Yamauchi (1999/06/12)
 *	EndHeader:
 */

#include <stdio.h>
#include <math.h>

#include "libsclient.h"


Bool estimate_current_pos(SeeInfo *sinf, Side side, PosState *pstate)
{
	int i ;
	int out_of_field = FALSE ;
	double dist = 2.0*PITCH_LENGTH ;
	double line_dir, obj_dir ;
	double tmp ;
	SeenObj *line = (SeenObj *)NULL, *flag = (SeenObj *)NULL ;
	SeenObj *goal = (SeenObj *)NULL, *obj ;
	Pos obj_pos ;
	const struct {
		FlagPos	name ;
		Pos		pos ;
	} flags[] = {
		/* 0 */{FP_L_T, {-PITCH_LENGTH/2.0, -PITCH_WIDTH/2.0}},
		/* 1 */{FP_L_T_30, {-PITCH_LENGTH/2.0 - 5, -30}},
		/* 2 */{FP_L_T_20, {-PITCH_LENGTH/2.0 - 5, -20}},
		/* 3 */{FP_L_T_10, {-PITCH_LENGTH/2.0 - 5, -10}},
		/* 4 */{FP_L_0, {-PITCH_LENGTH/2.0 - 5, 0.0}},
		/* 5 */{FP_L_B_30, {-PITCH_LENGTH/2.0 - 5, 30}},
		/* 6 */{FP_L_B_20, {-PITCH_LENGTH/2.0 - 5, 20}},
		/* 7 */{FP_L_B_10, {-PITCH_LENGTH/2.0 - 5, 10}},
		/* 8 */{FP_L_B, {-PITCH_LENGTH/2.0, PITCH_WIDTH/2.0}},
		/* 9 */{FP_R_T, {PITCH_LENGTH/2.0, -PITCH_WIDTH/2.0}}, 
		/* 10 */{FP_R_T_30, {PITCH_LENGTH/2.0 + 5, -30}},
		/* 11 */{FP_R_T_20, {PITCH_LENGTH/2.0 + 5, -20}},
		/* 12 */{FP_R_T_10, {PITCH_LENGTH/2.0 + 5, -10}},
		/* 13 */{FP_R_0, {PITCH_LENGTH/2.0 + 5, 0.0}},
		/* 14 */{FP_R_B_30, {PITCH_LENGTH/2.0 + 5, 30}},
		/* 15 */{FP_R_B_20, {PITCH_LENGTH/2.0 + 5, 20}},
		/* 16 */{FP_R_B_10, {PITCH_LENGTH/2.0 + 5, 10}},
		/* 17 */{FP_R_B, {PITCH_LENGTH/2.0, PITCH_WIDTH/2.0}}, 
		/* 18 */{FP_C_T, {0.0, -PITCH_WIDTH/2.0}},
		/* 19 */{FP_C, {0.0, 0.0}}, 
		/* 20 */{FP_C_B, {0.0, PITCH_WIDTH/2.0}}, 
		/* 21 */{FP_P_L_T, {-PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, 
					-PENALTY_AREA_WIDTH/2.0}}, 
		/* 22 */{FP_P_L_C, {-PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, 0.0}}, 
		/* 23 */{FP_P_L_B, {-PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, 
					PENALTY_AREA_WIDTH/2.0}}, 
		/* 24 */{FP_P_R_T, {PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, 
					-PENALTY_AREA_WIDTH/2.0}}, 
		/* 25 */{FP_P_R_C, {PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, 0.0}}, 
		/* 26 */{FP_P_R_B, {PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, 
					PENALTY_AREA_WIDTH/2.0}}, 
		/* 27 */{FP_G_L_T, {-PITCH_LENGTH/2.0, -GOAL_WIDTH/2.0}},  
		/* 28 */{FP_G_L_B, {-PITCH_LENGTH/2.0, GOAL_WIDTH/2.0}}, 
		/* 29 */{FP_G_R_T, {PITCH_LENGTH/2.0, -GOAL_WIDTH/2.0}}, 
		/* 30 */{FP_G_R_B, {PITCH_LENGTH/2.0, GOAL_WIDTH/2.0}},
		/* 31 */{FP_T_L_50, {-50, -PITCH_WIDTH/2.0 - 5}},
		/* 32 */{FP_T_L_40, {-40, -PITCH_WIDTH/2.0 - 5}},
		/* 33 */{FP_T_L_30, {-30, -PITCH_WIDTH/2.0 - 5}},
		/* 34 */{FP_T_L_20, {-20, -PITCH_WIDTH/2.0 - 5}},
		/* 35 */{FP_T_L_10, {-10, -PITCH_WIDTH/2.0 - 5}},
		/* 36 */{FP_T_R_50, {50, -PITCH_WIDTH/2.0 - 5}},
		/* 37 */{FP_T_R_40, {40, -PITCH_WIDTH/2.0 - 5}},
		/* 38 */{FP_T_R_30, {30, -PITCH_WIDTH/2.0 - 5}},
		/* 39 */{FP_T_R_20, {20, -PITCH_WIDTH/2.0 - 5}},
		/* 40 */{FP_T_R_10, {10, -PITCH_WIDTH/2.0 - 5}},
		/* 41 */{FP_T_0, {0.0, -PITCH_WIDTH/2.0 - 5}},
		/* 42 */{FP_B_L_50, {-50, PITCH_WIDTH/2.0 + 5}},
		/* 43 */{FP_B_L_40, {-40, PITCH_WIDTH/2.0 + 5}},
		/* 44 */{FP_B_L_30, {-30, PITCH_WIDTH/2.0 + 5}},
		/* 45 */{FP_B_L_20, {-20, PITCH_WIDTH/2.0 + 5}},
		/* 46 */{FP_B_L_10, {-10, PITCH_WIDTH/2.0 + 5}},
		/* 47 */{FP_B_R_50, {50, PITCH_WIDTH/2.0 + 5}},
		/* 48 */{FP_B_R_40, {40, PITCH_WIDTH/2.0 + 5}},
		/* 49 */{FP_B_R_30, {30, PITCH_WIDTH/2.0 + 5}},
		/* 50 */{FP_B_R_20, {20, PITCH_WIDTH/2.0 + 5}},
		/* 51 */{FP_B_R_10, {10, PITCH_WIDTH/2.0 + 5}},
		/* 52 */{FP_B_0, {0.0, PITCH_WIDTH/2.0 + 5}},
		{FP_Unknown, {0.0, 0.0}}
	} ;

	for(i = 0; i < sinf->n_obj; i++) {
		if (sinf->obj[i].type == OT_Flag) {
			if (!sinf->obj[i].inViewConeP)
				continue ;
			if (Abs(dist) > Abs(sinf->obj[i].dist)) {
				flag = &(sinf->obj[i]) ;
				dist = sinf->obj[i].dist ;
			}
		}
		else if (sinf->obj[i].type == OT_Line) {
			line = &(sinf->obj[i]) ;
			if (i != sinf->n_obj - 1 && sinf->obj[i+1].type == OT_Line)
				out_of_field = TRUE ;
			break ;
		}
		else if (sinf->obj[i].type == OT_Goal)
			goal = &(sinf->obj[i]) ;
	}

	if (line == (SeenObj *)NULL)
		return FALSE ;

	if (flag != (SeenObj *)NULL) {
		obj = flag ;
		for(i=0; ; i++) {
			if (flags[i].name == FP_Unknown)
				return FALSE ;
			if (flags[i].name == flag->id.fpos) {
				obj_pos.x = flags[i].pos.x ;
				obj_pos.y = flags[i].pos.y ;
				break ;
			}
		}
	}
	else if (goal != (SeenObj *)NULL) {
		obj = goal ;
		if (goal->id.side == S_LEFT)
			obj_pos.x = -PITCH_LENGTH/2.0 ;
		else if (goal->id.side == S_RIGHT)
			obj_pos.x = PITCH_LENGTH/2.0 ;
		else
			return FALSE ;
		obj_pos.y = 0.0 ;
	}
	else
		return FALSE ;

	line_dir = Deg2Rad(line->dir) ;

	switch (line->id.lpos) {
	case LP_T :
		if (out_of_field)  {
			pstate->dir = -line->dir - ((line->dir > 0) ? -180.0 : 0.0) ;
			pstate->pos.y = -PITCH_WIDTH/2.0 - Abs(line->dist * sin(line_dir)) ;
		}
		else {
			pstate->dir = -line->dir - ((line->dir < 0) ? 180.0 : 0.0) ;
			pstate->pos.y = -PITCH_WIDTH/2.0 + Abs(line->dist * sin(line_dir)) ;
		}

		if ((tmp = Pow(obj->dist) - Pow(obj_pos.y - pstate->pos.y)) < 0.0)
			tmp *= -1.0 ;

		obj_dir = obj->dir + pstate->dir ;
		if (obj_dir > 180.0)
			obj_dir -= 360.0 ;
		else if (obj_dir < -180.0)
			obj_dir += 360.0 ;

		if (Abs(obj_dir) < 90.0)
			pstate->pos.x = obj_pos.x - sqrt(tmp) ;
		else
			pstate->pos.x = obj_pos.x + sqrt(tmp) ;
		break ;

	case LP_B :
		if (out_of_field) {
			pstate->dir = -line->dir - ((line->dir < 0) ? 180.0 : 0.0) ;
			pstate->pos.y = PITCH_WIDTH/2.0 + Abs(line->dist * sin(line_dir)) ;
		}
		else {
			pstate->dir = -line->dir - ((line->dir > 0) ? -180.0 : 0.0) ;
			pstate->pos.y = PITCH_WIDTH/2.0 - Abs(line->dist * sin(line_dir)) ;
		}

		if ((tmp = Pow(obj->dist) - Pow(obj_pos.y - pstate->pos.y)) < 0.0)
			tmp *= -1.0 ;

		obj_dir = obj->dir + pstate->dir ;
		if (obj_dir > 180.0)
			obj_dir -= 360.0 ;
		else if (obj_dir < -180.0)
			obj_dir += 360.0 ;

		if (Abs(obj_dir) < 90.0)
			pstate->pos.x = obj_pos.x - sqrt(tmp) ;
		else
			pstate->pos.x = obj_pos.x + sqrt(tmp) ;
		break ;

	case LP_L :
		if (out_of_field) {
			pstate->dir = -line->dir - ((line->dir > 0) ? -90.0 : 90.0) ;
			pstate->pos.x = -PITCH_LENGTH/2.0 - Abs(line->dist * sin(line_dir)) ;
		}
		else {
			pstate->dir = -line->dir - ((line->dir < 0) ? -90.0 : 90.0) ;
			pstate->pos.x = -PITCH_LENGTH/2.0 + Abs(line->dist * sin(line_dir)) ;
		}

		if ((tmp = Pow(obj->dist) - Pow(obj_pos.x - pstate->pos.x)) < 0.0)
			tmp *= -1.0 ;

		obj_dir = obj->dir + pstate->dir ;
		if (obj_dir > 180.0)
			obj_dir -= 360.0 ;
		else if (obj_dir < -180.0)
			obj_dir += 360.0 ;

		if (obj_dir > 0.0)
			pstate->pos.y = obj_pos.y - sqrt(tmp) ;
		else
			pstate->pos.y = obj_pos.y + sqrt(tmp) ;
		break ;

	case LP_R :
		if (out_of_field) {
			pstate->dir = -line->dir - ((line->dir < 0) ? -90.0 : 90.0) ;
			pstate->pos.x = PITCH_LENGTH/2.0 + Abs(line->dist * sin(line_dir)) ;
		}
		else {
			pstate->dir = -line->dir - ((line->dir > 0) ? -90.0 : 90.0) ;
			pstate->pos.x = PITCH_LENGTH/2.0 - Abs(line->dist * sin(line_dir)) ;
		}

		if ((tmp = Pow(obj->dist) - Pow(obj_pos.x - pstate->pos.x)) < 0.0)
			tmp *= -1.0 ;

		obj_dir = obj->dir + pstate->dir ;
		if (obj_dir > 180.0)
			obj_dir -= 360.0 ;
		else if (obj_dir < -180.0)
			obj_dir += 360.0 ;

		if (obj_dir > 0.0)
			pstate->pos.y = obj_pos.y - sqrt(tmp) ;
		else
			pstate->pos.y = obj_pos.y + sqrt(tmp) ;
		break ;

	case LP_Unknown :
		return FALSE ;
	}

	if (side == S_RIGHT) {
		pstate->dir += ((pstate->dir > 0) ? -180.0 : 180.0) ;
		pstate->pos.x *= -1.0 ;
		pstate->pos.y *= -1.0 ;
	}
		
	return TRUE ;
}

