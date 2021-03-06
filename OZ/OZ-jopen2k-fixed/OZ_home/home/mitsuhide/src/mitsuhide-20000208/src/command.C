#include <stdio.h>
#include "networkinterface.h"
#include "command.h"
#include "mymath.h"
#include "vector.h"
#include "utility.h"
#include "seeinfo.h"
#include "param.h"
#include "commandinfo.h"
#include "worldmodel.h"

// $B=i4|2=(B
void Effector::init(NetworkInterface* netif, WorldModel* worldmodel) {
  this->netif = netif ;
  this->worldmodel = worldmodel ;
  type = CT_None ;
  arg1 = 0 ;
  arg2 = 0 ;
  neck_state = LOOK_AROUND ;
  flag = false ;
  neck_dir = 0 ;
}

// $B%3%^%s%I$N=i4|2=(B
void Effector::reset(void) {
  type = CT_None ;
  arg1 = 0 ;
  arg2 = 0 ;
}

// $B%3%^%s%I$NF~NO(B
void Effector::set(CommandType type, double arg1, double arg2) {
  this->type = type ;
  this->arg1 = arg1 ;
  this->arg2 = arg2 ;
}

// $B%3%^%s%I$N<B9T(B
void Effector::execute(void) {
  char msg[4096] ;
  double angle_body ;

  switch (type) {
  case CT_Kick :
    // $B%-%C%/(B
    // $B0z?t$N%A%'%C%/(B
    arg1 = NormalizeKickPower(arg1) ;
    arg2 = NormalizeMoment(arg2) ;
    // $B@$3&%b%G%k$X$N1F6A(B
    worldmodel->kick(arg1, arg2) ;
    sprintf(msg, "(kick %f %f)", arg1, arg2) ;
    break ;

  case CT_Dash :
    // $B%@%C%7%e(B
    // $B0z?t$N%A%'%C%/!u@$3&%b%G%k$X$N1F6A(B
    arg1 = worldmodel->dash(arg1) ;    
    // $B%3%^%s%I:n@.(B
    sprintf(msg, "(dash %f)", arg1) ;
    break ;

  case CT_Turn :
    // $B%?!<%s(B
    // $B0z?t$N%A%'%C%/(B
    arg1 = NormalizeMoment(arg1) ;
    // $B@$3&%b%G%k$X$N1F6A(B
    worldmodel->turn(arg1) ;
    // $B%3%^%s%I:n@.(B
    sprintf(msg, "(turn %f)", arg1) ;
    break ;

  case CT_Move :
    // $B%`!<%V(B
    // $B$3$3$G0z?t$N%A%'%C%/$r$9$Y$-(B
    // $B@$3&%b%G%k$X$N1F6A(B
    worldmodel->move(Vector(arg1, arg2)) ;
    // $B%3%^%s%I:n@.(B
    sprintf(msg, "(move %f %f)", arg1, arg2) ;
    break ;

  case CT_Catch :
    // $B%-%c%C%A(B
    // $B$3$3$G0z?t$N%A%'%C%/$r$9$Y$-(B
    // $B@$3&%b%G%k$X$N1F6A(B
    worldmodel->catch_() ;
    // $B%3%^%s%I:n@.(B
    sprintf(msg, "(catch %f)", arg1) ;
    break ;

  case CT_None :
  default :
    // $B%G%U%)%k%H(B
    // $B%3%^%s%I:n@.(B
    sprintf(msg, "(turn 0.0)") ;
    break ;

  }

  // $B%3%^%s%IAw?.(B
  netif->send_message(msg) ;

  if (flag) {
    double turn_neck ;
    switch (neck_state) {
    case LOOK_BALL :
      angle_body = worldmodel->self.get_angle_body() + worldmodel->self.get_turn_body() ;
      if (worldmodel->self.known_pos())
	turn_neck = normalize_angle(max(-90.0, min(90.0, normalize_angle(worldmodel->self.get_pos().direction(worldmodel->ball.get_pos()) - angle_body))) - worldmodel->self.get_angle_neck()) ;
      else
	turn_neck = normalize_angle(0.0 - worldmodel->self.get_angle_neck()) ;
      break ;
      
    case LOOK_AROUND :
    default :
      switch (worldmodel->get_vwidth()) {
      case VW_WIDE :
        neck_dir += 90.0 ;
	break ;
      case VW_NORMAL :
        neck_dir += 45.0 ;
	break ;
      case VW_NARROW :
        neck_dir += 22.5 ;
	break ;
      }
      if (neck_dir > 90.0)
	neck_dir = -90.0 ;
      turn_neck = normalize_angle(neck_dir - worldmodel->self.get_angle_neck()) ;
      break ;
    }
    
    turn_neck = NormalizeNeckMoment(turn_neck) ;
    worldmodel->self.set_turn_neck(turn_neck) ;
    sprintf(msg, "(turn_neck %f)", turn_neck) ;
    
    // $B%3%^%s%IAw?.(B
    netif->send_message(msg) ;
    flag = false ;
  }
}

// $B<s$N>uBV$N99?7(B
void Effector::set_neck_state(int state) {
  if (state != LOOK_AROUND && state != LOOK_BALL)
    return ;
  neck_state = state ;
}










