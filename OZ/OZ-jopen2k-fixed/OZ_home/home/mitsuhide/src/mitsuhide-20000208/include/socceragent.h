void sigio_handler(void);
void sigalrm_handler(void);


class SoccerAgent {
private:
  char   *host;      // ホスト名
  int    port;       // ポート番号
  double version;    // バージョン番号
  char   *team_name; // チーム名
  bool   goalie;     // キーパーフラグ   
  int unum;          // 背番号
  Side side;         // サイド

  NetworkInterface netif ; // ネットワークインターフェース

  // 同期処理変数
  bool sync_see_flag;
  bool sync;       
  ViewType type;   
  int n_sensebody;
  int past_n_sensebody; 
  char sync_see_msg[MESSAGE_LENGTH];
  SeeType  msec;

public:
  void init(int argc, char** argv);
  void main_loop(void);

  void sigio_handler(void);
  void sigalrm_handler(void);

private:  
  void receive_init(char* msg);
  void receive_see(char* msg);
  void receive_sensebody(char* msg);
  void receive_hear(char* msg);

  void see(char* msg);
  void sensebody(char* msg);
  void hear_referee(char* msg);
  void hear_say(char* msg); 

  void set_timer(int msec);
  void step(void);
  void send_change_view_command(void);

protected :    

  WorldModel worldmodel ;  // ワールドモデル
  Effector effector ;        // コマンドインタプリタ

private :

  // この関数を自分の派生クラスで定義する
  virtual void think(void) ;
};

extern SoccerAgent* sagent;








