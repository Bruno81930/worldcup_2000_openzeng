// Defender_GSR.java

package brainP;

import genericP.*;
// import brainP.*;
import playerP.*;


/********************************************************/
/*         Defender_GSR    DF_G $B%(!<%8%'%s%H(B            */
/********************************************************/

public class Defender_GSR extends Brain{

  int lastCatchTime,lastGoalKickTime;
  static boolean search  = Generic.NS_FALSE;
  static boolean needMove = true; // 5/19 move $B$N%A%'%C%/$NJQ99(B
  static boolean backPoint = false; // FreeKick $B$N8e$OI,$:Dj0LCV$K6/@)0\F0(B
  static boolean goToThePos = false; // 5/26 $B0\F0Cf(B
  static String turnStartDir="";          // $B%\!<%k$r8+<:$C$?$H$-$NC5:wJ}8~(B

  String selfStrategy;
  double ballCatchAngle = 0;       // 5/17 $B%\!<%k$NHt$s$G$-$?J}8~!J>e$+2<!K(B
  int myLevel = 0;
  int turnNumber = 0;
  int goalieKick = 0;              // FreeKick $B$r$7$?<!$N=V4V$K(B catch $B$N(B
                                   // $BH/9T$r$9$k$N$r2!$5$($k$?$a(B
  boolean goalieFreeKick = false;
  Point Sairyo;
  double sairyo = 999;
  int catchFlag = 0;
  int FreeKickWait = 0;             

/***************************************************************************/

  public Defender_GSR(Player p){
    super(p);
    lastCatchTime = 0;
    lastGoalKickTime = 0;
  }

/***************************************************************************/

