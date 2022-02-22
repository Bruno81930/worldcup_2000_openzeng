/*
 *	Header:
 *	File: netif.c
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *	Changed by Yusuke Yamauchi (1999/06/02)
 *	EndHeader:
 */

#include <stdio.h>
#include <stdlib.h>

#include "libsclient.h"

/*** functions in netif.c ***/
extern Bool    send_message(Socket sock, char *buf) ;
extern Bool    receive_message(Socket *sock, char *buf, int size) ;


Bool receive_info(Socket* socket, char* buffer, int size)
{
	int ret = FALSE ;
	char save = '\0';

	while(TRUE) {
		if (receive_message(socket, buffer, size)) {
			ret = TRUE ;
			save = buffer[0] ;
		}
		else
			break ;
	}

	buffer[0] = save ;
	return ret ;
}


Bool scan_info(char *buffer, SensorInfo *sinfo, char *teamname)
{
	Bool scan_visual_info(char *buffer, SeeInfo *sinfo, char *teamname) ;
	Bool scan_auditorial_info(char *buffer, HearInfo *hinfo) ;
	char com[16] ;

	sscanf(buffer, "(%s ", com) ;

	if (!strcmp(com, "see")) {
		sinfo->type = SIT_See ;
		return scan_visual_info(buffer, &(sinfo->info.see), teamname) ;
	}
	else if (!strcmp(com, "hear")) {
		sinfo->type = SIT_Hear ;
		return scan_auditorial_info(buffer, &(sinfo->info.hear)) ;
	}
	else if (!strcmp(com, "sense_body")) {
	    sinfo->type = SIT_Body ;
	    return scan_body_info(buffer, &(sinfo->info.body)) ;
	}
	else
		return FALSE ;

	return TRUE ;
}


static char *next_token(char *buf)
{
	int i = 0 ;

	while (TRUE) {
		if (*buf == '(')
			i++ ;
		else if (*buf == ')') {
			if (i == 0) {
				*buf = '\0' ;
				return NULL ;
			}
			i-- ;
		}
		else if (*buf == ' ' && i == 0)
			break ;
		else if (*buf == '\0')
			return NULL ;
		buf++ ;
	}

	*buf = '\0' ;
	buf++ ;

	return buf ;
}


