// InfoManager.java

package info_managerP;


import genericP.*;
import java.awt.geom.*;
import objectP.*;
import stokenizerP.*;
import parserP.*;

/********************************************************/
/*         InfoManager          $B>pJs$N4IM}(B,$B2C9)(B         */
/********************************************************/

public class InfoManager{

  String teamname;
  String side;
  int uniformNum;

  int seeTime;
  int prevSeeTime;
  int bodyTime;
  int hearTime;
  int lastUpdateEtcTime;

  int playmode;
    
  // $B%S%e!<%b!<%I(B
  String angleWidth;
  String quality;

  // $B%9%?%_%J4X78(B
  double stamina;
  double effort;
  double recovery;

  // $B0LCV4X78(B
  double x,y;
  double bodyFaceDir,headFaceDir; // $BBN(B,$B<s$N8~$-(B ***** $B@dBP:BI8(B *****
  double headAngle; // $B<s$N8~$-(B  ***** $BBNCf?4:BI8(B *****
  double speed,vx,vy;
  double ax,ay,pvx,pvy;
  final int puSize = 5;
  double ux,uy;
  double[] pux = new double[puSize];
  double[] puy = new double[puSize];
  // ux[],uy[] $B$H$7$F$b$h$$$N$@$1$I(B,$B$3$3$G$O8=:_$N>pJs$H2a5n$N>pJs$,(B
  // $B$4$C$A$c$K$J$i$J$$$h$&$K(B
  // ux[],uy[] $B$K$9$k>l9g$O%a%=%C%I$r?7$7$/Dj5A$7$F$=$l$r2p$7$F(B
  // $B8=:_$N>pJs$H2a5n$N>pJs$H$r$O$C$-$j6hJL$7$F<h$j=P$;$k$h$&$K(B
  // $B$9$k$3$H(B
  double pHeadFaceDir;  // $B0l$DA0$N(BSee$B>pJs$rF@$?$H$-$K<s$,LL$7$F$$$?J}8~(B
  // $B$3$NItJ,$O8eIU$1$K$J$C$F$7$^$C$?$?$aL5M}$d$j(B

  double pBallVX,pBallVY;

  // $BF@E@(B
  int ourPoint,opponentsPoint;

  // $B%+%&%s%?(B
  int kickCount;
  int dashCount;
  int turnCount;
  int sayCount;
  int turnNeckCount;

  // see $B$H(B sense $B$NF14|$r<h$k0Y(B
  private double nextBodyAbsDir;
  private double nextHeadRelDir;
  private double nextHeadAbsDir;

  private final int UNKNOWN = -999; // UNKOWN $B<1JL;R(B

  SeeInfoParser seeInfoParser = new SeeInfoParser();
  BodyInfoParser bodyInfoParser = new BodyInfoParser();
  HearInfoParser hearInfoParser = new HearInfoParser();

  PlayerTable playerTable = new PlayerTable();  //  $B%*%V%8%'%/%H$N@8@.$O!)!)(B
  StrategyTable strategyTable = new StrategyTable(playerTable);

  BallInfo ball = new BallInfo();
  BallInfo ballShadow = new BallInfo();;
  boolean knowsBallInfo;  //  $B%\!<%k$N>pJs$r;}$C$F$$$k$+$I$&$+(B
  int searchFlag;

  String selfString;
  String oppForwardString;

  String strategy;               //  $B:NMQ$7$F$$$k6(D4@oN,(B
  String strategyType;           //  $B@oN,$N%?%$%W(B
  double strategyCertainty;      //  $B@oN,$N?.MjEY(B
  boolean strategyFlag;          //  $B?75,$N@oN,$rJ9$$$?$+$I$&$+!)(B
  int planner;                   //  $B@oN,Ds0F<T(B
  int planTime = 0;              //  

  double ctStX,ctStY,ctSdX,ctSdY;                // $B%3!<%A$+$i$N%]%8%7%g%s(B
  boolean stdPosChanged = Generic.NS_FALSE;
  boolean evalSRFlag = Generic.NS_FALSE;

/***************************************************************************/

  //**************************************//
  // add for coach 0801 by Shinji Futamase//
  //**************************************//
  double ctkX, ctkY;               //FK$B;~$N%3!<%A$+$i;X<($5$l$?%-%C%/0LCV(B
                                    //$B8=:_$O(BGK$B$N$_$,:NMQ(B

  double ctmyX, ctmyY;              //FK$B;~$N%3!<%A$+$i;X<($5$l$?<+0LCV(B
                                    //$B8=:_$O(BGK$B$N$_$,:NMQ(B
 
  boolean freekickPosChanged = false;//$B%3!<%A$+$i$N;X<($,$"$C$?$+$I$&$+(B
  


/***************************************************************************/
  public InfoManager(String tname,int uNum,String s){
    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("Creating InfoManager.");
    }
    */

    teamname = tname;
    side = s;

    seeInfoParser.setSide(side);
    hearInfoParser.setSide(side);
    hearInfoParser.setTeamname(teamname);

    uniformNum = uNum;

    seeTime = 0;
    bodyTime = 0;

    stamina = Generic.STAMINA_MAX;
    effort = 1.0;
    recovery = 1.0;

    // $B0LCV4X78(B
    x = 0;y = 0;
    bodyFaceDir = 0;headFaceDir = 0; // $BBN(B,$B<s$N8~$-(B ***** $B@dBP:BI8(B *****
    headAngle = 0; // $B<s$N8~$-(B  ***** $BBNCf?4:BI8(B *****
    speed = vx = vy = pvx = pvy = 0;
    ax = ay = ux = uy = 0;
    pBallVX = pBallVY = 0;


    // $B%+%&%s%?(B
    kickCount = 0;
    dashCount = 0;
    turnCount = 0;
    sayCount = 0;
    turnNeckCount = 0;

    knowsBallInfo = Generic.NS_FALSE;
    searchFlag = 0;

    strategy = "private";
    strategyType = "show";
    strategyCertainty = 0.0;
    strategyFlag = Generic.NS_FALSE;
    planner = 0;

    // see $B$H(B sense $B$NF14|$r<h$k$?$a$N@)8f$N=i4|2=(B
    nextBodyAbsDir = UNKNOWN;
    nextHeadRelDir = UNKNOWN;
    nextHeadAbsDir = UNKNOWN;
  }

/***************************************************************************/

  void calcFaceDir(){  // headFaceDir,bodyFaceDir $B$N7W;;(B
    double tmpDir = 0;

    //$B%i%$%s>pJs$r;}$C$F$J$1$l$P99?7$7$J$$(B
    // updateByTurn() $B$@$1$G==J,$G$7$g(B
    if(this.seeInfoParser.getLineNum() == 0)
	return;

    LineInfo line = this.seeInfoParser.getLineInfo(0);
    
    String location;
    location = line.getLocation();
    double lineDir = line.getDir();
    boolean inField = isInField();

    if(location.equals("t"))
      if(inField)
        tmpDir = - lineDir - ((lineDir < 0) ? 180.0 : 0);
      else
        tmpDir = - lineDir - ((lineDir > 0) ? -180.0 : 0);
    else if(location.equals("b"))
      if(inField)
        tmpDir = - lineDir - ((lineDir > 0) ? -180.0 : 0);
      else
        tmpDir = - lineDir - ((lineDir < 0) ? 180.0 : 0);
    else if(location.equals("l"))
      if(inField)
        tmpDir = - lineDir - ((lineDir < 0) ? -90.0 : 90.0);
      else
        tmpDir = - lineDir - ((lineDir > 0) ? -90.0 : 90.0);
    else if(location.equals("r"))
      if(inField)
        tmpDir = - lineDir - ((lineDir > 0) ? -90.0 : 90.0);
      else
        tmpDir = - lineDir - ((lineDir < 0) ? -90.0 : 90.0);

    if(this.side.equals("r"))      // $B<+%5%$%I$,1&$N$H$-(B
      tmpDir += ((tmpDir > 0) ? -180.0 : 180.0);


    // $BM=B,$5$l$k3QEY$HBgI}$K0[$J$k$H$-$O99?7$r$7$J$$(B
    // $B$?$a$N%3!<%I$r8e$+$iDI2C!D$9$k$H$$$$$+$b$M(B

    this.headFaceDir = tmpDir;

    /*
    if(Generic.DEBUG){    // $B%G%P%C%0MQ(B
      System.out.println("headFaceDir : " + this.headFaceDir);
    }
    */

    this.bodyFaceDir = Generic.normDir(this.headFaceDir - this.headAngle);

    /*
    if(Generic.DEBUG){    // $B%G%P%C%0MQ(B
      System.out.println("bodyFaceDir : " + this.bodyFaceDir);
    }
    */

  }

/***************************************************************************/

  void calcPos(){ // $B8=:_0LCV$N7W;;(B
    FlagInfo[] flags = this.seeInfoParser.getFlags();
    int flagNumber = this.seeInfoParser.getFlagNum();
    // int goalNum = this.seeInfoParser.getGoalNum();

    // $B%U%i%0$b%4!<%k$b8+$($J$$$H$-$O99?7$7$J$$(B
    // updateEtc() $B$G==J,$@$H;W$&(B
    if(flagNumber == 0)
      return;

    if (flagNumber > 2){  // 10?
      // $B;03QB,NL$K$h$k7W;;(B
      double myX,myY;          // $B<+J,$N:BI8(B
      double xa = 0,ya = 0;    // 1 $BHVL\$K6a$$%U%i%0$N(B x $B:BI8(B y $B:BI8(B
      double xb = 0,yb = 0;    // 2 $BHVL\$K6a$$%U%i%0$N(B x $B:BI8(B y $B:BI8(B

      xa = flags[0].getX();
      ya = flags[0].getY();

      xb = flags[1].getX();
      yb = flags[1].getY();

      double angle; //$B%U%i%0$NAjBP3QEY(B

      double pridir = flags[0].getDir();
      double secdir = flags[1].getDir();

      if (pridir > secdir){
	angle = pridir - secdir;
      }
      else{
	angle = secdir - pridir;
      }

      angle = angle * Math.PI / 180;

      double theta = Math.asin(flags[0].getDist()*Math.sin(angle)/Point2D.distance(xa,ya,xb,yb));

      double alpha = Math.PI - angle - theta;

      if (flags[0].getDir() > flags[1].getDir()) alpha = alpha * -1;

      this.x = xa+((xb-xa)*Math.cos(alpha)-(yb-ya)*Math.sin(alpha))*flags[0].getDist()/Point2D.distance(xa,ya,xb,yb);

      this.y = ya+((yb-ya)*Math.cos(alpha)+(xb-xa)*Math.sin(alpha))*flags[0].getDist()/Point2D.distance(xa,ya,xb,yb);

      // self.setPos(myX,myY);

    }
    else{
      // $B%U%i%0>pJs$,(B 1 $BK\$7$+$J$$;~$N2r@O(B $B0lK\B,NL(B

      double xa = 0,ya = 0;    //$B2r@O$9$k%U%i%0$N(B x $B:BI8(B y $B:BI8(B
      
      xa = flags[0].getX();
      ya = flags[0].getY();
      
      double angle; //$B%U%i%0$N@dBP3Q(B
      
      angle = Generic.normDir(headFaceDir + flags[0].getDir());
      
      this.x = xa - flags[0].getDist() * Math.cos(angle * Math.PI / 180);
      this.y = ya - flags[0].getDist() * Math.sin(angle * Math.PI / 180);
    }

    // $B>e5-$N7W;;$O%i%$%s$.$o$GHs>o$K2x$7$$$N$G6/@)=$@5(B
    if (uniformNum == 1){
      if (this.x < -52.5){
        this.x = -52.5;
      }
      if (this.x >-20){
        this.x = -52.5;
      }
    }


  }

