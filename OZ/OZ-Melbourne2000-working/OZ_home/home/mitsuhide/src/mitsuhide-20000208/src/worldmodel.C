#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "networkinterface.h"
#include "mymath.h"
#include "vector.h"
#include "utility.h"
#include "seeinfo.h"
#include "param.h"
#include "commandinfo.h"
#include "worldmodel.h"

/*
 * ==================================================
 * Part : Movable Object
 * ==================================================
 */
// コンストラクタ
MovableObject::MovableObject(void) {
  pos = Vector(POS_ERR, POS_ERR) ;
  vel = Vector(VEL_ERR, VEL_ERR) ;
}

// リセット
void MovableObject::reset(void) {
  pos = Vector(POS_ERR, POS_ERR) ;
  vel = Vector(VEL_ERR, VEL_ERR) ;  
}

// 位置を知っているかの判定
bool MovableObject::known_pos(void) { 
  if (pos.get_x() != POS_ERR && pos.get_y() != POS_ERR)
    return true;
  else 
    return false;
}

// 速度を知っているかの判定
bool MovableObject::known_vel(void) { 
  if (vel.get_x() != VEL_ERR && vel.get_y() != VEL_ERR)
    return true;
  else 
    return false;
}

// 未整理
double MovableObject::distance(MovableObject obj) {
  return pos.distance(obj.pos);
}

double MovableObject::direction(MovableObject obj) {
  return pos.direction(obj.pos);
}

/*
 * ==================================================
 * Part : Ball
 * ==================================================
 */
// コンストラクタ
Ball::Ball(void) {
  acc = Vector(0, 0);
  acc_error = Vector(0, 0);
}

// 初期化
void Ball::init(void) {
  acc = Vector(0, 0) ;
  acc_error = Vector(0, 0) ;
}

// リセット
void Ball::reset(void) {
  MovableObject::reset();
}

void Ball::normalize_speed(void)
{
  /* 変数宣言 */
  double x1, x2;
  double y1, y2;
  Vector v1, v2, v3, v4, v5, v6, v7, v8;
  double tmp;
  Vector max, min;
  
  /* 初期化1 */
  x1 = vel.x + vel_error.x;
  x2 = vel.x - vel_error.x;
  y1 = vel.y + vel_error.y;
  y2 = vel.y - vel_error.y;

  /* 初期化2 */
  v1 = Vector(x1, y1);
  v2 = Vector(x1, y2);
  v3 = Vector(x2, y1);
  v4 = Vector(x2, y2);

  /* 初期化3 */
  if (x2 < 0 && 0 < x1) {
    v5 = Vector(0, y1);
    v6 = Vector(0, y2);
  } else {
    v5 = vel;
    v6 = vel;
  }
  if (y2 < 0 && 0 < y2) {
    v7 = Vector(x1, 0);
    v8 = Vector(x2, 0);
  } else {
    v7 = vel;
    v8 = vel;
  }
  
  /* player_speed_maxで切り落とす */
  if ((tmp = v1.r()) > BALL_SPEED_MAX)
    v1 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v2.r()) > BALL_SPEED_MAX) 
    v2 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v3.r()) > BALL_SPEED_MAX) 
    v3 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v4.r()) > BALL_SPEED_MAX) 
    v4 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v5.r()) > BALL_SPEED_MAX) 
    v5 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v6.r()) > BALL_SPEED_MAX) 
    v6 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v7.r()) > BALL_SPEED_MAX) 
    v7 *= (BALL_SPEED_MAX / tmp);
  if ((tmp = v8.r()) > BALL_SPEED_MAX) 
    v8 *= (BALL_SPEED_MAX / tmp);
  
  /* 座標の最大・最小値を求める */
  max = Vector(max(max(max(v1.x, v2.x),max(v3.x, v4.x)),
		   max(max(v5.x, v6.x),max(v7.x, v8.x))),
	       max(max(max(v1.y, v2.y),max(v3.y, v4.y)),
		   max(max(v5.y, v6.y),max(v7.y, v8.y))));
  min = Vector(min(min(min(v1.x, v2.x),min(v3.x, v4.x)),
		   min(min(v5.x, v6.x),min(v7.x, v8.x))),
	       min(min(min(v1.y, v2.y),min(v3.y, v4.y)),
		   min(min(v5.y, v6.y),min(v7.y, v8.y))));

  /* 速度の更新 */
  vel = (max + min) / 2.0;
  vel_error = (max - min) / 2.0;
}

void Ball::noise(void)
{
  /* 変数宣言 */
  double x1, x2;
  double y1, y2;
  Vector v1, v2, v3, v4;
  Vector max, min;
  
  /* 初期化1 */
  x1 = vel.x + vel_error.x;
  x2 = vel.x - vel_error.x;
  y1 = vel.y + vel_error.y;
  y2 = vel.y - vel_error.y;

  /* 初期化2 */
  v1 = Vector(x1, y1);
  v2 = Vector(x1, y2);
  v3 = Vector(x2, y1);
  v4 = Vector(x2, y2);

  /* 座標の最大・最小値を求める */
  max = Vector(max(x1 + v1.r() * BALL_RAND, x1 + v2.r() * BALL_RAND), 
	       max(y1 + v1.r() * BALL_RAND, y1 + v3.r() * BALL_RAND));
  min = Vector(min(x2 - v3.r() * BALL_RAND, x2 - v4.r() * BALL_RAND), 
	       min(y2 - v2.r() * BALL_RAND, y2 - v4.r() * BALL_RAND));  

  /* 速度の更新 */
  vel = (max + min) / 2.0;
  vel_error = (max - min) / 2.0;
}

void Ball::_inc(Vector player_pos, Vector next_player_pos)
{
  Vector past_ball_pos = pos;
  if (acc.x || acc.y) {
    if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
      vel += acc;
      vel_error += acc_error;
    }
    normalize_speed();
  }
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) { 
    noise();
  }
  if (pos.x != POS_ERR && pos.y != POS_ERR && 
      vel.x != VEL_ERR && vel.y != VEL_ERR) {
    pos += vel;
    pos_error += vel_error;
  }
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
    vel *= BALL_DECAY;
    vel_error *= BALL_DECAY;
  }
  // 衝突
  if (pos.x != POS_ERR && pos.y != POS_ERR && 
      vel.x != VEL_ERR && vel.y != VEL_ERR &&
      past_ball_pos.x != POS_ERR && past_ball_pos.y != POS_ERR) {
    if (pos.distance(player_pos) <= PLAYER_SIZE + BALL_SIZE &&
	pos.distance(next_player_pos) <= PLAYER_SIZE + BALL_SIZE &&
	past_ball_pos.distance(next_player_pos) > PLAYER_SIZE + BALL_SIZE) {
	double r = PLAYER_SIZE + BALL_SIZE;
	Vector dif = pos - player_pos;
	double d = pos.distance(player_pos);
	double th = 
	  fabs(normalize_angle(Atan(dif.y, dif.x) - Atan(vel.y, vel.x)));
	double l1 = d * Cos(th);
	double h = d * Sin(th);
	double cosp = h / r;
	double sinp = sqrt(1.0 - square(cosp));
	double l2 = r * sinp;
	Vector dv1 = vel;

        dv1 *= ((-(l1 + l2))/max(dv1.r(),EPS));

	dif = pos - next_player_pos;
	d = pos.distance(next_player_pos);
	th = 
	  fabs(normalize_angle(Atan(dif.y, dif.x) - Atan(vel.y, vel.x)));
	l1 = d * Cos(th);
	h = d * Sin(th);
	cosp = h / r;
	sinp = sqrt(1.0 - square(cosp));
	l2 = r * sinp;
        Vector dv2 = vel;

        dv2 *= ((-(l1 + l2))/max(dv2.r(),EPS));

	pos += ((dv1 + dv2) / 2.0);
        vel *= -0.1;
    }
  }
}

void Ball::_turn(void)
{
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) { 
    noise();
  }
  if (pos.x != POS_ERR && pos.y != POS_ERR && 
      vel.x != VEL_ERR && vel.y != VEL_ERR) {
    pos += vel;
    pos_error += vel_error;
  }
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
    vel *= BALL_DECAY;
    vel_error *= BALL_DECAY;
  }
}

/*
 * ==================================================
 * Part : Self
 * ==================================================
 */
// コンストラクタ
Self::Self(void) {
  acc = Vector(0, 0);
  acc_error = Vector(0, 0);
  angle_body = ANGLE_ERR;
  angle_neck = ANGLE_ERR;
  turn_body = 0 ;
  turn_body_error = 0 ;
  turn_neck = 0 ;
} 

// 初期化
void Self::init(Side side, int unum) {
  if (side == S_LEFT) {
    pos = Vector(-3.0 * unum, -HALF_PITCH_W - 3.0) ;
    angle_body = 0 ;
  } else if (side == S_RIGHT) {
    pos = Vector(-3.0 * unum, HALF_PITCH_W + 3.0) ;
    angle_body = 180 ;
  } else {
    fprintf(stderr, "Can't init self.\n") ;
    exit(0) ;
  }
  pos_error = Vector(0, 0) ;
  vel = Vector(0, 0) ;
  vel_error = Vector(0, 0) ;
  angle_body_error = 0 ;
  angle_neck = 0 ;
  angle_neck_error = 0 ;

  acc = Vector(0, 0) ;
  acc_error = Vector(0, 0) ;
  turn_body = 0 ;
  turn_body_error = 0 ;
  turn_neck = 0 ;
}

// リセット
void Self::reset(void) {
  MovableObject::reset() ;
  angle_body = ANGLE_ERR ;
  angle_neck = ANGLE_ERR ;
}

// 体の角度を知っているかの判定
bool Self::known_angle_body(void) {
  if (angle_body != ANGLE_ERR)
    return true;
  else 
    return false;
}

// 首の角度を知っているかの判定
bool Self::known_angle_neck(void) {
  if (angle_neck != ANGLE_ERR)
    return true;
  else 
    return false;
}