  public void playOn(){  
    double dashPower = Generic.STAMINA_INC_MAX;
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    String currentStrategy = super.infoManager.getSelfStrategy();
    // double strategyCertainty = super.infoManager.getStrategyCertainty();
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();
    NSobject openDir = new NSobject();  // NS object! openDir
    Point recXY = new Point();  // Point! recX, recY
    myLevel = 0;turnNumber=0;
    int currentTime = super.infoManager.getBodyTime();
    turnStartDir = super.infoManager.getTurnStartDir();
    sairyo = 999;

    if (goalieKick > 0) goalieKick--;

    // $B%-%c%C%A$O(B 5 $B%5%$%/%k$K(B 1 $B2s$7$+$G$-$J$$$N$G(B
    if (catchFlag > 0) catchFlag--;

    if (FreeKickWait > 0)
      FreeKickWait = 0;    

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if (goalieFreeKick){
      // !! Second Step $B%U%j!<%-%C%/8e$O%\!<%k$r0l;~E*$K8+<:$C$F$b(B
      //    $B$$$$$+$i(B,$B4p=`$H$J$j$=$&$J0LCV$KB.96La$j$^$7$g$&(B
      
      if (super.infoManager.getY() > 0){
	// (-47,5) $B$X6[5^0\F0(B
	if (super.infoManager.onThePos(-47,5)){
	  // $B<!2s$+$i$ODL>o$N9TF0!%@5LL$r8+$F$*$/(B
	  goalieFreeKick = false;
	  super.player.turn2(-super.infoManager.getBodyFaceDir());
	  super.player.turnNeck(-super.infoManager.getHeadFaceDir());
	}
	else{
	  double dirXY = super.infoManager.getDirOfThePos2(-47,5);
	  
	  if (Math.abs(dirXY - super.infoManager.getBodyFaceDir()) > 5){
	    // $B3QEY=$@5(B
	    super.player.turn2(dirXY-super.infoManager.getBodyFaceDir());
	  }
	  else{
	    // $BLT%@%C%7%e(B
	    super.player.dash(100);
	  }
	}
      }
      else{
	// (-47,-5) $B$X6[5^0\F0(B
	if (super.infoManager.onThePos(-47,-5)){
	  // $B<!2s$+$i$ODL>o$N9TF0!%@5LL$r8+$F$*$/(B
	  goalieFreeKick = false;
	  super.player.turn2(-super.infoManager.getBodyFaceDir());
	  super.player.turnNeck(-super.infoManager.getHeadFaceDir());
	}
	else{
	  double dirXY = super.infoManager.getDirOfThePos2(-47,-5);
	  
	  if (Math.abs(dirXY - super.infoManager.getBodyFaceDir()) > 5){
	    // $B3QEY=$@5(B
	    super.player.turn2(dirXY-super.infoManager.getBodyFaceDir());
	  }
	  else{
	    // $BLT%@%C%7%e(B
	    super.player.dash(100);
	  }	  
	}
      }
    }
    else if(super.infoManager.hasBallInfo()){
      //  $B%\!<%k>pJs$r$b$C$F$$$k(B
      double ballDir = super.infoManager.getBallDir();  // $B@dBPJ}8~(B
      double ballX = super.infoManager.getBallX();
      double ballY = super.infoManager.getBallY();
      
      // $BE,@50LCV$rC5$9(B
      if(ballX < -47){
        Generic.getDivisionPoint(ballX,ballY,- Generic.FIELD_X_WIDTH * 0.5,
                                                         0,3,1,super.tmpXY);
      }
      else if(ballX < -42){
        Generic.getDivisionPointForGoalie(super.infoManager.getPredBallX(1),
					  super.infoManager.getPredBallY(1),
					  ballX-5,super.tmpXY);
      }
      else if(ballX < -30){
        Generic.getDivisionPointForGoalie(super.infoManager.getPredBallX(1),
					  super.infoManager.getPredBallY(1),
					  -47.0,super.tmpXY);
      }
      else {
        super.tmpXY.setPointX(super.startX);
        super.tmpXY.setPointY(super.startY);
      }

      // $B%P%0$H$jMQ(B
      if(super.tmpXY.getPointX() < - Generic.FIELD_X_WIDTH * 0.5 + 1)
        super.tmpXY.setPointX(- Generic.FIELD_X_WIDTH * 0.5 + 1.5);

      /*
      System.out.println("dist_x" + super.tmpXY.getPointX());
      System.out.println("dist_y" + super.tmpXY.getPointY());
      */

      // $B0l;~E*$KJQ99(B 
      if(ballX < -25)
        dashPower = Generic.STAMINA_INC_MAX * 3;

      // $B9TF07hDj(B
      if (super.infoManager.isCatchable() 
           && super.infoManager.inPenaltyArea()
	   && currentTime - this.lastGoalKickTime > 10
	   && goalieKick == 0
	   && catchFlag == 0){
      // !!! First Step "$B%\!<%k%-%c%C%A(B"
	double nextBallDir = super.infoManager.getPredBallDirForGoalie(1);
        super.player.catchBall(nextBallDir);
	ballCatchAngle = ballY;
	catchFlag = 5; 
      }
      else if(super.infoManager.isKickable()){ 
	// !!! Second Step "$B%\!<%k$r=3$l$k(B"
        if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if((strategyPlan.equals("ballClear") && pointDiff <= 0) || 
             (strategyPlan.equals("ballClear") && 
                                   ballX < - Generic.FIELD_X_WIDTH * 0.25)){ 
            //  $B%A!<%`$,F1E@$^$?$OIi$1$F$$$k(B,$B$^$?$O(B
            //  $B%U%#!<%k%I$N(B1/4$B$3$($F$k>l9g(B,$B%\!<%k%/%j%":NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
        }
        else { //  $B<+8J$N@oN,$N:FI>2A(B
          if(currentStrategy.equals("ballClear") && pointDiff <= 0){ 
            //  $B%A!<%`$,F1E@$^$?$OIi$1$F$$$k>l9g(B,$B%\!<%k%/%j%"7QB3(B
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else { //  $B$=$NB>$N@oN,$rDs0F(B
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25)
              //  $B%U%#!<%k%I$N(B1/4$B$3$($F$k>l9g(B,$B%\!<%k%/%j%"Ds0F(B
              super.infoManager.setSelfStrategy("ballClear","plan");
	  }
        }

        selfStrategy = super.infoManager.getSelfStrategy();
        sayStrategy = Generic.NS_TRUE;  //  $BH/8@8"$"$j(B
	
	
	if(selfStrategy.equals("ballClear")){  //  $B6(D4@oN,(B"ballClear"
          if(super.infoManager.getPosOfReceiver(0,70,35,recXY) < 0.9){
      	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
            super.infoManager.getDirOfClearSpace(0,70,52.5,2,openDir);
	    super.player.shot(super.infoManager.getX() 
			    + 25 * Math.cos(Generic.digToRad(openDir.getNS())),
			    super.infoManager.getY() 
			    + 25 * Math.sin(Generic.digToRad(openDir.getNS())),
			    10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
        }
        else { //  $B$=$NB>$N@oN,(B
          if(super.infoManager.getPosOfReceiver(0,70,52.5,recXY) < 0.9){
      	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
            super.infoManager.getDirOfOpenSpace(0,70,52.5,2,openDir);
	    super.player.shot(super.infoManager.getX() 
			    + 25 * Math.cos(Generic.digToRad(openDir.getNS())),
			    super.infoManager.getY() 
			    + 25 * Math.sin(Generic.digToRad(openDir.getNS())),
			    10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
        }
      } 
      else if (super.infoManager.isBallInPenaltyArea(10)){
	// !!! Third Step "$B%Z%J%k%F%#%(%j%"Fb(B"
	// 5/26 $B$H$j$"$($:=q$$$?>uBV!%Cf?H$O8+D>$9I,MW@dBg(B
	
	// $B<BAu$K8~$1$F(B
	/*
	double a = (g_y - ballY)/(-52.5 - ballX);
	double b = g_y + a * 52.5;
	double Y1 = a * super.infoManager.getX() + b;
	double a = (g_y - ballY)/(-52.5 - ballX);
	double b = g_y + a * 52.5;
	double Y2 = a * super.infoManager.getX() + b;
	*/
	// $B<BAu$K8~$1$F(B

	if (super.infoManager.judgeCriticalCase()){
	  super.player.chaseBall(100);
	  super.player.lookAtBall();
	}
	else if (!super.infoManager.getNearSide()){
	  // $BE($NJ}$,%\!<%k$N6a$/$K$$$k(B
	  if (super.infoManager.getBallX() < super.infoManager.getX()){
	    // $B<+J,$h$j8e$m$K%\!<%k$,$"$k!J6[5^;vBV!K(B
	    super.player.chaseBall(100);
	    super.player.lookAtBall();
	  }
	  else if (super.infoManager.getBallDist() < 5){
	    if (super.infoManager.getOpponentEnableShot()){
	      if (!super.infoManager.onThePos(super.tmpXY.getPointX(),
					      super.tmpXY.getPointY())){
		// $BE,@50LCV$K0\F0$7$F%-%c%C%A=`Hw(B
		
		goToThePos = true;
		super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());
	      }
	      else{
		super.player.lookAtBall();
	      }
	    }	      
	    else{
	      if (super.infoManager.getNearestOpponetDist() < 5){
		if (!super.infoManager.onThePos(super.tmpXY.getPointX(),
						super.tmpXY.getPointY())){
		  // $BE,@50LCV$K0\F0$7$F%-%c%C%A=`Hw(B
		  
		  goToThePos = true;
		  super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());
		}
		else{
		  super.player.lookAtBall();
		}		
	      }
	      else{
		if (super.infoManager.getX() > -42){
		  if (!super.infoManager.onThePos(super.tmpXY.getPointX(),
						  super.tmpXY.getPointY())){
		    // $BE,@50LCV$K0\F0$7$F%-%c%C%A=`Hw(B
		    
		    goToThePos = true;
		    super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());
		  }
		  else{
		    super.player.lookAtBall();
		  }		
		}
		else{
		  super.player.chaseBall(100);
		  super.player.lookAtBall();	    
		}
	      }
	    }
	  }
	  else if (!super.infoManager.onThePos(super.tmpXY.getPointX(),
                                    super.tmpXY.getPointY())){
	    // $BE,@50LCV$K0\F0$7$F%-%c%C%A=`Hw(B
	    
	    goToThePos = true;
	    super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());	   

	  }
	  else if (super.infoManager.getOpponentShot()){
	    super.player.chaseBall(100);
	    super.player.lookAtBall();
	  }
	  else{
	    super.player.lookAtBall();
	  }
	}
	else{
	  super.player.chaseBall(100);
	  super.player.lookAtBall();
	}
      }
      else if (!super.infoManager.onThePos(super.tmpXY.getPointX(),
					   super.tmpXY.getPointY())){
	// !!! fourth Step "$B0\F0$NI,MW$,$"$k(B"
	
	goToThePos = true;
	super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());	
      }
      else {
	// !!! LAST Step "$B%\!<%k$rJaB*$9$k(B"
	
	// $B0\F0$NI,MW$,$J$/$J$C$?$N$G(B
	goToThePos = false;
	
	super.player.turn(super.infoManager.getMoment(ballDir));
	super.player.lookAtBall();      
      }
    }
  else {
    // !!! $B%\!<%k>pJs$r$b$C$F$$$J$$>l9g(B
    if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
      String strategyPlan = super.infoManager.getStrategyPlan();
      int planner = super.infoManager.getSPlanner();
      
      if(strategyPlan.equals("ballClear") && pointDiff < 0){ 
	//  $B%A!<%`$,Ii$1$F$$$k>l9g(B,$B%\!<%k%/%j%":NMQ(B
	super.infoManager.setSelfStrategy(strategyPlan,"show");
      }
    }
    else { //  $B<+8J$N@oN,$N:FI>2A(B
      if(currentStrategy.equals("ballClear") && pointDiff < 0){ 
	  //  $B%A!<%`$,Ii$1$F$$$k>l9g(B,$B%\!<%k%/%j%"7QB3(B
	super.infoManager.setSelfStrategy(currentStrategy,"show");
      }
    }