/***************************************************************************/

  void calcV(){   // $BB.EY$N7W;;(B
    this.vx = this.speed * Math.cos(Generic.digToRad(this.bodyFaceDir));
    this.vy = this.speed * Math.sin(Generic.digToRad(this.bodyFaceDir));

    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("VX : " + this.vx);
      System.out.println("VY : " + this.vy);
    }
    */
  }

/***************************************************************************/

  public void printBodyInfo(){
    System.out.println("************* BodyInfo *************");
    System.out.println("Time : " + this.bodyTime);
    System.out.println("AngleWidth : " + this.angleWidth);
    System.out.println("Quality : " + this.quality);
    System.out.println("Stamina : " + this.stamina);
    System.out.println("Effort : " + this.effort);
    System.out.println("Speed : " + this.speed );
    System.out.println("HeadAngle : " + this.headAngle);
  }

/***************************************************************************/
  /*
  public double normDir(double direction){  // $B3QEY$N@55,2=(B(Generic$B$HF1$8(B)
    if(direction > 180.0)
      return direction - 360.0;
    else if(direction < -180.0)
      return direction + 360.0;
    else
      return direction;
  }
  */
/***************************************************************************/

  public double getXMoveDist(double velX,double power,double direction){
    return velX + power * Math.cos(Generic.digToRad(direction));
  }

/***************************************************************************/

  public double getYMoveDist(double velY,double power,double direction){
    return velY + power * Math.sin(Generic.digToRad(direction));
  }
    
/***************************************************************************/

    // $BL\I8$H$9$k%9%T!<%I$KC#$9$k$?$a$N(B DashPower
    // $B$?$@$7(B,$B$=$N%9%T!<%I(Bm/cycle $B$G0\F0$9$k$o$1$G$O$J$$$3$H$KCm0U(B
    // $BLr$K$?$?$J$$$+$b(B?
  public double getDashPowerBySpeed(double targetSpeed){
    double actualPower;

    actualPower = (targetSpeed / Generic.PLAYER_DECAY - this.speed)
                   / Generic.DASH_POWER_RATE;

    return actualPower / this.effort;
  }

/***************************************************************************/

  // $B5wN%(B distance $B%a!<%H%k$r0\F0$9$k$?$a$N(B DashPower $B$r7W;;(B
  // $B$D$^$j(B m/cycle $B$G0\F0$7$?$$$J$i$3$C$A$r;H$&(B
  public double getDashPowerByMoveDist(double distance){
    double actualPower;
    
    actualPower = (distance - this.speed) / Generic.DASH_POWER_RATE;
    
    return actualPower / this.effort;
  }

/***************************************************************************/


  // actualAngle $BEY2sE>$9$k$?$a$N(B moment $B$r7W;;(B
  public double getMoment(double actualAngle){
    return actualAngle * (1.0 + Generic.INERTIA_MOMENT * this.speed);
  }

/***************************************************************************/

  public void getKickArgByAccel(double AX,double AY,
                                NSobject power,NSobject dir){ // NSobject!!
    double adir = Generic.RADIAN * Math.atan2(AY, AX);
                                               //$BI,MW$J2CB.EY$NJ}8~(B $B@dBP:BI87O(B
    
    dir.setNS(Generic.normDir(adir - this.bodyFaceDir)); // $BBNCf?4:BI87O$KD>$9(B

    double a = Math.sqrt( AX * AX + AY * AY ); // $BI,MW$J2CB.EY$NBg$-$5$r5a$a$k(B

    power.setNS(a / (Generic.KICK_POWER_RATE *
		(1 - 0.25 * Math.abs((Generic.normDir
                                    (getAbsDir(this.ball) - this.bodyFaceDir)))
		  / 180.0 - 0.25 * (getBallDist()
                   - Generic.PLAYER_SIZE - Generic.BALL_SIZE) 
                  / Generic.KICKABLE_MARGIN)));

    /*
    double a = AX * AX + AY * AY;

    power.setNS(Math.sqrt(a / 
                        (Generic.KICK_POWER_RATE * Generic.KICK_POWER_RATE)));
    */

    if(power.getNS() > 100)
      power.setNS(100);
  }

/***************************************************************************/

  // $B%\!<%k$r8=:_0LCV$+$i(B X,Y $B%a!<%H%k0\F0$5$;$k$?$a$NNO$HJ}8~$r5a$a$k(B
  // $B7k2L$OL>A0EO$7$G$"$k$3$H$KCm0U(B  X,Y $B$O@dBP:BI87O(B
  public void getKickArgByMoveDist(double X,double Y,
                                   NSobject power,NSobject dir){ //NSobject!!

    double ax = X - this.ball.getVX();  // $B$H$j$"$($:@dBP:BI87O$G$N(B
    double ay = Y - this.ball.getVY();  // $BI,MW$J2CB.EY$r5a$a$k(B

    /*
    double adir = Generic.RADIAN * Math.atan2(ay, ax);
                                           //$BI,MW$J2CB.EY$NJ}8~(B $B@dBP:BI87O(B
    
    dir.setNS(Generic.normDir(adir - this.bodyFaceDir); // $BBNCf?4:BI87O$KD>$9(B

    double a = Math.sqrt( ax * ax + ay * ay ); // $BI,MW$J2CB.EY$NBg$-$5$r5a$a$k(B

    power.setNS(a / (Generic.KICK_POWER_RATE *
		 (1 - 0.25 * Math.abs((Generic.normDir(getAbsDir(this.ball)
                                                              - bodyFaceDir)))
		  / 180.0 - 0.25 * (getBallDist()
                                    - Generic.PLAYER_SIZE - Generic.BALL_SIZE)
		  / Generic.KICKABLE_MARGIN));

    if(power.getNS() > 100)
      power.setNS(100);
    */

    getKickArgByAccel(ax,ay,power,dir);  // $B7k2L$OL>A0EO$7$G$"$k$3$H$KCm0U(B ?? 
  }

/***************************************************************************/
  // speed $B$O=iB.(B X,Y$B$OL\I8$N@dBP:BI8(B
  public  void getKickArgByDest(double X,double Y,double speed,
			  NSobject power,NSobject dir){
    dir.setNS(Generic.RADIAN * Math.atan2
                                (Y - this.ball.getY(),X - this.ball.getX()));
    double ballVX = speed * Math.cos(Generic.digToRad(dir.getNS()));
    double ballVY = speed * Math.sin(Generic.digToRad(dir.getNS()));
    
    double ax = ballVX / Generic.BALL_DECAY - this.ball.getVX();
    double ay = ballVY / Generic.BALL_DECAY - this.ball.getVY();

    getKickArgByAccel(ax,ay,power,dir); // $B7k2L$OL>A0EO$7$G$"$k$3$H$KCm0U(B??
  }

/***************************************************************************/

  // (X,Y)$B$X(BPowerShot$B$r7b$D$?$a$N(BKick$B$N0z?t$r5a$a$k(B
  // save $B$ON/$aF0:n$+$I$&$+(B
  public void getKickArgForPowerShot(double X,double Y,NSobject power,
                                       NSobject dir,boolean save){ //NSobject!
    double margin = 0.15;
    double destRelX = X - x;
    double destRelY = Y - y;
    double ballRelX = this.ball.getX() - x;
    double ballRelY = this.ball.getY() - y;
    
    double tmpX,tmpY,tmpDist1,tmpDist2;       // tmpX1,tmpX2,tmpY1,tmpY2
    double r = Generic.KICKABLE_AREA - margin;

    Point tmpXY1 = new Point();                        // Point !!
    Point tmpXY2 = new Point();

    //Generic.getCrossPoint(r,destRelX,destRelY,ballRelX,ballRelY,
    //                      tmpXY1,tmpXY2);                       // Point !!

    double destDist,centDir1,centDir2,centRelX1,centRelY1,centRelX2,centRelY2;
    double centDist1,centDist2,centRelX,centRelY;
    
    destDist = Math.sqrt(destRelX * destRelX + destRelY * destRelY);
    centDir1 = Generic.normDir(Generic.RADIAN * Math.atan2(destRelY,destRelX)
	+ (90 - Generic.RADIAN * Math.asin(0.5 / destDist)));
    centDir2 = Generic.normDir(Generic.RADIAN * Math.atan2(destRelY,destRelX)
	- (90 - Generic.RADIAN * Math.asin(0.5 / destDist)));
    centRelX1 = 0.5 * Math.cos(centDir1);
    centRelY1 = 0.5 * Math.sin(centDir1);
    centRelX2 = 0.5 * Math.cos(centDir2);
    centRelY2 = 0.5 * Math.sin(centDir2);
    centDist1 = Math.sqrt(Math.pow(centRelX1 - ballRelX,2)
		     + Math.pow(centRelY1 - ballRelY,2));
    centDist2 = Math.sqrt(Math.pow(centRelX2 - ballRelX,2)
		     + Math.pow(centRelY2 - ballRelY,2));
    if(centDist1 <= centDist2){
      centRelX = centRelX1;
      centRelY = centRelY1;
    }
    else {
      centRelX = centRelX2;
      centRelY = centRelY2;
    }
    
    if(save){
      double keepDir = Generic.RADIAN * Math.atan2(centRelY,centRelX)
                                                                - bodyFaceDir;
      double keepDist = Math.sqrt(centRelX * centRelX + centRelY * centRelY);
      getKickArgByKeepDir(keepDir,keepDist,power,dir);
                           // $B7k2L$OL>A0EO$7$G$"$k$3$H$KCm0U(B?? NSobject!!
      return;
    }

    Generic.getCrossPoint(r,destRelX,destRelY,centRelX,centRelY,
		          tmpXY1,tmpXY2);                          // Point !!

    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("InfoManager::getKickArgForPowerShot() destRelX : "
	                  + destRelX);
      System.out.println("InfoManager::getKickArgForPowerShot() destRelY : "
	                  + destRelY);
      System.out.println("InfoManager::getKickArgForPowerShot() centRelX : "
	                  + centRelX);
      System.out.println("InfoManager::getKickArgForPowerShot() centRelY : "
	                  + centRelY);
      System.out.println("InfoManager::getKickArgForPowerShot() tmpX1 : "
	                  + tmpXY1.getPointX());
      System.out.println("InfoManager::getKickArgForPowerShot() tmpY1 : "
	                  + tmpXY1.getPointY());
      System.out.println("InfoManager::getKickArgForPowerShot() tmpX2 : "
	                  + tmpXY2.getPointX());
      System.out.println("InfoManager::getKickArgForPowerShot() tmpY2 : "
	                  + tmpXY2.getPointY()); 
    }
    */

    tmpDist1 = Math.sqrt(Math.pow(destRelX - tmpXY1.getPointX(),2)
                         + Math.pow(destRelY - tmpXY1.getPointY(),2));
    tmpDist2 = Math.sqrt(Math.pow(destRelX - tmpXY2.getPointX(),2)
                         + Math.pow(destRelY - tmpXY2.getPointY(),2));
    
    //if(!save){
	if(tmpDist1 <= tmpDist2){
	    tmpX = tmpXY1.getPointX();
	    tmpY = tmpXY1.getPointY();
	}
        else {
	    tmpX = tmpXY2.getPointX();
	    tmpY = tmpXY2.getPointY();
	}
    /* } else {
	if(tmpDist1 <= tmpDist2){
	    tmpX = tmpXY2.getPointX();
	    tmpY = tmpXY2.getPointY();
	}
        else {
	    tmpX = tmpXY1.getPointX();
	    tmpY = tmpXY1.getPointY();
	}
    }
    */
    double keepDir = Generic.normDir(Generic.RADIAN * Math.atan2(tmpY,tmpX)
                                      - this.bodyFaceDir);
    double keepDist = Math.sqrt(tmpX * tmpX + tmpY * tmpY);

    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("InfoManager::getKickArgForPowerShot() keepDir : "
	                  + keepDir); 
      System.out.println("InfoManager::getKickArgForPowerShot() keepDist : "
	                  + keepDist);
    }
    */

    getKickArgByKeepDir(keepDir,keepDist,power,dir);  // NSobject??

  }

