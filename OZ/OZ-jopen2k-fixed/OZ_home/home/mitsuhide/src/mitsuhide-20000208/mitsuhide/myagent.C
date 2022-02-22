#include <libYowAI.h>
#include "myagent.h"

void MyAgent::think(void) {
  switch (worldmodel.get_playmode()) {
  case PM_BeforeKickOff :
  case PM_AfterGoal_Left :
  case PM_AfterGoal_Right :
    before_kick_off_play() ;
    break;
  case PM_KickOff_Left :
  case PM_KickIn_Left :
  case PM_FreeKick_Left :
  case PM_CornerKick_Left :
  case PM_OffSide_Left :
    if (worldmodel.get_side() == S_LEFT)
      own_restart_play();
    else 
      opponent_restart_play();
    break;
  case PM_KickOff_Right : 
  case PM_KickIn_Right :
  case PM_FreeKick_Right :
  case PM_CornerKick_Right :
  case PM_OffSide_Right :
    if (worldmodel.get_side() == S_RIGHT)
      own_restart_play();
    else 
      opponent_restart_play();
    break;
  case PM_GoalKick_Left :
  case PM_GoalKick_Right :
    opponent_restart_play();
    break ;
  default :
    play_on_play() ;
    break ;
  }
}

/*
 * ==================================================
 * 判断
 * ==================================================
 */
// プレー前
void MyAgent::before_kick_off_play(void) {
  Vector target = position(BEFORE_KICK_OFF) ;
  if (worldmodel.self.get_pos().distance(target) <= 1.0) 
    look_ball() ;
  else
    move(target);
}

// リスタート
void MyAgent::own_restart_play(void) {
  static bool restart_flag = false ;
  Vector target ;

  if (restarter()) {
    target = position(RESTARTER) ;
    if (worldmodel.self.get_pos().distance(target) <= 1.0)
      restart_flag = true ;
    if (restart_flag) {
      if (worldmodel.self.get_pos().distance(worldmodel.ball.get_pos()) <= KICKABLE) {
	double require_speed ;
	restart(target, require_speed) ;
	if (!single_kick(target, require_speed))
	  multi_kick(target, require_speed) ;
	restart_flag = false ;
      } else if (worldmodel.get_stamina() >= STAMINA_MAX * 0.9 + MAXPOWER)
	// スタミナが多い時
	move(worldmodel.ball.get_pos(), worldmodel.ball.get_vel(), 35) ;
      else
	// スタミナが少ない時
	move(worldmodel.ball.get_pos(), worldmodel.ball.get_vel(), 17.5) ;
    } else {
      if (worldmodel.get_stamina() >= STAMINA_MAX * 0.9 + MAXPOWER)
	// スタミナが多い時
	move(target, Vector(0, 0), 35, false) ;
      else
	// スタミナが少ない時
	move(target, Vector(0, 0), 17.5, false) ;
    }
  } else if (offside()) {
    // オフサイドポジションの時
    target = Vector(-52.5, worldmodel.self.get_pos().get_y()) ;
    if (worldmodel.get_stamina() <= STAMINA_MAX * max(EFFORT_DEC_THR, RECOVER_DEC_THR) + MAXPOWER)
      // スタミナが少ない時
      move(target, Vector(0, 0), 17.5) ;
    else
      // スタミナが多い時
      move(target, Vector(0, 0), 35) ;
  } else {
    // オフサイドポジションでない時
    target = position(PLAY_ON) ;
    if (worldmodel.self.get_pos().distance(target) <= 1.0)
      // ホームポジションに居る時
      look_ball() ;
    else if (worldmodel.get_stamina() >= STAMINA_MAX * 0.9 + MAXPOWER)
      // スタミナが多い時
      move(target, Vector(0, 0), 35) ;
    else
      // スタミナが少ない時
      move(target, Vector(0, 0), 17.5) ;
  }
}

// リスタート
void MyAgent::opponent_restart_play(void) {
  Vector target ;

  if (offside()) {
    // オフサイドポジションの時
    target = Vector(-52.5, worldmodel.self.get_pos().get_y()) ;
    if (worldmodel.get_stamina() <= STAMINA_MAX * max(EFFORT_DEC_THR, RECOVER_DEC_THR) + MAXPOWER)
      // スタミナが少ない時
      move(target, Vector(0, 0), 17.5) ;
    else
      // スタミナが多い時
      move(target, Vector(0, 0), 35) ;
  } else {
    // オフサイドポジションでない時
    target = position(PLAY_ON) ;
    if (worldmodel.self.get_pos().distance(target) <= 1.0)
      // ホームポジションに居る時
      look_ball() ;
    else if (worldmodel.get_stamina() >= STAMINA_MAX * 0.9 + MAXPOWER)
      // スタミナが多い時
      move(target, Vector(0, 0), 35) ;
    else
      // スタミナが少ない時
      move(target, Vector(0, 0), 17.5) ;
  }
}

// プレー中
void MyAgent::play_on_play(void) {

  if (!worldmodel.self.known_pos()) {
    // 自分の位置を知らない
    reset_kick_target() ;
    ;
  } else if (!worldmodel.ball.known_pos()) {
    // ボールの位置を知らない
    reset_kick_target() ;
    look_for_ball() ;
  } else
    // 自分とボールの位置を知っている
    if (worldmodel.self.get_pos().distance((worldmodel.ball.get_pos())) <= KICKABLE) {
      // ボールを蹴れる
      static Vector target ;
      static double require_speed ; 
      get_kick_target(target, require_speed) ;
      multi_kick(target, require_speed) ;
    } else {
      // ボールを蹴れない
      reset_kick_target() ;
      if (nearest() || receiver()) 
        // ボールに一番近いまたはパスの受け手である
        if (worldmodel.get_stamina() <= STAMINA_MAX * max(EFFORT_DEC_THR, RECOVER_DEC_THR) + MAXPOWER)
	  // スタミナが少ない時
	  move(worldmodel.ball.get_pos(), worldmodel.ball.get_vel(), 35) ;
        else
	  // スタミナが多い時
	  move(worldmodel.ball.get_pos(), worldmodel.ball.get_vel(), 100) ;
      else {
	// その他
	Vector target ;
	if (offside()) {
	  // オフサイドポジションの時
	  target = Vector(-52.5, worldmodel.self.get_pos().get_y()) ;
	  if (worldmodel.get_stamina() <= STAMINA_MAX * max(EFFORT_DEC_THR, RECOVER_DEC_THR) + MAXPOWER)
	    // スタミナが少ない時
	    move(target, Vector(0, 0), 35) ;
	  else
	    // スタミナが多い時
	    move(target, Vector(0, 0), 100) ;
	} else {
	  // オフサイドポジションでない時
	  target = position(PLAY_ON) ;
          if (worldmodel.self.get_pos().distance(target) <= 1.0)
	    // ホームポジションに居る時
	    look_ball() ;
	  else if (worldmodel.get_stamina() >= STAMINA_MAX * 0.9 + MAXPOWER)
	    // スタミナが多い時
	    move(target, Vector(0, 0), 35) ;
	  else
	    // スタミナが少ない時
	    move(target, Vector(0, 0), 17.5) ;
	}
      }
    }
}

/*
 * ==================================================
 * 判断材料
 * ==================================================
 */
// キックターゲット
void MyAgent::reset_kick_target(void) {
  flag = false ;
}