Bool scan_visual_info(char *buffer, SeeInfo *sinfo, char *teamname)
{
	static char *next_token(char *buf) ;
	char *next, *obj, *param, *tmp ;
	int num = 0, i ;
	char fp[4] ;

	sscanf(buffer, "(see %d ", &(sinfo->time)) ;
	buffer = next_token(buffer + 1) ;
	buffer = next_token(buffer) ;

	while(buffer != NULL) {
		next = next_token(buffer++) ;
		param = next_token(buffer++) ;

		if (param != NULL) {
			tmp = param ;
			param = next_token(tmp) ;
			sinfo->obj[num].dist = atof(tmp);
		}
		else
			sinfo->obj[num].dist = DIST_ERR ;

		if (param != NULL) {
			tmp = param ;
			param = next_token(tmp) ;
			sinfo->obj[num].dir = atof(tmp) ;
		}
		else
			sinfo->obj[num].dir = DIR_ERR ;

		if (param != NULL) {
			tmp = param ;
			param = next_token(tmp) ;
			sinfo->obj[num].dist_chng = atof(tmp) ;
		}
		else
			sinfo->obj[num].dist_chng = CHNG_ERR ;

		if (param != NULL) {
			tmp = param ;
			param = next_token(tmp) ;
			sinfo->obj[num].dir_chng = atof(tmp) ;
		}
		else
			sinfo->obj[num].dir_chng = CHNG_ERR ;

		if (param != NULL) {
			tmp = param ;
			param = next_token(tmp) ;
			sinfo->obj[num].face_dir = atof(tmp) ;
		}
		else
			sinfo->obj[num].face_dir = DIR_ERR ;

		obj = next_token(buffer) ;

		if (!strcmp(buffer, "ball")) { 
			sinfo->obj[num].type = OT_Ball ;
			sinfo->obj[num].inViewConeP = TRUE ;
		}
		else if(!strcmp(buffer, "Ball")) {
			sinfo->obj[num].type = OT_Ball ;
			sinfo->obj[num].inViewConeP = FALSE ;
		}
		else if(!strcmp(buffer, "player")) {
			sinfo->obj[num].type = OT_Player ;
			sinfo->obj[num].inViewConeP = TRUE ;
			sinfo->obj[num].id.player.team = WT_Unknown ;
			sinfo->obj[num].id.player.unum = UNUM_ERR ;
			if (obj != NULL) {
				buffer = obj ;
				obj = next_token(buffer) ;
				if (!strcmp(buffer, teamname))
					sinfo->obj[num].id.player.team = WT_Our ;
				else
					sinfo->obj[num].id.player.team = WT_Opponent ;
				
				if (obj != NULL) {
					(void)next_token(obj) ;
					sinfo->obj[num].id.player.unum = atoi(obj) ;
				}
			}
		}
		else if(!strcmp(buffer, "Player")) {
			sinfo->obj[num].type = OT_Player ;
			sinfo->obj[num].inViewConeP = FALSE ;
			sinfo->obj[num].id.player.team = WT_Unknown ;
			sinfo->obj[num].id.player.unum = UNUM_ERR ;
		}
		else if(!strcmp(buffer, "goal")) {
			sinfo->obj[num].type = OT_Goal ;
			sinfo->obj[num].inViewConeP = TRUE ;
			sinfo->obj[num].id.side = S_UNKNOWN ;
			if (obj != NULL) {
				buffer = obj ;
				obj = next_token(buffer) ;
				if (*buffer == 'l')
					sinfo->obj[num].id.side = S_LEFT ;
				else if (*buffer == 'r')
					sinfo->obj[num].id.side = S_RIGHT ;
			}
		}
		else if(!strcmp(buffer, "Goal")) {
			sinfo->obj[num].type = OT_Goal ;
			sinfo->obj[num].inViewConeP = FALSE ;
			sinfo->obj[num].id.side = S_UNKNOWN ;
		}
		else if(!strcmp(buffer, "flag")) {
			sinfo->obj[num].type = OT_Flag ;
			sinfo->obj[num].inViewConeP = TRUE ;
			sinfo->obj[num].id.fpos = FP_Unknown ;
			for (i=0 ; obj != NULL && i < 3 ; i++) {
				buffer = obj ;
				obj = next_token(buffer) ;
				fp[i] = *buffer ;
			}
			fp[i] = '\0' ;
			if(!strcmp(fp, "tl5"))
				sinfo->obj[num].id.fpos = FP_T_L_50 ;
			else if(!strcmp(fp, "tl4"))
				sinfo->obj[num].id.fpos = FP_T_L_40 ;
			else if(!strcmp(fp, "tl3"))
				sinfo->obj[num].id.fpos = FP_T_L_30 ;
			else if(!strcmp(fp, "tl2"))
				sinfo->obj[num].id.fpos = FP_T_L_20 ;
			else if(!strcmp(fp, "tl1"))
				sinfo->obj[num].id.fpos = FP_T_L_10 ;
			else if(!strcmp(fp, "t0"))
				sinfo->obj[num].id.fpos = FP_T_0 ;
			else if(!strcmp(fp, "tr1"))
				sinfo->obj[num].id.fpos = FP_T_R_10 ;
			else if(!strcmp(fp, "tr2"))
				sinfo->obj[num].id.fpos = FP_T_R_20 ;
			else if(!strcmp(fp, "tr3"))
				sinfo->obj[num].id.fpos = FP_T_R_30 ;
			else if(!strcmp(fp, "tr4"))
				sinfo->obj[num].id.fpos = FP_T_R_40 ;
			else if(!strcmp(fp, "tr5"))
				sinfo->obj[num].id.fpos = FP_T_R_50 ;
			else if(!strcmp(fp, "lt"))
				sinfo->obj[num].id.fpos = FP_L_T ;
			else if(!strcmp(fp, "ct"))
				sinfo->obj[num].id.fpos = FP_C_T ;
			else if(!strcmp(fp, "rt"))
				sinfo->obj[num].id.fpos = FP_R_T ;
			else if(!strcmp(fp, "lt3"))
				sinfo->obj[num].id.fpos = FP_L_T_30 ;
			else if(!strcmp(fp, "rt3"))
				sinfo->obj[num].id.fpos = FP_R_T_30 ;
			else if(!strcmp(fp, "lt2"))
				sinfo->obj[num].id.fpos = FP_L_T_20 ;
			else if(!strcmp(fp, "rt2"))
				sinfo->obj[num].id.fpos = FP_R_T_20 ;
			else if(!strcmp(fp, "plt"))
				sinfo->obj[num].id.fpos = FP_P_L_T ;
			else if(!strcmp(fp, "prt"))
				sinfo->obj[num].id.fpos = FP_P_R_T ;
			else if(!strcmp(fp, "lt1"))
				sinfo->obj[num].id.fpos = FP_L_T_10 ;
			else if(!strcmp(fp, "rt1"))
				sinfo->obj[num].id.fpos = FP_R_T_10 ;
			else if(!strcmp(fp, "glt"))
				sinfo->obj[num].id.fpos = FP_G_L_T ;
			else if(!strcmp(fp, "grt"))
				sinfo->obj[num].id.fpos = FP_G_R_T ;
			else if(!strcmp(fp, "l0"))
				sinfo->obj[num].id.fpos = FP_L_0 ;
			else if(!strcmp(fp, "plc"))
				sinfo->obj[num].id.fpos = FP_P_L_C ;
			else if(!strcmp(fp, "c"))
				sinfo->obj[num].id.fpos = FP_C ;
			else if(!strcmp(fp, "prc"))
				sinfo->obj[num].id.fpos = FP_P_R_C ;
			else if(!strcmp(fp, "r0"))
				sinfo->obj[num].id.fpos = FP_R_0 ;
			else if(!strcmp(fp, "glb"))
				sinfo->obj[num].id.fpos = FP_G_L_B ;
			else if(!strcmp(fp, "grb"))
				sinfo->obj[num].id.fpos = FP_G_R_B ;
			else if(!strcmp(fp, "lb1"))
				sinfo->obj[num].id.fpos = FP_L_B_10 ;
			else if(!strcmp(fp, "rb1"))
				sinfo->obj[num].id.fpos = FP_R_B_10 ;
			else if(!strcmp(fp, "plb"))
				sinfo->obj[num].id.fpos = FP_P_L_B ;
			else if(!strcmp(fp, "prb"))
				sinfo->obj[num].id.fpos = FP_P_R_B ;
			else if(!strcmp(fp, "lb2"))
				sinfo->obj[num].id.fpos = FP_L_B_20 ;
			else if(!strcmp(fp, "rb2"))
				sinfo->obj[num].id.fpos = FP_R_B_20 ;
			else if(!strcmp(fp, "lb3"))
				sinfo->obj[num].id.fpos = FP_L_B_30 ;
			else if(!strcmp(fp, "rb3"))
				sinfo->obj[num].id.fpos = FP_R_B_30 ;
			else if(!strcmp(fp, "lb"))
				sinfo->obj[num].id.fpos = FP_L_B ;
			else if(!strcmp(fp, "cb"))
				sinfo->obj[num].id.fpos = FP_C_B ;
			else if(!strcmp(fp, "rb"))
				sinfo->obj[num].id.fpos = FP_R_B ;
			else if(!strcmp(fp, "bl5"))
				sinfo->obj[num].id.fpos = FP_B_L_50 ;
			else if(!strcmp(fp, "bl4"))
				sinfo->obj[num].id.fpos = FP_B_L_40 ;
			else if(!strcmp(fp, "bl3"))
				sinfo->obj[num].id.fpos = FP_B_L_30 ;
			else if(!strcmp(fp, "bl2"))
				sinfo->obj[num].id.fpos = FP_B_L_20 ;
			else if(!strcmp(fp, "bl1"))
				sinfo->obj[num].id.fpos = FP_B_L_10 ;
			else if(!strcmp(fp, "b0"))
				sinfo->obj[num].id.fpos = FP_B_0 ;
			else if(!strcmp(fp, "br1"))
				sinfo->obj[num].id.fpos = FP_B_R_10 ;
			else if(!strcmp(fp, "br2"))
				sinfo->obj[num].id.fpos = FP_B_R_20 ;
			else if(!strcmp(fp, "br3"))
				sinfo->obj[num].id.fpos = FP_B_R_30 ;
			else if(!strcmp(fp, "br4"))
				sinfo->obj[num].id.fpos = FP_B_R_40 ;
			else if(!strcmp(fp, "br5"))
				sinfo->obj[num].id.fpos = FP_B_R_50 ;
			else {
				printf("scan_visual_info: miss obj\n") ;
				return FALSE ;
			}
		}
		else if(!strcmp(buffer, "Flag")) {
			sinfo->obj[num].type = OT_Flag ;
			sinfo->obj[num].inViewConeP = FALSE ;
			sinfo->obj[num].id.fpos = FP_Unknown ;
		}
		else if(!strcmp(buffer, "line")) {
			sinfo->obj[num].type = OT_Line ;
			sinfo->obj[num].inViewConeP = TRUE ;
			sinfo->obj[num].id.lpos = LP_Unknown ;
			if (obj != NULL) {
				buffer = obj ;
				obj = next_token(buffer) ;
				if (*buffer == 'l')
					sinfo->obj[num].id.lpos = LP_L ;
				else if (*buffer == 't')
					sinfo->obj[num].id.lpos = LP_T ;
				else if (*buffer == 'b')
					sinfo->obj[num].id.lpos = LP_B ;
				else if (*buffer == 'r')
					sinfo->obj[num].id.lpos = LP_R ;
			}
		}
		else if(!strcmp(buffer, "Line")) {
			sinfo->obj[num].type = OT_Line ;
			sinfo->obj[num].inViewConeP = FALSE ;
			sinfo->obj[num].id.lpos = LP_Unknown ;
		}
		else {
			printf("scan_visual_info: miss obj\n") ;
			return FALSE ;
		}

		buffer = next ;
		num++ ;
	}
	sinfo->n_obj = num ;

	return TRUE ;
}