/***************************************************************************/

  // $B%\!<%k$rAjBPJ}8~(B keepDir, $BAjBP5wN%(B keepDist $B$K0\F0$5$;$k(B
  // $BD>@\0\F0$G$-$J$$>l9g$O@\@~J}8~$X(B
  public void getKickArgByKeepDir(double keepDir,double keepDist,
			          NSobject power,NSobject dir){ // NSobject!!

    double margin = Generic.BALL_SIZE + 0.2;  // $B$A$g$C$HE,Ev(B
                                              // $BAjBP3QEY(B  ball$B$N$^$^$G(BOK?
    double ballDir = Generic.normDir(getAbsDir(this.ball) - this.bodyFaceDir);

    double ballDist = getBallDist();          // ball$B$N$^$^$G(BOK?

    // $BL\E*$N>l=j$N(B X,Y $BAjBP:BI8(B
    double destRelX = vx + 
	keepDist * Math.cos(Generic.digToRad(Generic.normDir(keepDir 
                                                        + this.bodyFaceDir)));
    double destRelY = vy +
	keepDist * Math.sin(Generic.digToRad(Generic.normDir(keepDir
                                                        + this. bodyFaceDir)));
    // $B%\!<%k$N(B X,Y $BAjBP:BI8(B
    double ballRelX = this.ball.getX() - x;
    double ballRelY = this.ball.getY() - y;

    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("InfoManager::getKickArgByKeepDir() destRelX : "
	                  + destRelX);
      System.out.println("InfoManager::getKickArgByKeepDir() destRelY : "
	                  + destRelY);
      System.out.println("InfoManager::getKickArgByKeepDir() ballRelX : "
	                  + ballRelX); 
      System.out.println("InfoManager::getKickArgByKeepDir() ballRelY : "
	                  + ballRelY);
    }
    */

    boolean direct = Generic.NS_TRUE;

    // $B%\!<%k$+$iL\E*$N>l=j$X$N5wN%(B  $BJQ?tL>J6$i$o$7$$$N$GCm0U(B!
    double destDist = Math.sqrt(Math.pow(destRelX - ballRelX,2)
			   + Math.pow(destRelY - ballRelY,2));

    // $B%\!<%k$+$iL\E*$N>l=j$X$NJ}8~$r7W;;(B
    double destDir = Generic.RADIAN * Math.atan2(destRelY - ballRelY,
                                                 destRelX - ballRelX);

    double mx,my;  // $B%\!<%k$r0\F0$5$;$k5wN%(B $B@dBP:BI8(B

    NSobject destDirNS = new NSobject(destDir);
    NSobject destDistNS = new NSobject(destDist);

    adjustDestToAvoidBody(destDirNS, destDistNS);

    if(Math.abs(Generic.normDir(keepDir - ballDir)) < 2 && 
       Math.abs(keepDist - ballDist) < 0.10){
	mx = 0;
	my = 0;
    }
   else {
     mx = destDistNS.getNS() * Math.cos(Generic.digToRad(destDirNS.getNS()));
     my = destDistNS.getNS() * Math.sin(Generic.digToRad(destDirNS.getNS()));
   }

    /*
   if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
     System.out.println("InfoManager::getKickArgByKeepDir() mx : " + mx
	                 + " my : " + my);
   }
   */

    getKickArgByMoveDist(mx,my,power,dir);  // NSobject??

  }

/***************************************************************************/
  // destDir,destDist $B$O(B $B%\!<%k$+$i$N@dBP3QEY(B,$BAjBP5wN%$G$"$k$3$H$KCm0U(B
  public boolean adjustDestToAvoidBody(NSobject destDir,NSobject destDist){
                                                                // NSobject??
    boolean direct = Generic.NS_TRUE;
    double upperDir,lowerDir;
    double margin = Generic.BALL_SIZE + 0.15;  // $B$A$g$C$HE,Ev(B
                                               // $BAjBP3QEY(B
    double ballDir = Generic.normDir(getAbsDir(this.ball) - this.bodyFaceDir);
    double ballDist = getBallDist();

    // $B@\@~J}8~$r7W;;$9$k(B
    if(ballDir >= 0){
      upperDir = Generic.normDir(- (180 - ballDir)
                 + Generic.RADIAN * Math.asin((Generic.PLAYER_SIZE + margin)
                 / ballDist));
      lowerDir = Generic.normDir(- (180 - ballDir)
	         - Generic.RADIAN * Math.asin((Generic.PLAYER_SIZE + margin)
                 / ballDist));
    }
    else {
      upperDir = Generic.normDir((180 + ballDir) 
	         - Generic.RADIAN * Math.asin((Generic.PLAYER_SIZE + margin)
                 / ballDist));
      lowerDir = Generic.normDir((180 + ballDir) 
	         + Generic.RADIAN * Math.asin((Generic.PLAYER_SIZE + margin)
                 / ballDist));
    }

    /*
   if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
     System.out.println("InfoManager::adjustDestToAvoidBody() ballDir : "
	                 + ballDir);
     System.out.println("InfoManager::adjustDestToAvoidBody() upperDir : "
	                 + upperDir);
     System.out.println("InfoManager::adjustDestToAvoidBody() lowerDir : "
	                 + lowerDir);
   }
   */

    // $B:#$N0LCV$+$iD>@\L\E*$N>l=j$K=3$l$k$+$I$&$+(B
    if(ballDir >= 0){
      if(upperDir >= lowerDir){
	if((lowerDir < Generic.normDir(destDir.getNS() - this.bodyFaceDir))
	   && (Generic.normDir(destDir.getNS() - this.bodyFaceDir) < upperDir))
	  direct = Generic.NS_FALSE;
	}
        else {
	  if((lowerDir < Generic.normDir(destDir.getNS() - this.bodyFaceDir)) 
	   || (Generic.normDir(destDir.getNS() - this.bodyFaceDir) < upperDir))
	    direct = Generic.NS_FALSE;
	}
    }
    else {
      if(upperDir <= lowerDir){
       if((lowerDir > Generic.normDir(destDir.getNS() - this.bodyFaceDir)) 
          && (Generic.normDir(destDir.getNS() - this.bodyFaceDir) > upperDir))
         direct = Generic.NS_FALSE;
      }
      else {
        if((lowerDir > Generic.normDir(destDir.getNS() - this.bodyFaceDir)) 
          || (Generic.normDir(destDir.getNS() - this.bodyFaceDir) > upperDir))
          direct = Generic.NS_FALSE;
      }
    }

    /*
    if(destDist.getNS() < ballDist - Generic.PLAYER_SIZE)
      direct = Generic.NS_TRUE;
    */

    double destRelX = destDist.getNS()
                      * Math.cos(Generic.digToRad(destDir.getNS()));
    double destRelY = destDist.getNS()
                      * Math.sin(Generic.digToRad(destDir.getNS()));
    double ballRelX = ball.getX() - x;
    double ballRelY = ball.getY() - y;

    double tmpDist1,tmpDist2;                // tmpX1,tmpX2,tmpY1,tmpY2,
    double r = Generic.PLAYER_SIZE - margin;
    
    Point tmpXY1 = new Point();                             // Point!!
    Point tmpXY2 = new Point();

    Generic.getCrossPoint(r,destRelX,destRelY,ballRelX,ballRelY,
	                    tmpXY1,tmpXY2);

    tmpDist1 = Math.sqrt(tmpXY1.getPointX() * tmpXY1.getPointX()
                          + tmpXY1.getPointY() * tmpXY1.getPointY());
    tmpDist2 = Math.sqrt(tmpXY2.getPointX() * tmpXY2.getPointX()
                          + tmpXY2.getPointY() * tmpXY2.getPointY());
    
    if(tmpDist1 > destDist.getNS() && tmpDist2 > destDist.getNS())
      direct = Generic.NS_TRUE;

    // $BD>@\=3$l$J$$>l9g$OA0J}$K6a$$@\@~J}8~$K=3$k(B
    if(!direct){
      if(Math.abs(upperDir) <= Math.abs(lowerDir))
	destDir.setNS(Generic.normDir(upperDir + this.bodyFaceDir));
      else
	destDir.setNS(Generic.normDir(lowerDir + this.bodyFaceDir));

      destDist.setNS(1.5 * Math.sqrt(ballDist * ballDist
                                 - Math.pow(Generic.PLAYER_SIZE + margin,2)));
    }

    /*
   if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
     if(direct)
       System.out.println("InfoManager::adjustDestToAvoidBody() Direct.");
     else
       System.out.println("InfoManager::adjustDestToAvoidBody() NonDirect!");
   }
   */

    return direct;

  }

/***************************************************************************/

  // $B%U%#!<%k%IFb$+$I$&$+(B
  public boolean isInField(){
    // $B8+$($F$$$k%i%$%s$,(B1$BK\$J$i%U%#!<%k%IFb(B 0,2$BK\$J$i%U%#!<%k%I30(B
    if((this.seeInfoParser.getLineNum() % 2) == 1){
      return Generic.NS_TRUE;
    }
    else {
      return Generic.NS_FALSE;
    }
  }

/***************************************************************************/

  // $BM?$($i$l$?%*%V%8%'%/%H$NJ}8~(B($B@dBP:BI8(B)$B$rJV$9(B
  // $B%W%l%$%d!<$N(B(X,Y) $B%*%V%8%'%/%H$N(B(X,Y) $B$+$i7W;;(B
  public double getAbsDir(ObjectInfo obj){
    return Generic.RADIAN * Math.atan2(obj.getY() - y, obj.getX() - x);
  }

/***************************************************************************/

    // $BM?$($i$l$?%*%V%8%'%/%H$X$N5wN%$rJV$9(B
    // $B%W%l%$%d!<$N(B(X,Y) $B%*%V%8%'%/%H$N(B(X,Y) $B$+$i7W;;(B
    // $B$3$l$"$d$7$$$+$b(B...$BBgDM@b(B
  public double getDist(ObjectInfo obj){
    return Math.sqrt(Math.pow(obj.getX() - x,2) + Math.pow(obj.getY() - y,2));
  }                                                           

/***************************************************************************/

  public void parseSeeInfo(String info){
    this.seeInfoParser.setInfo(info);
    this.seeInfoParser.parseInfo();
  }

/***************************************************************************/

  public void parseBodyInfo(String info){
    this.bodyInfoParser.setInfo(info);
    this.bodyInfoParser.parseInfo();
  }

/***************************************************************************/

  public void parseHearInfo(String info){
    this.hearInfoParser.setInfo(info);
    this.hearInfoParser.parseInfo();
  }