    // $B%\!<%k>pJs$r;}$C$F$$$J$$$H$-$N;XDj0LCV(B
    super.tmpXY.setPointX(super.startX);
    super.tmpXY.setPointY(super.startY);

    // $B%\!<%k$rC5$9(B
    if (goToThePos || !super.infoManager.onThePos(super.tmpXY.getPointX(),
					       super.tmpXY.getPointY())){
      super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());
      
      // goToThePos $B$N:FI>2A(B
      goToThePos = !super.infoManager.onThePos(super.tmpXY.getPointX(),
					       super.tmpXY.getPointY());      
    }
    else {
      // $B%\!<%k$rC5$9$H$-$N2sE>J}8~$rAj<j$N%4!<%kB&$+$i2s$k(B
      
      if (turnStartDir.equals("Right")){
	super.player.turn2(+45);
      }
      else if (turnStartDir.equals("Left")){
	super.player.turn2(-45);
      }
      else{
	// $B$^$:$"$jF@$J$$(B if $BJ8$@$,(B
	super.player.turn2(45);
      }
      
      // $B$H$j$"$($:?HBN$N8~$-$K@5BP$7$F$*$/(B
      //super.player.turnNeck(super.infoManager.getHeadFaceDir()-super.infoManager.getBodyFaceDir());

    }
    
    //  $B<+J,$N;}$D@oN,$H%\!<%k>pJs$rN.$9(B
    if(sayBallString || sayStrategy){
      StringBuffer str = new StringBuffer("(");  // sprintf!!
      
      //  $B%\!<%k>pJs$O(Bshot$B$N8e$N$_N.$9(B
      if(sayBallString)
	str.append(super.infoManager.getBallString());
      if(sayBallString && sayStrategy)
	str.append(" ");
      //  $B<+J,$N@oN,$r%V%m!<%I%-%c%9%H(B
      if(sayStrategy)
	str.append(super.infoManager.getStrategyString());
      
      str.append(")");
      
      super.sayBuff = str.toString(); 
      super.player.say(super.sayBuff);    
    }
  }
    //
    /*
      if(!currentStrategy.equals(super.infoManager.getSelfStrategy()))
      System.out.println("[" + currentTime + "] (" + super.player.getUniformNum()
      + ") " + super.infoManager.getStrategyString());
      
      System.out.println("[" + currentTime + "] (" + super.player.getUniformNum()
      + ")  <" + super.infoManager.getStrategyCertainty() + "> "
      + super.infoManager.getStrategyString());
      */
  }
    
