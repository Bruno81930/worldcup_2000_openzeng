void sigio_handler(void);
void sigalrm_handler(void);


class SoccerAgent {
private:
  char   *host;      // $B%[%9%HL>(B
  int    port;       // $B%]!<%HHV9f(B
  double version;    // $B%P!<%8%g%sHV9f(B
  char   *team_name; // $B%A!<%`L>(B
  bool   goalie;     // $B%-!<%Q!<%U%i%0(B   
  int unum;          // $BGXHV9f(B
  Side side;         // $B%5%$%I(B

  NetworkInterface netif ; // $B%M%C%H%o!<%/%$%s%?!<%U%'!<%9(B

  // $BF14|=hM}JQ?t(B
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

  WorldModel worldmodel ;  // $B%o!<%k%I%b%G%k(B
  Effector effector ;        // $B%3%^%s%I%$%s%?%W%j%?(B

private :

  // $B$3$N4X?t$r<+J,$NGI@8%/%i%9$GDj5A$9$k(B
  virtual void think(void) ;
};

extern SoccerAgent* sagent;








