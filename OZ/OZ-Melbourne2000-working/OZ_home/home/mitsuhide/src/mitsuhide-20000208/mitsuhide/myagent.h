// 自分のエージェントを作る

#define PLAY_ON         0
#define BEFORE_KICK_OFF 1
#define RESTARTER       2

class MyAgent : public SoccerAgent {

public :

  MyAgent(void) { flag = false ; } ;

private :

  // 行動決定
  void think(void) ;

  // 判断
  // プレー前
  void before_kick_off_play() ;
  // リスタート
  void own_restart_play(void) ;
  // リスタート
  void opponent_restart_play(void) ;
  // プレー時
  void play_on_play() ;

  // 判断材料
  // キックターゲット
  bool flag ;
  void reset_kick_target(void) ;
  void get_kick_target(Vector& target, double& require_speed) ;
  // シュート位置
  Vector shot(void) ;
  // パスできるかどうかの判定とパス位置
  bool pass(Vector& target) ;
  bool clear_line(int n) ;
  // スルーできるかどうかの判定とスルー位置
  bool throgh(Vector& target) ;
  // 緊急キックできるかどうかの判定と緊急キック位置
  bool imagency(Vector& target) ;
  // デフォルトキック位置
  void default_target(Vector& target, double& require_speed) ;
  // ドリブルキック位置
  void dribble(Vector& target, double& require_speed) ;
  // リスタートキック位置
  void restart(Vector& target, double& require_speed) ;

  // ボールに一番近いかの判定
  bool nearest(void) ;
  // ボールに一番近いかの判定
  bool nearest(bool without_goalie) ;
  // パスの受け手かの判定
  bool receiver(void) ;
  // パスの受け手かの判定
  bool receiver(bool without_goalie) ;
  // オフサイドポジションかの判定
  bool offside(void) ;
  // リスタートするプレーヤかの判定
  bool restarter(void) ;
  // ホームポジションを取得
  Vector position(int mode) ;

  // スキル
  // 移動
  bool move(Vector pos) ;
  bool move(Vector pos, Vector vel, double max_power) ;
  bool move(Vector pos, Vector vel, double max_power, bool without_ball) ;
  bool avoid_obstacle(Vector origin, Vector target, double max_power) ;
  bool avoid_obstacle(Vector origin, Vector target, double max_power, bool without_ball) ;

  //キック
  bool multi_kick(Vector target, double require_vel) ;
  bool single_kick(Vector target, double require_speed) ;
  bool turn_ball(double dir) ;
  bool trap_ball(void) ;

  // ボールを見る
  bool look_ball(void) ;

  // ボールを探す
  bool look_for_ball(void) ;

} ;