/***************************************************************************/

  // $B?HBN>pJs$r99?7(B
  public void updateBodyInfo(){
    bodyTime = this.bodyInfoParser.getTime();
    this.angleWidth = this.bodyInfoParser.getAngleWidth();
    this.quality = this.bodyInfoParser.getQuality();
    this.stamina =this. bodyInfoParser.getStamina();
    this.effort = this.bodyInfoParser.getEffort();
    this.speed = this.bodyInfoParser.getSpeed();
                                  // 5.18$B0J9_$O$3$3$rM-8z$K(B
    this.headAngle = this.bodyInfoParser.getHeadAngle();

    this.kickCount = this.bodyInfoParser.getKickCount();
    this.dashCount = this.bodyInfoParser.getDashCount();
    this.turnCount = this.bodyInfoParser.getTurnCount();
    this.sayCount = this.bodyInfoParser.getSayCount();
    this.turnNeckCount = this.bodyInfoParser.getTurnNeckCount();

    calcV();

    // See$B>pJs$NJ}$,(BSence$B>pJs$h$j@h$KMh$k$3$H$r9MN8$7$F(B
    if(bodyTime > seeTime){
      this.x += this.vx / Generic.PLAYER_DECAY;
      this.y += this.vy / Generic.PLAYER_DECAY; 
    }

    // $B$3$3$G%?!<%s$H(BSee$B$NF14|$r$H$k(B

    if (nextBodyAbsDir != UNKNOWN){
      this.bodyFaceDir = Generic.normDir(this.bodyFaceDir + nextBodyAbsDir);
      // self.setBodyAbsDir(Generic.normDir(getBodyAbsDir() + nextBodyAbsDir));
      // $BBN$N2sE>$r9MN8$KF~$l$F(B,$B%\!<%k$N3QEY$r=$@5(B
      ball.adjustBallDir(nextBodyAbsDir);
      nextBodyAbsDir = UNKNOWN;
    }
    if (nextHeadRelDir != UNKNOWN){
      // HeadRelDir $B$OKh%?!<%s99?7$5$l$F$$$k$N$G(B,$B=$@5$NI,MW$J$7(B
      // $BBN$N2sE>$r9MN8$KF~$l$F(B,$B%\!<%k$N3QEY$r=$@5(B
      ball.adjustBallDir(nextHeadRelDir);
      headFaceDir = Generic.normDir(this.headFaceDir + nextHeadRelDir);
      // self.setHeadAbsDir(Generic.normDir(getHeadAbsDir() + nextHeadRelDir));
      nextHeadRelDir = UNKNOWN;
    }
    if (nextHeadAbsDir != UNKNOWN){
      headFaceDir = Generic.normDir(this.headFaceDir + nextHeadAbsDir);
      // self.setHeadAbsDir(Generic.normDir(getHeadAbsDir() + nextHeadAbsDir));
      nextHeadAbsDir = UNKNOWN;
    }

  }
    
/***************************************************************************/

  // $B;k3P>pJs$r99?7(B
  public void updateSeeInfo(){
    this.prevSeeTime = this.seeTime;
    this.seeTime = this.seeInfoParser.getTime();

    // $B$3$3$G%?!<%s$H(BSee$B$NF14|$r$H$k(B
    // See$B>pJs$NJ}$,(BSence$B>pJs$h$j@h$KMh$k$3$H$r9MN8$7$F(B
    if(seeTime > bodyTime){
      if(nextHeadRelDir != UNKNOWN)
        headAngle += nextHeadRelDir;
      nextBodyAbsDir = nextHeadRelDir = nextHeadAbsDir = UNKNOWN;
    }

    calcFaceDir();
    calcPos();

    // $B%\!<%k$N>pJs$r;}$C$F$$$k$H$-$O%\!<%k$N>pJs$r99?7(B
    if(this.seeInfoParser.hasBallInfo()){
      this.knowsBallInfo = Generic.NS_TRUE;
      this.ballShadow = this.ball;  // $BA0$K8+$?%\!<%k$N>pJs$O;D$7$F$*$/(B
      this.ball = this.seeInfoParser.getBallInfo(); // ball$B$NCf?H@8@.(B
      this.ball.calcPos(this.x,this.y,this.headFaceDir);

      // $BA0$K8+$?%\!<%k$N>pJs$H:#$N%\!<%k$N>pJs$H$N4V$K$I$l$@$1;~4V$,(B
      // $B7P$C$F$$$k$+D4$Y$k(B    $B$&$^$/$$$+$J$$$o$3$l(B
      /*
      int blinkTime = this.ball.getTime() - this.ballShadow.getTime();
	if(blinkTime <= this.puSize && blinkTime != 0){
	  double ballMX,ballMY;
	  double totalUX = 0,totalUY = 0;
	  //$B$^$:<+J,$,$3$N4V$K$I$l$@$10\F0$7$?$+7W;;(B
	  if(this.lastUpdateEtcTime == this.seeTime)
	    for(int i = 0; i < blinkTime; i++){
	      totalUX += this.pux[i + 1];
	      totalUY += this.puy[i + 1];
            }
	  else
	    for(int i = 0; i < blinkTime; i++){
	      totalUX += this.pux[i];
	      totalUY += this.puy[i];
	    }
		
	  ballMX = - this.ballShadow.getRelX(this.pHeadFaceDir)
		   - totalUX + this.ball.getRelX(this.headFaceDir);
	  ballMY = - this.ballShadow.getRelY(this.pHeadFaceDir)
		   - totalUY + this.ball.getRelY(this.headFaceDir);

	  ball.setVX(ballMX / blinkTime * Generic.BALL_DECAY);
	  ball.setVY(ballMY / blinkTime * Generic.BALL_DECAY);
	}
	else */ 

        if(this.ball.hasChangeInfo()){
	  this.ball.calcRelVVH();
	  this.ball.calcRelVXY(this.headFaceDir);
	  // seeTime $B;~$N(B updateEtc() $B$,9T$o$l$F$$$k$+$I$&$+(B
	  if(this.lastUpdateEtcTime == this.seeTime){
	    this.ball.calcV(this.pvx,this.pvy);
	  } else {
	      this.ball.calcV(this.vx,this.vy);
	  }
	  if(getBallDist() < Generic.KICKABLE_AREA){ 
                                                     // Change$B>pJs$O?.MQ$7$J$$(B
	    this.ball.setVX(this.ballShadow.getVX());
	    this.ball.setVY(this.ballShadow.getVY());
	    this.ball.setAX(this.ballShadow.getAX());
	    this.ball.setAY(this.ballShadow.getAY());
	  }
	}
        else {
          this.ball.setVX(this.ballShadow.getVX());
          this.ball.setVY(this.ballShadow.getVY());
	}

	/*
        if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
          System.out.println("ballVX : " + this.ball.getVX());
          System.out.println("ballVY : " + ball.getVY());
        }
	*/

    } else {
	this.knowsBallInfo = Generic.NS_FALSE;
    }
    
    updatePlayerTable();

    this.pHeadFaceDir = this.headFaceDir;
  }

/***************************************************************************/

  public void updatePlayerTable(){
    // $B%W%l%$%d!<>pJs$N99?7(B
    int playerNum = this.seeInfoParser.getPlayerNum();
    PlayerInfo player;
    
    this.playerTable.makeShadows(); // $B;DA|$r;D$9(B

    for(int i = 0; i < playerNum; i++){
      player = this.seeInfoParser.getPlayerInfo(i);
      player.calcPos(this.x,this.y,this.headFaceDir);
      String tname = player.getTeamname();
      int uNum = player.getUniformNum();
      if(tname.equals(""))
	this.playerTable.addUnknownPlayer(player);
      else if(tname.equals(teamname)){
	if(uNum != 0)   //  $BGXHV9f$,(B 0 $B$G$J$1$l$P(B
	  this.playerTable.setTeammateWithUNum(player,uNum);
	else
	  this.playerTable.addTeammate(player);
      }
      else {
	if(uNum != 0)   //  $BGXHV9f$,(B 0 $B$G$J$1$l$P(B
	  this.playerTable.setOpponentWithUNum(player,uNum);
	else
	  this.playerTable.addOpponent(player);
      }
    }

    this.playerTable.classifyUnknownPlayers(this.seeTime - prevSeeTime,
                                            this.x,this.y,
                                            this.headFaceDir,this.angleWidth);

    /*    
    if(Generic.DEBUG2){   // $B%G%P%C%0MQ(B2
      playerTable.printInfo();
    }
    */

  }


 /***************************************************************************/

   // $BD03P>pJs$r99?7(B
   public void updateHearInfo(){
     int playmodeOld = this.playmode;

     this.hearTime = this.hearInfoParser.getTime();

     if(this.hearInfoParser.fromReferee()){
       this.playmode = this.hearInfoParser.getPlaymode();

       //  playmode$B$,(Bplay_on$B0J30$X0\9T$7$?$i@oN,%F!<%V%k$r=i4|2=(B 
       if(this.playmode != playmodeOld && playmode != 13)
	 this.strategyTable.refresh();

       if(this.playmode == HearInfoParser.PM_OUR_GOAL){
	 this.ourPoint = this.hearInfoParser.getOurPoint();
       }
       else if(this.playmode == HearInfoParser.PM_OPPONENTS_GOAL){
	 this.opponentsPoint = this.hearInfoParser.getOpponentsPoint();
       }
       else if(this.playmode == HearInfoParser.PM_OUR_KICK_OFF ||
	 this.playmode == HearInfoParser.PM_OPPONENTS_KICK_OFF){
	 this.ball.setX(0.0);
	 this.ball.setY(0.0);
       }
     }
     else if(this.hearInfoParser.fromTeammate()){
       String message;

       message = this.hearInfoParser.getMessage();

       STokenizer tokenizer = new STokenizer(message);

       while(tokenizer.hasMoreSTokens()){
         String token;
         token = tokenizer.nextSToken();
         STokenizer subTokenizer = new STokenizer(token);
         token = subTokenizer.nextSToken();

         // $B%\!<%k$N>pJs(B $B$I$&$d$i(B lookAtBall $B$N$*$+$2$GLr$KN)$C$F$J$$(B
         if(token.equals("NIT_ball")){
           int t = Integer.valueOf(subTokenizer.nextSToken()).intValue();
	   // $B<+J,$N8+$?%\!<%k>pJs$h$j?7$7$1$l$P:NMQ(B
           if(t - this.ball.getTime() > 0){
             this.ball.setTime(t);
             this.ball.setX(
           	      Double.valueOf(subTokenizer.nextSToken()).doubleValue());
             this.ball.setY( 
		      Double.valueOf(subTokenizer.nextSToken()).doubleValue());
             this.ball.setVX( 
        	      Double.valueOf(subTokenizer.nextSToken()).doubleValue());
	     this.ball.setVY( 
		      Double.valueOf(subTokenizer.nextSToken()).doubleValue());
	     this.ball.setAX( 
		      Double.valueOf(subTokenizer.nextSToken()).doubleValue());
	     this.ball.setAY( 
		      Double.valueOf(subTokenizer.nextSToken()).doubleValue());
             this.ball.setCertainty(1.0);

	     int timeDiff = this.hearTime - this.ball.getTime();

             for(int i = 0; i < timeDiff; i++){
               this.ball.calcNextState();
	     }
	   }
	 }

         /*  $B$h$/$o$+$i$J$$$N$G$H$j$"$($:=|$/!#;k3P>pJs$H$+$V$k!)(B
         else if(token.equals("player")){
           if(Integer.valueOf(subTokenizer.nextSToken()).intValue()
		 - this.hearTime < 3){
             // $BL#J}$N%]%8%7%g%s>pJs(B  $B3X=,MQ!D$H$b$+$.$i$s$+(B
             token = subTokenizer.nextSToken();
             if(token.equals(teamname)){
               int unum;
	       double tx,ty;
	       double tDist, tDir; 
	       unum = Integer.valueOf(subTokenizer.nextSToken()).intValue();
	       tx = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
	       ty = Double.valueOf(subTokenizer.nextSToken()).doubleValue();

	       tDist = Generic.DIST(this.x,this.y,tx,ty);
	       tDir = Generic.RADIAN * Math.atan2(ty - this.y,tx - this.x)
				       - this.headFaceDir;

	       PlayerInfo teammate = new PlayerInfo(tDist,tDir);
	       teammate.setTeamname(this.teamname);
	       teammate.setUniformNum(unum);

	       if(!this.playerTable.hasTeammateInfo(unum))
	         this.playerTable.setTeammateWithUNum(teammate,unum);
	     }
	     else {
	       double tx,ty;
	       double tDist, tDir; 
	       tx = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
	       ty = Double.valueOf(subTokenizer.nextSToken()).doubleValue();

	       tDist = Generic.DIST(this.x,this.y,tx,ty);
	       tDir = Generic.RADIAN * Math.atan2(ty - this.y,tx - this.x)
				       - this.headFaceDir;

	       PlayerInfo opponent = new PlayerInfo(tDist,tDir);

	       if(token.equals("opponent"))            // Array Error$BKI$.MQ(B
	         this.playerTable.addOpponent(opponent);
	       else
	         this.playerTable.addUnknownPlayer(opponent);
	     }
	   }
         }
         */

         //  $B%A!<%`%a%$%H$N6(D4@oN,$K4X$9$k>pJs(B
         //  (strategy <sName> <unum> <sType>) 
         else if(token.equals("NIT_strategy")){
           strategyFlag = Generic.NS_FALSE;

           String sName;
           String sType;
           int unum;

           sName = subTokenizer.nextSToken();
           unum = Integer.valueOf(subTokenizer.nextSToken()).intValue();
           sType = subTokenizer.nextSToken();

           this.strategyTable.setPlayerStrategy(unum,sName,sType);

	   if(sType.equals("plan")){
             strategyFlag = Generic.NS_TRUE;
             planner = unum;
             planTime = this.hearTime;
           }
         }
       }
     }
     else if(this.hearInfoParser.fromCoach()){
       //$B$3$3$K%3!<%A$+$i$NL?Na$K$h$k9TF0$r$+$/(B
       String message;
       message = this.hearInfoParser.getMessage();

       STokenizer tokenizer = new STokenizer(message);

       while(tokenizer.hasMoreSTokens()){
         String token;
         token = tokenizer.nextSToken();
         STokenizer subTokenizer = new STokenizer(token);
         token = subTokenizer.nextSToken();
       
	 // $B%]%8%7%g%s>pJs(B
	 if(token.equals("Pos")){
           int unum;
           unum = Integer.valueOf(subTokenizer.nextSToken()).intValue();
           if(unum == uniformNum){
	     ctStX = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
	     ctStY = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
	     ctSdX = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
	     ctSdY = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
             this.stdPosChanged = Generic.NS_TRUE;
             System.out.println("uNum : " + this.uniformNum + 
                             " StartPos Change : " + ctStX + "," + ctStY +
                             " StdPos Change : " + ctSdX + "," + ctSdY);
	   }
	 }
	 else if(token.equals("SR")){
           // $B$H$j$"$($:%m!<%+%kJQ?t$K3JG<!#$^$@MxMQ$O$7$F$J$$!#(B
           // int time;
           String sName;
           String eval;
           int unum;
   
           // time = Integer.valueOf(subTokenizer.nextSToken()).intValue();
	   unum = Integer.valueOf(subTokenizer.nextSToken()).intValue();
           if(unum == uniformNum){
	     sName = subTokenizer.nextSToken();
             eval = subTokenizer.nextSToken();
	     // $B$"$H(Btime$B$bJ];}$7$J$$$H$$$+$s$M!#(B
	     this.evalSRFlag = Generic.NS_TRUE;
	   }
	 }
         else if (token.equals("GK")){

	   //
      	   //added 0803 for GK catch by S.F
           //

           ctmyX= Double.valueOf(subTokenizer.nextSToken()).doubleValue();
           ctmyY= Double.valueOf(subTokenizer.nextSToken()).doubleValue();
           ctkX = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
           ctkY = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
	   freekickPosChanged = Generic.NS_TRUE;
		
    	 }      
       }
     }
   }

