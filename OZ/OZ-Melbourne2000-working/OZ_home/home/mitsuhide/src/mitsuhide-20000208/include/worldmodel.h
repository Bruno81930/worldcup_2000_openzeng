#define N_PLAYER_PER_TEAM 11

#define UNUM_ERR 0

#define POS_ERR   1024
#define VEL_ERR   1024
#define ANGLE_ERR -360

#define PITCH_WIDTH             68.0
#define HALF_PITCH_W		PITCH_WIDTH/2.0 

/*
 * ==================================================
 * Part : Movable Object
 * ==================================================
 */

class MovableObject {

protected :

  Vector pos ;       // 位置
  Vector pos_error ; // 位置の誤差
  Vector vel ;       // 速度
  Vector vel_error ; // 速度の誤差

public :

  // コンストラクタ
  MovableObject(void) ; 
  // リセット
  void reset(void) ;

  // 位置を知っているかの判定
  bool known_pos(void) ; 
  // 速度を知っているかの判定
  bool known_vel(void) ; 

  // 位置を得る
  Vector get_pos(void) { return pos; } ;
  // 速度を得る
  Vector get_vel(void) { return vel; } ;

  // 未整理
  double distance(MovableObject obj);
  double direction(MovableObject obj);

};


/*
 * ==================================================
 * Part : Ball
 * ==================================================
 */

class Ball : public MovableObject {

friend class WorldModel;
  
private :

  Vector acc ;           // 加速度
  Vector acc_error ;     // 加速度の誤差

public :

  // コンストラクタ
  Ball(void) ;
  // 初期化
  void init(void) ;
  // リセット
  void reset(void) ;

  void noise(void);
  void normalize_speed(void);
  void _inc(Vector player_pos, Vector next_player_pos);
  void _turn(void);

} ;


/*
 * ==================================================
 * Part : Self
 * ==================================================
 */

class Self : public MovableObject {

  friend class WorldModel;
  
private :

  double angle_body ;       // 体の角度
  double angle_body_error ; // 体の角度の誤差
  double angle_neck ;       // 首の角度
  double angle_neck_error ; // 首の角度の誤差 
  Vector acc ;              // 加速度
  Vector acc_error ;        // 加速度の誤差

  double turn_body ;        // 体の角度変化
  double turn_body_error ;  // 体の角度変化の誤差
  double turn_neck ;        // 首の角度変化

public :

  // コンストラクタ
  Self(void) ;
  // 初期化
  void init(Side side, int unum) ;
  // リセット
  void reset(void) ;

  // 体の角度を知っているかの判定
  bool known_angle_body(void) ;
  // 首の角度を知っているかの判定
  bool known_angle_neck(void) ;
  // 体の角度の取得
  double get_angle_body(void) { return angle_body; } ;
  // 首の角度の取得 
  double get_angle_neck(void) { return angle_neck; } ;
  // 体の角度変化の取得
  double get_turn_body(void) { return turn_body; } ;

  // 更新
  void set_turn_neck(double turn_neck) { this->turn_neck = turn_neck; } ;

  // 未整理
  void normalize_speed(Vector *vel, Vector *vel_error);
  void noise(Vector *vel, Vector *vel_error);
  void _inc(void);
  void _turn(void);
};


/*
 * ==================================================
 * Part : Player
 * ==================================================
 */

class Player : public MovableObject {

  friend class WorldModel;
  
private :

  int unum ;                // 背番号
  double angle_body ;       // 体の角度
  double angle_body_error ; // 体の角度の誤差
  double angle_neck ;       // 首の角度
  double angle_neck_error ; // 首の角度の誤差

public :
  
  // コンストラクタ
  Player(void) ;
  // リセット
  void reset(void) ;

  // 背番号を知っているかの判定
  bool known_unum() ;
  // 体の角度を知っているかの判定
  bool known_angle_body(void) ;
  // 首の角度を知っているかの判定
  bool known_angle_neck(void) ;

  // 背番号の取得
  int get_unum(void) { return unum; } ;
  // 体の角度の取得
  double get_angle_body(void) { return angle_body; } ;
  // 首の角度の取得
  double get_angle_neck(void) { return angle_neck; } ;

