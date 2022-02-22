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

// 初期化
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

// コマンドの初期化
void Effector::reset(void) {
  type = CT_None ;
  arg1 = 0 ;
  arg2 = 0 ;
}

// コマンドの入力
void Effector::set(CommandType type, double arg1, double arg2) {
  this->type = type ;
  this->arg1 = arg1 ;
  this->arg2 = arg2 ;
}

// コマンドの実行
void Effector::execute(void) {
  char msg[4096] ;
  double angle_body ;

  switch (type) {
  case CT_Kick :
    // キック
    // 引数のチェック
    arg1 = NormalizeKickPower(arg1) ;
    arg2 = NormalizeMoment(arg2) ;
    // 世界モデルへの影響
    worldmodel->kick(arg1, arg2) ;
    sprintf(msg, "(kick %f %f)", arg1, arg2) ;
    break ;

  case CT_Dash :
    // ダッシュ
    // 引数のチェック＆世界モデルへの影響
    arg1 = worldmodel->dash(arg1) ;    
    // コマンド作成
    sprintf(msg, "(dash %f)", arg1) ;
    break ;

  case CT_Turn :
    // ターン
    // 引数のチェック
    arg1 = NormalizeMoment(arg1) ;
    // 世界モデルへの影響
    worldmodel->turn(arg1) ;
    // コマンド作成
    sprintf(msg, "(turn %f)", arg1) ;
    break ;

  case CT_Move :
    // ムーブ
    // ここで引数のチェックをすべき
    // 世界モデルへの影響
    worldmodel->move(Vector(arg1, arg2)) ;
    // コマンド作成
    sprintf(msg, "(move %f %f)", arg1, arg2) ;
    break ;

  case CT_Catch :
    // キャッチ
    // ここで引数のチェックをすべき
    // 世界モデルへの影響
    worldmodel->catch_() ;
    // コマンド作成
    sprintf(msg, "(catch %f)", arg1) ;
    break ;

  case CT_None :
  default :
    // デフォルト
    // コマンド作成
    sprintf(msg, "(turn 0.0)") ;
    break ;

  }

  // コマンド送信
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
    
    // コマンド送信
    netif->send_message(msg) ;
    flag = false ;
  }
}

// 首の状態の更新
void Effector::set_neck_state(int state) {
  if (state != LOOK_AROUND && state != LOOK_BALL)
    return ;
  neck_state = state ;
}