/***************************************************************************/

  // Move $B$K$h$k0LCV>pJs$N99?7(B
  public void updateByMove(double X,double Y){
    this.x = X;
    this.y = Y;
    this.vx = 0.0;
    this.vy = 0.0;
  }
    
/***************************************************************************/

    // $B%-%C%/$K$h$k%\!<%k>pJs$N99?7(B
  public void updateByKick(double power, double dir){
    if(getBallDist() <= Generic.KICKABLE_AREA){
      double adir = Generic.normDir(bodyFaceDir + dir);// $B=3$kJ}8~$N@dBP:BI87O(B

      /*
      double a = power * Generic.KICK_POWER_RATE;
      */

      double a = power * Generic.KICK_POWER_RATE *
	 (1 - 0.25 * Math.abs((Generic.normDir(getAbsDir(ball)- bodyFaceDir)))
                       / 180.0 - 0.25 * (getBallDist() - Generic.PLAYER_SIZE
                       - Generic.BALL_SIZE) / Generic.KICKABLE_MARGIN);

      this.ball.setAX(a * Math.cos(Generic.digToRad(adir)));
      this.ball.setAY(a * Math.sin(Generic.digToRad(adir)));

      /*
      if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
        System.out.println("ballAX : " + this.ball.getAX());
        System.out.println("ballVY : " + this.ball.getAY());
      }
      */

    }
  }

/***************************************************************************/

  // $B%@%C%7%e$K$h$k2CB.EY$H%9%?%_%J$N99?7(B
  public void updateByDash(double power){
    if(power > this.stamina)       // $B%Q%o!<$h$j%9%?%_%J$,>/$J$$>l9g$O(B
      power = this.stamina;        // $B%Q%o!<$r%9%?%_%JNL$K@)8B(B

    this.stamina -= power;
    
    power *= Generic.DASH_POWER_RATE;

    this.ax = power * Math.cos(Generic.digToRad(this.bodyFaceDir));
    this.ay = power * Math.sin(Generic.digToRad(this.bodyFaceDir));
  }
    
/***************************************************************************/

  // $B%?!<%s$K$h$k(B bodyFaceDir,headFaceDir $B$N99?7(B
  public void updateByTurn(double moment){
    double actualAngle = moment / (1.0 + Generic.INERTIA_MOMENT * this.speed);
    
    // this.bodyFaceDir = Generic.normDir(this.bodyFaceDir + actualAngle);
    // this.headFaceDir = Generic.normDir(this.headFaceDir + actualAngle);

    nextBodyAbsDir = actualAngle;
    nextHeadAbsDir = actualAngle;
  }
    
/***************************************************************************/

  // $B<s2s$7$K$h$k(B headFaceDir,headAngle $B$N99?7(B
  public void updateByTurnNeck(double angle){
    nextHeadRelDir = angle;
    if(nextHeadRelDir >= 90){
      nextHeadRelDir = 90;
    }
    else if(nextHeadRelDir <= -90){
      nextHeadRelDir = -90;
    }
    /*
    this.headAngle += angle;
    if(this.headAngle >= 90)
      this.headAngle = 90;
    else if(this.headAngle <= -90)
      this.headAngle = -90;
    */
    // this.headFaceDir = Generic.normDir(this.bodyFaceDir + this.headAngle);

    // headAngle = nextHeadRelDir$B$G$9(B
  }

/***************************************************************************/

  // $B8=:_0LCV(B,$B%\!<%k>pJs$N99?7(B   $B%3%^%s%I$rH/9T$7$?8e$K9T$&$3$H(B
  // (8/22)$B",$rJQ99$7$F(B,sense$B>pJs$N99?7D>A0$K$d$k!#(B
  // $B$?$@$7(Bsense$B$h$j$b(Bsee$B$,@h$K$/$k$3$H$b9MN8$9$k!#(B
  public void updateEtc(){
    for(int i = this.puSize - 1; i > 0; i--){
      this.pux[i] = this.pux[i - 1];
      this.puy[i] = this.puy[i - 1];
    }
    this.pux[0] = this.ux;
    this.puy[0] = this.uy;
    
    this.ux = this.vx + this.ax;
    this.uy = this.vy + this.ay;

    this.pvx = this.vx;  // $B0l$DA0$NB.EY$r;D$7$F$*$/(B
    this.pvy = this.vy;

    this.ax = 0;
    this.ay = 0;

    //this.x = this.x + this.ux;
    //this.y = this.y + this.uy;
     
    // $BB.EY$O(Bsense$B$G99?7$5$l$k(B
    this.vx = this.ux * Generic.PLAYER_DECAY;
    this.vy = this.uy * Generic.PLAYER_DECAY;

    this.pBallVX = this.ball.getVX();
    this.pBallVY = this.ball.getVY();


    //  $B%A!<%`%a%$%H$N6(D4@oN,%F!<%V%k$N99?7(B
    this.strategyTable.updateStrategyTable();
    //  $B<+J,$N6(D4@oN,$N99?7(B
    strategyNextState();

    // sense$B>pJs$N99?7D>A0$K$d$k!#(B
    this.lastUpdateEtcTime = this.bodyTime; // $B:G8e$K99?7$7$?;~4V(B


    // $B%o!<%k%I%b%G%k$N99?7!#(Bsense$B$h$j$b(Bsee$B$,@h$KMh$?$H$-$O$d$i$J$/$F$h$7(B
    // lastUpdateEtcTime < seeTime $B$J$i(Breturn$B$H$7$?$$$H$3$m$@$,!"(B
    // beforeKickOff$B$J$I;~4V$,?J$^$J$$>l9g$,$"$k$N$G!"L5M}$d$j!#(B
    // $B$$$$9M$($,Ib$+$P$J$$$N$GKh2s99?7$9$k!#8m:9$O5$$K$7$J$$!#(B
      //  $B<~0O$N%W%l%$%d!<>pJs$N99?7(B
      playerTable.updatePlayerTable();

    // $B%\!<%k>pJs$N99?7!#L5M}$d$j!#(B
    // $BLdBj$O(BbeforeKickOff$B$J$I;~4V$,?J$^$J$$>l9g!#$G$b%\!<%k$O$=$N$H$-(B
    // $BDd;_$7$F$$$k!#$?$V$s!#(B
    if(this.ball.getTime() < lastUpdateEtcTime){
      this.ball.calcNextState();

      // $B%\!<%k$H$N>WFM$N=hM}(B
      if(getBallDist() < Generic.PLAYER_SIZE){

        this.ball.setVX(this.ball.getVX() * 0.1);
        this.ball.setVY(this.ball.getVY() * 0.1);

        double ballDir = getAbsDir(this.ball);
	
        this.ball.setX(this.x + (Generic.PLAYER_SIZE + Generic.BALL_SIZE)
                     * Math.cos(Generic.digToRad(ballDir)));
        this.ball.setY(this.y + (Generic.PLAYER_SIZE + Generic.BALL_SIZE)
                     * Math.sin(Generic.digToRad(ballDir)));
      }
    }


  }

/***************************************************************************/

  public int getBodyTime(){
    return this.bodyTime;
  }

/***************************************************************************/

  public int getPlaymode(){
    return this.playmode;
  }

/***************************************************************************/

  public double getBodyFaceDir(){
    if(nextBodyAbsDir != UNKNOWN)
      return Generic.normDir(this.bodyFaceDir + nextBodyAbsDir);
    else
      return this.bodyFaceDir;
  }

/***************************************************************************/

  public double getHeadAngle(){
    if(nextHeadRelDir != UNKNOWN)
      return Generic.normDir(this.headAngle + nextHeadRelDir);
    else
      return this.headAngle;
  }

/***************************************************************************/

  // $B<s$N8~$-$N@dBPJ}8~$,M_$7$$(B
  public double getHeadFaceDir(){ 
    if(nextHeadAbsDir != UNKNOWN)
      return Generic.normDir(this.headFaceDir + nextHeadAbsDir);
    else
      return headFaceDir;
  }

/***************************************************************************/

  public double getX(){
    return this.x;
  }

/***************************************************************************/

  public double getY(){
    return this.y;
  }

/***************************************************************************/

  public double getVX(){
    return this.vx;
  }

/***************************************************************************/

  public double getVY(){
    return this.vy;
  }

/***************************************************************************/

  public double getStamina(){
    return this.stamina;
  }

/***************************************************************************/

  public double getEffort(){
    return this.effort;
  }


 /***************************************************************************/

   public boolean isKickable(){
     double margin = 0.1;
     
     if(getBallDist() <= Generic.KICKABLE_AREA - margin){
       return Generic.NS_TRUE;
     }
     else{ 
       return Generic.NS_FALSE;
     }

     /*
     // 1$B%5%$%/%k8e$K(B0.985m$B0JFb$K$"$k>l9g(BKickable
     if(Generic.DIST(this.x + this.vx,this.y + this.vy,
                    this.ball.getPredX(1),this.ball.getPredY(1))
	<= Generic.KICKABLE_AREA - margin){
       // System.out.println(uniformNum + " Kickable! " + getPredBallDist(1));
       return Generic.NS_TRUE;
     }
     else{ 
       return Generic.NS_FALSE;
     }
     */
   }