void MyAgent::get_kick_target(Vector& target, double& require_speed) {
  const Vector goal(52.5, 0) ;

  if (flag)
    return ;
  
  flag = true ;
  if (worldmodel.self.get_pos().distance(goal) <= 15.0) {
    // シュート
    target = shot() ;
    require_speed = 2.7 ;
  } else if (worldmodel.get_offsideline() <= worldmodel.self.get_pos().get_x()) {
    // オフサイドポジション
    bool shot_flag = false ;
    target = worldmodel.get_opponent_goalie_pos() ;
    if (target.get_x() != POS_ERR && target.get_y() != POS_ERR) {
      if (worldmodel.ball.get_pos().distance(target) <= 12.5)
	shot_flag = true ;
      else 
	shot_flag = false ;
    } else {
      if (worldmodel.ball.get_pos().distance(goal) <= 12.5)
	shot_flag = true ;
      else 
	shot_flag = false ;
    }
    if (worldmodel.get_stamina() <= STAMINA_MAX * 0.35)
      shot_flag = true ;

    if (shot_flag) {
      // シュート
      target = shot() ;
      require_speed = 2.7 ;
    } else if (45.0 < worldmodel.self.get_pos().get_x() &&
	       fabs(worldmodel.self.get_pos().get_y()) > 20.16) {
      // センタリング
      target = Vector(36, 0) ;
      require_speed = 2.7 ;
    } else {
      // ドリブル
      dribble(target, require_speed) ;
    }
  } else {
    // オフサイドポジションではない
    if (pass(target)) {
      // パス
      require_speed = 0.8 ;
    } else if (36.0 <= worldmodel.self.get_pos().get_x() &&
	       fabs(worldmodel.self.get_pos().get_y()) <= 20.16) {
      // シュート
      target = shot() ;
      require_speed = 2.7 ;
    } else if(throgh(target)) {
      // スルーパス
      require_speed = 0.0 ;
    } else if (imagency(target)) {
      // 緊急キック
      require_speed = 0.0 ;
    } else {
      // デフォルトキック
      default_target(target, require_speed) ;
    }
  }
}

// シュート位置
Vector MyAgent::shot(void) {
  const Vector goal_left(52.5, -7.01) ;
  const Vector goal_right(52.5, 7.01) ;
  double tmp_dir ;
  double min_dir = min(worldmodel.self.get_pos().direction(goal_left), 
		       worldmodel.self.get_pos().direction(goal_right)) ;
  double max_dir = max(worldmodel.self.get_pos().direction(goal_left), 
		       worldmodel.self.get_pos().direction(goal_right)) ;
  int number = 0 ;
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!worldmodel.opponent[i].known_pos())
      continue;
    double dir = worldmodel.self.get_pos().direction(worldmodel.opponent[i].get_pos()) ;
    if (min_dir < dir && dir < max_dir) {
      number++ ;
      tmp_dir = dir ;
    }
  }

  if (number == 0)
    if (worldmodel.self.get_pos().get_x() > 0)
      return Vector(52.5, 5.01) ;
    else 
      return Vector(52.5, -5.01) ;
  else if (number == 1) {
    if (fabs(normalize_angle(tmp_dir - worldmodel.self.get_pos().direction(goal_left))) <
	fabs(normalize_angle(tmp_dir - worldmodel.self.get_pos().direction(goal_right))))
      return Vector(52.5, 5.51) ;
    else
      return Vector(52.5, -5.51) ;
  } else {
    if (worldmodel.self.get_pos().get_x() > 0)
      return Vector(52.5, 5.51) ;
    else 
      return Vector(52.5, -5.51) ;
  }
}

// パスできるかどうかの判定とパス位置
bool MyAgent::pass(Vector& target) {
  // 初期化
  bool pass[N_PLAYER_PER_TEAM - 1] ;
  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
    pass[i] = false ;
  }

  // 判定
  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
    if (!worldmodel.teammate[i].known_pos()) 
      continue ;
    if (worldmodel.self.get_pos().distance(worldmodel.teammate[i].get_pos()) > 40.0)
      continue ;
    if (worldmodel.self.get_pos().distance(worldmodel.teammate[i].get_pos()) < 10.0)
      continue ;
    if (worldmodel.get_offsideline() < worldmodel.teammate[i].get_pos().get_x()) continue ;
    if (clear_line(i)) 
      pass[i] = true ;
  }

  int num = -1 ;
  double x_max = worldmodel.self.get_pos().get_x() + 5.0 ;
  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
    if (!pass[i])
      continue;
    if (x_max < worldmodel.teammate[i].get_pos().get_x()) {
      x_max = worldmodel.teammate[i].get_pos().get_x() ;
      num = i ;
    }
  }

  if (num == -1)
    return false ;
  else {
    target = worldmodel.teammate[num].get_pos() ;
    return true ;
  }
}

bool MyAgent::clear_line(int n) {
  double dist = worldmodel.ball.get_pos().distance(worldmodel.teammate[n].get_pos()) ;
  double dir = worldmodel.ball.get_pos().direction(worldmodel.teammate[n].get_pos()) ;
  Vector rotate_receiver = worldmodel.teammate[n].get_pos().rotate(worldmodel.ball.get_pos(), -dir) ;

  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!worldmodel.opponent[i].known_pos())
      continue ;
    Vector rotate_opp = worldmodel.opponent[i].get_pos().rotate(worldmodel.ball.get_pos(), -dir) ;    
    if (fabs(worldmodel.ball.get_pos().direction(rotate_opp)) > 20.0) continue ;
    if (worldmodel.ball.get_pos().get_x() < rotate_opp.get_x() && 
        rotate_opp.get_x() < rotate_receiver.get_x() + dist / 5.0)
      return false ;
  }

  return true ;
}

// スルーできるかどうかの判定とスルー位置
bool MyAgent::throgh(Vector& target) {
  const Vector goal(52.5, 0.0);
  double through_dir = DIR_ERR;

  double dist = (52.5 + worldmodel.get_offsideline()) / 2.0 ;
  for (double dir = -60.0; dir <= 60.0; dir += 10.0) {
    Vector pos = worldmodel.ball.get_pos() + Vector(1.0, Tan(dir)) * dist ;

    if (pos.get_x() < -52.5 || 52.5 < pos.get_x() || 
	pos.get_y() < -34.0 || 34.0 < pos.get_y())
      continue ;

    bool flag = true ;
    for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
      if (!worldmodel.opponent[i].known_pos())
	continue ;
      Vector rotate_opp = worldmodel.opponent[i].get_pos().rotate(worldmodel.ball.get_pos(), -dir) ;
      if (fabs(worldmodel.ball.get_pos().direction(rotate_opp)) > 30.0) continue;
      if (worldmodel.ball.get_pos().get_x() + worldmodel.ball.get_pos().distance(pos) * 1.2 < rotate_opp.get_x()) continue ;
      flag = false ;
    }    

    if (flag) {
      if (through_dir == DIR_ERR) {
	through_dir = dir ;
        target = pos ;
      } else if (fabs(normalize_angle(dir - worldmodel.ball.get_pos().direction(goal))) < 
	  fabs(normalize_angle(through_dir - worldmodel.ball.get_pos().direction(goal)))) {
        through_dir = dir ; 
	target = pos ;
      }
    }
  }

  if (through_dir == DIR_ERR)
    return false ;
  else {
    return true ;
  }
}

