#include <math.h>
#include <libsclient.h>

#include "client.h"



void down_line(Mem *mem)
{
	if(mem->f_wait == 0) {
		if(turn(mem, -180.0) == -180.0)
			mem->f_wait = 1 ;
	}
	else {
		dash(mem, STAMINA_INC_MAX * 2.0) ;
		mem->f_wait = 0 ;
	}
}

void up_line(Mem *mem)
{
	if(mem->f_wait == 0) {
		if(turn(mem, 0.0) == 0.0)
			mem->f_wait = 1 ;
	}
	else {
		dash(mem, STAMINA_INC_MAX * 2.0) ;
		mem->f_wait = 0 ;
	}
}

void haichi(Mem *mem)
{
	double dist, dir ;

	if(mem->f_haichi == 0 && mem->unum > 1)
		mem->unum = (mem->unum + 2) % 10 + 2 ;

	if(mem->f_haichi < 2) {
		move(mem, mem->haichi[mem->unum - 1].x, mem->haichi[mem->unum - 1].y) ;
		mem->f_haichi ++ ;
	}
	else {
		xy2rt(- mem->haichi[mem->unum - 1].x, - mem->haichi[mem->unum - 1].y,
		      &dist, &dir) ;
		turn(mem, dir) ;

		mem->myinfo.vx = 0.0 ;
		mem->myinfo.vy = 0.0 ;
		mem->myinfo.vr = 0.0 ;

		mem->ballinfo.px = 0.0 ;
		mem->ballinfo.py = 0.0 ;
		mem->ballinfo.vx = 0.0 ;
		mem->ballinfo.vy = 0.0 ;
		mem->ballinfo.vr = 0.0 ;
		
		mem->ballinfo.c_unvisible = 0 ;
	}
}

/*
 * ¥À¥Ö¥ë¥­¥Ã¥¯
 * ope1: ¥­¥Ã¥¯¤¹¤ëÊý¸þ(ÀäÂÐ³ÑÅÙ)
 */
void doublekick(Mem *mem)
{
	double tmp ;

	mem->p_mode=10;
	if(mem->f_attack == 2) {
		kick(mem, 100.0, mem->ope1) ;
		mem->f_attack = 0 ;
		mem->p_mode=0;
	}
	else {
		kick(mem, 30.0, mem->ope1) ;
		mem->f_attack = 2 ;
	}
}

/*
 * ¥Ü¡¼¥ë¤ÎÊý¸þ¤ò¸þ¤¯
 */
void turnball(Mem *mem)
{
	double dist, dir ;

	if(mem->ballinfo.c_unvisible > 2)
		dir = norm(mem->myinfo.dir + 45.0) ;
	else {
		dir = mem->ballinfo.pt ;
	}

	turn(mem, dir) ;
}


/*
 * $(B%\!<%k$rDI$$$+$1$k(B
 */
