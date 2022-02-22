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

  Vector pos ;       // $B0LCV(B
  Vector pos_error ; // $B0LCV$N8m:9(B
  Vector vel ;       // $BB.EY(B
  Vector vel_error ; // $BB.EY$N8m:9(B

public :

  // $B%3%s%9%H%i%/%?(B
  MovableObject(void) ; 
  // $B%j%;%C%H(B
  void reset(void) ;

  // $B0LCV$rCN$C$F$$$k$+$NH=Dj(B
  bool known_pos(void) ; 
  // $BB.EY$rCN$C$F$$$k$+$NH=Dj(B
  bool known_vel(void) ; 

  // $B0LCV$rF@$k(B
  Vector get_pos(void) { return pos; } ;
  // $BB.EY$rF@$k(B
  Vector get_vel(void) { return vel; } ;

  // $BL$@0M}(B
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

  Vector acc ;           // $B2CB.EY(B
  Vector acc_error ;     // $B2CB.EY$N8m:9(B

public :

  // $B%3%s%9%H%i%/%?(B
  Ball(void) ;
  // $B=i4|2=(B
  void init(void) ;
  // $B%j%;%C%H(B
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

  double angle_body ;       // $BBN$N3QEY(B
  double angle_body_error ; // $BBN$N3QEY$N8m:9(B
  double angle_neck ;       // $B<s$N3QEY(B
  double angle_neck_error ; // $B<s$N3QEY$N8m:9(B 
  Vector acc ;              // $B2CB.EY(B
  Vector acc_error ;        // $B2CB.EY$N8m:9(B

  double turn_body ;        // $BBN$N3QEYJQ2=(B
  double turn_body_error ;  // $BBN$N3QEYJQ2=$N8m:9(B
  double turn_neck ;        // $B<s$N3QEYJQ2=(B

public :

  // $B%3%s%9%H%i%/%?(B
  Self(void) ;
  // $B=i4|2=(B
  void init(Side side, int unum) ;
  // $B%j%;%C%H(B
  void reset(void) ;

  // $BBN$N3QEY$rCN$C$F$$$k$+$NH=Dj(B
  bool known_angle_body(void) ;
  // $B<s$N3QEY$rCN$C$F$$$k$+$NH=Dj(B
  bool known_angle_neck(void) ;
  // $BBN$N3QEY$N<hF@(B
  double get_angle_body(void) { return angle_body; } ;
  // $B<s$N3QEY$N<hF@(B 
  double get_angle_neck(void) { return angle_neck; } ;
  // $BBN$N3QEYJQ2=$N<hF@(B
  double get_turn_body(void) { return turn_body; } ;

  // $B99?7(B
  void set_turn_neck(double turn_neck) { this->turn_neck = turn_neck; } ;

  // $BL$@0M}(B
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

  int unum ;                // $BGXHV9f(B
  double angle_body ;       // $BBN$N3QEY(B
  double angle_body_error ; // $BBN$N3QEY$N8m:9(B
  double angle_neck ;       // $B<s$N3QEY(B
  double angle_neck_error ; // $B<s$N3QEY$N8m:9(B

public :
  
  // $B%3%s%9%H%i%/%?(B
  Player(void) ;
  // $B%j%;%C%H(B
  void reset(void) ;

  // $BGXHV9f$rCN$C$F$$$k$+$NH=Dj(B
  bool known_unum() ;
  // $BBN$N3QEY$rCN$C$F$$$k$+$NH=Dj(B
  bool known_angle_body(void) ;
  // $B<s$N3QEY$rCN$C$F$$$k$+$NH=Dj(B
  bool known_angle_neck(void) ;

  // $BGXHV9f$N<hF@(B
  int get_unum(void) { return unum; } ;
  // $BBN$N3QEY$N<hF@(B
  double get_angle_body(void) { return angle_body; } ;
  // $B<s$N3QEY$N<hF@(B
  double get_angle_neck(void) { return angle_neck; } ;

  // $BL$@0M}(B
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

  Ball ball ;                              // $B%\!<%k(B
  Self self ;                              // $B<+J,(B
  Player teammate[N_PLAYER_PER_TEAM - 1] ; // $B<+J,C#$N%A!<%`(B
  Player opponent[N_PLAYER_PER_TEAM] ;     // $BAj<j$N%A!<%`(B

