// $B<+J,$N%(!<%8%'%s%H$r:n$k(B

#define PLAY_ON         0
#define BEFORE_KICK_OFF 1
#define RESTARTER       2

class MyAgent : public SoccerAgent {

public :

  MyAgent(void) { flag = false ; } ;

private :

  // $B9TF07hDj(B
  void think(void) ;

  // $BH=CG(B
  // $B%W%l!<A0(B
  void before_kick_off_play() ;
  // $B%j%9%?!<%H(B
  void own_restart_play(void) ;
  // $B%j%9%?!<%H(B
  void opponent_restart_play(void) ;
  // $B%W%l!<;~(B
  void play_on_play() ;

  // $BH=CG:`NA(B
  // $B%-%C%/%?!<%2%C%H(B
  bool flag ;
  void reset_kick_target(void) ;
  void get_kick_target(Vector& target, double& require_speed) ;
  // $B%7%e!<%H0LCV(B
  Vector shot(void) ;
  // $B%Q%9$G$-$k$+$I$&$+$NH=Dj$H%Q%90LCV(B
  bool pass(Vector& target) ;
  bool clear_line(int n) ;
  // $B%9%k!<$G$-$k$+$I$&$+$NH=Dj$H%9%k!<0LCV(B
  bool throgh(Vector& target) ;
  // $B6[5^%-%C%/$G$-$k$+$I$&$+$NH=Dj$H6[5^%-%C%/0LCV(B
  bool imagency(Vector& target) ;
  // $B%G%U%)%k%H%-%C%/0LCV(B
  void default_target(Vector& target, double& require_speed) ;
  // $B%I%j%V%k%-%C%/0LCV(B
  void dribble(Vector& target, double& require_speed) ;
  // $B%j%9%?!<%H%-%C%/0LCV(B
  void restart(Vector& target, double& require_speed) ;

  // $B%\!<%k$K0lHV6a$$$+$NH=Dj(B
  bool nearest(void) ;
  // $B%\!<%k$K0lHV6a$$$+$NH=Dj(B
  bool nearest(bool without_goalie) ;
  // $B%Q%9$N<u$1<j$+$NH=Dj(B
  bool receiver(void) ;
  // $B%Q%9$N<u$1<j$+$NH=Dj(B
  bool receiver(bool without_goalie) ;
  // $B%*%U%5%$%I%]%8%7%g%s$+$NH=Dj(B
  bool offside(void) ;
  // $B%j%9%?!<%H$9$k%W%l!<%d$+$NH=Dj(B
  bool restarter(void) ;
  // $B%[!<%`%]%8%7%g%s$r<hF@(B
  Vector position(int mode) ;

  // $B%9%-%k(B
  // $B0\F0(B
  bool move(Vector pos) ;
  bool move(Vector pos, Vector vel, double max_power) ;
  bool move(Vector pos, Vector vel, double max_power, bool without_ball) ;
  bool avoid_obstacle(Vector origin, Vector target, double max_power) ;
  bool avoid_obstacle(Vector origin, Vector target, double max_power, bool without_ball) ;

  //$B%-%C%/(B
  bool multi_kick(Vector target, double require_vel) ;
  bool single_kick(Vector target, double require_speed) ;
  bool turn_ball(double dir) ;
  bool trap_ball(void) ;

  // $B%\!<%k$r8+$k(B
  bool look_ball(void) ;

  // $B%\!<%k$rC5$9(B
  bool look_for_ball(void) ;

} ;