/***************************************************************************/

  public void beforeKickOff(){
    if(super.infoManager.onThePos(super.startX,super.startY)){
      if(super.infoManager.hasBallInfo()){
        super.player.turn2(super.infoManager.getBallDir());
      } else {
         super.player.turn2(45);
      }
    } else {
        super.player.move(super.startX,super.startY);
	super.player.changeView("normal","high");
    }
  }

/***************************************************************************/

  public void ourFreeKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint(); 
    backPoint=true;
    double kickMargin = 0.4;       // $B%-%C%/$N@:EY8~>eMQ(B,$BCM$OE,Ev(B(0.35$B0J>e(B)	
    // System.out.println("BodyFace " + super.infoManager.getBodyFaceDir());
    goalieKick = 3;
    goalieFreeKick = true;

    turnNumber++;

    /*
      FreeKick $B$NN.$l(B
       Level Action
         1.   move $B$r9T$&(B
	 3.   $B=3$kJ}8~$rC5$9(BA($B:#$O%Q%9(B)
	 4.   $B=3$kJ}8~$rC5$9(BB($B:#$O%Q%9(B)
	 5.   $B%\!<%k$K6a$E$/(B
	 6.   $B%\!<%k$r=3$k(B
    */

    if (myLevel == 0){
      if(super.infoManager.onThePos(-42,12) || 
	   super.infoManager.onThePos(-42,-12)){
	// $B=jDj$N0LCV$K$$$k$J$i(B
	myLevel++;
      }
    }
    else if (myLevel == 1){
      // System.out.println("BodyFace " + super.infoManager.getBodyFaceDir());
      if(Math.abs(super.infoManager.getBodyFaceDir()) < 5 ){
	// $BA0$r$A$c$s$H8+$F$$$k$J$i(B
	//myLevel=6;    
      }
      myLevel=6;
    }
    else if (myLevel == 5){
      if(super.infoManager.getBallDist() < kickMargin){
	myLevel++;
      }      
    }
    
    
    
    if(myLevel == 0){
      // $B$^$:<+J,$N0LCV$r9g$o$9!%(B
      
      if (ballCatchAngle > 0){
	// $B%\!<%k$,2<$+$iHt$s$G$-$?!%>/$J$/$F$b2<$K$OE($,$$$k$N$G(B
	super.player.move(-42,-12);
	super.player.lookAtBall();
	// System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // Level 0-1");
      }
      else{
	super.player.move(-42,12);
	super.player.lookAtBall();
	// System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // Level 0-2"); 
      }
    }
    
    if (myLevel == 1){
      super.player.turn2(30);
      super.player.lookAtBall();
      // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // Level 1"); 
    }
  
  if (myLevel == 6){
    if(this.infoManager.hasBallInfo()){
	double ballX = super.infoManager.getBallX();
	double ballY = super.infoManager.getBallY();
	
	// kick $B$NJ}8~Ey$r7h$a$k(B
	NSobject openDirNS = new NSobject();  // NS object! openDir
	Point recXY = new Point();  // Point! recX, recY
	Point oppXY = new Point();  // Point! recX, recY
	
	super.infoManager.getPosOfOpponentsForward(oppXY);
	if(oppXY.getPointX() < - Generic.FIELD_X_WIDTH * 0.25){ 
	  // $BE((BFW$B$,%U%#!<%k%I(B1/4$B0J>e96$a$F$$$k>l9g(B,$B%\!<%k%/%j%"Ds0F(B
	  super.infoManager.setSelfStrategy("ballClear","plan");
	  sayStrategy = Generic.NS_TRUE;
	}
	
	double value = super.infoManager.getPosOfReceiver(super.infoManager.getBodyFaceDir(),45,35,recXY,10);
	
	if (value < sairyo){
	  sairyo = value;
	  Sairyo = recXY;
	}

	if (turnNumber > 13){
	  if(sairyo < 1.5){  //  $B$9$3$7%7%S%"$K$7$F$_$?(B(1.7 -> 1.5)
	    super.infoManager.getPosOfOpponentsForward(oppXY);
	    if(recXY.getPointX() > oppXY.getPointY()){
	      //  $B%Q%9Aj<j$,E((BFW$B$h$jA0J}$K$$$k>l9g(B,$B%+%&%s%?!<Ds0F(B
	      super.infoManager.setSelfStrategy("counter","plan");
	      sayStrategy = Generic.NS_TRUE;
	    }
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),100);
	    // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // $B8+J}$K%Q%9$r=3$k(B");
	    sayBallString = Generic.NS_TRUE;
	  }
	  else {
	    //  $B%Q%9Aj<j$d%*!<%W%s%9%Z!<%9$,$J$$$H$-(B.$B%\!<%k%/%j%"Ds0F(B
	    super.infoManager.setSelfStrategy("ballClear","plan");
	    sayStrategy = Generic.NS_TRUE;
	    value = super.infoManager.getDirOfOpenSpace(0,70,25,2,openDirNS);
	    super.player.shot(super.infoManager.getX() + 30
			      * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30
			      * Math.sin(Generic.digToRad(openDirNS.getNS())),100);
	    //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	    // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // $B$H$K$+$/A0$K6/$/=3$k(B");
	    sayBallString = Generic.NS_TRUE;
	  }
	}
    }
    else {
      // $B%\!<%k>pJs$,$J$$$N$J$i%\!<%k$rC5$9(B
      this.player.turn(this.infoManager.getMoment(30));
      // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // ball$B$rC5$9(B");
    }
  }
    
  //  $B<+J,$N;}$D@oN,$H%\!<%k>pJs$rN.$9(B
  if(sayBallString || sayStrategy){
      StringBuffer str = new StringBuffer("(");  // sprintf!!
      
      //  $B%\!<%k>pJs$O(Bshot$B$N8e$N$_N.$9(B
      if(sayBallString)
	str.append(super.infoManager.getBallString());
      if(sayBallString && sayStrategy)
	str.append(" ");
      //  $B<+J,$N@oN,$r%V%m!<%I%-%c%9%H(B
      if(sayStrategy)
	str.append(super.infoManager.getStrategyString());
      
      str.append(")");
      
      super.sayBuff = str.toString(); 
      super.player.say(super.sayBuff);    
    }
    
  /*
    System.out.println("(" + super.player.getUniformNum() + ") strategy : "
    + super.infoManager.getStrategyString());             
    */
  }
  