// 緊急キックできるかどうかの判定と緊急キック位置
bool MyAgent::imagency(Vector& target) {
  double imagency_dir = DIR_ERR;

  double dist = 35.0 ;
  for (double dir = -90.0; dir <= 90.0; dir += 15.0) {
    Vector pos = worldmodel.ball.get_pos() + polar2vector(dist, dir) ;

    if (pos.get_x() < -52.5 || 52.5 < pos.get_x() || 
	pos.get_y() < -34.0 || 34.0 < pos.get_y())
      continue ;

    bool flag = true;
    for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
      if (!worldmodel.opponent[i].known_pos())
	continue ;
      Vector rotate_opp = worldmodel.opponent[i].get_pos().rotate(worldmodel.ball.get_pos(), -dir) ;
      if (fabs(worldmodel.ball.get_pos().direction(rotate_opp)) > 30.0) continue;
      if (worldmodel.ball.get_pos().get_x() + worldmodel.ball.get_pos().distance(pos) * 1.2 < rotate_opp.get_x()) continue ;
      flag = false ;
    }    

    if (flag) {
      if (imagency_dir == DIR_ERR) {
	imagency_dir = dir ;
	target = pos ;
      } else if (fabs(dir) < fabs(imagency_dir)) {
        imagency_dir = dir ;
	target = pos ;
      }
    }
  }

  if (imagency_dir == DIR_ERR)
    return false ;
  else {
    return true ;
  }
}

// デフォルトキック位置
void MyAgent::default_target(Vector& target, double& require_speed) {
  const double offside_margin = 5.0;
  double length = HALF_PITCH_L + worldmodel.get_offsideline() - offside_margin ;
  Vector position[10] = 
    {Vector(-HALF_PITCH_L + length, -17.0), 
     Vector(-HALF_PITCH_L + length, 0.0),
     Vector(-HALF_PITCH_L + length, 17.0),
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, -25.5), 
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, -8.5),
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, 8.5), 
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, 25.5), 
     Vector(-HALF_PITCH_L + length / 3.0, -17.0), 
     Vector(-HALF_PITCH_L + length / 3.0, 0.0), 
     Vector(-HALF_PITCH_L + length / 3.0, 17.0)};
  int number;
    
  number = 1 ;

  switch (number) {
  case 0 :
    target = Vector(42.5, -10.0);
    require_speed = 0.0;
    break;
  case 1 :
    if (worldmodel.self.get_pos().get_y() > 0)
      target = Vector(42.5, 10.0);
    else
      target = Vector(42.5, -10.0);
    require_speed = 0.0;
    break;
  case 2 :
    target = Vector(42.5, 10.0);
    require_speed = 0.0;
    break;
  case 3 :
    target = Vector(42.5, -10.0);
    require_speed = 2.7;
    break;
  case 4 :
    target = Vector(42.5, -10.0);
    require_speed = 2.7;
    break;
  case 5 :
    target = Vector(42.5, 10.0);
    require_speed = 2.7;
    break;
  case 6 :
    target = Vector(42.5, 10.0);
    require_speed = 2.7;
    break;
  case 7 :
    target = position[3];
    require_speed = 2.7;
    break;
  case 8 :
    if (worldmodel.self.get_pos().get_y() > 0)
      target = position[6];
    else
      target = position[3];
    require_speed = 2.7;
    break;
  case 9 :
    target = position[6];
    require_speed = 2.7;
    break;
  default :
    if (worldmodel.self.get_pos().get_y() > 0)
      target = Vector(52.5, 5.01);
    else
	target = Vector(52.5, -5.01);
    require_speed = 2.7;
    break;
  }
}

// ドリブルキック位置
void MyAgent::dribble(Vector& target, double& require_speed) {
  Vector self_pos = worldmodel.self.get_pos() ;
  Vector self_vel = worldmodel.self.get_vel() ;
  double self_angle_body = worldmodel.self.get_angle_body() ;
  Vector ball_pos = worldmodel.ball.get_pos() ;
  Vector ball_vel ;
  if (worldmodel.ball.known_vel())
    ball_vel = worldmodel.ball.get_vel() ;
  else
    ball_vel = Vector(0, 0) ;

  Vector dribble_target ;
  if (worldmodel.self.get_pos().get_y() < -7.01)
    dribble_target = Vector(52.5, -7.01);
  else if (worldmodel.self.get_pos().get_y() < 7.01)
    dribble_target = Vector(52.5, 0.0);
  else
    dribble_target = Vector(52.5, 7.01);

  Vector self_pos_after_three_cycle = self_pos + self_vel * 1.56 + polar2vector(1.44, self_pos.direction(dribble_target)) ;
  
  // 直進可能
  target = self_pos_after_three_cycle + polar2vector(0.735, self_pos.direction(self_pos_after_three_cycle)) ; 
  require_speed = ball_pos.distance(target) / 2.8816 * 0.884736 ;
}

