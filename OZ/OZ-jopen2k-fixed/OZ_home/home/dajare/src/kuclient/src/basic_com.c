/*
 *	Header:
 *	File: basic_com.c
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *	Changed by Yusuke Yamauchi (1999/06/12)
 *	EndHeader:
 */

#include <stdio.h>

#include "libsclient.h"

/*** functions in netif.c ***/
extern Bool send_message(Socket sock, char *buf) ;
extern Bool receive_message(Socket *sock, char *buf, int size) ;


PlayMode get_playmode(char *pmodestr)
{
	char *pm_strings[] = PLAYMODE_STRINGS ;
	PlayMode pm ;

	for (pm=0; pm_strings[pm] != NULL; pm++) {
		if (!strcmp(pmodestr, pm_strings[pm]))
			return pm ;
	}
	return PM_Error ;
}


InitInfo send_com_init(Socket *sock, char *teamname, PlayStile playstile)
{
	PlayMode get_playmode(char *pmodestr) ;
	InitInfo info ;
	char msg_buf[BUFSIZE], com[16], side ;
	int i ;

	if (playstile == PS_Goalie)
		sprintf(msg_buf, "(init %s (version 5.00) (goalie))", teamname) ;
	else
		sprintf(msg_buf, "(init %s (version 5.00))", teamname) ;
	if (!send_message(*sock, msg_buf)) {
		info.side = S_UNKNOWN ;
		return info ;
	}

	while(!receive_message(sock, msg_buf, BUFSIZE))
		/* wait for reply */ ;

	sscanf(msg_buf, "(%s ", com) ;
	if (!strcmp(com, "init")) {
		sscanf(msg_buf, "(init %c %d %s)",
					&side, &info.unum, info.playmodestr) ; 
		if (side == 'l')
			info.side = S_LEFT ;
		else if (side == 'r')
			info.side = S_RIGHT ;
		for(i=0; info.playmodestr[i] != ')'; i++);
		info.playmodestr[i] = '\0' ;
		info.playmode = get_playmode(info.playmodestr) ;
	}
	else 
		info.side = S_UNKNOWN ;

	return info ;
}


InitInfo send_com_reconnect(Socket *sock, char *teamname, int unum)
{
	InitInfo info ;
	char msg_buf[BUFSIZE], com[16], side ;
	int i ;

	sprintf(msg_buf, "(reconnect %s %d)", teamname, unum) ;
	if (!send_message(*sock, msg_buf)) {
		info.side = S_UNKNOWN ;
		return info ;
	}

	while(!receive_message(sock, msg_buf, BUFSIZE))
		/* wait for reply */ ;

	sscanf(msg_buf, "(%s ", com) ;
	if (!strcmp(com, "reconnect")) {
		sscanf(msg_buf, "(reconnect %c %s)", &side, info.playmodestr) ; 
		if (side == 'l')
			info.side = S_LEFT ;
		else if (side == 'r')
			info.side = S_RIGHT ;
		for(i=0; info.playmodestr[i] != ')'; i++);
		info.playmodestr[i] = '\0' ;
		info.playmode = get_playmode(info.playmodestr) ;
	}
	else 
		info.side = S_UNKNOWN ;

	return info ;
}


Bool send_com_move(Socket *sock, double x, double y)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(move %f %f)\n", x, y) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_turn(Socket *sock, double moment)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(turn %f)\n", moment) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_dash(Socket *sock, double power)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(dash %f)\n", power) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_kick(Socket *sock, double power, double dir)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(kick %f %f)\n", power, dir) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_catch(Socket *sock, double dir)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(catch %f)\n", dir) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_turn_neck(Socket *sock, double angle)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(turn_neck %f)\n", angle) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_say(Socket *sock, char *message)
{
	char msg_buf[BUFSIZE] ;

	sprintf(msg_buf, "(say %s)\n", message) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


Bool send_com_change_view(Socket *sock, ViewWidth width, ViewQuality quality)
{
	char msg_buf[BUFSIZE], w_str[8], q_str[8] ;

	switch (width) {
	case VW_Narrow:
		strcpy(w_str, "narrow") ;
		break ;
	case VW_Normal:
		strcpy(w_str, "normal") ;
		break ;
	case VW_Wide:
		strcpy(w_str, "wide") ;
		break ;
	default:
		return FALSE ;
	}

	switch (quality) {
	case VQ_high:
		strcpy(q_str, "high") ;
		break ;
	case VQ_low:
		strcpy(q_str, "low") ;
		break ;
	default:
		return FALSE ;
	}

	sprintf(msg_buf, "(change_view %s %s)\n", w_str, q_str) ;
	if (!send_message(*sock, msg_buf))
		return FALSE ;

	return TRUE ;
}


