typedef enum {
  CT_Kick ,
  CT_Dash ,
  CT_Turn ,
  CT_Move ,
  CT_Catch ,
  CT_None
} CommandType ;

#define LOOK_AROUND 0
#define LOOK_BALL   1

class WorldModel ;

class Effector {

public :

  // 初期化
  void init(NetworkInterface* netif, WorldModel* worldmodel) ;
  // コマンドの初期化
  void reset(void) ;
  // コマンドの入力
  void set(CommandType type, double arg1, double arg2) ;
  // コマンドの実行
  void execute(void) ;

  // 首の状態の更新
  void set_neck_state(int state) ;
  // 視覚情報を得た
  void set_flag(void) { flag = true ; } ;

private :

  // ネットワークインタフェースへのポインタ
  NetworkInterface* netif ;
  // 世界モデルへのポインタ
  WorldModel* worldmodel ;
  // コマンドの種類
  CommandType type ;
  // コマンドの引数
  double arg1 ;
  double arg2 ;

  // 首の状態
  int neck_state ;
  bool flag ;
  double neck_dir ;

} ;