private :

  int time ;             // $B;~9o(B
  PlayMode playmode ;    // $B%W%l!<%b!<%I(B
  int t_score ;          // $B<+J,C#$N%A!<%`$N%9%3%"(B
  int o_score ;          // $BAj<j%A!<%`$N%9%3%"(B

  Side side ;            // $B<+J,C#$N%A!<%`$N%5%$%I(B
  char* team_name ;      // $B<+J,C#$N%A!<%`$N%A!<%`L>(B

  int unum ;             // $B<+J,$NGXHV9f(B

  double stamina ;       // $B<+J,$NC;4|%9%?%_%J(B
  double effort ;        // $B<+J,$ND94|%9%?%_%J(B
  double recovery ;      // $B<+J,$N2sI|NO(B
  ViewWidth vwidth ;     // $B<+J,$N;k3P$NI}(B
  ViewQuality vquality ; // $B<+J,$N;k3P$N<A(B

  double offsideline ;   // $B%*%U%5%$%I%i%$%s(B
  int goalie_number ; // $BAj<j%-!<%Q!<$NHV9f(B

public :

  // $B=i4|2=(B
  void init(Side side, char* team_name, int unum) ;
  // $B%j%;%C%H(B
  void reset(void) ;
  // $BH=CG:`NA$N99?7(B
  void comm(void) ;
  // $B%G%P%C%0>pJs$K$h$k99?7(B
  void debug(char* msg) ;
  // $B@$3&%b%G%k$NI=<((B
  void print(void) ;
 
  // $B;~9o$N<hF@(B
  int get_time(void) { return time ; } ;
  // $B%W%l!<%b!<%I$N<hF@(B
  PlayMode get_playmode(void) { return playmode ; } ;
  // $B<+J,C#$N%A!<%`$N%9%3%"$N<hF@(B
  int get_t_score(void) { return t_score ; } ;
  // $BAj<j%A!<%`$N%9%3%"$N<hF@(B
  int get_o_score(void) { return o_score ; } ;
  // $B<+J,C#$N%A!<%`$N%5%$%I$N<hF@(B
  Side get_side(void) { return side ; } ;
  // $B<+J,$NGXHV9f$N<hF@(B
  int get_unum(void) { return unum ; } ;
  // $B<+J,$NC;4|%9%?%_%J$N<hF@(B
  double get_stamina(void) { return stamina ; } ;
  // $B<+J,$ND94|%9%?%_%J$N<hF@(B
  double get_effort(void) { return effort ; } ;
  // $B<+J,$N2sI|NO$N<hF@(B
  double get_recovery(void) { return recovery ; } ;
  // $B<+J,$N;k3P$NI}$N<hF@(B
  ViewWidth get_vwidth(void) { return vwidth;  } ;
  // $B<+J,$N;k3P$N<A$N<hF@(B
  ViewQuality get_vquality(void) { return vquality ; } ;
  // $B%*%U%5%$%I%i%$%s$N<hF@(B
  double get_offsideline(void) { return offsideline ; } ;
  // $BAj<j$N%-!<%Q!<$N0LCV$N<hF@(B
  Vector get_opponent_goalie_pos(void) { return opponent[goalie_number].pos ; }

  // $B;~9o$N99?7(B
  void set_time(int time) { this->time = time ; } ;
  // $B%W%l!<%b!<%I$N99?7(B
  void set_playmode(PlayMode playmode) { this->playmode = playmode ; } ;
  // $B<+J,C#$N%A!<%`$N%9%3%"$N99?7(B
  void set_t_score(int t_score) { this->t_score = t_score; } ;
  // $BAj<j%A!<%`$N%9%3%"$N99?7(B
  void set_o_score(int o_score) { this->o_score = o_score; } ;
  // $B<+J,$NC;4|%9%?%_%J$N99?7(B
  void set_stamina(double stamina) { this->stamina = stamina ; } ;
  // $B<+J,$ND94|%9%?%_%J$N99?7(B
  void set_effort(double effort) { this->effort = effort ; } ;
  // $B<+J,$N2sI|NO$N99?7(B
  void set_recovery(double recovery) { this->recovery = recovery ; } ;
  // $B<+J,$N;k3P$NI}$N99?7(B
  void set_vwidth(ViewWidth vwidth) { this->vwidth = vwidth ; } ;
  // $B<+J,$N;k3P$N<A$N99?7(B
  void set_vquality(ViewQuality vquality) { this->vquality = vquality ; } ;

  // $BL$@0M}(B
  void add_seeinfo(char* msg) ;

  // $B%3%^%s%I$N1F6A(B
  // $B%-%C%/$N1F6A(B
  void kick(double power, double dir) ;
  // $B%@%C%7%e$N1F6A(B
  double dash(double power) ;
  // $B%?!<%s$N1F6A(B
  void turn(double dir) ;
  // $B%`!<%V$N1F6A(B
  void move(Vector pos) ;
  // $B%-%c%C%A$N1F6A(B
  void catch_(void) ;

} ;