/***************************************************************************/
  
  public void ourGoalKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();
    Point oppXY = new Point();  // Point! recX, recY

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    super.infoManager.getPosOfOpponentsForward(oppXY);
    if(oppXY.getPointX() < - Generic.FIELD_X_WIDTH * 0.25){ 
      // $BE((BFW$B$,%U%#!<%k%I(B1/4$B0J>e96$a$F$$$k>l9g(B,$B%\!<%k%/%j%"Ds0F(B
      super.infoManager.setSelfStrategy("ballClear","plan");
      sayStrategy = Generic.NS_TRUE;
    }

    if(!super.infoManager.hasBallInfo()){
      super.player.turn2(45);
    }
    else if(super.infoManager.isKickable2()){ //  $B%\!<%k$r;}$C$F$$$k(B
      NSobject openDirNS = new NSobject();  // NS object! openDir
      Point recXY = new Point();  // Point! recX, recY

      if(super.infoManager.getPosOfReceiver(0,30,35,recXY) < 1.5){
                                        //  $B$9$3$7%7%S%"$K$7$F$_$?(B(1.7 -> 1.5)
        if(recXY.getPointX() > oppXY.getPointX()){
          //  $B%Q%9Aj<j$,E((BFW$B$h$jA0J}$K$$$k>l9g(B,$B%+%&%s%?!<Ds0F(B
          super.infoManager.setSelfStrategy("counter","plan");
          sayStrategy = Generic.NS_TRUE;
        }

        super.player.shot(recXY.getPointX(),recXY.getPointY(),100);
        sayBallString = Generic.NS_TRUE;
        this.lastGoalKickTime = super.infoManager.getBodyTime();
      }
      else if(super.infoManager.getDirOfOpenSpace(0,30,25,2,openDirNS) < 1.7){
       	super.player.shot(super.infoManager.getX() + 25 *
                          Math.cos(Generic.digToRad(openDirNS.getNS())),
       	                  super.infoManager.getY() + 25 * 
                          Math.sin(Generic.digToRad(openDirNS.getNS())),100);
        sayBallString = Generic.NS_TRUE;
        this.lastGoalKickTime = super.infoManager.getBodyTime();
      }
      else {
        //  $B%Q%9Aj<j$d%*!<%W%s%9%Z!<%9$,$J$$$H$-(B,$B%\!<%k%/%j%"Ds0F(B
        super.infoManager.setSelfStrategy("ballClear","plan");
        sayStrategy = Generic.NS_TRUE;
        super.infoManager.getDirOfOpenSpace(0,45,25,2,openDirNS);
       	super.player.shot(super.infoManager.getX() + 30 *
                          Math.cos(Generic.digToRad(openDirNS.getNS())),
       	                  super.infoManager.getY() + 30 * 
                          Math.sin(Generic.digToRad(openDirNS.getNS())),100);
	                                           //  $B$H$K$+$/A0$K6/$/=3$l!*(B
        sayBallString = Generic.NS_TRUE;
        this.lastGoalKickTime = super.infoManager.getBodyTime();
      }
    }
    else {
      super.player.chaseBall(dashPower);
      super.player.lookAtBall();
    }

    //  $B<+J,$N;}$D@oN,$H%\!<%k>pJs$rN.$9(B
    if(sayBallString || sayStrategy){
      StringBuffer str = new StringBuffer("(");  // sprintf!!

      //  $B%\!<%k>pJs$O(Bshot$B$N8e$N$_N.$9(B
      if(sayBallString)
        str.append(super.infoManager.getBallString());
      if(sayBallString && sayStrategy)
        str.append(" ");
      //  $B<+J,$N@oN,$r%V%m!<%I%-%c%9%H(B
      if(sayStrategy)
        str.append(super.infoManager.getStrategyString());

      str.append(")");

      super.sayBuff = str.toString(); 
      super.player.say(super.sayBuff);    
    }