// リスタートキック位置
void MyAgent::restart(Vector& target, double& require_speed) {
  require_speed = 0.2 ;

  if (worldmodel.ball.known_pos()) {
    target = worldmodel.ball.get_pos() ;
    if (fabs(-52.5 - worldmodel.ball.get_pos().get_x()) <= 2.0) {
      if (fabs(-34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	target.set_x(target.get_x() + 5.0);
        target.set_y(target.get_y() + 5.0);
      } else if (fabs(34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	target.set_x(target.get_x() + 5.0);
        target.set_y(target.get_y() - 5.0);
      } else {
	target.set_x(target.get_x() + 5.0);
      }
    } else if (fabs(52.5 - worldmodel.ball.get_pos().get_x()) <= 2.0) {
      if (fabs(-34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	target.set_x(target.get_x() - 5.0);
        target.set_y(target.get_y() + 5.0);
      } else if (fabs(34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	target.set_x(target.get_x() - 5.0);
        target.set_y(target.get_y() - 5.0);
      } else {
	target.set_x(target.get_x() - 5.0);
      }
    } else {
      if (fabs(-34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
        target.set_y(target.get_y() + 5.0);
      } else if (fabs(34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
        target.set_y(target.get_y() - 5.0);
      } else {
	target.set_x(target.get_x() + 5.0);
      }
    }
  } else {
    target = Vector(52.5, 0) ;
  }
}

// ボールに一番近いかの判定
bool MyAgent::nearest(void) {
  return nearest(false) ;
}

// ボールに一番近いかの判定
bool MyAgent::nearest(bool without_goalie) {
  if (worldmodel.ball.known_pos()) {
    double dist ;
    Vector target = worldmodel.ball.get_pos() ;
    dist = worldmodel.self.get_pos().distance(target) ;
    for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
      if (!worldmodel.teammate[i].known_pos())
	continue ;
      if (without_goalie && worldmodel.teammate[i].get_unum() == 1) continue ;
      if (worldmodel.teammate[i].get_pos().distance(target) < dist)
        return false ;
    }  
    return true ;
  } else {
    return false ;
  }
}

// パスの受け手かの判定
bool MyAgent::receiver(void) {
  return receiver(false) ;
}

// パスの受け手かの判定
bool MyAgent::receiver(bool without_goalie) {
  if (worldmodel.ball.known_pos()) {
    double dist ;
    Vector target = worldmodel.ball.get_pos() ;
    if (worldmodel.ball.known_vel()) {
      Vector vel = worldmodel.ball.get_vel() ;
      while (vel.r() > 0.8) {
        target += vel ;
        vel *= BALL_DECAY ;
      }
      bool near = true ;
      dist = worldmodel.self.get_pos().distance(target) ;
      for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
	if (!worldmodel.teammate[i].known_pos())
	  continue ;
	if (without_goalie && worldmodel.teammate[i].get_unum() == 1) continue ;
	if (worldmodel.teammate[i].get_pos().distance(target) < dist)
	  near =  false ;
      }  
      if (near)
	return true ;
      while (vel.r() > 0.001) {
        target += vel ;
        vel *= BALL_DECAY ;
      }
    } else {
      return false ;
    }
    dist = worldmodel.self.get_pos().distance(target) ;
    for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
      if (!worldmodel.teammate[i].known_pos())
	continue ;
      if (without_goalie && worldmodel.teammate[i].get_unum() == 1) continue ;
      if (worldmodel.teammate[i].get_pos().distance(target) < dist)
        return false ;
    }  
    return true ;
  } else {
    return false ;
  }
}

// オフサイドポジションかの判定
bool MyAgent::offside(void) {
  if (worldmodel.get_offsideline() <= worldmodel.self.get_pos().get_x())
    return true ;
  else
    return false ;
}

// リスタートするプレーヤかの判定
bool MyAgent::restarter(void) {
  if ((worldmodel.get_playmode() == PM_FreeKick_Left ||
       worldmodel.get_playmode() == PM_FreeKick_Right))
    if (nearest() || receiver())
      return true;
    else 
      return false;
    
  if (nearest(true) || receiver(true))
    return true;
  else
    return false;
}

// ホームポジションを取得
Vector MyAgent::position(int mode) {
  const double offside_margin = 5.0;
  double length = HALF_PITCH_L + worldmodel.get_offsideline() - offside_margin ;
  Vector pos[2][10] = 
    {{Vector(-HALF_PITCH_L + length, -17.0), 
     Vector(-HALF_PITCH_L + length, 0.0),
     Vector(-HALF_PITCH_L + length, 17.0),
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, -25.5), 
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, -8.5),
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, 8.5), 
     Vector(-HALF_PITCH_L + length * 2.0 / 3.0, 25.5), 
     Vector(-HALF_PITCH_L + length / 3.0, -17.0), 
     Vector(-HALF_PITCH_L + length / 3.0, 0.0), 
     Vector(-HALF_PITCH_L + length / 3.0, 17.0)},
     {Vector(-1.0, -17.0), 
      Vector(-1.0, 0.0),
      Vector(-1.0, 17.0),
      Vector(-1.0, -25.5), 
      Vector(-1.0, -8.5),
      Vector(-1.0, 8.5), 
      Vector(-1.0, 25.5), 
      Vector(-25.0, -17.0), 
      Vector(-25.0, 0.0), 
      Vector(-25.0, 17.0)}} ;
  int number;

  if (mode == RESTARTER) {
    if (worldmodel.ball.known_pos()) {
      Vector position = worldmodel.ball.get_pos() ;
      if (fabs(-52.5 - worldmodel.ball.get_pos().get_x()) <= 2.0) {
	if (fabs(-34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	  position.set_x(position.get_x() - 1.5) ;
	  position.set_y(position.get_y() - 1.5) ;
	} else if (fabs(34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	  position.set_x(position.get_x() - 1.5) ;
	  position.set_y(position.get_y() + 1.5) ;
	} else {
	  position.set_x(position.get_x() - 1.5) ;
	}
      } else if (fabs(52.5 - worldmodel.ball.get_pos().get_x()) <= 2.0) {
	if (fabs(-34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	  position.set_x(position.get_x() + 1.5) ;
	  position.set_y(position.get_y() - 1.5) ;
	} else if (fabs(34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	  position.set_x(position.get_x() + 1.5) ;
	  position.set_y(position.get_y() + 1.5) ;
	} else {
	  position.set_x(position.get_x() + 1.5) ;
	}
      } else {
	if (fabs(-34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	  position.set_y(position.get_y() - 1.5) ;
	} else if (fabs(34.0 - worldmodel.ball.get_pos().get_y()) <= 2.0) {
	  position.set_y(position.get_y() + 1.5) ;
	} else {
	  position.set_x(position.get_x() - 1.5) ;
	}
      }
      return position ;
    } else {
      return worldmodel.self.get_pos() ;
    }
  }

  number = 1 ;
  return pos[mode][number] ;  
}

/*
 * ==================================================
 * スキル
 * ==================================================
 */
bool MyAgent::move(Vector pos) {
  effector.set(CT_Move, pos.get_x(), pos.get_y()) ;
  return true ;
}

bool MyAgent::move(Vector pos, Vector vel, double max_power)
{
  return move(pos, vel, max_power, true) ;
}

bool MyAgent::move(Vector pos, Vector vel, double max_power, bool without_ball)
{
  Vector player_pos = worldmodel.self.get_pos() ;
  Vector player_vel = worldmodel.self.get_vel() ;
  double player_angle_body = worldmodel.self.get_angle_body() ;
  double stamina = worldmodel.get_stamina() ;
  double effort = worldmodel.get_effort() ;
  double recovery = worldmodel.get_recovery() ;

  if (vel.get_x() == VEL_ERR || vel.get_y() == VEL_ERR) {
    vel = Vector(0, 0) ;
  }

  Vector origin_pos, origin_vel, target_pos, target_vel;
  double power;
  double speed;
  double dist;
  double sta, eff, rec;
  double turn_body;
  double margin;

  // DDD...Kの場合
  origin_pos = player_pos;
  origin_vel = player_vel;
  target_pos = pos;
  target_vel = vel;
  dist = KICKABLE;
  speed = 0;
  sta = stamina;
  eff = effort;
  rec = recovery;
  turn_body = 0;
  // D
  if (sta < max_power)
    power = sta;
  else 
    power = max_power;
  speed += power * eff * DASH_POWER_RATE;
  if (speed > PLAYER_SPEED_MAX)
    speed = PLAYER_SPEED_MAX;
  dist += speed;
  speed *= PLAYER_DECAY;
  origin_pos += origin_vel;
  origin_vel *= PLAYER_DECAY;
  target_pos += target_vel;
  target_vel *= BALL_DECAY;
  sta -= power;
  if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
    if (rec > RECOVER_MIN)
      rec -= RECOVER_DEC;
  }
  if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
    if (eff > EFFORT_MIN)
      eff -= EFFORT_DEC;
  }
  if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
    if (eff < EFFORT_MAX) {
      eff += EFFORT_INC;
      if (eff > EFFORT_MAX)
	eff = EFFORT_MAX;
    }
  } 
  sta += rec * STAMINA_INC_MAX;
  if (sta > STAMINA_MAX)
    sta = STAMINA_MAX;
  // DD...K
  while (origin_pos.distance(target_pos) > dist) {
    if (sta < max_power)
      power = sta;
    else 
      power = max_power;
    speed += power * eff * DASH_POWER_RATE;
    if (speed > PLAYER_SPEED_MAX)
      speed = PLAYER_SPEED_MAX;
    dist += speed;
    speed *= PLAYER_DECAY;
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    sta -= power;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }

  margin = Asin(min(1.0, KICKABLE / origin_pos.distance(target_pos)));
  if (fabs(normalize_angle(origin_pos.direction(target_pos) - 
			   player_angle_body)) <= margin + turn_body) {
    if (!avoid_obstacle(origin_pos, target_pos, max_power, without_ball)) {
      effector.set(CT_Dash, max_power, 0) ;
    }
    return true;
  }

  // TDDD...Cの場合
  origin_pos = player_pos;
  origin_vel = player_vel;
  target_pos = pos;
  target_vel = vel;
  dist = KICKABLE;
  speed = 0;
  sta = stamina;
  eff = effort;
  rec = recovery;
  turn_body = 0;
  // T
  for (int i = 0; i < 1; i++) {
    turn_body += MAXMOMENT / (1.0 + INERTIA_MOMENT * origin_vel.r());
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }
  // D
  if (sta < max_power)
    power = sta;
  else 
    power = max_power;
  speed += power * eff * DASH_POWER_RATE;
  if (speed > PLAYER_SPEED_MAX)
    speed = PLAYER_SPEED_MAX;
  dist += speed;
  speed *= PLAYER_DECAY;
  origin_pos += origin_vel;
  origin_vel *= PLAYER_DECAY;
  target_pos += target_vel;
  target_vel *= BALL_DECAY;
  sta -= power;
  if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
    if (rec > RECOVER_MIN)
      rec -= RECOVER_DEC;
  }
  if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
    if (eff > EFFORT_MIN)
      eff -= EFFORT_DEC;
  }
  if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
    if (eff < EFFORT_MAX) {
      eff += EFFORT_INC;
      if (eff > EFFORT_MAX)
	eff = EFFORT_MAX;
    }
  } 
  sta += rec * STAMINA_INC_MAX;
  if (sta > STAMINA_MAX)
    sta = STAMINA_MAX;
  // DD...C
  while (origin_pos.distance(target_pos) > dist) {
    if (sta < max_power)
      power = sta;
    else 
      power = max_power;
    speed += power * eff * DASH_POWER_RATE;
    if (speed > PLAYER_SPEED_MAX)
      speed = PLAYER_SPEED_MAX;
    dist += speed;
    speed *= PLAYER_DECAY;
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    sta -= power;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }
  margin = Asin(min(1.0, KICKABLE / origin_pos.distance(target_pos)));
  if (fabs(normalize_angle(origin_pos.direction(target_pos) - 
			   player_angle_body)) <= margin + turn_body) {
    if (!avoid_obstacle(origin_pos, target_pos, max_power, without_ball)) {
      effector.set(CT_Turn, 
		   normalize_angle(origin_pos.direction(target_pos) - player_angle_body) * (1.0 + INERTIA_MOMENT * player_vel.r()),
		   0) ;
    }
    return true;
  }

  // TTDDD...Cの場合
  origin_pos = player_pos;
  origin_vel = player_vel;
  target_pos = pos;
  target_vel = vel;
  dist = KICKABLE;
  speed = 0;
  sta = stamina;
  eff = effort;
  rec = recovery;
  turn_body = 0;
  // TT
  for (int i = 0; i < 2; i++) {
    turn_body += MAXMOMENT / (1.0 + INERTIA_MOMENT * origin_vel.r());
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }
  // D
  if (sta < max_power)
    power = sta;
  else 
    power = max_power;
  speed += power * eff * DASH_POWER_RATE;
  if (speed > PLAYER_SPEED_MAX)
    speed = PLAYER_SPEED_MAX;
  dist += speed;
  speed *= PLAYER_DECAY;
  origin_pos += origin_vel;
  origin_vel *= PLAYER_DECAY;
  target_pos += target_vel;
  target_vel *= BALL_DECAY;
  sta -= power;
  if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
    if (rec > RECOVER_MIN)
      rec -= RECOVER_DEC;
  }
  if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
    if (eff > EFFORT_MIN)
      eff -= EFFORT_DEC;
  }
  if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
    if (eff < EFFORT_MAX) {
      eff += EFFORT_INC;
      if (eff > EFFORT_MAX)
	eff = EFFORT_MAX;
    }
  } 
  sta += rec * STAMINA_INC_MAX;
  if (sta > STAMINA_MAX)
    sta = STAMINA_MAX;
  // DD...C
  while (origin_pos.distance(target_pos) > dist) {
    if (sta < max_power)
      power = sta;
    else 
      power = max_power;
    speed += power * eff * DASH_POWER_RATE;
    if (speed > PLAYER_SPEED_MAX)
      speed = PLAYER_SPEED_MAX;
    dist += speed;
    speed *= PLAYER_DECAY;
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    sta -= power;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }
  margin = Asin(min(1.0, KICKABLE / origin_pos.distance(target_pos)));
  if (fabs(normalize_angle(origin_pos.direction(target_pos) - 
			   player_angle_body)) <= margin + turn_body) {
    if (!avoid_obstacle(origin_pos, target_pos, max_power, without_ball)) {
      effector.set(CT_Turn, 
		   normalize_angle(origin_pos.direction(target_pos) - player_angle_body) * (1.0 + INERTIA_MOMENT * player_vel.r()),
		   0) ;
    }
    return true;
  }

  // TTTDDD...Cの場合
  origin_pos = player_pos;
  origin_vel = player_vel;
  target_pos = pos;
  target_vel = vel;
  dist = KICKABLE;
  speed = 0;
  sta = stamina;
  eff = effort;
  rec = recovery;
  turn_body = 0;
  // TTT
  for (int i = 0; i < 3; i++) {
    turn_body += MAXMOMENT / (1.0 + INERTIA_MOMENT * origin_vel.r());
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }
  // D
  if (sta < max_power)
    power = sta;
  else 
    power = max_power;
  speed += power * eff * DASH_POWER_RATE;
  if (speed > PLAYER_SPEED_MAX)
    speed = PLAYER_SPEED_MAX;
  dist += speed;
  speed *= PLAYER_DECAY;
  origin_pos += origin_vel;
  origin_vel *= PLAYER_DECAY;
  target_pos += target_vel;
  target_vel *= BALL_DECAY;
  sta -= power;
  if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
    if (rec > RECOVER_MIN)
      rec -= RECOVER_DEC;
  }
  if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
    if (eff > EFFORT_MIN)
      eff -= EFFORT_DEC;
  }
  if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
    if (eff < EFFORT_MAX) {
      eff += EFFORT_INC;
      if (eff > EFFORT_MAX)
	eff = EFFORT_MAX;
    }
  } 
  sta += rec * STAMINA_INC_MAX;
  if (sta > STAMINA_MAX)
    sta = STAMINA_MAX;
  // DD...C
  while (origin_pos.distance(target_pos) > dist) {
    if (sta < max_power)
      power = sta;
    else 
      power = max_power;
    speed += power * eff * DASH_POWER_RATE;
    if (speed > PLAYER_SPEED_MAX)
      speed = PLAYER_SPEED_MAX;
    dist += speed;
    speed *= PLAYER_DECAY;
    origin_pos += origin_vel;
    origin_vel *= PLAYER_DECAY;
    target_pos += target_vel;
    target_vel *= BALL_DECAY;
    sta -= power;
    if (sta <= RECOVER_DEC_THR * STAMINA_MAX) {
      if (rec > RECOVER_MIN)
	rec -= RECOVER_DEC;
    }
    if (sta <= EFFORT_DEC_THR * STAMINA_MAX) {
      if (eff > EFFORT_MIN)
	eff -= EFFORT_DEC;
    }
    if (sta <= EFFORT_INC_THR * STAMINA_MAX) {
      if (eff < EFFORT_MAX) {
	eff += EFFORT_INC;
	if (eff > EFFORT_MAX)
	  eff = EFFORT_MAX;
      }
    } 
    sta += rec * STAMINA_INC_MAX;
    if (sta > STAMINA_MAX)
      sta = STAMINA_MAX;
  }
  margin = Asin(min(1.0, KICKABLE / origin_pos.distance(target_pos)));
  if (fabs(normalize_angle(origin_pos.direction(target_pos) - 
			   player_angle_body)) <= margin + turn_body) {
    if (!avoid_obstacle(origin_pos, target_pos, max_power, without_ball)) {
      effector.set(CT_Turn, 
		   normalize_angle(origin_pos.direction(target_pos) - player_angle_body) * (1.0 + INERTIA_MOMENT * player_vel.r()),
		   0) ;
    }
    return true;
  }

  if (!avoid_obstacle(origin_pos, target_pos, max_power, without_ball)) {
    effector.set(CT_Turn, 
		 normalize_angle(origin_pos.direction(target_pos) - player_angle_body) * (1.0 + INERTIA_MOMENT * player_vel.r()),
		   0) ;
  }  
  return true;
}