  // 未整理
  void lost(void);
  void see(int unum, Vector pos, Vector pos_error,
	   Vector vel, Vector vel_error,
	   double angle_body, double angle_body_error,
	   double angle_neck, double angle_neck_error);  
  void noise(void);
  void _inc(void);
  void _turn(void);

} ;


/*
 * ==================================================
 * Part : WorldModel
 * ==================================================
 */

class WorldModel {

public :

  Ball ball ;                              // ボール
  Self self ;                              // 自分
  Player teammate[N_PLAYER_PER_TEAM - 1] ; // 自分達のチーム
  Player opponent[N_PLAYER_PER_TEAM] ;     // 相手のチーム

private :

  int time ;             // 時刻
  PlayMode playmode ;    // プレーモード
  int t_score ;          // 自分達のチームのスコア
  int o_score ;          // 相手チームのスコア

  Side side ;            // 自分達のチームのサイド
  char* team_name ;      // 自分達のチームのチーム名

  int unum ;             // 自分の背番号

  double stamina ;       // 自分の短期スタミナ
  double effort ;        // 自分の長期スタミナ
  double recovery ;      // 自分の回復力
  ViewWidth vwidth ;     // 自分の視覚の幅
  ViewQuality vquality ; // 自分の視覚の質

  double offsideline ;   // オフサイドライン
  int goalie_number ; // 相手キーパーの番号

public :

  // 初期化
  void init(Side side, char* team_name, int unum) ;
  // リセット
  void reset(void) ;
  // 判断材料の更新
  void comm(void) ;
  // デバッグ情報による更新
  void debug(char* msg) ;
  // 世界モデルの表示
  void print(void) ;
 
  // 時刻の取得
  int get_time(void) { return time ; } ;
  // プレーモードの取得
  PlayMode get_playmode(void) { return playmode ; } ;
  // 自分達のチームのスコアの取得
  int get_t_score(void) { return t_score ; } ;
  // 相手チームのスコアの取得
  int get_o_score(void) { return o_score ; } ;
  // 自分達のチームのサイドの取得
  Side get_side(void) { return side ; } ;
  // 自分の背番号の取得
  int get_unum(void) { return unum ; } ;
  // 自分の短期スタミナの取得
  double get_stamina(void) { return stamina ; } ;
  // 自分の長期スタミナの取得
  double get_effort(void) { return effort ; } ;
  // 自分の回復力の取得
  double get_recovery(void) { return recovery ; } ;
  // 自分の視覚の幅の取得
  ViewWidth get_vwidth(void) { return vwidth;  } ;
  // 自分の視覚の質の取得
  ViewQuality get_vquality(void) { return vquality ; } ;
  // オフサイドラインの取得
  double get_offsideline(void) { return offsideline ; } ;
  // 相手のキーパーの位置の取得
  Vector get_opponent_goalie_pos(void) { return opponent[goalie_number].pos ; }

  // 時刻の更新
  void set_time(int time) { this->time = time ; } ;
  // プレーモードの更新
  void set_playmode(PlayMode playmode) { this->playmode = playmode ; } ;
  // 自分達のチームのスコアの更新
  void set_t_score(int t_score) { this->t_score = t_score; } ;
  // 相手チームのスコアの更新
  void set_o_score(int o_score) { this->o_score = o_score; } ;
  // 自分の短期スタミナの更新
  void set_stamina(double stamina) { this->stamina = stamina ; } ;
  // 自分の長期スタミナの更新
  void set_effort(double effort) { this->effort = effort ; } ;
  // 自分の回復力の更新
  void set_recovery(double recovery) { this->recovery = recovery ; } ;
  // 自分の視覚の幅の更新
  void set_vwidth(ViewWidth vwidth) { this->vwidth = vwidth ; } ;
  // 自分の視覚の質の更新
  void set_vquality(ViewQuality vquality) { this->vquality = vquality ; } ;

  // 未整理
  void add_seeinfo(char* msg) ;

  // コマンドの影響
  // キックの影響
  void kick(double power, double dir) ;
  // ダッシュの影響
  double dash(double power) ;
  // ターンの影響
  void turn(double dir) ;
  // ムーブの影響
  void move(Vector pos) ;
  // キャッチの影響
  void catch_(void) ;

} ;