void Self::normalize_speed(Vector *vel, Vector *vel_error)
{
  /* 変数宣言 */
  double x1, x2;
  double y1, y2;
  Vector v1, v2, v3, v4, v5, v6, v7, v8;
  double tmp;
  Vector max, min;
  
  /* 初期化1 */
  x1 = vel->x + vel_error->x;
  x2 = vel->x - vel_error->x;
  y1 = vel->y + vel_error->y;
  y2 = vel->y - vel_error->y;

  /* 初期化2 */
  v1 = Vector(x1, y1);
  v2 = Vector(x1, y2);
  v3 = Vector(x2, y1);
  v4 = Vector(x2, y2);

  /* 初期化3 */
  if (x2 < 0 && 0 < x1) {
    v5 = Vector(0, y1);
    v6 = Vector(0, y2);
  } else {
    v5 = *vel;
    v6 = *vel;
  }
  if (y2 < 0 && 0 < y2) {
    v7 = Vector(x1, 0);
    v8 = Vector(x2, 0);
  } else {
    v7 = *vel;
    v8 = *vel;
  }
  
  /* player_speed_maxで切り落とす */
  if ((tmp = v1.r()) > PLAYER_SPEED_MAX)
    v1 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v2.r()) > PLAYER_SPEED_MAX) 
    v2 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v3.r()) > PLAYER_SPEED_MAX) 
    v3 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v4.r()) > PLAYER_SPEED_MAX) 
    v4 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v5.r()) > PLAYER_SPEED_MAX) 
    v5 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v6.r()) > PLAYER_SPEED_MAX) 
    v6 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v7.r()) > PLAYER_SPEED_MAX) 
    v7 *= (PLAYER_SPEED_MAX / tmp);
  if ((tmp = v8.r()) > PLAYER_SPEED_MAX) 
    v8 *= (PLAYER_SPEED_MAX / tmp);
  
  /* 座標の最大・最小値を求める */
  max = Vector(max(max(max(v1.x, v2.x),max(v3.x, v4.x)),
		   max(max(v5.x, v6.x),max(v7.x, v8.x))),
	       max(max(max(v1.y, v2.y),max(v3.y, v4.y)),
		   max(max(v5.y, v6.y),max(v7.y, v8.y))));
  min = Vector(min(min(min(v1.x, v2.x),min(v3.x, v4.x)),
		   min(min(v5.x, v6.x),min(v7.x, v8.x))),
	       min(min(min(v1.y, v2.y),min(v3.y, v4.y)),
		   min(min(v5.y, v6.y),min(v7.y, v8.y))));

  /* 速度の更新 */
  *vel = (max + min) / 2.0;
  *vel_error = (max - min) / 2.0;
}

void Self::noise(Vector *vel, Vector *vel_error)
{
  /* 変数宣言 */
  double x1, x2;
  double y1, y2;
  Vector v1, v2, v3, v4;
  Vector max, min;
  
  /* 初期化1 */
  x1 = vel->x + vel_error->x;
  x2 = vel->x - vel_error->x;
  y1 = vel->y + vel_error->y;
  y2 = vel->y - vel_error->y;

  /* 初期化2 */
  v1 = Vector(x1, y1);
  v2 = Vector(x1, y2);
  v3 = Vector(x2, y1);
  v4 = Vector(x2, y2);

  /* 座標の最大・最小値を求める */
  max = Vector(max(x1 + v1.r() * PLAYER_RAND, x1 + v2.r() * PLAYER_RAND), 
	       max(y1 + v1.r() * PLAYER_RAND, y1 + v3.r() * PLAYER_RAND));
  min = Vector(min(x2 - v3.r() * PLAYER_RAND, x2 - v4.r() * PLAYER_RAND), 
	       min(y2 - v2.r() * PLAYER_RAND, y2 - v4.r() * PLAYER_RAND));  

  /* 速度の更新 */
  *vel = (max + min) / 2.0;
  *vel_error = (max - min) / 2.0;
}

void Self::_inc(void) {
  if (acc.x || acc.y) {
    vel += acc;
    vel_error += acc_error;
    normalize_speed(&vel, &vel_error);
  } 
}

void Self::_turn(void) {
  angle_body = normalize_angle(angle_body + turn_body);
  angle_body_error = NormalizeAngleError(angle_body_error + turn_body_error); 
  angle_neck = NormalizeNeckAngle(angle_neck + turn_neck);
  noise(&vel, &vel_error);    
  pos += vel;
  pos_error += vel_error;
  vel *= PLAYER_DECAY;
  vel_error *= PLAYER_DECAY;
}


/*
 * ==================================================
 * Part : Player
 * ==================================================
 */
// コンストラクタ
Player::Player(void) {
  unum = UNUM_ERR;
  angle_body = ANGLE_ERR;
  angle_neck = ANGLE_ERR;
}

// リセット
void Player::reset(void) {
  MovableObject::reset() ;
  unum = UNUM_ERR ;
  angle_body = ANGLE_ERR ;
  angle_neck = ANGLE_ERR ;
}

// 背番号を知っているかの判定
bool Player::known_unum(void) {
  if (unum != UNUM_ERR)
    return true;
  else 
    return false;
}

// 体の角度を知っているかの判定
bool Player::known_angle_body(void) {
  if (angle_body != ANGLE_ERR)
    return true;
  else 
    return false;
}

// 首の角度を知っているかの判定
bool Player::known_angle_neck(void) {
  if (angle_neck != ANGLE_ERR)
    return true;
  else 
    return false;
}

void Player::lost(void)
{
  pos = Vector(POS_ERR, POS_ERR);
  vel = Vector(VEL_ERR, VEL_ERR);
  angle_body = ANGLE_ERR;
  angle_neck = ANGLE_ERR;
}

void Player::see(int unum, 
                 Vector pos, Vector pos_error, Vector vel, Vector vel_error,
                 double angle_body, double angle_body_error,
                 double angle_neck, double angle_neck_error)
{
  if (unum != UNUM_ERR)
    this->unum = unum;
  if (pos.x != POS_ERR && pos.y != POS_ERR) {
    this->pos = pos;
    this->pos_error = pos_error;
  }
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
    this->vel = vel;
    this->vel_error = vel_error;
  }
  if (angle_body != ANGLE_ERR) {
    this->angle_body = angle_body;
    this->angle_body_error = angle_body_error;
  }
  if (angle_neck != ANGLE_ERR) {
    this->angle_neck = angle_neck;
    this->angle_neck_error = angle_neck_error;
  }
}

void Player::noise(void)
{
  /* 変数宣言 */
  double x1, x2;
  double y1, y2;
  Vector v1, v2, v3, v4;
  Vector max, min;
  
  /* 初期化1 */
  x1 = vel.x + vel_error.x;
  x2 = vel.x - vel_error.x;
  y1 = vel.y + vel_error.y;
  y2 = vel.y - vel_error.y;

  /* 初期化2 */
  v1 = Vector(x1, y1);
  v2 = Vector(x1, y2);
  v3 = Vector(x2, y1);
  v4 = Vector(x2, y2);

  /* 座標の最大・最小値を求める */
  max = Vector(max(x1 + v1.r() * PLAYER_RAND, x1 + v2.r() * PLAYER_RAND), 
	       max(y1 + v1.r() * PLAYER_RAND, y1 + v3.r() * PLAYER_RAND));
  min = Vector(min(x2 - v3.r() * PLAYER_RAND, x2 - v4.r() * PLAYER_RAND), 
	       min(y2 - v2.r() * PLAYER_RAND, y2 - v4.r() * PLAYER_RAND));  

  /* 速度の更新 */
  vel = (max + min) / 2.0;
  vel_error = (max - min) / 2.0;
}

void Player::_inc(void)
{
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) { 
    noise();
  }
  if (pos.x != POS_ERR && pos.y != POS_ERR && 
      vel.x != VEL_ERR && vel.y != VEL_ERR) {
    pos += vel;
    pos_error += vel_error;
  }
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
    vel *= PLAYER_DECAY;
    vel_error *= PLAYER_DECAY;
  }
}

void Player::_turn(void)
{
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) { 
    noise();
  }
  if (pos.x != POS_ERR && pos.y != POS_ERR && 
      vel.x != VEL_ERR && vel.y != VEL_ERR) {
    pos += vel;
    pos_error += vel_error;
  }
  if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
    vel *= PLAYER_DECAY;
    vel_error *= PLAYER_DECAY;
  }
}

/*
 * ==================================================
 * Part : WorldModel
 * ==================================================
 */
// 初期化
void WorldModel::init(Side side, char* team_name, int unum) {
  time = 0 ;
  playmode = PM_BeforeKickOff ;
  t_score = 0 ;
  o_score = 0 ;

  this->side = side ;
  this->team_name = team_name ;

  this->unum = unum ;

  stamina = STAMINA_MAX ;
  effort = EFFORT_MAX ;
  recovery = RECOVER_MAX ;
  vwidth = VW_NORMAL ;
  vquality = VQ_HIGH ;

  ball.init();
  self.init(side, unum);

  offsideline = 0 ;
  goalie_number = 0 ;
  
  // monitor.init();
}

// リセット
void WorldModel::reset(void) {
  self.acc = Vector(0, 0) ;
  self.acc_error = Vector(0, 0) ;
  self.turn_body = 0 ;
  self.turn_body_error = 0 ;
  self.turn_neck = 0 ;
  ball.init() ;
}

// 判断材料の更新
void WorldModel::comm(void) {
  // オフサイドラインの取得
  int n = 0;
  double first = 0.0;
  double second = 0.0;
  double dumy;

  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!opponent[i].known_pos())
      break;
    n++;
    if (second < opponent[i].pos.get_x()) {
      second = opponent[i].pos.get_x();
      if (first < second) {
        dumy = first;
        first = second;
        second = dumy;
      }
    }    
  }

  if (n == N_PLAYER_PER_TEAM)
    offsideline = second;

  // 相手のキーパーの位置を取得
  int tmp_number;
  n = 0;
  double max_x = -52.5;
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!opponent[i].known_pos())
      break;
    n++;
    if (max_x < opponent[i].pos.get_x()) {
      max_x = opponent[i].pos.get_x();
      tmp_number = i;
    }    
  }

  if (n == N_PLAYER_PER_TEAM)
    goalie_number = tmp_number;  
}

// 世界モデルの表示
void WorldModel::print(void) {

}