bool MyAgent::avoid_obstacle(Vector origin, Vector target, double max_power)
{
  avoid_obstacle(origin, target, max_power, true) ;
}

bool MyAgent::avoid_obstacle(Vector origin, Vector target, double max_power, bool without_ball)
{
  double dir;
  double dist;
  double min_dist;
  double target_dir;
  Vector rotate_target; 
  Vector obstacle;
  Vector pos;

  min_dist = 5.0;
  dir = origin.direction(target);
  rotate_target = target.rotate(origin, -dir);
  if (!without_ball) {
    if (worldmodel.ball.known_pos()) {
      pos = worldmodel.ball.get_pos().rotate(origin, -dir);
      if (fabs(pos.get_y() - origin.get_y()) <= PLAYER_SIZE + BALL_SIZE) {
	if (origin.get_x() <= pos.get_x() && pos.get_x() <= rotate_target.get_x()) {
	  dist = origin.distance(worldmodel.ball.get_pos());
	  if (dist < min_dist) {
	    min_dist = dist;
	    obstacle = pos;
	  }
	}
      }
    }
  }
  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
    if (!worldmodel.teammate[i].known_pos())
      continue;
    pos = worldmodel.teammate[i].get_pos().rotate(origin, -dir);
    if (fabs(pos.get_y() - origin.get_y()) > PLAYER_SIZE * 2.0)
       continue;
    if (pos.get_x() < origin.get_x() || rotate_target.get_x() < pos.get_x())
      continue;
    dist = origin.distance(worldmodel.teammate[i].get_pos());
    if (dist < min_dist) {
      min_dist = dist;
      obstacle = pos;
    }
  }
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!worldmodel.opponent[i].known_pos())
      continue;
    pos = worldmodel.opponent[i].get_pos().rotate(origin, -dir);
    if (fabs(pos.get_y() - origin.get_y()) > PLAYER_SIZE * 2.0)
       continue;
    if (pos.get_x() < origin.get_x() || rotate_target.get_x() < pos.get_x())
      continue;
    dist = origin.distance(worldmodel.opponent[i].get_pos());
    if (dist < min_dist) {
      min_dist = dist;
      obstacle = pos;
    }
  }

  if (min_dist == 5.0) {
    return false;
  } else {
    dist = origin.distance(obstacle);
    dist = max(PLAYER_SIZE * 2.0, dist);
    target_dir = Asin(PLAYER_SIZE * 2.0 / dist);
    if (origin.get_y() < obstacle.get_y())
      target_dir *= -1.0;

    target = origin + polar2vector(50.0, target_dir);
    target = target.rotate(origin, dir);

    double margin = Asin(KICKABLE / origin.distance(target));
    if (fabs(normalize_angle(origin.direction(target) - 
			     worldmodel.self.get_angle_body())) <= margin) {
      effector.set(CT_Dash, max_power, 0) ;
    } else {
      effector.set(CT_Turn, 
		   normalize_angle(origin.direction(target) - worldmodel.self.get_angle_body()) * (1.0 + INERTIA_MOMENT * worldmodel.self.get_vel().r()),
		   0) ;
    }    
    return true;
  }
}