void gotoball(Mem *mem)
{
	int i, tmp ;
	double dir, dist, tmp2, tmp3 ;

	sum_vector(mem->ballinfo.pr, mem->ballinfo.pt,
		   mem->ballinfo.vr , mem->ballinfo.vt,
		   &dist, &dir);
	sum_vector(dist, dir,
		   - mem->myinfo.vr ,
		   mem->myinfo.vt,
		   &dist, &dir) ;

	/*tmp2 = mem->ballinfo.vr * sin(Deg2Rad(mem->ballinfo.vt - mem->ballinfo.pt)) ;
	if(Abs(tmp2) >= 0.9)
		dir = mem->ballinfo.pt ;
	else
		dir = norm(mem->ballinfo.pt + Rad2Deg(asin(tmp2 / 0.9))) ; */

	if(norm(dir - mem->myinfo.dir) > 0.0)
		dir += 5.0 ;
	else
		dir -= 5.0 ;

	/* $(B%\!<%k$NJ}8~$r8~$$$F$$$J$$!"$+$D!"%@%C%7%eD>8e$J$i%?!<%s%b!<%I$K$9$k(B */
	tmp2 = norm(dir - mem->myinfo.dir) ;
	if(Abs(tmp2) > 10.0 && mem->f_defense == 2) {
		mem->f_defense = 0 ;
		dir = norm(dir) ;
	}

	/* $(B%?!<%s%b!<%I;~!"$^$?$O%@%C%7%eD>8e(B */
	if(mem->c_opponent > 0 && (mem->f_defense == 2)) {
		/* $(BE($,<YKb$J$i8r$o$9(B */
		tmp = 0 ;
		for(i = 1 ; i < mem->c_opponent ; i++)
			if(mem->opponent[i].pr < mem->opponent[tmp].pr)
				tmp = i ;
		tmp2 = Rad2Deg(asin(PLAYER_SIZE * 2.0 / mem->opponent[tmp].pr)) ;
		tmp3 = norm(mem->opponent[tmp].pt - dir) ;
		if(Abs(tmp3) < tmp2) {
			if(tmp3 > 0.0)
				tmp2 = - tmp2 ;
			dir = norm(mem->opponent[tmp].pt + tmp2) ;
			mem->f_defense = 0 ;
		}
	}

	/* $(B%?!<%s%b!<%I;~$K%?!<%s$r$9$k(B */
	if(mem->f_defense == 0) {
		if(turn(mem, dir) == dir)
			mem->f_defense = 1 ;
	}
	/* $(B%?!<%s%b!<%I$G$J$$>l9g$O!"%@%C%7%e$r$9$k(B */
	else {
		/*tmp2 = (dist - PLAYER_SIZE - BALL_SIZE) / (1.0 + PLAYER_DECAY)
			/ DASH_POWER_RATE / mem->myinfo.effort ;
		if(tmp2 > mem->myinfo.stamina)
			tmp2 = mem->myinfo.stamina ;
		else if(tmp2 > 100.0)
			tmp2 = 100.0 ; */
		dash(mem, 100.0) ;
		mem->f_defense = 2 ;
	}
}



/****************************************************************************************
 *											*
 *	°Ê²¼¡¢¹ÔÆ°¤òÁªÂò								*
 *											*
 ****************************************************************************************/

void wait(Mem *mem)
{
	mem->f_attack = mem->f_defense = 0 ;

	if(mem->ballinfo.c_unvisible < 3 && mem->ballinfo.px < mem->myinfo.px)
		down_line(mem) ;
	else if(mem->ballinfo.c_unvisible < 3 && mem->ballinfo.px > mem->myinfo.px &&
		mem->myinfo.stamina > STAMINA_MAX - STAMINA_INC_MAX &&
		mem->myinfo.px < mem->haichi[mem->unum-1].x + 30.0 && mem->unum != 1)
		up_line(mem) ;
	else
		turnball(mem) ;
}

void defense(Mem *mem)
{
	mem->f_attack = mem->f_wait = 0 ;

	gotoball(mem) ;
}