/***************************************************************************/

  public boolean isKickable2(){
    double margin = 0.3;
    /*
     // 1$B%5%$%/%k8e(B0.685m$B0JFb$K$"$k>l9g(BKickable
     if(Generic.DIST(this.x + this.vx,this.y + this.vy,
                    this.ball.getPredX(1),this.ball.getPredY(1))
	<= Generic.KICKABLE_AREA - margin){
       return Generic.NS_TRUE;
     }
     else {
       return Generic.NS_FALSE;
     } 
    */

    if(getBallDist() <= Generic.KICKABLE_AREA - margin){
      return Generic.NS_TRUE;
    }
    else {
      return Generic.NS_FALSE;
    }
  }

/***************************************************************************/

  public boolean isCatchable(){
    /*
    // this.x + this.vx = 1$B%5%$%/%k8e$N<+J,$N:BI8(B
    if(Generic.DIST(this.x + this.vx,this.y + this.vy,
                    this.ball.getPredX(1),this.ball.getPredY(1))
       <= Generic.CATCHABLE_AREA_L){
      return Generic.NS_TRUE;
    }
    else {
      return Generic.NS_FALSE;
    }
    */

    if(Generic.DIST(this.x,this.y,this.ball.getX(),this.ball.getY())
       <= Generic.CATCHABLE_AREA_L)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;

  }