//
/*
System.out.println("(" + super.player.getUniformNum() + ") strategy : "
                         + super.infoManager.getStrategyString());             
*/
  }

/***************************************************************************/

  public void ourCornerKick(){
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
       dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      if(ballX < -15)
        dashPower = Generic.STAMINA_INC_MAX * 2;

      if(!super.infoManager.onThePos(super.stdX,super.stdY)){
        super.player.goToThePos(super.stdX,super.stdY,dashPower);
      }
      super.player.lookAtBall();
    } else {
	super.player.turn2(45);
    }
  }

/***************************************************************************/

  public void ourKickIn(){
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      if(ballX < -15)
        dashPower = Generic.STAMINA_INC_MAX * 2;

      if(!super.infoManager.onThePos(super.stdX,super.stdY)){
        super.player.goToThePos(super.stdX,super.stdY,dashPower);
      }
      super.player.lookAtBall();
    } else {
	super.player.turn2(45);
    }
  }

/***************************************************************************/

  public void opponentsFreeKick(){
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      if(ballX < -15)
        dashPower = Generic.STAMINA_INC_MAX * 2;

      if(!super.infoManager.onThePos(super.stdX,super.stdY)){
        super.player.goToThePos(super.stdX,super.stdY,dashPower);
      }
      super.player.lookAtBall();
    } else {
	super.player.turn2(45);
    }
  }