void attack(Mem *mem)
{
	int i, j, center, num[11], tmp ;
	double left, right, tmp2 ;

	mem->f_defense = mem->f_wait = 0 ;

	/************************************************************************
	 *	¥À¥Ö¥ë¥­¥Ã¥¯Ãæ¤Ê¤é¥­¥Ã¥¯¤òÂ³¤±¤ë				*
	 ************************************************************************/
	if(mem->f_attack == 2) {
		doublekick(mem) ;

		return ;
	}

	/************************************************************************
	 *	¥·¥å¡¼¥È²ÄÇ½¤«¤É¤¦¤«						*
	 ************************************************************************/

	/* Å¨¤òº¸¤Ë¸«¤¨¤ë¤Û¤¦¤«¤é¥½¡¼¥È */
	for(i = 0 ; i < mem->c_opponent ; num[i] = i++) ;
	for(i = 1 ; i < mem->c_opponent ; i++) {
		tmp = num[i] ;
		for(j = i ; j > 0 &&
		    norm(mem->opponent[num[j-1]].pt - mem->goalinfo[0][1].pt)
		    > norm(mem->opponent[tmp].pt - mem->goalinfo[0][1].pt);
		    num[j] = num [--j]) ;
		num[j] = tmp ;
	}
	/* °ìÈÖ¥´¡¼¥ë¤ÎÃæ¿´Â¦¤Ë¸«¤¨¤ëÅ¨¤òÃµ¤¹ È */
	for(tmp = 0 ; tmp < mem->c_opponent &&
	    norm(mem->opponent[num[tmp]].pt - mem->goalinfo[0][1].pt) < 0.0 ; tmp++) ;

	/* ¥·¥å¡¼¥È¥³¡¼¥¹¤Î·×»» */
	left = right = mem->goalinfo[0][1].pt ;
	if(tmp >= mem->c_opponent)
		center = mem->c_opponent - 1 ;
	else
		center = tmp ;
	for(i = center ; i >= 0 ; i--) {
		if(norm(mem->opponent[num[i]].pt - right + 30.0) > 0.0 &&
		   norm(mem->opponent[num[i]].pt - right - 30.0) < 0.0)
			right = norm(mem->opponent[num[i]].pt + 30.0) ;
		if(norm(mem->opponent[num[i]].pt - left - 30.0) < 0.0 &&
		   norm(mem->opponent[num[i]].pt - left + 30.0) > 0.0)
			left = norm(mem->opponent[num[i]].pt - 30.0) ;
		else
			break ;
	}
	if(tmp <= 0)
		center = 1 ;
	else
		center = tmp ;
	for(i = center - 1 ; i < mem->c_opponent ; i++) {
		if(norm(mem->opponent[num[i]].pt - left - 30.0) < 0.0 &&
		   norm(mem->opponent[num[i]].pt - left + 30.0) > 0.0)
			left = norm(mem->opponent[num[i]].pt - 30.0) ;
		if(norm(mem->opponent[num[i]].pt - right + 30.0) > 0.0 &&
		   norm(mem->opponent[num[i]].pt - right - 30.0) < 0.0)
			 right = norm(mem->opponent[num[i]].pt + 30.0) ;
		else
			break ;
	}
	if(Abs(norm(left  - mem->goalinfo[0][1].pt)) <
	   Abs(norm(right - mem->goalinfo[0][1].pt))  )
		tmp2 = left ;
	else
		tmp2 = right ;

	/* ·×»»¤·¤¿¥³¡¼¥¹¤Ç¥·¥å¡¼¥È²ÄÇ½¤«¤É¤¦¤« */
	if(norm(tmp2 - mem->goalinfo[0][0].pt) > 0.0 &&
	   norm(tmp2 - mem->goalinfo[0][2].pt) < 0.0   ) {
		mem->ope1 = tmp2 ;
		doublekick(mem) ;

		return ;
	}

	/************************************************************************
	 *	¥Ñ¥¹²ÄÇ½¤«							*
	 ************************************************************************/

	/* Ì£Êý¤ò¥´¡¼¥ëÊý¸þ¤Ë¶á¤¤½ç¤Ë¥½¡¼¥È */
	for(i = 0 ; i < mem->c_our ; num[i] = i++) ;
	for(i = 1 ; i < mem->c_our ; i++) {
		tmp = num[i] ;
		for(j = i ; j > 0 &&
		    Abs(norm(mem->our[num[j-1]].pt - mem->goalinfo[0][1].pt))
		    > Abs(norm(mem->our[tmp].pt - mem->goalinfo[0][1].pt)) ;
		    num[j] = num [--j]) ;
		num[j] = tmp ;
	}
	/* ¥Ñ¥¹²ÄÇ½¤ÊÌ£Êý¤¬Â¸ºß¤¹¤ë¤« */
	for(i = 0 ; i < mem->c_our ; i++) {
		for(j = 0 ; j < mem->c_opponent ; j++)
			if(Abs(norm(mem->our[num[i]].pt - mem->opponent[j].pt)) < 20.0
			   && mem->our[num[i]].pr > mem->opponent[j].pr - 5.0
			   && mem->our[num[i]].pr < 5.0)
				break ;
		if(j == mem->c_opponent)
			break ;
	}
	if(i != mem->c_our) {
		/* Ì£Êý¤ÎÊý¸þ¤¬¥´¡¼¥ëÊý¸þ¤Ê¤é¥Ñ¥¹¤¹¤ë */
		if(norm(mem->our[num[i]].pt - mem->goalinfo[1][0].pt) < 0.0 &&
		   norm(mem->our[num[i]].pt - mem->goalinfo[1][2].pt) > 0.0) {
			mem->ope1 = mem->our[num[i]].pt ;
			doublekick(mem) ;

			return ;
		}
	}

	/************************************************************************
	 *	clear								*
	 ************************************************************************/

	if(norm(tmp2 - mem->goalinfo[1][0].pt) < 0.0 &&
	   norm(tmp2 - mem->goalinfo[1][2].pt) > 0.0) {
		mem->ope1 = tmp2 ;
		doublekick(mem) ;
	}
	else {
		mem->ope1 = mem->goalinfo[0][1].pt ;
		doublekick(mem) ;
	}
}