/***************************************************************************/

  public boolean hasBallInfo(){
    //    return this.knowsBallInfo;

    if(this.ball.getCertainty() > 0.5){
      searchFlag = 0;
      return Generic.NS_TRUE;
    }
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public double getBallCertainty(){
    return this.ball.getCertainty();
  }

/***************************************************************************/

  public double getBallX(){
    return this.ball.getX();
  }

/***************************************************************************/

  public double getBallY(){
    return this.ball.getY();
  }

/***************************************************************************/

  public double getBallVX(){
    return this.ball.getVX();
  }

/***************************************************************************/

  public double getBallVY(){
    return this.ball.getVY();
  }

/***************************************************************************/

  public double getBallDir(){
    return Generic.normDir(getAbsDir(this.ball) - this.bodyFaceDir);
  }

/***************************************************************************/

  public double getAbsBallDir(){
    return getAbsDir(this.ball);
  }

/***************************************************************************/

  public double getBallDist(){
    return Generic.DIST(this.x,this.y,this.ball.getX(),this.ball.getY());
  }

/***************************************************************************/

  public String getBallString(){
    return this.ball.toStringBallInfo();
  }

/***************************************************************************/

  public double getAbsGoalDir(){
    double absGoalDir = Generic.RADIAN
                 * Math.atan2(0 - this.y,Generic.FIELD_X_WIDTH * 0.5 - this.x);
    return absGoalDir;
  }

/***************************************************************************/

  public double getGoalDist(){
    double goalX = Generic.FIELD_X_WIDTH * 0.5;
    double goalY = 0;

    return Math.sqrt((goalX - this.x) * (goalX - this.x)
                     + (goalY - this.y) * (goalY - this.y));
  }

/***************************************************************************/

  // $B%\!<%k$rDI$&I,MW$,$"$k$+$I$&$+(B
  // n $B?M0J>e$NL#J}$,<+J,$h$j%\!<%k$K6a$1$l$PI,MW$J$7(B
  public boolean needToChaseBall(int n){
    //if(this.playerTable.getOrderFrom(this.ball.getX(),this.ball.getY(),
    //                                 this.x,this.y) <= n);
    if(this.playerTable.getOrderFrom(this.ball.getPredX(20),
             this.ball.getPredY(20),this.x,this.y) <= n)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public boolean needToMark(double oppX,double oppY,int n){
    if(this.playerTable.getOrderFrom(oppX,oppY,this.x,this.y) <= n)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  // $B;XDj$7$?:BI8$K<+J,$,$$$k$+$I$&$+(B
  public boolean onThePos(double X,double Y){
    double margin = 1.0;  // $B8m:9HO0O(B
    
    if(Math.sqrt(Math.pow(X - this.x,2) + Math.pow(Y - this.y,2)) < margin)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public boolean inPenaltyArea(){
    if(this.x <= - Generic.PENALTY_AREA_X_WIDTH * 0.5 
       && Math.abs(y) <= Generic.PENALTY_AREA_Y_WIDTH * 0.5)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public boolean isBallInPenaltyArea(int t){
    // $B$A$g$C$H(B Y $B<4$r$;$P$a$F$_$?(B
    if(this.ball.getPredX(t) <= - Generic.PENALTY_AREA_X_WIDTH * 0.5 &&
       Math.abs(this.ball.getPredY(t))
                                 <= Generic.PENALTY_AREA_Y_WIDTH * 0.5 - 4.0)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }
    
/***************************************************************************/

  // $B;XDj$7$?:BI8$NJ}8~$r5a$a$k(B $BAjBP(B
  public double getDirOfThePos(double X,double Y){
    double absDir = Generic.RADIAN * Math.atan2(Y - this.y,X - this.x);
    
    return Generic.normDir(absDir - this.bodyFaceDir);
  }

/***************************************************************************/

  // $B;XDj$7$?:BI8$N5wN%$r5a$a$k(B $BAjBP(B
  public double getDistOfThePos(double X,double Y){
    return Math.sqrt(Math.pow(X - this.x,2) + Math.pow(Y - this.y,2));
  }

/***************************************************************************/

  // t $B%5%$%/%k8e$N%\!<%k$H$N5wN%(B
  public double getPredBallDist(int t){
    return Math.sqrt(Math.pow(this.ball.getPredX(t) - this.x,2)
                     + Math.pow(this.ball.getPredY(t) - this.y,2));
  }

/***************************************************************************/

  public double getPredBallDir(int t){
    return Generic.normDir(Generic.RADIAN 
                     * Math.atan2(this.ball.getPredY(t)- this.y,
                                this.ball.getPredX(t) - this.x)
                           - getBodyFaceDir());
  }

/***************************************************************************/

  // t $B%5%$%/%k$G?J$a$k5wN%(B
  public double getMovableDist(int t,double power){
    double dist = 0,V = 0;
    
    for(int i = 0; i < t; i++){
      dist = dist + V + power * Generic.DASH_POWER_RATE;
      V *= Generic.PLAYER_DECAY;
    }

    return dist;
  }
    
/***************************************************************************/

  public double getPredBallX(int t){
    return this.ball.getPredX(t);
  }

/***************************************************************************/

  public double getPredBallY(int t){
    return this.ball.getPredY(t);
  }

/***************************************************************************/

  public double getPredBallVX(int t){
    return this.ball.getPredVX(t);
  }

/***************************************************************************/
  /*
  public int getPlayerNum(){
    return this.seeInfoParser.getPlayerNum();
  }
  */
/***************************************************************************/
  /*
  public PlayerInfo getPlayerInfo(int n){
    return this.seeInfoParser.getPlayerInfo(n);
  }
  */
/***************************************************************************/

  public boolean adjustDirToAvoidPlayer(double dist,NSobject dir){
                                                    // NSobject!
    // dist$B$O(Bt$B%5%$%/%k8e$N%\!<%k$N5wN%(B

    double margin = 50.0;

    int playerNum = this.seeInfoParser.getPlayerNum();
    for(int i = 0; i < playerNum; i++){
      PlayerInfo player = this.seeInfoParser.getPlayerInfo(i);
      player.calcPos(this.x,this.y,this.headFaceDir);
	
      double playerDir = Generic.normDir(getAbsDir(player) - this.bodyFaceDir);
      double playerDist = Generic.DIST(this.x,this.y,
                                       player.getX(),player.getY());

      /*	
      if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
        System.out.println("InfoManager::adjustDirToAvoidPlayer() dirDiff : "
  	                    + Generic.normDir(playerDir - dir.getNS()));
        System.out.println("InfoManager::adjustDirToAvoidPlayer() dir : "
	                    + dir.getNS());
        System.out.println("InfoManager::adjustDirToAvoidPlayer() playerDir : "
	                    + playerDir);
      }
      */

      // $BE(%W%l%$%d!<$H$N5wN%(B < t$B%5%$%/%k8e$N%\!<%k$N5wN%(B &&
      // $BE(%W%l%$%d!<$H$N5wN%(B < 5m &&
      // $BE(%W%l%$%d!<$H%\!<%k$H$NAjBP3Q(B($B@dBPCM(B) < margin / $BE($H$N5wN%(B
      // $B:G8e$,Ff$J$s$@$h$J$"(B
      if(playerDist < dist && playerDist < 5
	&& Math.abs(Generic.normDir(playerDir - dir.getNS()))
           < margin / playerDist){
       if(Generic.normDir(playerDir - dir.getNS()) >= 0)
	 dir.setNS(playerDir - margin / playerDist);
       else
	 dir.setNS(playerDir + margin / playerDist);
	    
       return Generic.NS_TRUE;
      }
    }
    return Generic.NS_FALSE;
  }

/***************************************************************************/

  public boolean adjustDirToAvoidBall(double dist,NSobject dir){ // NSobject!!
    double margin = 35.0;
    double ballDir = Generic.normDir(getAbsDir(this.ball) - this.bodyFaceDir);
    double ballDist = getBallDist();

    /*	
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("InfoManager::adjustDirToAvoidBall() dirDiff : "
	                  + Generic.normDir(ballDir - dir.getNS()));
      System.out.println("InfoManager::adjustDirToAvoidBall() dir : "
	                  + dir.getNS());
      System.out.println("InfoManager::adjustDirToAvoidBall() ballDir : "
	                  + ballDir);
    }
    */

    if(ballDist < dist && ballDist < 3 
       && Math.abs(Generic.normDir(ballDir - dir.getNS()))
          < margin / ballDist){
      if(Generic.normDir(ballDir - dir.getNS()) >= 0)
	dir.setNS(ballDir - margin / ballDist);
      else
	dir.setNS(ballDir + margin / ballDist);
	
      return Generic.NS_TRUE;
    }
    return Generic.NS_FALSE;
  }

/***************************************************************************/

  public double evalLine(double X,double Y){
    return this.playerTable.evalLine(this.x,this.y,X,Y);
  }

/***************************************************************************/

  public double getOffsideLine(){
    return this.playerTable.getOffsideLine(this.ball.getX());
  }

/***************************************************************************/

  public double getPosOfReceiver(double dir,double dirDiff,double dist,
                                 Point recXY){                    // Point!!
    return this.playerTable.getPosOfReceiver(dir,dirDiff,dist,this.x,
                                             this.y,recXY);
  }

/***************************************************************************/
  // for goalie
  public double getPosOfReceiver(double dir,double dirDiff,double dist,
                                 Point recXY,int p){              // Point!!
    return this.playerTable.getPosOfReceiver(dir,dirDiff,dist,this.x,
                                             this.y,recXY,p);
  }

/***************************************************************************/

  public double getPosOfNearestOpponent(double dir,double dirDiff,double dist,
                                        Point oppXY){  //   Point!!
    return this.playerTable.getPosOfNearestOpponent(dir,dirDiff,dist,
                                                    this.x,this.y,oppXY);
  }

/***************************************************************************/

  public int getOurPoint(){
    return this.ourPoint;
  }

/***************************************************************************/

  public int getOpponentsPoint(){
    return this.opponentsPoint;
  }

/***************************************************************************/

  public double getDirOfOpenSpace(double dir,double dirDiff,double dist,
			     double interval,NSobject openDir){
    double minValue = 1000;
    double tmpValue;
    
    int n = (int)(dirDiff / interval) + 1;
    double tmpDir = dir;

    for(int i = 0; i < n; i++){
      if(Math.abs(this.y + dist * Math.sin(Generic.digToRad(tmpDir)))
                   > Generic.FIELD_Y_WIDTH * 0.5)
	continue;
	tmpValue = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			   this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
	if(tmpValue < minValue){
	  minValue = tmpValue;
	  openDir.setNS(tmpDir);
        }
	tmpDir = Generic.normDir(tmpDir + interval);
    }

    tmpDir = dir;

    for(int i = 0; i < n; i++,tmpDir = Generic.normDir(tmpDir - interval)){
      if(Math.abs(this.y + dist * Math.sin(Generic.digToRad(tmpDir)))
          > Generic.FIELD_Y_WIDTH * 0.5)
	continue;
	tmpValue = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			   this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
	if(tmpValue < minValue){
	  minValue = tmpValue;
	  openDir.setNS(tmpDir);
	}
    }

    tmpDir = dir;
    tmpValue = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
    if(tmpValue < minValue){
      minValue = tmpValue;
      openDir.setNS(tmpDir);
    }
    
    return minValue;
  }

/***************************************************************************/

  public String getSelfString(){  // sprintf$B$NCV$-49$((B
    StringBuffer selfString = new StringBuffer("(player ");
    selfString.append(this.bodyTime).append(" ").append(this.teamname)
              .append(" ").append(this.uniformNum).append(" ").append(this.x)
              .append(" ").append(this.y).append(")");
    return selfString.toString();
  }

/***************************************************************************/

  public double getTeammateX(int unum){
    return this.playerTable.getTeammateX(unum);
  }

/***************************************************************************/

  public void getPosOfOpponentsForward(Point oppXY){
    this.playerTable.getPosOfOpponentsForward(oppXY);
  }

/***************************************************************************/

  public String getOpponentsForwardString(){  // sprintf$B$NCV$-49$((B
    Point oppXY = new Point();
    this.playerTable.getPosOfOpponentsForward(oppXY);

    StringBuffer oppForwardString = new StringBuffer("(player ");
    oppForwardString.append(this.bodyTime).append(" ").append("opponent")
           .append(" ").append(oppXY.getPointX()).append(" ")
           .append(oppXY.getPointY()).append(")");
    return oppForwardString.toString();
  }

/*********************$B$3$3$+$iDI2C%a%=%C%I(B**********************************/

  public double getDirOfClearSpace(double dir,double dirDiff,double dist,
			     double interval,NSobject openDir){
    double minValue = 1000;
    double tmpValue;
    
    int n = (int)(dirDiff / interval) + 1;
    double tmpDir = dir;

    if(this.y >= 0){
      for(int i = 0; i < n; i++,tmpDir = Generic.normDir(tmpDir + interval)){
        if(Math.abs(this.y + dist * Math.sin(Generic.digToRad(tmpDir)))
                     > Generic.FIELD_Y_WIDTH * 0.5)
	  continue;
	  tmpValue
                = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			   this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
	  if(tmpValue < minValue){
	    minValue = tmpValue;
	    openDir.setNS(tmpDir);
          }
      }
    }
    else {
      for(int i = 0; i < n; i++,tmpDir = Generic.normDir(tmpDir - interval)){
        if(Math.abs(this.y + dist * Math.sin(Generic.digToRad(tmpDir)))
            > Generic.FIELD_Y_WIDTH * 0.5)
	  continue;
	  tmpValue
                = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			   this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
	  if(tmpValue < minValue){
	    minValue = tmpValue;
	    openDir.setNS(tmpDir);
	  }
      }
    }
    tmpDir = dir;
    tmpValue = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
    if(tmpValue < minValue){
      minValue = tmpValue;
      openDir.setNS(tmpDir);
    }
    
    return minValue;
  }

/***************************************************************************/

  public double getDirOfChangeSpace(double dir,double dirDiff,double dist,
			     double interval,NSobject openDir){
    double minValue = 1000;
    double tmpValue;
    
    int n = (int)(dirDiff / interval) + 1;
    double tmpDir = dir;

    if(this.y < 0){
      for(int i = 0; i < n; i++,tmpDir = Generic.normDir(tmpDir + interval)){
        if(Math.abs(this.y + dist * Math.sin(Generic.digToRad(tmpDir)))
                     > Generic.FIELD_Y_WIDTH * 0.5)
	  continue;
	  tmpValue
                = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			   this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
	  if(tmpValue < minValue){
	    minValue = tmpValue;
	    openDir.setNS(tmpDir);
          }
      }
    }
    else {
      for(int i = 0; i < n; i++,tmpDir = Generic.normDir(tmpDir - interval)){
        if(Math.abs(this.y + dist * Math.sin(Generic.digToRad(tmpDir)))
            > Generic.FIELD_Y_WIDTH * 0.5)
	  continue;
	  tmpValue
                = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			   this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
	  if(tmpValue < minValue){
	    minValue = tmpValue;
	    openDir.setNS(tmpDir);
	  }
      }
    }
    tmpDir = dir;
    tmpValue = evalLine(this.x + dist * Math.cos(Generic.digToRad(tmpDir)),
			this.y + dist * Math.sin(Generic.digToRad(tmpDir)));
    if(tmpValue < minValue){
      minValue = tmpValue;
      openDir.setNS(tmpDir);
    }
    
    return minValue;
  }

/***************************************************************************/

  public double getSideChangeReceiver(double dir,double dirDiff,double dist,
                                 Point recXY){                    // Point!!
    return this.playerTable.getSideChangeReceiver(dir,dirDiff,dist,this.x,
                                             this.y,recXY);
  }

/***************************************************************************/

  public boolean parseSideChange(){
    return this.playerTable.parseSide(this.ball.getY());
  }

/***************************************************************************/

  public int forwardFW(double dir, double dirDiff, double dist){
    return this.playerTable.forwardFW(dir,dirDiff,dist,this.x,this.y);
  }

/***************************************************************************/

  public double getAbsTopPostDir(){
    double absTopPostDir = Generic.RADIAN * Math.atan2(
                                Generic.GOAL_WIDTH * 0.5 - this.y,
                                Generic.FIELD_X_WIDTH * 0.5 - this.x);
    return absTopPostDir;
  }

/***************************************************************************/

  public double getShootAngle(){
    double absTopPostDir = Generic.RADIAN * Math.atan2(
                                Generic.GOAL_WIDTH * 0.5 - this.y,
                                Generic.FIELD_X_WIDTH * 0.5 - this.x);
    double absDdownPostDir = Generic.RADIAN * Math.atan2(
                                - Generic.GOAL_WIDTH * 0.5 - this.y,
                                Generic.FIELD_X_WIDTH * 0.5 - this.x);
    return (absTopPostDir - absDdownPostDir) / 2.0;
  }

/***************************************************************************/

  public boolean hasPlayerInfo(int unum){

    if(this.playerTable.getTeammateCertainty(unum) > 0.5)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

   public boolean isBallTrap(){
     // $B<+J,$H$NAjBPB.EY$,(B2m/s,$B%\!<%k$N3QEY(B($B@dBPCM(B)$B$,(B100$BEY0J2<$G$"$l$P(B,
     // trap$B$G$-$F$$$k$H$9$k!#(B
     // $B%\!<%k$H$N5wN%$O(BisKickable$B$GH=Dj:Q$_$H$9$k!#(B00/6/15
     if(this.ball.getRelV() < 0.2 && getAbsDir(this.ball) < 100)
     //if(this.ball.getV() < 1.0)
       return Generic.NS_TRUE;
     else
       return Generic.NS_FALSE;
   }

/***************************************************************************/

   // $BM?$($i$l$?%*%V%8%'%/%H$N(Bt$B%5%$%/%k8e$NJ}8~(B($B@dBP:BI8(B)$B$rJV$9(B
   public double getAbsPredDir(int t,ObjectInfo obj){
     return Generic.RADIAN * Math.atan2(obj.getPredY(t) - this.y,
                                        obj.getPredX(t) - this.x);
   }

/***************************************************************************/

   public double getBallRelV(){
     return this.ball.getRelV();
   }

/***************************************************************************/

  public double getPredX(int t){     // $B;~4V(B t $B8e$N(BX$B:BI8$rM=B,(B
    double predX = x;
    double predVX = vx;
    
    for(int i = 0; i < t; i++){
      predX += predVX;
      predVX *= Generic.PLAYER_DECAY;
    }
    
    return predX;
  }

/***************************************************************************/

  public double getPredY(int t){     // $B;~4V(B t $B8e$N(BY$B:BI8$rM=B,(B
    double predY = y;
    double predVY = vy; 

    for(int i = 0; i < t; i++){
      predY += predVY;
      predVY *= Generic.PLAYER_DECAY;
    }
    
    return predY;
  }

 /***************************************************************************/

   // $B%U%j!<$NH=Dj!#;k3&Fb(B($B5wN%;XDj(B)$B!\(BNeighborhood -0.5m$BFb$KE($,$$$k$+$I$&$+!)(B
   public boolean getFreeFlag(double dist){
     double width;
     // viewMode$B$K9g$o$;$FH=Dj$9$k;k3&HO0O$r7hDj(B
     if(angleWidth.equals("normal"))       
       width = 90 / 2;
     else if(angleWidth.equals("wide"))
       width = 180 / 2;
     else
       width = 45 / 2;

     return this.playerTable.checkMarkedOpponent(this.headFaceDir,width,
						 dist,this.x,this.y);
   }

/**************************************************************************/

  public boolean hasCoachPos(){
    if(this.stdPosChanged){
      return Generic.NS_TRUE;
    }
    else{
      return Generic.NS_FALSE;
    }
  }

/**************************************************************************/

  public void setCoachPosFlag(boolean flag){
    this.stdPosChanged = flag;
  }

/**************************************************************************/

  public double getCoachStdPosX(){
    return this.ctSdX;
  }

/**************************************************************************/

  public double getCoachStdPosY(){
    return this.ctSdY;
  }

/**************************************************************************/

  public double getCoachStartPosX(){
    return this.ctStX;
  }

/**************************************************************************/

  public double getCoachStartPosY(){
    return this.ctStY;
  }

 /***************************************************************************/

 //
 //GK FreeKick$BMQ(B add 0803 by Shinji Futamase
 //$B$3$3$+$i(B 


/**************************************************************************/

  public boolean hasCoachGKFreekickPos(){
      
    if(this.freekickPosChanged){
      return Generic.NS_TRUE;
    }
    else{
      return Generic.NS_FALSE;
    }

  }

/**************************************************************************/

  public void setCoachPosGKFreeKickFlag(boolean flag){
 
   freekickPosChanged = flag;

  }

/**************************************************************************/

  public double getCoachGKFreekickPosX(){

    return this.ctkX;

  }

/**************************************************************************/

  public double getCoachGKFreekickPosY(){

    return this.ctkY;

  }

/**************************************************************************/

  public double getCoachGKFreekickMyPosX(){

    return this.ctmyX;

  }

/**************************************************************************/

  public double getCoachGKFreekickMyPosY(){
      
    return this.ctmyY;

  }

  //
  //$B$3$3$^$G(B
  //

/**************************************************************************/

   // t $B%5%$%/%k$G?J$a$k5wN%(B
   public double getMovableDist(int t,double power,double dir){
     double dist = 0;
     double u = 0;
     // dir$BJ}8~$X$NB.EY@.J,$r=i4|CM$K@_Dj(B  00/6/19
     double V = this.speed * Math.cos(Generic.digToRad(dir));

     for(int i = 0; i < t; i++){
       u = V + power * Generic.DASH_POWER_RATE;
       if(u > Generic.PLAYER_SPEED_MAX)
         u = 1.0;
       dist += u;
       V = Generic.PLAYER_DECAY * u;
     }
     /*  // $BB.EY$N7W;;$*$+$7$$$s$8$c$J$$!)(B
     for(int i = 0; i < t; i++){
       dist = dist + V + power * Generic.DASH_POWER_RATE;
       V *= Generic.PLAYER_DECAY;
     }
     */

     return dist;
   }

/***************************************************************************/

  // $B;XDj$7$?:BI8$NJ}8~$r5a$a$k(B $B@dBP(B
  public double getDirOfThePos2(double X,double Y){
    double absDir = Generic.RADIAN * Math.atan2(Y - this.y,X - this.x);
    
    return absDir;
  }

/***************************************************************************/

  // 5/20 $BE($,%7%e!<%H$rBG$C$F$/$k2DG=@-$,$"$k$+$I$&$+(B
  public boolean getOpponentEnableShot(){
    double boalXX = this.ball.getX();
    double boalYY = this.ball.getY();

    for(int i=0;i<11;i++){
      //System.out.println("DIST "+i+" :" + Generic.DIST(this.playerTable.getOpponetsPlayer(i).getX(),this.playerTable.getOpponetsPlayer(i).getY(),boalXX,boalYY));
      //System.out.println("Ball "+boalXX + " , "+ boalYY);
      //System.out.println("Player " + this.playerTable.getOpponetsPlayer(i).getX() + " , " + this.playerTable.getOpponetsPlayer(i).getY());
      if (Generic.DIST(this.playerTable.getOpponetsPlayer(i).getX(),this.playerTable.getOpponetsPlayer(i).getY(),boalXX,boalYY) < Generic.KICKABLE_AREA*2){
	return true;
      }
    }
    return false;
  }

/***************************************************************************/

  // $B%-!<%Q!<MQ(B
  public boolean getNearSide(){
    double boalXX = this.ball.getX();
    double boalYY = this.ball.getY();

    for(int i=0;i<11;i++){
      if (Generic.DIST(this.playerTable.getOpponetsPlayer(i).getX(),this.playerTable.getOpponetsPlayer(i).getY(),boalXX,boalYY) < Generic.DIST(x,y,boalXX,boalYY)){
	return false;
      }
    }
    return true;    
  }

  /***************************************************************************/

  public int getMyUnum(){
    return this.uniformNum;
  }

  /***************************************************************************/

  public String getMyAngleWidth(){
    return this.angleWidth;
  }

  /***************************************************************************/

  public String getTurnStartDir(){
    if (side.equals("l")){
      return "Right";
    }
      return "Left";
  }

  /***************************************************************************/

  public String getMySide(){
    return side;
  }

/***************************************************************************/

  // $B%-!<%Q!<MQ(B $BAjBP$G$J$/@dBP$rJV$9(B
  // $B#1%5%$%/%k8e$N<+J,$N:BI8$KJQ99(B(8/24)
  public double getPredBallDir2(int t){
    return Generic.normDir(Generic.RADIAN 
                     * Math.atan2(this.ball.getPredY(t)- this.y,
                               this.ball.getPredX(t) - this.x));
  }

  /***************************************************************************/

  // $B%-!<%Q!<MQ(B
  public boolean getOpponentShot(){
    if (this.ball.getV() > 1.2){
      return true;
    }
    return false;
  }

  /***************************************************************************/

  public int getSeeTime(){
    return seeTime;
  }

  /***************************************************************************/

  public boolean judgeCriticalCase(){
    boolean judge = false;
    double boalXX = this.ball.getX();
    double boalYY = this.ball.getY();
    double enableShotX = 999;
    double enableShotY = 999; 

    if (getOpponentEnableShot()){

      for(int i=0;i<11;i++){
	if (Generic.DIST(this.playerTable.getOpponetsPlayer(i).getX(),this.playerTable.getOpponetsPlayer(i).getY(),boalXX,boalYY) < Generic.KICKABLE_AREA*2){
	  enableShotX=this.playerTable.getOpponetsPlayer(i).getX();
	  enableShotY=this.playerTable.getOpponetsPlayer(i).getY(); 
	}
      }

      if (enableShotX == 999 && enableShotY == 999){
	
      } 
      else{
	judge = true;
	for(int i=0;i<11;i++){
	  if (this.playerTable.getTeammate(i).getX()<enableShotX && 
	      Math.abs(this.playerTable.getTeammate(i).getY()-enableShotY)<5){
	    judge = false;
	  }
	}
      }
      
    }
    return judge;
  }

  /***************************************************************************/

  public double getNearestOpponetDist(){
    double min = 1000;

    for(int i=0;i<11;i++){
      if (Generic.DIST(this.playerTable.getOpponetsPlayer(i).getX(),this.playerTable.getOpponetsPlayer(i).getY(),x,y) < min){
	min = Generic.DIST(this.playerTable.getOpponetsPlayer(i).getX(),this.playerTable.getOpponetsPlayer(i).getY(),x,y);
      }
    }
    return min;
  }

  /***************************************************************************/

  public double getPredBallDirForGoalie(int t){
    /*  // $B0l%5%$%/%k8e$N<+J,$N0LCV$O(B (x,y) + (vx,vy)$B$G$$$$$H;W$&!#(B
    return Generic.normDir(Generic.RADIAN 
                           * Math.atan2(this.ball.getPredY(t) - 
					this.y + this.vy,
                                        this.ball.getPredX(t) - 
					this.x + this.vx)
                           - getBodyFaceDir());
    */
    return Generic.normDir(Generic.RADIAN 
                           * Math.atan2(this.ball.getPredY(t) - 
					this.y + this.vy * 0.4,
                                        this.ball.getPredX(t) - 
					this.x + this.vx * 0.4)
                           - getBodyFaceDir());

  }

/***************************************************************************/

  public boolean adjustDirToAvoidPlayer(double X,double Y,
                                        double dist,NSobject dir){
                                                    // NSobject!
    // dist$B$O(Bt$B%5%$%/%k8e$N%\!<%k$N5wN%(B
    // X,Y$B$O(Bt$B%5%$%/%k8e$N%\!<%k$N:BI8(B
    double margin = 50.0;

    int playerNum = this.seeInfoParser.getPlayerNum();
    for(int i = 0; i < playerNum; i++){
      PlayerInfo player = this.seeInfoParser.getPlayerInfo(i);
      player.calcPos(this.x,this.y,this.headFaceDir);
	
      double playerDir = Generic.normDir(getAbsDir(player) - this.bodyFaceDir);
      double playerDist = Generic.DIST(this.x,this.y,
                                       player.getX(),player.getY());
      double playerToBall = Generic.DIST(X,Y,player.getX(),player.getY());

      // $BE(%W%l%$%d!<$H$N5wN%(B < t$B%5%$%/%k8e$N%\!<%k$N5wN%(B &&
      // $BE(%W%l%$%d!<$H$N5wN%(B < 5m &&
      // $BE(%W%l%$%d!<$H%\!<%k$H$NAjBP3Q(B($B@dBPCM(B) < margin / $BE($H$N5wN%(B
      // $B:G8e$,Ff$J$s$@$h$J$"(B
      // $B$H$$$&$o$1$GE($,<+J,$h$j%\!<%k$K6a$+$C$?$i$rDI2C!#(B
      if(playerDist < dist && playerDist < 5 && playerToBall < dist 
	&& Math.abs(Generic.normDir(playerDir - dir.getNS()))
           < margin / playerDist){
       if(Generic.normDir(playerDir - dir.getNS()) >= 0)
	 dir.setNS(playerDir - margin / playerDist);
       else
	 dir.setNS(playerDir + margin / playerDist);
	    
       return Generic.NS_TRUE;
      }
    }
    return Generic.NS_FALSE;
  }

/**********************$B$3$3$+$i6(D4@oN,>pJs(B*********************************/

  public boolean hasSelfStrategy(){ 
    if(strategyCertainty > 0.5)
      return Generic.NS_TRUE;
    else 
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public double getReceivePartner(String S,String tp,double dir,
                          double dirDiff,double dist, Point recXY){ // Point!!
    return this.strategyTable.getReceivePartner(S,tp,dir,dirDiff,dist,
                                                         this.x,this.y,recXY);
  }

/***************************************************************************/

  public double getReceivePlanner(String S,double goalDir,
               double dirDiff,double dist,double interval,NSobject  openDir){
    double value = 1000; 
    int uNum = this.strategyTable.getStrategyPartner(S);
    if(uNum != 0){
      PlayerInfo  planner = this.playerTable.getTeammateInfo(uNum);
      if(planner.getCertainty() > 0.1){
	double tx = planner.getX();
        double ty = planner.getY();
        double tdir = Generic.RADIAN * Math.atan2(ty - this.y,tx - this.x);
        double tdist = Generic.DIST(this.x,this.y,tx,ty);
	if(tdist <= dist){
          double pathAngle = (tdir - goalDir) / 2; 
          value = getDirOfOpenSpace(goalDir + pathAngle,dirDiff,dist,
                                                            interval,openDir);
	}
      }
    }
    return value;
  }

/***************************************************************************/
 
  public void updateStrategyTable(){
    this.strategyTable.updateStrategyTable();

    this.strategyCertainty -= StrategyInfo.S_less;
    if(strategyCertainty < 0){
      strategyCertainty = 0.0;
      strategy = "private";
      strategyType = "show";
    }

  }

/***************************************************************************/

  public void parseStrategy(String S){ 
    if(S == "followUp"){
    }


  }

/***************************************************************************/

  public void parseHearStrategy(){ 
    System.out.println(" parse Hear Strategy ok? ");

  }

/***************************************************************************/

  public void setSelfStrategy(String S, String tp){ 
    this.strategy = S;
    this.strategyType = tp;
    this.strategyCertainty = 1.0;
  }

/***************************************************************************/

  public int getStrategyPartner(String S){ 
    return this.strategyTable.getStrategyPartner(S);
  }

/***************************************************************************/

  public String getSelfStrategy(){ 
    return this.strategy;
  }

/***************************************************************************/

  public String getSelfSType(){ 
    return this.strategyType;
  }

/***************************************************************************/

  public double getStrategyCertainty(){ 
    return this.strategyCertainty;
  }

/***************************************************************************/

  public boolean getStrategyFlag(){ 
    return this.strategyFlag;
  }

/***************************************************************************/

  public String getStrategyPlan(){
    return this.strategyTable.getPlayerStrategy(planner);
  }

/***************************************************************************/

  public int getSPlanner(){
    return this.planner;
  }

/***************************************************************************/

  public void strategyNextState(){ 
    this.strategyCertainty -= StrategyInfo.S_less;
    if(this.strategyCertainty < 0.0){
      this.strategyCertainty = 0.0;
      strategy = "private";
      strategyType = "show";
    }
  }

/***************************************************************************/

  public String getStrategyString(){ 
    StringBuffer strBuff = new StringBuffer("(NIT_strategy ");
    strBuff.append(strategy).append(" ")
       .append(this.uniformNum).append(" ").append(this.strategyType)
       .append(" ").append(this.x).append(" ").append(this.y).append(")");
    return strBuff.toString();
  }

/***************************************************************************/

  public double searchBall(){

    // System.out.println(uniformNum + " searchBall start");

    if(searchFlag == 0){
      // $B%\!<%k$N?.MjEY$,(B0.1$B0J>e$"$l$P$=$N;DA|$rDI$&$h$&$K%?!<%s$9$k!#(B
      if(getBallCertainty() > 0.1){
        double angle = getBallDir();
        if(angle > 0){
          searchFlag = 1;
          return 75;
	}
        else{
          searchFlag = -1;
          return -75;
	}
      }       
      else{
        // $B$H$j$"$($:;~7W2s$j$K(B75$BEY2sE>(B
        searchFlag = 1;
        return 75;
      }
    }
    // $B;~7W2s$j$K2sE>$7$F$$$?>l9g(B,$B7QB3$7$F2sE>(B
    else if(searchFlag == 1){
      return 75;
    }
    // $B5U(B
    else{
      return -75;
    }
  }

}