Bool scan_auditorial_info(char *buffer, HearInfo *hinfo)
{
	static char *next_token(char *buf) ;
	char tmp[64] ;
	int i ;

	sscanf(buffer, "(hear %d %s", &(hinfo->time), tmp) ;
	buffer = next_token(buffer + 1) ;
	buffer = next_token(buffer) ;
	buffer = next_token(buffer) ;

	for(i = strlen(buffer); i >= 0; i--)
		if (buffer[i] == ')') {
			buffer[i] = '\0' ;
			break ;
		}

	strcpy(hinfo->message, buffer) ;

	if (!strcmp(tmp, "referee")) {
		hinfo->sender = ST_Referee ;
		hinfo->direction = 0.0 ;
	}
	else if (!strcmp(tmp, "self")) {
		hinfo->sender = ST_Self ;
		hinfo->direction = 0.0 ;
	}
	else {
		hinfo->sender = ST_Player ;
		hinfo->direction = atof(tmp) ;
	}

	return TRUE ;
}


Bool scan_body_info(char *buffer, BodyInfo *binfo)
{
	static char *next_token(char *buf) ;
	char tmp1[16], tmp2[16] ;

	sscanf(buffer, "(sense_body %d ", &(binfo->time)) ;
	buffer = next_token(buffer + 1) ;
	buffer = next_token(buffer) ;
        buffer = next_token(buffer) ;

	while(buffer != NULL) {
		sscanf(buffer, "(%s ",  tmp1) ;
		if (!strcmp(tmp1, "view_mode")) {
			sscanf(buffer, "(view_mode %s %s", tmp1, tmp2) ;

			if (!strcmp(tmp1, "high")) {
				binfo->viewquality = VQ_high ;
			}
			else if (!strcmp(tmp1, "low")) {
				binfo->viewquality = VQ_low ;
			}

			if (!strcmp(tmp2, "narrow")) {
				binfo->viewwidth = VW_Narrow ;
			}
			else if (!strcmp(tmp2, "normal")) {
				binfo->viewwidth = VW_Normal ;
			}
			else if (!strcmp(tmp2, "wide")) {
				binfo->viewwidth = VW_Wide ;
			}
		}
		else if (!strcmp(tmp1, "stamina")) {
			sscanf(buffer, "(stamina %lf %lf", &(binfo->stamina), &(binfo->effort)) ;
		}
		else if (!strcmp(tmp1, "speed")) {
			sscanf(buffer, "(speed %lf", &(binfo->speed)) ;
		}
		else if (!strcmp(tmp1, "kick")) {
			sscanf(buffer, "(kick %d", &(binfo->kick)) ;
		}
		else if (!strcmp(tmp1, "dash")) {
			sscanf(buffer, "(dash %d", &(binfo->dash)) ;
		}
		else if (!strcmp(tmp1, "turn")) {
			sscanf(buffer, "(turn %d", &(binfo->turn)) ;
		}
		else if (!strcmp(tmp1, "say")) {
			sscanf(buffer, "(say %d", &(binfo->say)) ;
		}
		else if (!strcmp(tmp1, "turn_neck")) {
			sscanf(buffer, "(turn_neck %d", &(binfo->turn_neck)) ;
		}
		else if (!strcmp(tmp1, "head_angle")) {
			sscanf(buffer, "(head_angle %lf", &(binfo->head_angle)) ;
		}
		else {
			printf("scan_body_info: miss obj\n") ;
			return FALSE ;
		}
                buffer=next_token(buffer) ;
	}

	return TRUE ;
}