/****************************************************************************************
 *											*
 *	ÀïÎ¬¥â¡¼¥É¤òÁªÂò								*
 *											*
 ****************************************************************************************/

/*  p_mode:1 */
/* player pozisyon hantei */
int place(Mem *mem)
{
  mem->p_mode=1;
  /* player num 1 */
  if(mem->unum == 1){
    if(mem->myinfo.px >= -52.5 && mem->myinfo.px <= -36.0){
      if(mem->myinfo.py >= -20.16 && mem->myinfo.py <= 20.16){
	if(mem->myinfo.px >= -52.5 && mem->myinfo.px <= -46.5){
	  if(mem->myinfo.py >= -5.0 && mem->myinfo.py <= 5.0){
           mem->p_mode=0;
	    return(2);
	  }
	}
       mem->p_mode=0;
	return(1);
      }
    }
    mem->p_mode=0;
    return(0);
  }  
}

/* p_mode:2 */
/* go default */

void move_def(Mem *mem)
{
  double r,t;

  mem->p_mode=2;
  if(mem->sw_1 == 0){
    mem->sw_1++;
    xy2rt(-50.0-mem->myinfo.px,0.0-mem->myinfo.py,&r,&t);
    turn(mem,t);
  }
  else if(mem->sw_1 <= 3){
    mem->sw_1++;
    dash(mem,60);
  }
  else if(mem->sw_1 == 4){
    mem->sw_1++;
    turn(mem,mem->ballinfo.pt);
  }
    else if(mem->sw_1 == 5){
    mem->sw_1++;
    turn(mem,mem->ballinfo.pt);
  }
  else{
    mem->sw_1=0;
    mem->p_mode=0;
    return;
  }  
}

/* p_mode:4 */
/* miokuri handan */
void miokuri(Mem *mem)
{
  if(mem->ballinfo.vr >= 0.6){
    if(mem->ballinfo.vt >= -30.0 && mem->ballinfo.vt <= 30.0){
      turn(mem,mem->ballinfo.pt);
      mem->p_mode=0;
      return;
    }
  }
  mem->p_mode=0;
  gotoball(mem);
}

/* p_mode:3 */
/* dash handan */
void d_handan(Mem *mem)
{
  
  int i;
  double tmp1,tmp2,dir;
  
  if(mem->c_our != 0){
    for( i=0 ; i <= mem->c_our ; i++ ){
      tmp1=Abs(mem->ballinfo.pt - mem->our[i].pt);
      tmp2=Abs(mem->ballinfo.pr - mem->our[i].pr);
      if(tmp1 <= 5.0 && tmp2 < mem->ballinfo.pr - 4.0){
	turn(mem,mem->ballinfo.pt);
	mem->p_mode=0;
	return;
      }
    }
  }
  mem->p_mode=0;
  miokuri(mem);
}