bool MyAgent::multi_kick(Vector target, double require_speed) {
  static bool right = false ;
  static bool left = false ;

  if (single_kick(target, require_speed)) {
    right = false ;
    left = false ;
    return true ;
  }

  Vector player_pos = worldmodel.self.get_pos() ;
  Vector player_vel = worldmodel.self.get_vel() ;
  double player_angle_body = worldmodel.self.get_angle_body() ;
  Vector ball_pos = worldmodel.ball.get_pos() ;
  Vector ball_vel = worldmodel.ball.get_vel() ;

  Vector next_player_pos = player_pos + player_vel ;
  double dist_diff = (player_pos.distance(ball_pos) - PLAYER_SIZE - BALL_SIZE) / KICKABLE_MARGIN ;
  dist_diff = min(max(0.0, dist_diff), 1.0) ;
  double dir_diff = fabs(normalize_angle(player_pos.direction(ball_pos) - player_angle_body) / MAXMOMENT) ;
  double effective_kick_power_rate = KICK_POWER_RATE * (1.0 - 0.25 * (dist_diff) - 0.25 * (dir_diff)) ;  

  Vector require_vel, next_ball_pos ;
  double speed, sdir ;

  if (ball_vel.r() >= MAXPOWER * KICK_POWER_RATE * 0.625 * 0.8 &&
      fabs(normalize_angle(ball_vel.th() - ball_pos.direction(target))) <= 15.0) {
    right = false ;
    left = false ;
    speed = ball_pos.distance(target) * (1 - BALL_DECAY) + require_speed ;
    sdir = ball_pos.direction(target) ;
    require_vel = polar2vector(speed, sdir) - ball_vel ;
    
    next_ball_pos = ball_pos + polar2vector(speed, sdir) ;

    if (player_pos.distance(next_ball_pos) > PLAYER_SIZE + BALL_SIZE * 1.1 &&
	next_player_pos.distance(next_ball_pos) > PLAYER_SIZE + BALL_SIZE * 1.1) {
      if (require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
	effector.set(CT_Kick,
	require_vel.r() / effective_kick_power_rate,
	normalize_angle(require_vel.th() - player_angle_body)) ;
	return true ;
      } else {
	effector.set(CT_Kick, MAXPOWER, normalize_angle(require_vel.th() - player_angle_body)) ;
	return true ;
      }
    }
  }
  
  next_ball_pos = ball_pos + ball_vel + polar2vector(MAXPOWER * effective_kick_power_rate, ball_pos.direction(target)) ;

  if (ball_pos.distance(next_ball_pos) >= 
      MAXPOWER * KICK_POWER_RATE * 0.625 &&
      fabs(normalize_angle(ball_pos.direction(next_ball_pos) - 
			   next_ball_pos.direction(target))) <= 15.0 * 0.8 &&
      player_pos.distance(next_ball_pos) > PLAYER_SIZE + BALL_SIZE * 1.1 &&
      next_player_pos.distance(next_ball_pos) > 
      PLAYER_SIZE + BALL_SIZE * 1.1 &&
      next_player_pos.distance(next_ball_pos) <= KICKABLE * 0.9) {
    effector.set(CT_Kick, MAXPOWER, normalize_angle(ball_pos.direction(target) - player_angle_body)) ;
    return true ;
  }

  double min_dist = 1000.0;
  double tmp_dist;
  Vector enemy_pos = Vector(52.5, 0.0);
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!worldmodel.opponent[i].known_pos())
      continue;
    tmp_dist = player_pos.distance(worldmodel.opponent[i].get_pos()) ;
    if (tmp_dist < min_dist) {
      min_dist = tmp_dist;
      enemy_pos = worldmodel.opponent[i].get_pos() ;
    }
  }

  if (right) {
    return turn_ball(normalize_angle(player_pos.direction(target) -
			      135.0 - player_pos.direction(ball_pos))) ;
  } else if (left) {
    return turn_ball(normalize_angle(player_pos.direction(target) +
			      135.0 - player_pos.direction(ball_pos))) ;
  } else if (fabs(normalize_angle(player_pos.direction(target) + 135.0 - 
			   player_pos.direction(enemy_pos))) <
      fabs(normalize_angle(player_pos.direction(target) - 135.0 - 
			   player_pos.direction(enemy_pos)))) {
    right = true ;
    return turn_ball(normalize_angle(player_pos.direction(target) -
			      135.0 - player_pos.direction(ball_pos)));
  } else {
    left = true ;
    return turn_ball(normalize_angle(player_pos.direction(target) +
			      135.0 - player_pos.direction(ball_pos)));
  }
}