void WorldModel::add_seeinfo(char* msg) {
  int n_goal;
  SeenGoal goal[N_GOAL];
  int n_flag;
  SeenFlag flag[N_FLAG];
  int n_ball;
  SeenBall ball[N_BALL];
  int n_player;
  SeenPlayer player[N_PLAYER];
  int n_line;
  SeenLine line[N_LINE];

  n_goal = n_flag = n_ball = n_player = n_line = 0;

  msg = next_token(msg + 1);
  if(msg != NULL) msg = next_token(msg);

  char *next, *obj, *param, *tmp;
  double p[6];
  int n_param;
  double dist, dist_chng, dir_chng;
  double dir, body_dir, face_dir;
  FlagName flagptr = FN_c;

  while (msg != NULL) {
    next = next_token(msg++);
    param = next_token(msg++);
    obj = next_token(msg);

    n_param = 0;
    while (param != NULL) {
      while (*param == ' ') param++;
      tmp = param ;
      param = next_token(tmp) ;
      p[n_param] = atof(tmp);
      n_param++;
    }

    dist = DIST_ERR;
    dir = DIR_ERR;
    dist_chng = CHNG_ERR;
    dir_chng = CHNG_ERR;
    body_dir = DIR_ERR;
    face_dir = DIR_ERR;
    if (n_param == 1) {
      dir = p[0];
    } else if (n_param == 2) {
      dist = p[0];
      dir = p[1];
    } else if (n_param == 4) {
      dist = p[0];
      dir = p[1];
      dist_chng = p[2];
      dir_chng = p[3];
    } else if (n_param == 6) {
      dist = p[0];
      dir = p[1];
      dist_chng = p[2];
      dir_chng = p[3];
      body_dir = p[4];
      face_dir = p[5];
    } else {
      printf("Agent::see(): parse error\n");
      return;
    }

    if (!strcmp(msg, "goal")) {
      goal[n_goal].id = GID_UNKNOWN;
      if (obj != NULL) {
        msg = obj;
	if (*msg == 'l')
	  goal[n_goal].id = GID_LEFT;
	else if (*msg == 'r')
	  goal[n_goal].id = GID_RIGHT;
      }
      goal[n_goal].dist = calc_static_dist(dist);
      goal[n_goal].dist_error = calc_static_dist_error(dist);
      goal[n_goal].dir = calc_dir(dir);
      goal[n_goal].dir_error = calc_dir_error(dir);
      goal[n_goal].dist_chng = calc_dist_chng(dist, dist_chng, goal[n_goal].dist);
      goal[n_goal].dist_chng_error = calc_dist_chng_error(dist, dist_chng, goal[n_goal].dist, goal[n_goal].dist_error);
      goal[n_goal].dir_chng = calc_dir_chng(dist, dir_chng);
      goal[n_goal].dir_chng_error = calc_dir_chng_error();
      n_goal++;
    } else if(!strcmp(msg, "Goal")) {
      goal[n_goal].id = GID_UNKNOWN;
      goal[n_goal].dist = calc_static_dist(dist);
      goal[n_goal].dist_error = calc_static_dist_error(dist);
      goal[n_goal].dir = calc_dir(dir);
      goal[n_goal].dir_error = calc_dir_error(dir);
      goal[n_goal].dist_chng = calc_dist_chng(dist, dist_chng, goal[n_goal].dist);
      goal[n_goal].dist_chng_error = calc_dist_chng_error(dist, dist_chng, goal[n_goal].dist, goal[n_goal].dist_error);
      goal[n_goal].dir_chng = calc_dir_chng(dist, dir_chng);
      goal[n_goal].dir_chng_error = calc_dir_chng_error();
      n_goal++;
    } else if(!strcmp(msg, "flag")) {
      flag[n_flag].id.name = FN_UNKNOWN;
      if (obj != NULL) {
	int namelen;
	msg = obj;
	for(namelen = 0; 
	    (msg[namelen] != ')')&&(msg[namelen] != '\000');
	    namelen++);
	for(int i = flagptr; i < FN_MAX; i++) {
	  if(namelen == strlen(Flags[i].namestr) &&
	     !strncmp(msg ,Flags[i].namestr, namelen)) {
	    flag[n_flag].id.name = Flags[i].name;
            flagptr = (FlagName)(i + 1);
	  }
	}
      }
      flag[n_flag].dist = calc_static_dist(dist);
      flag[n_flag].dist_error = calc_static_dist_error(dist);
      flag[n_flag].dir = calc_dir(dir);
      flag[n_flag].dir_error = calc_dir_error(dir);
      flag[n_flag].dist_chng = calc_dist_chng(dist, dist_chng, flag[n_flag].dist);
      flag[n_flag].dist_chng_error = calc_dist_chng_error(dist, dist_chng, flag[n_flag].dist, flag[n_flag].dist_error);
      flag[n_flag].dir_chng = calc_dir_chng(dist, dir_chng);
      flag[n_flag].dir_chng_error = calc_dir_chng_error();
      n_flag++;
    } else if(!strcmp(msg, "Flag")) {
      flag[n_flag].id.name = FN_UNKNOWN;
      flag[n_flag].dist = calc_static_dist(dist);
      flag[n_flag].dist_error = calc_static_dist_error(dist);
      flag[n_flag].dir = calc_dir(dir);
      flag[n_flag].dir_error = calc_dir_error(dir);
      flag[n_flag].dist_chng = calc_dist_chng(dist, dist_chng, flag[n_flag].dist);
      flag[n_flag].dist_chng_error = calc_dist_chng_error(dist, dist_chng, flag[n_flag].dist, flag[n_flag].dist_error);
      flag[n_flag].dir_chng = calc_dir_chng(dist, dir_chng);
      flag[n_flag].dir_chng_error = calc_dir_chng_error();
      n_flag++;
    } else if (!strcmp(msg, "ball")) { 
      ball[n_ball].dist = calc_dynamic_dist(dist);
      ball[n_ball].dist_error = calc_dynamic_dist_error(dist);
      ball[n_ball].dir = calc_dir(dir);
      ball[n_ball].dir_error = calc_dir_error(dir);
      ball[n_ball].dist_chng = calc_dist_chng(dist, dist_chng, ball[n_ball].dist);
      ball[n_ball].dist_chng_error = calc_dist_chng_error(dist, dist_chng, ball[n_ball].dist, ball[n_ball].dist_error);
      ball[n_ball].dir_chng = calc_dir_chng(dist, dir_chng);
      ball[n_ball].dir_chng_error = calc_dir_chng_error();
      n_ball++;
    }
    else if(!strcmp(msg, "Ball")) {
      ball[n_ball].dist = calc_dynamic_dist(dist);
      ball[n_ball].dist_error = calc_dynamic_dist_error(dist);
      ball[n_ball].dir = calc_dir(dir);
      ball[n_ball].dir_error = calc_dir_error(dir);
      ball[n_ball].dist_chng = calc_dist_chng(dist, dist_chng, ball[n_ball].dist);
      ball[n_ball].dist_chng_error = calc_dist_chng_error(dist, dist_chng, ball[n_ball].dist, ball[n_ball].dist_error);
      ball[n_ball].dir_chng = calc_dir_chng(dist, dir_chng);
      ball[n_ball].dir_chng_error = calc_dir_chng_error();
      n_ball++;
    } else if(!strcmp(msg, "player")) {
      player[n_player].id.team = TID_UNKNOWN;
      player[n_player].id.unum = UNUM_ERR;
      if (obj != NULL) {
	msg = obj;
	obj = next_token(msg);
	if (!strcmp(msg, team_name))
	  player[n_player].id.team = TID_TEAMMATE;
	else
	  player[n_player].id.team = TID_OPPONENT;	
	if (obj != NULL) {
	  (void)next_token(obj) ;
	  player[n_player].id.unum = atoi(obj);
	}
      }
      player[n_player].dist = calc_dynamic_dist(dist);
      player[n_player].dist_error = calc_dynamic_dist_error(dist);
      player[n_player].dir = calc_dir(dir);
      player[n_player].dir_error = calc_dir_error(dir);
      player[n_player].dist_chng = calc_dist_chng(dist, dist_chng, player[n_player].dist);
      player[n_player].dist_chng_error = calc_dist_chng_error(dist, dist_chng, player[n_player].dist, player[n_player].dist_error);
      player[n_player].dir_chng = calc_dir_chng(dist, dir_chng);
      player[n_player].dir_chng_error = calc_dir_chng_error();
      player[n_player].body_dir = calc_dir(body_dir);
      player[n_player].body_dir_error = calc_dir_error(body_dir);
      player[n_player].face_dir = calc_dir(face_dir);
      player[n_player].face_dir_error = calc_dir_error(face_dir);
      n_player++;
    } else if(!strcmp(msg, "Player")) {
      player[n_player].id.team = TID_UNKNOWN;
      player[n_player].id.unum = UNUM_ERR;
      player[n_player].dist = calc_dynamic_dist(dist);
      player[n_player].dist_error = calc_dynamic_dist_error(dist);
      player[n_player].dir = calc_dir(dir);
      player[n_player].dir_error = calc_dir_error(dir);
      player[n_player].dist_chng = calc_dist_chng(dist, dist_chng, player[n_player].dist);
      player[n_player].dist_chng_error = calc_dist_chng_error(dist, dist_chng, player[n_player].dist, player[n_player].dist_error);
      player[n_player].dir_chng = calc_dir_chng(dist, dir_chng);
      player[n_player].dir_chng_error = calc_dir_chng_error();
      player[n_player].body_dir = calc_dir(body_dir);
      player[n_player].body_dir_error = calc_dir_error(body_dir);
      player[n_player].face_dir = calc_dir(face_dir);
      player[n_player].face_dir_error = calc_dir_error(face_dir);
      n_player++;
    } else if(!strcmp(msg, "line")) {
      if (obj != NULL) {
	msg = obj;
	if (*msg == 'l')
	  line[n_line].id = LID_LEFT;
	else if (*msg == 't')
	  line[n_line].id = LID_TOP;
	else if (*msg == 'b')
	  line[n_line].id = LID_BOTTOM;
	else if (*msg == 'r')
	  line[n_line].id = LID_RIGHT;
        else {
          printf("Agent::see() : wrong line\n");
          return;	  
	}
      }
      line[n_line].dist = calc_static_dist(dist);
      line[n_line].dist_error = calc_static_dist_error(dist);
      line[n_line].dir = calc_dir(dir);
      line[n_line].dir_error = calc_dir_error(dir);
      n_line++;
    } else {
      printf("see() : miss obj %s\n", msg);
      return;
    }
    msg = next;
  }    

  flagptr = FN_c;
  for (int i = 0; i < n_flag; i++) {
    if (flag[i].id.name != FN_UNKNOWN) {
      if (flag[i].id.name != FN_r_b_30)
        flagptr = Flags[(int)flag[i].id.name + 1].name;
    } else {
      flag[i].id.begin = flagptr;
      if (flagptr != FN_r_b_30)
        flagptr = Flags[(int)flagptr + 1].name;
    } 
  }

  flagptr = FN_r_b_30;
  for (int i = n_flag - 1; i >= 0; i--) {
    if (flag[i].id.name != FN_UNKNOWN) {
      if (flag[i].id.name != FN_c)
	flagptr = Flags[(int)flag[i].id.name - 1].name;
    } else {
      flag[i].id.end = flagptr;
      if (flag[i].id.begin == flag[i].id.end)
        flag[i].id.name = flag[i].id.begin;
      if (flagptr != FN_c)
	flagptr = Flags[(int)flagptr - 1].name;
    } 
  }

  // 自分の情報について

  Vector pos(POS_ERR, POS_ERR);
  Vector pos_error;
  Vector vel(VEL_ERR, VEL_ERR);
  Vector vel_error;
  double angle_face(ANGLE_ERR);
  double angle_face_error;

  bool out_of_field;
  bool look_line = true;

  if (n_line == 1) {
    out_of_field = false;
    if (line[0].dir == DIR_ERR || line[0].dir == 0) {
      look_line = false;
    }
  } else if (n_line == 2) {
    out_of_field = true;
    if (line[0].dist == DIST_ERR) {
      line[0] = line[1];
    }
    if (line[0].dir == DIR_ERR || line[0].dir == 0) {
      look_line = false;
    }
  } else {
    look_line = false;
  }

  if (look_line) {
  if (out_of_field) {
    switch (line[0].id) {
    case LID_LEFT :
      angle_face = -line[0].dir - ((line[0].dir > 0) ? -90.0 : 90.0);
      if (line[0].dist != DIST_ERR) {
	pos.x = -HALF_PITCH_L - fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.x = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    case LID_TOP :
      angle_face = -line[0].dir - ((line[0].dir > 0) ? -180.0 : 0.0);
      if (line[0].dist != DIST_ERR) {
	pos.y = -HALF_PITCH_W - fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.y = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    case LID_RIGHT :
      angle_face = -line[0].dir - ((line[0].dir > 0) ? 90.0 : -90.0);
      if (line[0].dist != DIST_ERR) {
	pos.x = HALF_PITCH_L + fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.x = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    case LID_BOTTOM :
      angle_face = -line[0].dir - ((line[0].dir < 0) ? 180.0 : 0.0);
      if (line[0].dist != DIST_ERR) {
	pos.y = HALF_PITCH_W + fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.y = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    default :
      printf("see() : can't calc\n");
      return;
    }
    angle_face_error = line[0].dir_error;
  } else {
    switch (line[0].id) {
    case LID_LEFT :
      angle_face = -line[0].dir - ((line[0].dir < 0) ? -90.0 : 90.0);
      if (line[0].dist != DIST_ERR) {
	pos.x = -HALF_PITCH_L + fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.x = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    case LID_TOP :
      angle_face = -line[0].dir - ((line[0].dir < 0) ? 180.0 : 0.0);
      if (line[0].dist != DIST_ERR) {
	pos.y = -HALF_PITCH_W + fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.y = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    case LID_RIGHT :
      angle_face = -line[0].dir - ((line[0].dir > 0) ? -90.0 : 90.0);
      if (line[0].dist != DIST_ERR && line[0].dir != DIR_ERR) {
	pos.x = HALF_PITCH_L - fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.x = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    case LID_BOTTOM :
      angle_face = -line[0].dir - ((line[0].dir > 0) ? -180.0 : 0.0);
      if (line[0].dist != DIST_ERR) {
	pos.y = HALF_PITCH_W - fabs(ave_dist_by_sin(line[0].dist, 
                line[0].dist_error, line[0].dir, line[0].dir_error));
	pos_error.y = error_dist_by_sin(line[0].dist, 
                      line[0].dist_error, line[0].dir, line[0].dir_error);
      }
      break;
    default :
      printf("see() : can't calc\n");
      return;
    }
    angle_face_error = line[0].dir_error;
  }

  bool unknown_goalp = false;
  bool unknown_flagp = false;

  for (int j = 0; j < n_goal; j++) {
    Vector tmp_pos, tmp_pos_error;
    if (goal[j].id == GID_UNKNOWN) {
      unknown_goalp = true;
      continue;
    }
    if (goal[j].id == GID_LEFT) {
      if (goal[j].dist != DIST_ERR && goal[j].dir != DIR_ERR) {
        tmp_pos.x = -HALF_PITCH_L - 
            ave_dist_by_cos(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
        tmp_pos_error.x = error_dist_by_cos(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
        tmp_pos.y = 0.0 - ave_dist_by_sin(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
        tmp_pos_error.y = error_dist_by_sin(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);

        if (goal[j].dist_chng != CHNG_ERR && goal[j].dir_chng != CHNG_ERR) {
	  vel.x = -goal[j].dist_chng * (-HALF_PITCH_L - tmp_pos.x) / goal[j].dist + 
                 DEG2RAD * goal[j].dir_chng * -tmp_pos.y;
	  vel_error.x = division_error(goal[j].dist_chng * (-HALF_PITCH_L - tmp_pos.x), 
                         multiplication_error(goal[j].dist_chng, goal[j].dist_chng_error, (-HALF_PITCH_L - tmp_pos.x), 
                         tmp_pos_error.x), goal[j].dist, goal[j].dist_error) + 
                       DEG2RAD * multiplication_error(goal[j].dir_chng, goal[j].dir_chng_error, -tmp_pos.y, 
                         tmp_pos_error.x);
	  vel.y = -goal[j].dist_chng * -tmp_pos.y / goal[j].dist - 
                 DEG2RAD * goal[j].dir_chng * (-HALF_PITCH_L - tmp_pos.x);
	  vel_error.y = division_error(goal[j].dist_chng * -tmp_pos.y, 
                         multiplication_error(goal[j].dist_chng, goal[j].dist_chng_error, -tmp_pos.y, 
                         tmp_pos_error.y), goal[j].dist, goal[j].dist_error) + 
                       DEG2RAD * multiplication_error(goal[j].dir_chng, goal[j].dir_chng_error, (-HALF_PITCH_L - tmp_pos.x), 
                         tmp_pos_error.x);
	}

        if (pos.x == POS_ERR) {
          pos.x = tmp_pos.x;
          pos_error.x = tmp_pos_error.x;
	} else if (pos.x + pos_error.x < tmp_pos.x - tmp_pos_error.x ||
                   tmp_pos.x + tmp_pos_error.x < pos.x - pos_error.x) {
          if (tmp_pos_error.x < pos_error.x) {
            pos.x = tmp_pos.x;
	    pos_error.x = tmp_pos_error.x;
	  }
	} else {
          pos.x = (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) + 
                  max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
          pos_error.x 
                = (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) -
                  max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	}
        if (pos.y == POS_ERR) {
          pos.y = tmp_pos.y;
          pos_error.y = tmp_pos_error.y;
	} else if (pos.y + pos_error.y < tmp_pos.y - tmp_pos_error.y ||
                   tmp_pos.y + tmp_pos_error.y < pos.y - pos_error.y) {
          if (tmp_pos_error.y < pos_error.y) {
            pos.y = tmp_pos.y;
	    pos_error.y = tmp_pos_error.y;
	  }
	} else {
          pos.y = (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) + 
                  max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
          pos_error.y 
                = (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) -
                  max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	}
      }
    } else if (goal[j].id == GID_RIGHT) {
      if (goal[j].dist != DIST_ERR && goal[j].dir != DIR_ERR) {
        tmp_pos.x = HALF_PITCH_L - 
            ave_dist_by_cos(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
        tmp_pos_error.x = error_dist_by_cos(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
        tmp_pos.y = 0.0 - ave_dist_by_sin(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
        tmp_pos_error.y = error_dist_by_sin(goal[j].dist, goal[j].dist_error, 
            goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);

        if (goal[j].dist_chng != CHNG_ERR && goal[j].dir_chng != CHNG_ERR) {
	  vel.x = -goal[j].dist_chng * (HALF_PITCH_L - tmp_pos.x) / goal[j].dist + 
                 DEG2RAD * goal[j].dir_chng * -tmp_pos.y;
	  vel_error.x = division_error(goal[j].dist_chng * (HALF_PITCH_L - tmp_pos.x), 
                         multiplication_error(goal[j].dist_chng, goal[j].dist_chng_error, (HALF_PITCH_L - tmp_pos.x), 
                         tmp_pos_error.x), goal[j].dist, goal[j].dist_error) + 
                       DEG2RAD * multiplication_error(goal[j].dir_chng, goal[j].dir_chng_error, -tmp_pos.y, 
                         tmp_pos_error.x);
	  vel.y = -goal[j].dist_chng * -tmp_pos.y / goal[j].dist - 
                 DEG2RAD * goal[j].dir_chng * (HALF_PITCH_L - tmp_pos.x);
	  vel_error.y = division_error(goal[j].dist_chng * -tmp_pos.y, 
                         multiplication_error(goal[j].dist_chng, goal[j].dist_chng_error, -tmp_pos.y, 
                         tmp_pos_error.y), goal[j].dist, goal[j].dist_error) + 
                       DEG2RAD * multiplication_error(goal[j].dir_chng, goal[j].dir_chng_error, (HALF_PITCH_L - tmp_pos.x), 
                         tmp_pos_error.x);
	}

        if (pos.x == POS_ERR) {
          pos.x = tmp_pos.x;
          pos_error.x = tmp_pos_error.x;
	} else if (pos.x + pos_error.x < tmp_pos.x - tmp_pos_error.x ||
                   tmp_pos.x + tmp_pos_error.x < pos.x - pos_error.x) {
          if (tmp_pos_error.x < pos_error.x) {
            pos.x = tmp_pos.x;
	    pos_error.x = tmp_pos_error.x;
	  }
	} else {
          pos.x = (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) + 
                  max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
          pos_error.x 
                = (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) -
                  max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	}
        if (pos.y == POS_ERR) {
          pos.y = tmp_pos.y;
          pos_error.y = tmp_pos_error.y;
	} else if (pos.y + pos_error.y < tmp_pos.y - tmp_pos_error.y ||
                   tmp_pos.y + tmp_pos_error.y < pos.y - pos_error.y) {
          if (tmp_pos_error.y < pos_error.y) {
            pos.y = tmp_pos.y;
	    pos_error.y = tmp_pos_error.y;
	  }
	} else {
          pos.y = (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) + 
                  max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
          pos_error.y 
                = (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) -
                  max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	}
      }
    }
  }

  for (int j = 0; j < n_flag; j++) {
    Vector tmp_pos, tmp_pos_error;
    if (flag[j].id.name == FN_UNKNOWN) {
      unknown_flagp = true;
      continue;
    }
    if (flag[j].dist != DIST_ERR && flag[j].dir != DIR_ERR) {
      tmp_pos.x = Flags[flag[j].id.name].pos.x - 
          ave_dist_by_cos(flag[j].dist, flag[j].dist_error, 
	  flag[j].dir + angle_face, flag[j].dir_error + angle_face_error);
      tmp_pos_error.x = error_dist_by_cos(flag[j].dist, flag[j].dist_error, 
            flag[j].dir + angle_face, flag[j].dir_error + angle_face_error);
      tmp_pos.y = Flags[flag[j].id.name].pos.y - 
          ave_dist_by_sin(flag[j].dist, flag[j].dist_error, 
          flag[j].dir + angle_face, flag[j].dir_error + angle_face_error);
      tmp_pos_error.y = error_dist_by_sin(flag[j].dist, flag[j].dist_error, 
            flag[j].dir + angle_face, flag[j].dir_error + angle_face_error);
    
      if (flag[j].dist_chng != CHNG_ERR && flag[j].dir_chng != CHNG_ERR) {
	vel.x = -flag[j].dist_chng * (Flags[flag[j].id.name].pos.x - tmp_pos.x) / flag[j].dist + 
                 DEG2RAD * flag[j].dir_chng * (Flags[flag[j].id.name].pos.y - tmp_pos.y);
	vel_error.x = division_error(flag[j].dist_chng * (Flags[flag[j].id.name].pos.x - tmp_pos.x), 
                         multiplication_error(flag[j].dist_chng, flag[j].dist_chng_error, (Flags[flag[j].id.name].pos.x - tmp_pos.x), 
                         tmp_pos_error.x), flag[j].dist, flag[j].dist_error) + 
                       DEG2RAD * multiplication_error(flag[j].dir_chng, flag[j].dir_chng_error, (Flags[flag[j].id.name].pos.y - tmp_pos.y), 
                         tmp_pos_error.x);
	vel.y = -flag[j].dist_chng * (Flags[flag[j].id.name].pos.y - tmp_pos.y) / flag[j].dist - 
                 DEG2RAD * flag[j].dir_chng * (Flags[flag[j].id.name].pos.x - tmp_pos.x);
	vel_error.y = division_error(flag[j].dist_chng * (Flags[flag[j].id.name].pos.y - tmp_pos.y), 
                         multiplication_error(flag[j].dist_chng, flag[j].dist_chng_error, (Flags[flag[j].id.name].pos.y - tmp_pos.y), 
                         tmp_pos_error.y), flag[j].dist, flag[j].dist_error) + 
                       DEG2RAD * multiplication_error(flag[j].dir_chng, flag[j].dir_chng_error, (Flags[flag[j].id.name].pos.x - tmp_pos.x), 
                         tmp_pos_error.x);
      }

      if (pos.x == POS_ERR) {
	pos.x = tmp_pos.x;
	pos_error.x = tmp_pos_error.x;
      } else if (pos.x + pos_error.x < tmp_pos.x - tmp_pos_error.x ||
		 tmp_pos.x + tmp_pos_error.x < pos.x - pos_error.x) {
	if (tmp_pos_error.x < pos_error.x) {
	  pos.x = tmp_pos.x;
	  pos_error.x = tmp_pos_error.x;
	}
      } else {
	pos.x = (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) + 
               max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	pos_error.x 
	  = (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) -
	     max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
      }
      if (pos.y == POS_ERR) {
        pos.y = tmp_pos.y;
	pos_error.y = tmp_pos_error.y;
      } else if (pos.y + pos_error.y < tmp_pos.y - tmp_pos_error.y ||
		 tmp_pos.y + tmp_pos_error.y < pos.y - pos_error.y) {
	if (tmp_pos_error.y < pos_error.y) {
	  pos.y = tmp_pos.y;
	  pos_error.y = tmp_pos_error.y;
	}
      } else {
        pos.y = (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) + 
                max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	pos_error.y 
	  = (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) -
	     max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
      }
    }
  }

  if (pos.x != POS_ERR && pos.y != POS_ERR) {
    if (unknown_goalp) {
      for (int j = 0; j < n_goal; j++) {
        Vector tmp_pos, tmp_pos_error;
        Vector goal_l(-HALF_PITCH_L, 0.0);
        Vector goal_r(HALF_PITCH_L, 0.0);
        if (goal[j].id != GID_UNKNOWN)
          continue;
        tmp_pos.x = pos.x + goal[j].dist * Cos(angle_face + goal[j].dir);
        tmp_pos.y = pos.y + goal[j].dist * Sin(angle_face + goal[j].dir);
        if (tmp_pos.distance(goal_l) < tmp_pos.distance(goal_r)) {
	  if (goal[j].dist != DIST_ERR && goal[j].dir != DIR_ERR) {
	    tmp_pos.x = goal_l.x - 
	      ave_dist_by_cos(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
	    tmp_pos_error.x = 
              error_dist_by_cos(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
	    tmp_pos.y = goal_l.y - 
              ave_dist_by_sin(goal[j].dist, goal[j].dist_error, 
              goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
	    tmp_pos_error.y =
              error_dist_by_sin(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);

	    if (pos.x == POS_ERR) {
	      pos.x = tmp_pos.x;
	      pos_error.x = tmp_pos_error.x;
	    } else if (pos.x + pos_error.x < tmp_pos.x - tmp_pos_error.x ||
		       tmp_pos.x + tmp_pos_error.x < pos.x - pos_error.x) {
	      if (tmp_pos_error.x < pos_error.x) {
		pos.x = tmp_pos.x;
		pos_error.x = tmp_pos_error.x;
	      }
	    } else {
	      pos.x = 
                (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) + 
		max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	      pos_error.x = 
                (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) -
		max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	    }
	    if (pos.y == POS_ERR) {
	      pos.y = tmp_pos.y;
	      pos_error.y = tmp_pos_error.y;
	    } else if (pos.y + pos_error.y < tmp_pos.y - tmp_pos_error.y ||
		       tmp_pos.y + tmp_pos_error.y < pos.y - pos_error.y) {
	      if (tmp_pos_error.y < pos_error.y) {
		pos.y = tmp_pos.y;
		pos_error.y = tmp_pos_error.y;
	      }
	    } else {
	      pos.y = 
                (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) + 
		max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	      pos_error.y = 
                (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) -
		max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	    }
	  }
	} else {
	  if (goal[j].dist != DIST_ERR && goal[j].dir != DIR_ERR) {
	    tmp_pos.x = goal_r.x - 
	      ave_dist_by_cos(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
	    tmp_pos_error.x = 
              error_dist_by_cos(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
	    tmp_pos.y = goal_r.y - 
              ave_dist_by_sin(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);
	    tmp_pos_error.y = 
              error_dist_by_sin(goal[j].dist, goal[j].dist_error, 
	      goal[j].dir + angle_face, goal[j].dir_error + angle_face_error);

	    if (pos.x == POS_ERR) {
	      pos.x = tmp_pos.x;
	      pos_error.x = tmp_pos_error.x;
	    } else if (pos.x + pos_error.x < tmp_pos.x - tmp_pos_error.x ||
		       tmp_pos.x + tmp_pos_error.x < pos.x - pos_error.x) {
	      if (tmp_pos_error.x < pos_error.x) {
		pos.x = tmp_pos.x;
		pos_error.x = tmp_pos_error.x;
	      }
	    } else {
	      pos.x = 
                (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) + 
		max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	      pos_error.x = 
                (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) -
	        max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	    }
	    if (pos.y == POS_ERR) {
	      pos.y = tmp_pos.y;
	      pos_error.y = tmp_pos_error.y;
	    } else if (pos.y + pos_error.y < tmp_pos.y - tmp_pos_error.y ||
		       tmp_pos.y + tmp_pos_error.y < pos.y - pos_error.y) {
	      if (tmp_pos_error.y < pos_error.y) {
	        pos.y = tmp_pos.y;
		pos_error.y = tmp_pos_error.y;
	      }
	    } else {
	      pos.y = 
                (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) + 
		max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	      pos_error.y = 
                (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) -
		max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	    }
	  }
	}
      }
    }
    if (unknown_flagp) {
      for (int j = 0; j < n_flag; j++) {
        Vector tmp_pos, tmp_pos_error;
        int tmp_num;
        double tmp_dist;
        if (flag[j].id.name != FN_UNKNOWN)
          continue;
        tmp_pos.x = pos.x + flag[j].dist * Cos(angle_face + flag[j].dir);
        tmp_pos.y = pos.y + flag[j].dist * Sin(angle_face + flag[j].dir);
        tmp_num = flag[j].id.end;
        tmp_dist = tmp_pos.distance(Flags[tmp_num].pos);
        for (int k = flag[j].id.begin; k < flag[j].id.end; k++) {
          if (tmp_pos.distance(Flags[k].pos) < tmp_dist) {
            tmp_num = k;
            tmp_dist = tmp_pos.distance(Flags[k].pos);
	  }
	}

	if (flag[j].dist != DIST_ERR && flag[j].dir != DIR_ERR) {
	  tmp_pos.x = Flags[tmp_num].pos.x - 
	    ave_dist_by_cos(flag[j].dist, flag[j].dist_error, 
	    flag[j].dir + angle_face, flag[j].dir_error + 
            angle_face_error);
	  tmp_pos_error.x = 
            error_dist_by_cos(flag[j].dist, flag[j].dist_error, 
	    flag[j].dir + angle_face, flag[j].dir_error + 
            angle_face_error);
	  tmp_pos.y = Flags[tmp_num].pos.y - 
	    ave_dist_by_sin(flag[j].dist, flag[j].dist_error, 
	    flag[j].dir + angle_face, flag[j].dir_error + 
            angle_face_error);
	  tmp_pos_error.y = 
            error_dist_by_sin(flag[j].dist, flag[j].dist_error, 
	    flag[j].dir + angle_face, flag[j].dir_error + 
            angle_face_error);

	  if (pos.x == POS_ERR) {
	    pos.x = tmp_pos.x;
	    pos_error.x = tmp_pos_error.x;
	  } else if (pos.x + pos_error.x < tmp_pos.x - tmp_pos_error.x ||
		     tmp_pos.x + tmp_pos_error.x < pos.x - pos_error.x) {
	    if (tmp_pos_error.x < pos_error.x) {
	      pos.x = tmp_pos.x;
	      pos_error.x = tmp_pos_error.x;
	    }
	  } else {
	    pos.x = 
              (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) + 
	      max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	    pos_error.x = 
              (min(pos.x + pos_error.x, tmp_pos.x + tmp_pos_error.x) -
	      max(pos.x - pos_error.x, tmp_pos.x - tmp_pos_error.x)) / 2.0;
	  }
	  if (pos.y == POS_ERR) {
	    pos.y = tmp_pos.y;
	    pos_error.y = tmp_pos_error.y;
	  } else if (pos.y + pos_error.y < tmp_pos.y - tmp_pos_error.y ||
		     tmp_pos.y + tmp_pos_error.y < pos.y - pos_error.y) {
	    if (tmp_pos_error.y < pos_error.y) {
	      pos.y = tmp_pos.y;
	      pos_error.y = tmp_pos_error.y;
	    }
	  } else {
	    pos.y = 
              (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) + 
	      max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	    pos_error.y = 
              (min(pos.y + pos_error.y, tmp_pos.y + tmp_pos_error.y) -
	      max(pos.y - pos_error.y, tmp_pos.y - tmp_pos_error.y)) / 2.0;
	  }
	}        
      }
    }
  }

  if (side == S_RIGHT) {
    if (angle_face != ANGLE_ERR)
      angle_face = 
        normalize_angle(angle_face + ((angle_face > 0) ? -180.0 : 180.0));
    if (pos.x != POS_ERR)
      pos.x *= -1.0;
    if (pos.y != POS_ERR)
      pos.y *= -1.0;
  }

  if (pos.x != POS_ERR && pos.y != POS_ERR) {
    self.pos = pos;
    self.pos_error = pos_error;
  } 

  if (angle_face != ANGLE_ERR) {
    self.angle_body = 
      normalize_angle(angle_face - self.angle_neck);
    self.angle_body_error = 
      NormalizeAngleError(angle_face_error + self.angle_neck_error);
  }
  }

  // ボールの情報について

  pos = Vector(POS_ERR, POS_ERR) ;
  vel = Vector(VEL_ERR, VEL_ERR) ;
  angle_face = 
    normalize_angle(self.angle_body + self.angle_neck);
  angle_face_error = 
    NormalizeAngleError(self.angle_body_error + 
                        self.angle_neck_error);

  if (n_ball == 1) {
    if (ball[0].dist != DIST_ERR && ball[0].dir != DIR_ERR) {
      pos.x = ave_dist_by_cos(ball[0].dist, ball[0].dist_error,
               ball[0].dir + angle_face, 
               ball[0].dir_error + angle_face_error) + self.pos.x;
      pos_error.x = error_dist_by_cos(ball[0].dist, ball[0].dist_error,
               ball[0].dir + angle_face, 
               ball[0].dir_error + angle_face_error) + self.pos_error.x;
      pos.y = ave_dist_by_sin(ball[0].dist, ball[0].dist_error,
               ball[0].dir + angle_face, 
               ball[0].dir_error + angle_face_error) + self.pos.y;
      pos_error.y = error_dist_by_sin(ball[0].dist, ball[0].dist_error,
               ball[0].dir + angle_face, 
               ball[0].dir_error + angle_face_error) + self.pos_error.y;

      if (ball[0].dist_chng != CHNG_ERR && ball[0].dir_chng != CHNG_ERR) {
        vel.x = self.vel.x + ball[0].dist_chng * (pos.x - self.pos.x) / ball[0].dist - 
                 DEG2RAD * ball[0].dir_chng * (pos.y - self.pos.y);
        vel_error.x = division_error(ball[0].dist_chng * (pos.x - self.pos.x), 
                         multiplication_error(ball[0].dist_chng, ball[0].dist_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x)), ball[0].dist, ball[0].dist_error) + 
                       DEG2RAD * multiplication_error(ball[0].dir_chng, ball[0].dir_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + self.pos_error.y));
        vel.y = self.vel.y + ball[0].dist_chng * (pos.y - self.pos.y) / ball[0].dist + 
                 DEG2RAD * ball[0].dir_chng * (pos.x - self.pos.x);
        vel_error.y = division_error(ball[0].dist_chng * (pos.y - self.pos.y), 
                         multiplication_error(ball[0].dist_chng, ball[0].dist_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + self.pos_error.y)), ball[0].dist, ball[0].dist_error) + 
                       DEG2RAD * multiplication_error(ball[0].dir_chng, ball[0].dir_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x));
      }
    }
    if (pos.x != POS_ERR && pos.y != POS_ERR) {
      this->ball.pos = pos;
      this->ball.pos_error = pos_error;
    }
    if (vel.x != VEL_ERR && vel.y != VEL_ERR) {
      this->ball.vel = vel;
      this->ball.vel_error = vel_error;
    }
  } else {
    if (this->ball.known_pos()) {
      if (self.get_pos().distance(this->ball.get_pos()) <= 3.0 * 0.9) {
	this->ball.pos = Vector(POS_ERR, POS_ERR);
	this->ball.vel = Vector(VEL_ERR, VEL_ERR);
      } else if (vwidth == VW_NARROW && 
                 fabs(normalize_angle(self.get_pos().direction(this->ball.get_pos()) - angle_face)) < 22.5 * 0.9) {
	this->ball.pos = Vector(POS_ERR, POS_ERR);
	this->ball.vel = Vector(VEL_ERR, VEL_ERR);
      } else if (vwidth == VW_NORMAL && 
                 fabs(normalize_angle(self.direction(this->ball) - angle_face)) < 45.0 * 0.9) {
	this->ball.pos = Vector(POS_ERR, POS_ERR);
	this->ball.vel = Vector(VEL_ERR, VEL_ERR);
      } else if (vwidth == VW_WIDE && 
                 fabs(normalize_angle(self.direction(this->ball) - angle_face)) < 90.0 * 0.9) {
	this->ball.pos = Vector(POS_ERR, POS_ERR);
	this->ball.vel = Vector(VEL_ERR, VEL_ERR);
      }
    }
  }

  // 他のプレイヤの情報について
  double angle_body;
  double angle_body_error;
  double angle_neck;
  double angle_neck_error;

  // フラグの初期化
  bool teammatep[N_PLAYER_PER_TEAM - 1];
  bool opponentp[N_PLAYER_PER_TEAM];  
  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
    teammatep[i] = false;
  }
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    opponentp[i] = false;
  }

  bool equal;   // 一致
  bool near;    // 一致できた
  bool unknown; // 一致させた
  bool unknown_t; // 一致させた
  bool unknown_o; // 一致させた
  int e_num;
  int n_num;
  int u_num;
  int t_num;
  int o_num;
  TeamID team;
  double min_dist;

  // チームと背番号の判るオブジェクトについて
  for (int i = 0; i < n_player; i++) {
    if (player[i].id.team == TID_UNKNOWN || player[i].id.unum == UNUM_ERR)
      continue;

    pos = Vector(POS_ERR, VEL_ERR);
    vel = Vector(VEL_ERR, VEL_ERR);
    angle_body = ANGLE_ERR;
    angle_neck = ANGLE_ERR;

    if (player[i].dist != DIST_ERR && player[i].dir != DIR_ERR) {
      pos.x = ave_dist_by_cos(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos.x;
      pos_error.x = error_dist_by_cos(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos_error.x;
      pos.y = ave_dist_by_sin(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos.y;
      pos_error.y = error_dist_by_sin(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos_error.y;

      if (player[i].dist_chng != CHNG_ERR && player[i].dir_chng != CHNG_ERR) {
        vel.x = self.vel.x + player[i].dist_chng * (pos.x - self.pos.x) / player[i].dist - 
                 DEG2RAD * player[i].dir_chng * (pos.y - self.pos.y);
        vel_error.x = division_error(player[i].dist_chng * (pos.x - self.pos.x), 
                         multiplication_error(player[i].dist_chng, player[i].dist_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x)), player[i].dist, player[i].dist_error) + 
                       DEG2RAD * multiplication_error(player[i].dir_chng, player[i].dir_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + self.pos_error.y));
        vel.y = self.vel.y + player[i].dist_chng * (pos.y - self.pos.y) / player[i].dist + 
                 DEG2RAD * player[i].dir_chng * (pos.x - self.pos.x);
        vel_error.y = division_error(player[i].dist_chng * (pos.y - self.pos.y), 
                         multiplication_error(player[i].dist_chng, player[i].dist_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + pos_error.y)), player[i].dist, player[i].dist_error) + 
                       DEG2RAD * multiplication_error(player[i].dir_chng, player[i].dir_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x));
      }
      
      if (player[i].body_dir != DIR_ERR && player[i].face_dir != DIR_ERR) {
        angle_body = normalize_angle(player[i].body_dir + angle_face);
        angle_body_error = player[i].body_dir_error + angle_face_error;
        angle_neck = normalize_angle(player[i].face_dir + angle_face);
        angle_neck_error = player[i].face_dir_error + angle_face_error;
      }

      equal = false;   // 一致
      near  = false;   // 一致できた
      unknown = false; // 一致させた
      min_dist = 1000.0;
      if (player[i].id.team == TID_TEAMMATE) {
	for (int j = 0; j < N_PLAYER_PER_TEAM - 1; j++) {
	  if (teammatep[j])
	    continue;
          if (teammate[j].unum == player[i].id.unum) {
            equal = true;
            e_num = j;
            break;
	  }
          if (teammate[j].unum == UNUM_ERR) {
	    if (teammate[j].pos.x != POS_ERR &&
		teammate[j].pos.y != POS_ERR) {
	      dist = pos.distance(teammate[j].pos);
              if (dist < min_dist) {
                near = true;
                min_dist = dist;
                n_num = j;
	      }
	    } else if (!unknown) {
              unknown = true;
              u_num = j;
	    }
	  }
	}

        if (equal) {
          teammate[e_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[e_num] = true;
	} else if (near && min_dist < 5.0) {
          teammate[n_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[n_num] = true;
	} else if (unknown) {
          teammate[u_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[u_num] = true;
	} else if (near) {
          teammate[n_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[n_num] = true;
	} else {
          printf("error1t\n");
	}

      } else if (player[i].id.team == TID_OPPONENT) {
	for (int j = 0; j < N_PLAYER_PER_TEAM; j++) {
	  if (opponentp[j])
	    continue;
          if (opponent[j].unum == player[i].id.unum) {
            equal = true;
            e_num = j;
            break;
	  }
          if (opponent[j].unum == UNUM_ERR) {
	    if (opponent[j].pos.x != POS_ERR &&
		opponent[j].pos.y != POS_ERR) {
	      dist = pos.distance(opponent[j].pos);
              if (dist < min_dist) {
                near = true;
                min_dist = dist;
                n_num = j;
	      }
	    } else if (!unknown) {
              unknown = true;
              u_num = j;
	    }
	  }
	}

        if (equal) {
          opponent[e_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[e_num] = true;
	} else if (near && min_dist < 5.0) {
          opponent[n_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[n_num] = true;
	} else if (unknown) {
          opponent[u_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[u_num] = true;
	} else if (near) {
          opponent[n_num].see(player[i].id.unum, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[n_num] = true;
	} else {
          printf("error1o\n");
	}
      }
    }
  }

  // チームの判るオブジェクトについて
  for (int i = 0; i < n_player; i++) {
    if (player[i].id.team == TID_UNKNOWN || player[i].id.unum != UNUM_ERR)
      continue;

    pos = Vector(POS_ERR, VEL_ERR);
    vel = Vector(VEL_ERR, VEL_ERR);
    angle_body = ANGLE_ERR;
    angle_neck = ANGLE_ERR;

    if (player[i].dist != DIST_ERR && player[i].dir != DIR_ERR) {
      pos.x = ave_dist_by_cos(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos.x;
      pos_error.x = error_dist_by_cos(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos_error.x;
      pos.y = ave_dist_by_sin(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos.y;
      pos_error.y = error_dist_by_sin(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos_error.y;
      
      if (player[i].dist_chng != CHNG_ERR && player[i].dir_chng != CHNG_ERR) {
        vel.x = self.vel.x + player[i].dist_chng * (pos.x - self.pos.x) / player[i].dist - 
                 DEG2RAD * player[i].dir_chng * (pos.y - self.pos.y);
        vel_error.x = division_error(player[i].dist_chng * (pos.x - self.pos.x), 
                         multiplication_error(player[i].dist_chng, player[i].dist_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x)), player[i].dist, player[i].dist_error) + 
                       DEG2RAD * multiplication_error(player[i].dir_chng, player[i].dir_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + self.pos_error.y));
        vel.y = self.vel.y + player[i].dist_chng * (pos.y - self.pos.y) / player[i].dist + 
                 DEG2RAD * player[i].dir_chng * (pos.x - self.pos.x);
        vel_error.y = division_error(player[i].dist_chng * (pos.y - self.pos.y), 
                         multiplication_error(player[i].dist_chng, player[i].dist_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + pos_error.y)), player[i].dist, player[i].dist_error) + 
                       DEG2RAD * multiplication_error(player[i].dir_chng, player[i].dir_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x));
      }

      if (player[i].body_dir != DIR_ERR && player[i].face_dir != DIR_ERR) {
        angle_body = normalize_angle(player[i].body_dir + angle_face);
        angle_body_error = player[i].body_dir_error + angle_face_error;
        angle_neck = normalize_angle(player[i].face_dir + angle_face);
        angle_neck_error = player[i].face_dir_error + angle_face_error;
      }

      near  = false;   // 一致できた
      unknown = false; // 一致させた
      min_dist = 1000.0;
      if (player[i].id.team == TID_TEAMMATE) {
	for (int j = 0; j < N_PLAYER_PER_TEAM - 1; j++) {
	  if (teammatep[j])
	    continue;
	  if (teammate[j].pos.x != POS_ERR &&
	      teammate[j].pos.y != POS_ERR) {
	    dist = pos.distance(teammate[j].pos);
	    if (dist < min_dist) {
	      near = true;
	      min_dist = dist;
	      n_num = j;
	    }
	  } else if (!unknown) {
	    unknown = true;
	    u_num = j;
	  }
	}

	if (near && min_dist < 5.0) {
          teammate[n_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[n_num] = true;
	} else if (unknown) {
          teammate[u_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[u_num] = true;
	} else if (near) {
          teammate[n_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[n_num] = true;
	} else {
          printf("error2t\n");
	}
      } else if (player[i].id.team == TID_OPPONENT) {
	for (int j = 0; j < N_PLAYER_PER_TEAM; j++) {
	  if (opponentp[j])
	    continue;
	  if (opponent[j].pos.x != POS_ERR &&
	      opponent[j].pos.y != POS_ERR) {
	    dist = pos.distance(opponent[j].pos);
	    if (dist < min_dist) {
	      near = true;
	      min_dist = dist;
	      n_num = j;
	    }
	  } else if (!unknown) {
	    unknown = true;
	    u_num = j;
	  }
	}

	if (near && min_dist < 5.0) {
          opponent[n_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[n_num] = true;
	} else if (unknown) {
          opponent[u_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[u_num] = true;
	} else if (near) {
          opponent[n_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          opponentp[n_num] = true;
	} else {
          printf("error2o\n");
	}
      }
    }
  }

  // 判らないオブジェクトについて 
  for (int i = 0; i < n_player; i++) {
    TeamID team;
    if (player[i].id.team != TID_UNKNOWN || player[i].id.unum != UNUM_ERR)
      continue;

    pos = Vector(POS_ERR, VEL_ERR);
    vel = Vector(VEL_ERR, VEL_ERR);
    angle_body = ANGLE_ERR;
    angle_neck = ANGLE_ERR;

    if (player[i].dist != DIST_ERR && player[i].dir != DIR_ERR) {
      pos.x = ave_dist_by_cos(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos.x;
      pos_error.x = error_dist_by_cos(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos_error.x;
      pos.y = ave_dist_by_sin(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos.y;
      pos_error.y = error_dist_by_sin(player[i].dist, player[i].dist_error,
               player[i].dir + angle_face, 
               player[i].dir_error + angle_face_error) + self.pos_error.y;
      
      if (player[i].dist_chng != CHNG_ERR && player[i].dir_chng != CHNG_ERR) {
        vel.x = self.vel.x + player[i].dist_chng * (pos.x - self.pos.x) / player[i].dist - 
                 DEG2RAD * player[i].dir_chng * (pos.y - self.pos.y);
        vel_error.x = division_error(player[i].dist_chng * (pos.x - self.pos.x), 
                         multiplication_error(player[i].dist_chng, player[i].dist_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x)), player[i].dist, player[i].dist_error) + 
                       DEG2RAD * multiplication_error(player[i].dir_chng, player[i].dir_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + self.pos_error.y));
        vel.y = self.vel.y + player[i].dist_chng * (pos.y - self.pos.y) / player[i].dist + 
                 DEG2RAD * player[i].dir_chng * (pos.x - self.pos.x);
        vel_error.y = division_error(player[i].dist_chng * (pos.y - self.pos.y), 
                         multiplication_error(player[i].dist_chng, player[i].dist_chng_error, (pos.y - self.pos.y), 
                         (pos_error.y + pos_error.y)), player[i].dist, player[i].dist_error) + 
                       DEG2RAD * multiplication_error(player[i].dir_chng, player[i].dir_chng_error, (pos.x - self.pos.x), 
                         (pos_error.x + self.pos_error.x));
      }

      if (player[i].body_dir != DIR_ERR && player[i].face_dir != DIR_ERR) {
        angle_body = normalize_angle(player[i].body_dir + angle_face);
        angle_body_error = player[i].body_dir_error + angle_face_error;
        angle_neck = normalize_angle(player[i].face_dir + angle_face);
        angle_neck_error = player[i].face_dir_error + angle_face_error;
      }

      near  = false;     // 一致できた
      unknown_t = false; // 一致させた
      unknown_o = false; // 一致させた
      min_dist = 1000.0;
      for (int j = 0; j < N_PLAYER_PER_TEAM - 1; j++) {
	if (teammatep[j])
	  continue;
	if (teammate[j].pos.x != POS_ERR &&
	    teammate[j].pos.y != POS_ERR) {
	  dist = pos.distance(teammate[j].pos);
	  if (dist < min_dist) {
            team = TID_TEAMMATE;
	    near = true;
	    min_dist = dist;
	    n_num = j;
	  }
	} else if (!unknown_t) {
	  unknown_t = true;
	  t_num = j;
	}
      }
      for (int j = 0; j < N_PLAYER_PER_TEAM; j++) {
	if (opponentp[j])
	  continue;
	if (opponent[j].pos.x != POS_ERR &&
	    opponent[j].pos.y != POS_ERR) {
	  dist = pos.distance(opponent[j].pos);
	  if (dist < min_dist) {
            team = TID_OPPONENT;
	    near = true;
	    min_dist = dist;
	    n_num = j;
	  }
	} else if (!unknown_o) {
	  unknown_o = true;
	  o_num = j;
	}
      }

      if (near && min_dist < 5.0) {
        if (team == TID_TEAMMATE) {
          teammate[n_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[n_num] = true;
	} else {
	  opponent[n_num].see(UNUM_ERR, pos, pos_error, 
			      vel, vel_error, angle_body, angle_body_error, 
			      angle_neck, angle_neck_error);
	  opponentp[n_num] = true;
	}
      } else if (unknown_t && !unknown_o) {
	teammate[t_num].see(UNUM_ERR, pos, pos_error, 
			    vel, vel_error, angle_body, angle_body_error, 
			    angle_neck, angle_neck_error);
	teammatep[t_num] = true;
      } else if (!unknown_t && unknown_o) {
	opponent[o_num].see(UNUM_ERR, pos, pos_error, 
			    vel, vel_error, angle_body, angle_body_error, 
			    angle_neck, angle_neck_error);
	opponentp[o_num] = true;
      } else if (unknown_t && unknown_o) {
        if (pos.x > 0) {
	  opponent[o_num].see(UNUM_ERR, pos, pos_error, 
			      vel, vel_error, angle_body, angle_body_error, 
			      angle_neck, angle_neck_error);
	  opponentp[o_num] = true;
	} else {
	  teammate[t_num].see(UNUM_ERR, pos, pos_error, 
			      vel, vel_error, angle_body, angle_body_error, 
			      angle_neck, angle_neck_error);
	  teammatep[t_num] = true;
	}
      } else if (near) {
        if (team == TID_TEAMMATE) {
          teammate[n_num].see(UNUM_ERR, pos, pos_error, 
                              vel, vel_error, angle_body, angle_body_error, 
                              angle_neck, angle_neck_error);
          teammatep[n_num] = true;
	} else {
	  opponent[n_num].see(UNUM_ERR, pos, pos_error, 
			      vel, vel_error, angle_body, angle_body_error, 
			      angle_neck, angle_neck_error);
	  opponentp[n_num] = true;
	}
      } else {
	printf("error3\n");
      }
    }
  }

  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++) {
    if (teammatep[i])
      continue;
    if (teammate[i].pos.x != POS_ERR && teammate[i].pos.y != POS_ERR) {
      if (self.distance(teammate[i]) <= 3.0 * 0.9) {
        teammate[i].lost();
      } else if (vwidth == VW_NARROW && 
                 fabs(normalize_angle(self.direction(teammate[i]) - angle_face)) < 22.5 * 0.9) {
        teammate[i].lost();
      } else if (vwidth == VW_NORMAL && 
                 fabs(normalize_angle(self.direction(teammate[i]) - angle_face)) < 45.0 * 0.9) {
	teammate[i].lost();
      } else if (vwidth == VW_WIDE && 
                 fabs(normalize_angle(self.direction(teammate[i]) - angle_face)) < 90.0 * 0.9) {
        teammate[i].lost();
      }
    }
  }
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (opponentp[i])
      continue;
    if (opponent[i].pos.x != POS_ERR && opponent[i].pos.y != POS_ERR) {
      if (self.distance(opponent[i]) <= 3.0 * 0.9) {
        opponent[i].lost();
      } else if (vwidth == VW_NARROW && 
                 fabs(normalize_angle(self.direction(opponent[i]) - angle_face)) < 22.5 * 0.9) {
        opponent[i].lost();
      } else if (vwidth == VW_NORMAL && 
                 fabs(normalize_angle(self.direction(opponent[i]) - angle_face)) < 45.0 * 0.9) {
	opponent[i].lost();
      } else if (vwidth == VW_WIDE && 
                 fabs(normalize_angle(self.direction(opponent[i]) - angle_face)) < 90.0 * 0.9) {
        opponent[i].lost();
      }
    }
  }
}

// デバッグ情報による更新
void WorldModel::debug(char* msg) {
  // ヘッダの処理
  char pmstr[4096];
  int l_score, r_score;
  sscanf(msg, "(debug %d %s %d %d %lf %lf %lf ", &time, pmstr, &l_score, &r_score, &stamina, &effort, &recovery);
  if (side == S_LEFT) {
    t_score = l_score;
    o_score = r_score;
  } else {
    t_score = r_score;
    o_score = l_score;
  }

  // 文字列からプレーモードへの変換
  int score;
  playmode = PM_Error;
  for (int pm = 0; pm_entry[pm].name != NULL; pm++) {
    if (!strcmp(pmstr, pm_entry[pm].name)) {
      playmode = pm_entry[pm].id;
    }
  }
  if (playmode == PM_Error) {
    if (strlen(pmstr) > 5 && !strncmp(pmstr, "goal_", 5)) {
      sscanf(&pmstr[7], "%d ", score);
      if (pmstr[5] == 'l') {
	playmode = PM_AfterGoal_Left;
      } else if (pmstr[5] == 'r') {
	playmode = PM_AfterGoal_Right;
      }
    }
  }

  // 世界モデルのリセット
  ball.reset();
  for (int i = 0; i < N_PLAYER_PER_TEAM - 1; i++)
    teammate[i].reset();
  for (int i = 0; i < N_PLAYER_PER_TEAM; i++)
    opponent[i].reset();

  // ボディの処理
  char* buffer;
  char* next;
  char* param;
  char* obj;
  char* tmp;

  buffer = next_token(msg + 1);
  if(buffer != NULL) buffer = next_token(buffer);
  if(buffer != NULL) buffer = next_token(buffer);
  if(buffer != NULL) buffer = next_token(buffer);
  if(buffer != NULL) buffer = next_token(buffer);
  if(buffer != NULL) buffer = next_token(buffer);
  if(buffer != NULL) buffer = next_token(buffer);
  if(buffer != NULL) buffer = next_token(buffer);

  while (buffer != NULL) {
    next = next_token(buffer++);
    param = next_token(buffer++);
    obj = next_token(buffer);

    if (!strcmp(buffer, "ball")) {
      // ボール情報の処理
      if (param != NULL) {
        tmp = param;
        param = next_token(tmp);
	ball.pos.set_x(side * atof(tmp));
      }

      if (param != NULL) {
        tmp = param;
        param = next_token(tmp);
	ball.pos.set_y(side * atof(tmp));
      }

      if (param != NULL) {
        tmp = param;
        param = next_token(tmp);
	ball.vel.set_x(side * atof(tmp));
      }

      if (param != NULL) {
        tmp = param;
        param = next_token(tmp);
	ball.vel.set_y(side * atof(tmp));
      }
    } else if (!strcmp(buffer, "player")) {
      // プレーヤ情報の処理
      if (obj != NULL) {
	tmp = obj;
	obj = next_token(tmp);
        if (!strcmp(tmp, team_name)) {
          if (obj != NULL) {
             (void)next_token(obj);
           
             if (atoi(obj) == unum) {
	       // 自分の処理
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 self.pos.set_x(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 self.pos.set_y(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 self.vel.set_x(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 self.vel.set_y(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 if (side == S_RIGHT) 
		   self.angle_body = (atof(tmp) > 0) ? atof(tmp) - 180 : atof(tmp) + 180;
		 else  
		   self.angle_body = atof(tmp);
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 if (side == S_RIGHT) 
		   self.angle_neck = (atof(tmp) > 0) ? atof(tmp) - 180 : atof(tmp) + 180;
		 else  
		   self.angle_neck = atof(tmp);
	       }
	     } else {  
               // 自分以外の処理
               int num ;
               if (atoi(obj) < unum)
		 num = atoi(obj) - 1 ;
               else
		 num = atoi(obj) - 2 ;

               teammate[num].unum = atoi(obj) ;

	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 teammate[num].pos.set_x(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 teammate[num].pos.set_y(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 teammate[num].vel.set_x(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 teammate[num].vel.set_y(side * atof(tmp));
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 if (side == S_RIGHT) 
		   teammate[num].angle_body = (atof(tmp) > 0) ? atof(tmp) - 180 : atof(tmp) + 180;
		 else  
		   teammate[num].angle_body = atof(tmp);
	       }
	       
	       if (param != NULL) {
		 tmp = param;
		 param = next_token(tmp);
		 if (side == S_RIGHT) 
		   teammate[num].angle_neck = (atof(tmp) > 0) ? atof(tmp) - 180 : atof(tmp) + 180;
		 else  
		   teammate[num].angle_neck = atof(tmp);
	       }
	     }
	  }
	} else {
          if (obj != NULL) {
	    (void)next_token(obj);
	    
	    opponent[atoi(obj) - 1].unum = atoi(obj) ;

	    if (param != NULL) {
	      tmp = param;
	      param = next_token(tmp);
	      opponent[atoi(obj) - 1].pos.set_x(side * atof(tmp));
	    }
	    
	    if (param != NULL) {
	      tmp = param;
	      param = next_token(tmp);
	      opponent[atoi(obj) - 1].pos.set_y(side * atof(tmp));
	    }
	    
	    if (param != NULL) {
	      tmp = param;
	      param = next_token(tmp);
	      opponent[atoi(obj) - 1].vel.set_x(side * atof(tmp));
	    }
	    
	    if (param != NULL) {
	      tmp = param;
	      param = next_token(tmp);
	      opponent[atoi(obj) - 1].vel.set_y(side * atof(tmp));
	    }
	    
	    if (param != NULL) {
	      tmp = param;
	      param = next_token(tmp);
              if (side == S_RIGHT) 
		opponent[atoi(obj) - 1].angle_body = (atof(tmp) > 0) ? atof(tmp) - 180 : atof(tmp) + 180;
              else  
	        opponent[atoi(obj) - 1].angle_body = atof(tmp);
	    }
	    
	    if (param != NULL) {
	      tmp = param;
	      param = next_token(tmp);
              if (side == S_RIGHT) 
		opponent[atoi(obj) - 1].angle_neck = (atof(tmp) > 0) ? atof(tmp) - 180 : atof(tmp) + 180;
              else  
	        opponent[atoi(obj) - 1].angle_neck = atof(tmp);
	    }
	  }
	}
      }
    }

    buffer = next;
  }

  int n = 0;
  double first = 0.0;
  double second = 0.0;
  double dumy;

  for (int i = 0; i < N_PLAYER_PER_TEAM; i++) {
    if (!opponent[i].known_pos())
      break;
    n++;
    if (second < opponent[i].pos.get_x()) {
      second = opponent[i].pos.get_x();
      if (first < second) {
        dumy = first;
        first = second;
        second = dumy;
      }
    }    
  }

  if (n == N_PLAYER_PER_TEAM)
    offsideline = second;
}

// コマンドの影響
// キックの影響
void WorldModel::kick(double power, double dir) {

  // プレーモードによる判定
  if (playmode == PM_BeforeKickOff || playmode == PM_AfterGoal_Left || 
      playmode == PM_AfterGoal_Right || playmode == PM_OffSide_Left || 
      playmode == PM_OffSide_Right)
    return ;

  // キッカブルによる判定
  if (ball.known_pos()) {
    if (self.pos.distance(ball.pos) > KICKABLE)
      return ;
  } else {
    return ;
  }

  // ボールの加速度への影響
  Vector tmp = ball.pos - self.pos ;
  double dir_diff = fabs(normalize_angle(tmp.th() - self.angle_body)) ;
  double dist_ball = tmp.r() - PLAYER_SIZE - BALL_SIZE ;
  dir = normalize_angle(dir + self.angle_body) ;

  ball.acc = polar2vector(1.0, dir) ;    
  ball.acc *= power * KICK_POWER_RATE *
    (1.0 - 0.25 * dir_diff / Rad2Deg(PI) - 0.25 * dist_ball / KICKABLE_MARGIN ) ;
  ball.acc_error = Vector(0, 0) ;
}

// ダッシュの影響
double WorldModel::dash(double power) {
  // 必要なパワーの取得
  double power_need = (power < 0) ? NormalizeDashPower(power) * -2 
                                  : NormalizeDashPower(power) ;
  if (power_need > stamina)
    power_need = stamina ;   
  power = (power < 0) ? power_need / -2 : power_need ;

  // 加速度の影響
  double effective_dash_power = effort * power ;  
  self.acc = Vector((max_cos(self.angle_body, self.angle_body_error) + 
		     min_cos(self.angle_body, self.angle_body_error)) / 2.0,  
		    (max_sin(self.angle_body, self.angle_body_error) + 
		     min_sin(self.angle_body, self.angle_body_error)) / 2.0) ;
  self.acc_error = Vector((max_cos(self.angle_body, self.angle_body_error) - 
			   min_cos(self.angle_body, self.angle_body_error)) / 2.0,  
			  (max_sin(self.angle_body, self.angle_body_error) - 
			   min_sin(self.angle_body, self.angle_body_error)) / 2.0) ;
  self.acc *= ReduceDashPower(effective_dash_power) ;
  self.acc_error *= ReduceDashPower(effective_dash_power) ;

  // スタミナの影響
  stamina -= power_need ;  

  // 正規化されたパワー
  return power ;
}

// ターンの影響
void WorldModel::turn(double dir) {
  Vector v1, v2, v3, v4 ;
  double max_vel, min_vel ;
  double max_dir, min_dir ;
  
  v1 = Vector(self.vel.get_x() + self.vel_error.get_x(), 
	      self.vel.get_y() + self.vel_error.get_y()) ;
  v2 = Vector(self.vel.get_x() + self.vel_error.get_x(), 
	      self.vel.get_y() - self.vel_error.get_y()) ;
  v3 = Vector(self.vel.get_x() - self.vel_error.get_x(), 
	      self.vel.get_y() + self.vel_error.get_y()) ;
  v4 = Vector(self.vel.get_x() - self.vel_error.get_x(), 
	      self.vel.get_y() - self.vel_error.get_y()) ;
  max_vel = max(max(v1.r(), v2.r()), max(v3.r(), v4.r())) ;
  min_vel = min(min(v1.r(), v2.r()), min(v3.r(), v4.r())) ;
  if (dir >= 0) {
    max_dir = dir / (1.0 + INERTIA_MOMENT * min_vel) * 1.1 ;
    min_dir = dir / (1.0 + INERTIA_MOMENT * max_vel) * 0.9 ;
  } else {
    max_dir = dir / (1.0 + INERTIA_MOMENT * max_vel) * 0.9 ;
    min_dir = dir / (1.0 + INERTIA_MOMENT * min_vel) * 1.1 ;
  }
  self.turn_body = normalize_angle((max_dir + min_dir) / 2.0) ;
  self.turn_body_error = (max_dir - min_dir) / 2.0 ;
}

// ムーブの影響
void WorldModel::move(Vector pos) {
  self.pos = pos ;
  self.pos_error = Vector(0, 0) ;
}

// キャッチの影響
void WorldModel::catch_(void) {
  ball.vel = Vector(0, 0) ;
  ball.vel_error = Vector(0, 0) ;
}


