/* p_mode:6 */
/* drible kansuu */
void drible(Mem *mem)
{
  int i,tmp1=0;

  if(mem->c_our >= 2){
    for(i=0 ; i < mem->c_our-1 ; i++){
      if(mem->our[i+1].px >= mem->our[tmp1].px){
        tmp1=i;
      }
    }
    kick(mem,BALL_SPEED_MAX,mem->our[tmp1].pt);
    return;
  }
  else if(mem->c_our == 1){
    kick(mem,BALL_SPEED_MAX,mem->our[0].pt);
    return;
  }
  else{
    kick(mem,BALL_SPEED_MAX,0.0);
  }
}

/* p_mode:5 */
/* pass kansuu */

void pass(Mem *mem)
{

  int i,j,num[11],tmp;
  double r,t,tmp1,tmp2,tmp3,tmp4,tmp5;
  /************************************************************************
   *	pass kanou ka ?							*
   ************************************************************************/
  
  mem->p_mode=5;
  if(mem->sw_1 == 0){
    mem->sw_1=1;
    turn(mem,0.0);
    return;
  }
  else{
    mem->sw_1=0;
    /* teki wo hidarikara sort */
    for(i = 0 ; i < mem->c_opponent ; num[i] = i++) ;
    for(i = 1 ; i < mem->c_opponent ; i++) {
      tmp = num[i] ;
      for(j = i ; j > 0 &&
	    norm(mem->opponent[num[j-1]].pt - mem->goalinfo[0][1].pt)
	    > norm(mem->opponent[tmp].pt - mem->goalinfo[0][1].pt);
	  num[j] = num [--j]) ;
      num[j] = tmp ;
    }
    /* mikata no sort */
    for(i = 0 ; i < mem->c_our ; num[i] = i++) ;
    for(i = 1 ; i < mem->c_our ; i++) {
      tmp = num[i] ;
      for(j = i ; j > 0 &&
	    Abs(norm(mem->our[num[j-1]].pt - mem->goalinfo[0][1].pt))
	    > Abs(norm(mem->our[tmp].pt - mem->goalinfo[0][1].pt)) ;
	  num[j] = num [--j]) ;
      num[j] = tmp ;
    }
    /* pass dekiru mikata iruka? */
    for(i = 0 ; i < mem->c_our ; i++) {
      for(j = 0 ; j < mem->c_opponent ; j++)
	if(Abs(norm(mem->our[num[i]].pt - mem->opponent[j].pt)) < 10.0
	   && mem->our[num[i]].pr > mem->opponent[j].pr + 5.0)
	  break ;
    if(j == mem->c_opponent)
      break ;
    }
    if(i != mem->c_our) {
      /* mikataga goal houkounara pass */
      if(Abs(norm(mem->our[num[i]].pt - mem->goalinfo[0][1].pt) < 90.0)) {
	mem->ope1 = mem->our[num[i]].pt ;
	mem->p_mode=0;
	doublekick(mem) ;
	return ;
      }
    }
    mem->p_mode=0;
    drible(mem);
  }
}



/* Making data */