// ワンタッチキック
bool MyAgent::single_kick(Vector target, double require_speed) {
  // worldmodelからの初期化
  Vector self_pos = worldmodel.self.get_pos() ;
  Vector self_vel = worldmodel.self.get_vel() ;
  double self_angle_body = worldmodel.self.get_angle_body() ;
  Vector ball_pos = worldmodel.ball.get_pos() ;
  Vector ball_vel;
  if (worldmodel.ball.known_vel())
    ball_vel = worldmodel.ball.get_vel() ;
  else
    ball_vel = Vector(0, 0) ;

  // next_self_posを取得
  Vector next_self_pos = self_pos + self_vel ;
  
  // effective_kick_power_rateを取得
  double dist_diff = (self_pos.distance(ball_pos) - PLAYER_SIZE - BALL_SIZE) / KICKABLE_MARGIN ;
  dist_diff = min(max(0.0, dist_diff), 1.0) ;
  double dir_diff = fabs(normalize_angle(self_pos.direction(ball_pos) - self_angle_body) / MAXMOMENT) ;
  double effective_kick_power_rate = KICK_POWER_RATE * (1.0 - 0.25 * (dist_diff) - 0.25 * (dir_diff)) ;  

  // require_velを取得
  double speed = ball_pos.distance(target) * (1.0 - BALL_DECAY) + require_speed ;
  speed = min(speed, BALL_SPEED_MAX) ;
  double sdir = ball_pos.direction(target) ;
  Vector require_vel = polar2vector(speed, sdir) - ball_vel ;

  Vector next_ball_pos ;
  if (require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
    // next_ball_posを取得
    next_ball_pos = ball_pos + polar2vector(speed, sdir) ;

    if (self_pos.distance(next_ball_pos) >= PLAYER_SIZE + BALL_SIZE &&
	next_self_pos.distance(next_ball_pos) >= PLAYER_SIZE + BALL_SIZE) {
      // ワンタッチキックできる
      effector.set(CT_Kick,
		   require_vel.r() / effective_kick_power_rate,
		   normalize_angle(require_vel.th() - self_angle_body)) ;
      return true ;
    } else {
      // ワンタッチキックできない
      return false ;
    }
  } else {
    // next_ball_posを取得
    next_ball_pos = ball_pos + ball_vel + 
      polar2vector(MAXPOWER * effective_kick_power_rate, require_vel.th()) ;

    if (require_vel.r() * 0.75 <= MAXPOWER * effective_kick_power_rate &&
	fabs(normalize_angle(ball_pos.direction(next_ball_pos) - require_vel.th())) <= 18.0 &&
	self_pos.distance(next_ball_pos) >= PLAYER_SIZE + BALL_SIZE &&
	next_self_pos.distance(next_ball_pos) >= PLAYER_SIZE + BALL_SIZE) {
      // ワンタッチキックできる
      effector.set(CT_Kick,
		   require_vel.r() / effective_kick_power_rate,
		   normalize_angle(require_vel.th() - self_angle_body)) ;
      return true ;
    } else {
      // ワンタッチキックできない
      return false ;
    }
  }
}

bool MyAgent::turn_ball(double dir) {
  Vector player_pos = worldmodel.self.get_pos() ;
  Vector player_vel = worldmodel.self.get_vel() ;
  double player_angle_body = worldmodel.self.get_angle_body() ;
  Vector ball_pos = worldmodel.ball.get_pos() ;
  Vector ball_vel = worldmodel.ball.get_vel() ;

  Vector next_player_pos = player_pos + player_vel ;
  double dist_diff = (player_pos.distance(ball_pos) - PLAYER_SIZE - BALL_SIZE) / KICKABLE_MARGIN ;
  dist_diff = min(max(0.0, dist_diff), 1.0) ;
  double dir_diff = fabs(normalize_angle(player_pos.direction(ball_pos) - player_angle_body) / MAXMOMENT) ;
  double effective_kick_power_rate = KICK_POWER_RATE * (1.0 - 0.25 * (dist_diff) - 0.25 * (dir_diff)) ;  

  Vector require_vel, next_ball_pos ;
  double speed, sdir ;
  double tmp_dir ;

  tmp_dir = normalize_angle(player_pos.direction(ball_pos) + dir) ;
  next_ball_pos = next_player_pos + polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN / 2.0, tmp_dir) ;
  speed = ball_pos.distance(next_ball_pos) ;
  if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
    sdir = ball_pos.direction(next_ball_pos) ;
    require_vel = polar2vector(speed, sdir) - ball_vel ;
    
    if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1 &&
	next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1 &&
	require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
      effector.set(CT_Kick, 
		   require_vel.r() / effective_kick_power_rate, 
		   normalize_angle(require_vel.th() - player_angle_body)) ;
      return true ;
    }
  }

  double inc_dir = 5.0 ;
  int i = 0 ;
  while (inc_dir < fabs(dir)) {
    tmp_dir = normalize_angle(player_pos.direction(ball_pos) + dir + inc_dir) ;
    next_ball_pos = next_player_pos + polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN / 2.0, tmp_dir) ;
    speed = ball_pos.distance(next_ball_pos) ;
    if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
      sdir = ball_pos.direction(next_ball_pos) ;
      require_vel = polar2vector(speed, sdir) - ball_vel;
      
      if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
        effector.set(CT_Kick, 
		     require_vel.r() / effective_kick_power_rate,
		     normalize_angle(require_vel.th() - player_angle_body)) ;
	return true ;
      }
    }
    tmp_dir = normalize_angle(player_pos.direction(ball_pos) + dir - inc_dir) ;
    next_ball_pos = next_player_pos + polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN / 2.0, tmp_dir) ;
    speed = ball_pos.distance(next_ball_pos) ;
    if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
      sdir = ball_pos.direction(next_ball_pos);
      require_vel = polar2vector(speed, sdir) - ball_vel;
      
      if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
	effector.set(CT_Kick, 
		     require_vel.r() / effective_kick_power_rate, 
		     normalize_angle(require_vel.th() - player_angle_body)) ;
	return true ;
      }
    }
    inc_dir += 5.0 * i;
    i++;
  }

  tmp_dir = player_pos.direction(ball_pos);
  next_ball_pos = next_player_pos + polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN / 2.0, tmp_dir);
  speed = ball_pos.distance(next_ball_pos);
  if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
    sdir = ball_pos.direction(next_ball_pos);
    require_vel = polar2vector(speed, sdir) - ball_vel;
    
    if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
      effector.set(CT_Kick, 
		   require_vel.r() / effective_kick_power_rate,
		   normalize_angle(require_vel.th() - player_angle_body)) ;
      return true ;
    }
  }

  // 外
  tmp_dir = normalize_angle(player_pos.direction(ball_pos) + dir);
  next_ball_pos = next_player_pos + polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN * 0.8, tmp_dir);
  speed = ball_pos.distance(next_ball_pos);
  if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
    sdir = ball_pos.direction(next_ball_pos);
    require_vel = polar2vector(speed, sdir) - ball_vel;
    
    if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1 &&
	next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1 &&
	require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
      effector.set(CT_Kick,
		   require_vel.r() / effective_kick_power_rate,
		   normalize_angle(require_vel.th() - player_angle_body)) ;
      return true ;
    }
  }

  inc_dir = 5.0 ;
  i = 0 ;
  while (inc_dir < fabs(dir)) {
    tmp_dir = normalize_angle(player_pos.direction(ball_pos) + dir + inc_dir);
    next_ball_pos = next_player_pos + polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN * 0.8, tmp_dir) ;
    speed = ball_pos.distance(next_ball_pos) ;
    if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
      sdir = ball_pos.direction(next_ball_pos) ;
      require_vel = polar2vector(speed, sdir) - ball_vel ;
      
      if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
	effector.set(CT_Kick, 
		     require_vel.r() / effective_kick_power_rate, 
		     normalize_angle(require_vel.th() - player_angle_body)) ;
	return true ;
      }
    }
    tmp_dir = normalize_angle(player_pos.direction(ball_pos) + dir - inc_dir) ;
    next_ball_pos = next_player_pos + 
      polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN * 0.8, tmp_dir) ;
    speed = ball_pos.distance(next_ball_pos) ;
    if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
      sdir = ball_pos.direction(next_ball_pos) ;
      require_vel = polar2vector(speed, sdir) - ball_vel ;
      
      if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	  require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
	effector.set(CT_Kick,
		     require_vel.r() / effective_kick_power_rate,
		     normalize_angle(require_vel.th() - player_angle_body)) ;
	return true ;
      }
    }
    inc_dir += 5.0 * i ;
    i++ ;
  }

  tmp_dir = player_pos.direction(ball_pos) ;
  next_ball_pos = next_player_pos + 
    polar2vector(PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN * 0.8, tmp_dir) ;
  speed = ball_pos.distance(next_ball_pos) ;
  if (speed <= MAXPOWER * KICK_POWER_RATE / 2.0 / BALL_DECAY * 0.8) {
    sdir = ball_pos.direction(next_ball_pos) ;
    require_vel = polar2vector(speed, sdir) - ball_vel ;
    
    if (player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	next_player_pos.distance(next_ball_pos) > (PLAYER_SIZE + BALL_SIZE) * 1.1  &&
	require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
      effector.set(CT_Kick,
		   require_vel.r() / effective_kick_power_rate,
		   normalize_angle(require_vel.th() - player_angle_body)) ;
      return true ;
    }
  }

  return trap_ball() ;
}