/***************************************************************************/

  public void opponentsGoalKick(){
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      if(ballX < -15)
        dashPower = Generic.STAMINA_INC_MAX * 2;

      if(!super.infoManager.onThePos(super.stdX,super.stdY)){
        super.player.goToThePos(super.stdX,super.stdY,dashPower);
      }
      super.player.lookAtBall();
    } else {
	super.player.turn2(45);
    }
  }

/***************************************************************************/

  public void opponentsCornerKick(){
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      if(ballX < -15)
        dashPower = Generic.STAMINA_INC_MAX * 2;

      if(!super.infoManager.onThePos(super.stdX,super.stdY)){
        super.player.goToThePos(super.stdX,super.stdY,dashPower);
      }
      super.player.lookAtBall();
    } else {
	super.player.turn2(45);
    }
  }

/***************************************************************************/

  public void opponentsKickIn(){
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      if(ballX < -15)
        dashPower = Generic.STAMINA_INC_MAX * 2;

      if(!super.infoManager.onThePos(super.stdX,super.stdY)){
        super.player.goToThePos(super.stdX,super.stdY,dashPower);
      }
      super.player.lookAtBall();
    } else {
	super.player.turn2(45);
    }
  }

/***************************************************************************/

  public void opponentsKickOff(){
    if(super.infoManager.onThePos(super.startX,super.startY)){
      if(super.infoManager.hasBallInfo()){
        super.player.turn2(super.infoManager.getBallDir());
      } else {
          super.player.turn2(45);
	}
    } else {
	super.player.move(super.startX,super.startY);
	super.player.changeView("normal","high");
    }
  }

/***************************************************************************/

  public void ourGoal(){
    if(super.infoManager.onThePos(super.startX,super.startY)){
      if(super.infoManager.hasBallInfo()){
        super.player.turn2(super.infoManager.getBallDir());
      } else {
         super.player.turn2(45);
      }
    } else {
	super.player.move(super.startX,super.startY);
	super.player.changeView("normal","high");
    }
  }

/***************************************************************************/

  public void opponentsGoal(){
    if(super.infoManager.onThePos(super.startX,super.startY)){
      if(super.infoManager.hasBallInfo()){
        super.player.turn2(super.infoManager.getBallDir());
      } else {
          super.player.turn2(45);
      }
    } else {
	super.player.move(super.startX,super.startY);
	super.player.changeView("normal","high");
    }
  }

/***************************************************************************/

}