/****************************************/
/*                    player betu handan                           */
/****************************************/
void gk(Mem *mem)
{

  double dir,tmp1,tmp2;
  /* p_mode:0 */
  if(mem->p_mode == 0){
    if((mem->side == S_LEFT && 
	mem->plmode == PM_FreeKick_L) ||
       (mem->side == S_RIGHT && mem->plmode == PM_FreeKick_R)){
      if(mem->sw_2 == 0){
	mem->ballinfo.vr = 0.0;
	if(mem->myinfo.py <= 0){
	  move(mem,-37.5,17.0);
	  mem->sw_2++;
	  return;
	}
	else{
	  move(mem,-37.5,-17.0);
	  mem->sw_2++;
	  return;
	}
      }
      else if(mem->sw_2 == 1){
	mem->sw_2=0;
	pass(mem) ;
	return ;
      }
    }
    /* tairyoku aruka? */
    if(mem->myinfo.stamina >= STAMINA_MAX*0.5){
      /* bo-ru mieruka? */
      if(mem->ballinfo.f_visible >= 2){
        /* defaul_iki naika? */
        if( place(mem) >= 1){
          /* ball left or right? */
	  tmp2=Abs(mem->ballinfo.pt);
	  if(tmp2 <= 60.0){
	    /* distance for ball */
	    /* far */
	    if(mem->ballinfo.pr >= 23.0){
	      /* ball no ichi */
	      if(mem->ballinfo.px >= -35.0){
		/* default_chi iruka? */
		if(place(mem) == 2){
		  turn(mem,mem->ballinfo.pt);
		  return;
		}
		else{
		  move_def(mem);
		  return;
		}
	      }
	      else{
		gotoball(mem);
	      }
	    }
	    /* mid */
	    else if(mem->ballinfo.pr >= 10.0){	    
	      if(mem->ballinfo.px < -35.0){
		gotoball(mem);
	      }
	      else{
		d_handan(mem);
	      }
	    }
	    /* near */
	    else if(mem->ballinfo.pr >= 1.7){
	      miokuri(mem);
	    }
	    /* catch */
  	    else{
	      dir=norm(mem->ballinfo.pt - mem->myinfo.dir);
	      send_com_catch(&mem->sock, dir);
	    }
	  }
	  /* hidari ni mieta */
	  else{
	    if(mem->ballinfo.pr < 1.7){
	      dir=norm(mem->ballinfo.pt - mem->myinfo.dir);
	      send_com_catch(&mem->sock, dir);
 	    }
	    else{
	      gotoball(mem);
	    }
	  }
        }
        /* default ni inai */
        else{
	  /* ball left or right? */
	  tmp2=Abs(mem->ballinfo.pt);
	  if(tmp2 <= 60.0){
	    /* distance for ball */
	    /* far & mid */
	    if(mem->ballinfo.pr >= 10.0){
	      move_def(mem);
	    }
	    /* near */
	    else if(mem->ballinfo.pr > KICKABLE_AREA){
	      if(mem->myinfo.px <= -30.0){
	        d_handan(mem);
	      }
	      else{
	        move_def(mem);
	      }
	    }
	    /* kick */
	    else{
	      pass(mem);
	    }
	  }
	  /* hidari ni mieta */
	  else{
	    if(mem->ballinfo.pr <= KICKABLE_AREA){
	      pass(mem);
	    }
	    else{
	      gotoball(mem);
	    }
	  }       
        }
      }
      /* ball mienai */
      else{
        /* defult_chi iru? */
        if( place(mem) == 2){
      	    turn(mem,mem->myinfo.dir + 40.0);
	    return;
        }
        else{
	  move_def(mem);
	  return;
        }
      }
    }
    /* tairyoku nai */
    else{
      /* bo-ru mieruka? */
      if(mem->ballinfo.f_visible >= 2){
        /* defaul_iki naika? */
        if( place(mem) >= 1){
	  /* distance for ball */
	  /* far & mid */
	  if(mem->ballinfo.pr >= 10.0){
	    if(mem->ballinfo.px >= -35.0){
	      turn(mem,mem->ballinfo.pt);
	      return;
	    }
	    else{
	      gotoball(mem);
	    }
	  }
	  /* near */
	  else if(mem->ballinfo.pr >= 1.7){
	    miokuri(mem);
	  }
	  /* catch */
	  else{
	    dir=norm(mem->ballinfo.pt - mem->myinfo.dir);
	    send_com_catch(&mem->sock, dir);
	  }
        }
        /* default ni inai */
        else{
	  /* distance for ball */
	  /* far & mid */
	  if(mem->ballinfo.pr >= 10.0){
	    move_def(mem);
	  }
	  /* near */
	  else if(mem->ballinfo.pr > KICKABLE_AREA){
	    if(mem->myinfo.px <= -30.0){
	      d_handan(mem);
	    }
	    else{
	      move_def(mem);
	    }
	  }
	  /* kick */
	  else{
	    pass(mem);
	  }
        }
      }
      /* ball mienai */
      else{
        /* defult_chi iru? */
        if( place(mem) == 2){
	  turn(mem,mem->myinfo.dir + 40.0);
        }
        else{
	  move_def(mem);
        }
      }
    }
  }

  else if(mem->p_mode == 2){
    move_def(mem);
  }
  else if(mem->p_mode == 5){
    pass(mem);
  }  
  else if(mem->p_mode == 10){
    doublekick(mem);
  }
}