bool MyAgent::trap_ball(void) {
  Vector player_pos = worldmodel.self.get_pos() ;
  Vector player_vel = worldmodel.self.get_vel() ;
  double player_angle_body = worldmodel.self.get_angle_body() ;
  Vector ball_pos = worldmodel.ball.get_pos() ;
  Vector ball_vel = worldmodel.ball.get_vel() ;

  Vector next_player_pos ;
  double dist_diff = (player_pos.distance(ball_pos) - PLAYER_SIZE - BALL_SIZE) / KICKABLE_MARGIN ;
  dist_diff = min(max(0.0, dist_diff), 1.0) ;
  double dir_diff = fabs(normalize_angle(player_pos.direction(ball_pos) - player_angle_body) / MAXMOMENT) ;
  double effective_kick_power_rate = KICK_POWER_RATE * (1.0 - 0.25 * (dist_diff) - 0.25 * (dir_diff)) ; 

  Vector require_vel, next_ball_pos ;
  double speed, sdir ;

  next_player_pos = player_pos + player_vel ;
  speed = ball_pos.distance(next_player_pos) - PLAYER_SIZE - BALL_SIZE ;
  sdir = ball_pos.direction(next_player_pos) ;
  require_vel = polar2vector(speed, sdir) - ball_vel ;

  if (require_vel.r() <= MAXPOWER * effective_kick_power_rate) {
    effector.set(CT_Kick, 
		 require_vel.r() / effective_kick_power_rate, 
		 normalize_angle(require_vel.th() - player_angle_body)) ;
    return true ;
  }

  speed = ball_pos.distance(player_pos) - PLAYER_SIZE - BALL_SIZE ;
  sdir = ball_pos.direction(player_pos) ;
  require_vel = polar2vector(speed, sdir) - ball_vel ;
    
  if (require_vel.r() <= MAXPOWER * effective_kick_power_rate &&
      next_player_pos.distance(ball_pos + polar2vector(speed, sdir)) <= KICKABLE) {
    effector.set(CT_Kick,
		 require_vel.r() / effective_kick_power_rate,
		 normalize_angle(require_vel.th() - player_angle_body)) ;
    return true ;
  }

  next_player_pos = player_pos + player_vel / 2.0 ;
  speed = ball_pos.distance(next_player_pos) ;
  sdir = ball_pos.direction(next_player_pos) ;
  require_vel = polar2vector(speed, sdir) - ball_vel ;
  next_ball_pos = ball_pos + ball_vel +
    polar2vector(MAXPOWER * effective_kick_power_rate, require_vel.th()) ;
  next_player_pos = player_pos + player_vel ;
  if (player_pos.distance(next_ball_pos) <= KICKABLE &&
      next_player_pos.distance(next_ball_pos) <= KICKABLE) {
    effector.set(CT_Kick, MAXPOWER, normalize_angle(require_vel.th() - player_angle_body)) ;
    return true ;
  }

  effector.set(CT_Kick, MAXPOWER, normalize_angle(ball_vel.th() - 180.0 - player_angle_body)) ;
  return true ;
}

// ボールを見る
// 自分の体をボールの方向へ向ける
bool MyAgent::look_ball(void) {
  // ボールの位置がわからない時はボールを探す
  if (!worldmodel.ball.known_pos()) 
    return look_for_ball() ;

  // ボールの位置を取得
  Vector ball_pos ;
  if (worldmodel.get_playmode() == PM_BeforeKickOff ||
      worldmodel.get_playmode() == PM_AfterGoal_Left ||
      worldmodel.get_playmode() == PM_AfterGoal_Right)
    ball_pos = Vector(0, 0) ;
  else 
    ball_pos = worldmodel.ball.get_pos() ;

  // コマンドを更新
  effector.set(CT_Turn, 
	       normalize_angle(worldmodel.self.get_pos().direction(ball_pos) - 
			       worldmodel.self.get_angle_body()) * 
	       (1.0 + INERTIA_MOMENT * worldmodel.self.get_vel().r()), 
	       0.0) ;
  return true ;
}

// ボールを探す(デフォルト用)
// 相手フィールドの方向を向く
bool MyAgent::look_for_ball(void) {
  // コマンドを更新
  effector.set(CT_Turn, 
	       normalize_angle(worldmodel.self.get_pos().direction(Vector(0.0, 0.0)) - 
			       worldmodel.self.get_angle_body()) * 
	       (1.0 + INERTIA_MOMENT * worldmodel.self.get_vel().r()), 
	       0.0) ;
  return true ;
}



