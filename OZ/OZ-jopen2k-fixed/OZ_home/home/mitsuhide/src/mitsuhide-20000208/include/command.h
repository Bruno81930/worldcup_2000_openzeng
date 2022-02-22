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

  // $B=i4|2=(B
  void init(NetworkInterface* netif, WorldModel* worldmodel) ;
  // $B%3%^%s%I$N=i4|2=(B
  void reset(void) ;
  // $B%3%^%s%I$NF~NO(B
  void set(CommandType type, double arg1, double arg2) ;
  // $B%3%^%s%I$N<B9T(B
  void execute(void) ;

  // $B<s$N>uBV$N99?7(B
  void set_neck_state(int state) ;
  // $B;k3P>pJs$rF@$?(B
  void set_flag(void) { flag = true ; } ;

private :

  // $B%M%C%H%o!<%/%$%s%?%U%'!<%9$X$N%]%$%s%?(B
  NetworkInterface* netif ;
  // $B@$3&%b%G%k$X$N%]%$%s%?(B
  WorldModel* worldmodel ;
  // $B%3%^%s%I$N<oN`(B
  CommandType type ;
  // $B%3%^%s%I$N0z?t(B
  double arg1 ;
  double arg2 ;

  // $B<s$N>uBV(B
  int neck_state ;
  bool flag ;
  double neck_dir ;

} ;