void handan(Mem *mem)
{
	int i ;
	double dist, dir, tmp2 ;

	/* ¥­¥Ã¥¯¥ª¥ÕÁ° */
	if(mem->plmode == PM_BeforeKickOff ||
	   mem->plmode == PM_Goal_L ||
	   mem->plmode == PM_Goal_R) {
		haichi(mem) ;
		return ;
	}
	
	/* Áê¼ê¤Î¥Õ¥ê¡¼¥­¥Ã¥¯»þ */
	if(mem->side == S_RIGHT && (
		mem->plmode == PM_KickOff_L ||
		mem->plmode == PM_FreeKick_L ||
		mem->plmode == PM_CornerKick_L ||
		mem->plmode == PM_GoalKick_L
	   ) ||
	   mem->side == S_LEFT && (
		mem->plmode == PM_KickOff_R ||
		mem->plmode == PM_FreeKick_R ||
		mem->plmode == PM_CornerKick_R ||
		mem->plmode == PM_GoalKick_R
	   )) {
	        wait(mem) ;
		return ;
	}
	
	if(mem->side == S_RIGHT && (
	   mem->plmode == PM_KickIn_L
	   ) ||
	   mem->side == S_LEFT && (
		mem->plmode == PM_KickIn_R
	   )) {
	  if( place(mem) <= 1){
	    move_def(mem) ;
	    return ;
	  }
	  else{
	    turn(mem,mem->ballinfo.pt);
	  }
	}
	
	gk(mem);
	return;

	if(Abs(norm(mem->ballinfo.pt - mem->myinfo.dir)) > 25.0 &&
	   mem->sensor.info.body.viewwidth != VW_Wide)
	          send_com_change_view(&mem->sock, VW_Wide, VQ_high) ;
	else if(Abs(norm(mem->ballinfo.pt - mem->myinfo.dir)) < 25.0 &&
	   mem->sensor.info.body.viewwidth != VW_Normal)
	    send_com_change_view(&mem->sock, VW_Normal, VQ_high) ;
	if(mem->f_attack == 0 && mem->ballinfo.pr < KICKABLE_AREA
	   && mem->ballinfo.c_unvisible < 3)
		mem->f_attack = 1 ;

	if(mem->f_attack > 0) {
		attack(mem) ;
		return ;
	}

	if(mem->myinfo.stamina >= STAMINA_MAX * 0.3 + MAXPOWER &&
	   mem->ballinfo.c_unvisible < 3) {
		for(i = 0 ; i < mem->c_our ; i++) {
			sum_vector(mem->ballinfo.pr, mem->ballinfo.pt,
				   - mem->our[i].pr, mem->our[i].pt, &dist, &dir) ;
			if(dist < mem->ballinfo.pr + 1.0)
				break ;
		}
		if(i == mem->c_our) {
			defense(mem) ;
			return ;
		}
	} 

	wait(mem) ;
}
       
