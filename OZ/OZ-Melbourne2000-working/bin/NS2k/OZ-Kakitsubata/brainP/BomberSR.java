// BomberSR.java

package brainP;

import genericP.*;
// import brainP.*;
import playerP.*;

/********************************************************/
/*         BomberSR   FW $B%(!<%8%'%s%H(B                   */
/********************************************************/

public class BomberSR extends Brain{

  static int lastSearchTime = 0;    
  static int searchStep = 3;

  String selfStrategy;

/***************************************************************************/

  public  BomberSR(Player p){
    super(p);
  }

/***************************************************************************/

  public void playOn(){   
    int currentTime = super.infoManager.getBodyTime();
    boolean freeFlag = Generic.NS_FALSE;
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    String currentStrategy = super.infoManager.getSelfStrategy();
    // double strategyCertainty = super.infoManager.getStrategyCertainty();
    NSobject openDirNS = new NSobject();     // NSobject!!
    Point recXY = new Point();               // Point!!
    Point oppXY = new Point();               // Point!!
    Point markOppXY = new Point();               // Point!!

    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    double staminaThreshold;
    double dashPower;

    // $B%9%?%_%J4IM}MQ!#5^B$;EMM$N$?$a%P%i%s%9$o$k$$$+$b!#(B
    if(currentTime > 5500){                   // $B%,%s%P%k%7%9%F%`(B
      staminaThreshold = 0;
      dashPower = Generic.STAMINA_INC_MAX * 2;
    }
    else if(pointDiff < -1){                  // $BIi$1$F$$$k(B
      staminaThreshold = Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR;
      dashPower = Generic.STAMINA_INC_MAX * 1.2;
    }
    else {
      staminaThreshold = Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR + 400;
      dashPower = Generic.STAMINA_INC_MAX;
    }

    if(super.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      double ballY = super.infoManager.getBallY();
      double predBallX = super.infoManager.getPredBallX(10);
      double predBallY = super.infoManager.getPredBallY(10);
      double ballDir = super.infoManager.getBallDir();
      double absGoalDir = super.infoManager.getAbsGoalDir();
      double goalDist = super.infoManager.getGoalDist();
      double offSideLine = super.infoManager.getOffsideLine();
      double shootAngle = super.infoManager.getShootAngle();
      double absShootDir = super.infoManager.getAbsTopPostDir() - shootAngle;
      double ballRelV = super.infoManager.getBallRelV();

      // absShootDir$B$N;;=P$OL5M}$d$j$@$1$I$3$l$G(BOK$B$+!)(B
      shootAngle = Math.abs(shootAngle);

      // $B%U%j!<$+$I$&$+$NH=Dj!#(B
      // $B;k3&Fb(B($B5wN%;XDj(B=3.0m)$B!\(BNeighborhood -0.5m(2.5m)$BFb$KE($,$$$k$+$I$&$+(B
      freeFlag = super.infoManager.getFreeFlag(3.0);
      /*
      if(freeFlag)
        System.out.println(super.player.getUniformNum() + " free!");
      */
      
      /*  $B@o67$K$h$j:BI8JQ2=$5$;$?$$!#$3$l$O$$$^$$$A(B
      if(ballX < - Generic.FIELD_X_WIDTH * 0.25 && pointDiff < 0)
        super.tmpXY.setPointX(super.stdX - 15);
      else
      */
      super.tmpXY.setPointX(offSideLine - 3);
      super.tmpXY.setPointY(super.stdY + ballY / 3.0);

      double distBallPos = Generic.DIST(ballX,ballY,
                           super.infoManager.getX(),super.infoManager.getY());
      double predDistBallPos = Generic.DIST(predBallX,predBallY,
                             super.tmpXY.getPointX(),super.tmpXY.getPointY());

      //  FW$B$OH?7b$N5!2q$rBT$D$Y$7(B
      if(ballX < -10){
	dashPower = Generic.STAMINA_INC_MAX * 0.6;
      }
      else if(ballX > Generic.FIELD_X_WIDTH * 0.25 && Math.abs(ballY) < 20
                    && pointDiff <= 0){ // $B%A%c%s%9$K$O5$9g$$$G!*(B
        staminaThreshold = Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR - 400;
      }
      

      if(super.infoManager.isKickable()){
        if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  $B%+%&%s%?!<$r:NMQCf$K%\!<%k%/%j%"$rJ9$$$?$i(B,$B%+%&%s%?!<GK4~(B
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter")){ 
            //  $B%+%&%s%?!<$N9g?^$rJ9$$$?$i(B,$B%+%&%s%?!<:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("1-2Return") && 
            super.infoManager.hasPlayerInfo(super.infoManager.getSPlanner())){ 
            //  1-2$B%j%?!<%s$NDs<(<T$,3NG'$G$-$F$$$k>l9g(B,1-2$B%j%?!<%s:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"reply");
	  }
        }
        else { //  $B<+8J$N@oN,$N:FI>2A(B
          if(currentStrategy.equals("counter") && 
                          super.infoManager.forwardFW(absGoalDir,60,0) >= 1){ 
            //  $BA0J}$KL#J}(BFW$B$,$$$k>l9g(B,$B%+%&%s%?!<7QB3(B
            super.infoManager.setSelfStrategy(currentStrategy,"reply");
	  }
          else if(currentStrategy.equals("sideChange") && 
                                      super.infoManager.parseSideChange()){ 
            //  $B%\!<%k$N$"$k%U%#!<%k%I$KE($,=8Cf$7$F$$$k>l9g(B,$B%5%$%I%A%'%s%8:NMQ(B
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else { //  $B$=$NB>$N@oN,$rDs0F(B
            if(ballX < Generic.FIELD_X_WIDTH * 0.25 && 
                         super.infoManager.forwardFW(absGoalDir,60,0) >= 1){
              //  $BE(?XCfHW$GL#J}(BFW$B$,A0J}$K$$$k>l9g(B,$B%+%&%s%?!<Ds0F(B
              super.infoManager.setSelfStrategy("counter","plan");
	    }
	  }
        }

        selfStrategy = super.infoManager.getSelfStrategy();
        sayStrategy = Generic.NS_TRUE;  //  $BH/8@8"$"$j(B

        // $B%U%j!<$G%H%i%C%W$G$-$F$$$J$$>l9g(B
        if(freeFlag && !super.infoManager.isBallTrap()){
          // $B$^$:%\!<%k$NB.EY$r;&$9!#(B
          if(ballRelV >= 0.2){
            super.player.trapBall();
            // System.out.println(currentTime+" trapBall");
	  // $BE(%4!<%kJ}8~$K%\!<%k$r(B
          }else if(Math.abs(ballDir) > 100){
            if(ballDir >= 0){
              super.player.turnBall(90);
              // System.out.println(currentTime+" trunBall");
            }
            else{
              super.player.turnBall(-90);         
              // System.out.println(currentTime+" trunBall");
            }
	  }
          super.player.lookAtBall();
          sayBallString = Generic.NS_TRUE;
          sayStrategy = Generic.NS_FALSE;
	}
        else

        if(selfStrategy.equals("counter")){  //  $B6(D4@oN,(B"counter"
	  if(goalDist < 20){
	    super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                goalDist,2,openDirNS);
	    if(super.infoManager.isBallTrap()){
	      super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	      super.player.lookAtBall();
	    }
            else {
	      super.player.trapBall();
	      super.player.lookAtBall();
	    } 
            sayBallString = Generic.NS_TRUE;
            // System.out.println("$B%+%&%s%?!<%7%e!<%H!*!*(B");
	  }
          else if(freeFlag){  //  $B<~$j$KE($,$$$J$$(B
            if((super.infoManager.
	        getDirOfOpenSpace(absGoalDir,15,7,3,openDirNS) < 1.7) &&
	       (Math.abs(Generic.normDir(openDirNS.getNS() - 
                                  super.infoManager.getBodyFaceDir())) < 5) &&
	       (Math.abs(ballDir) < 100)){
	      if(ballDir >= 0)
	        super.player.dribble(100,70);
	      else
	        super.player.dribble(100,-70);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B2ZNo$J%I%j%V%k!*!*(B");
              if(super.infoManager.getStamina() > staminaThreshold){
                //  $B%9%?%_%J$,$"$k>l9g(B,1-2$B%j%?!<%s!*(B
                super.infoManager.setSelfStrategy("1-2Return","plan");
	      }
	    }
	    else if(super.infoManager.getPosOfReceiver(absGoalDir,40,25,recXY)
                                                                      < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B$D$P$5$!!*!*(B");
	    }
            else {
              super.infoManager.getDirOfOpenSpace(absGoalDir,30,25,2,
                                                           openDirNS);
	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);            
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	  }
          else {  //  $B%U%j!<$8$c$J$$$H$-(B
  	    if(super.infoManager.getPosOfReceiver(absGoalDir,40,20,recXY)
                                                                      < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B$_$9$.!*!*(B");
	    }
	    else if(super.infoManager.getDirOfOpenSpace(absGoalDir,15,15,2,
                                                           openDirNS) < 1.7){
              if(super.infoManager.forwardFW(0,90,0) >= 1){
                //  $B%4!<%kJ}8~$K%*!<%W%s%9%Z!<%9$,$"$jL#J}$,$$$k(B,$B%9%k!<%Q%9!*(B
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("$B6/Nu$J%9%k!<%Q%9!*!*(B");
	      }
	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B%7%e!<%H!*!*(B");
	    }
	    else {
              super.infoManager.
		      getDirOfOpenSpace(absGoalDir,30,7,5,openDirNS);
	      if(Math.abs(Generic.normDir(openDirNS.getNS() - 
                                    super.infoManager.getBodyFaceDir())) < 3){
	         if(Math.abs(ballDir) < 100){
	           if(ballDir >= 0)
		     super.player.dribble(100,70);
	           else
		     super.player.dribble(100,-70);
                   // System.out.println("$B@dL/$J%I%j%V%k!*!*(B");
                   if(super.infoManager.getStamina() > staminaThreshold){
                     //  $B%9%?%_%J$,$"$k>l9g(B,1-2$B%j%?!<%s!*(B
                     super.infoManager.setSelfStrategy("1-2Return","plan");
		   }
	         }
                 else {
	           if(ballDir >= 0)
		     super.player.turnBall(90);
	           else
		   super.player.turnBall(-90);
	         }
	      }
              else {
	        super.player.turn(super.infoManager.
                                   getMoment(Generic.normDir(openDirNS.getNS()
                                   - super.infoManager.getBodyFaceDir())));
	        super.player.lookAtBall();
	      } 
              sayBallString = Generic.NS_TRUE;
	    }
	  }
	}
        else if(selfStrategy.equals("1-2Return")){  //  $B6(D4@oN,(B"1-2Return"
          if(freeFlag){  //  $B<~$j$KE($,$$$J$$(B
	    if(goalDist < 18){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      if(super.infoManager.isBallTrap()){
	        super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	        super.player.lookAtBall();
	      }
              else {
	        super.player.trapBall();
	        super.player.lookAtBall();
	      } 
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B$&$*$*$)$*!*!*(B");
	    }
            else if((super.infoManager.
	        getDirOfOpenSpace(absGoalDir,30,7,3,openDirNS) < 1.7) &&
	       (Math.abs(Generic.normDir(openDirNS.getNS() - 
                                  super.infoManager.getBodyFaceDir())) < 5) &&
	       (Math.abs(ballDir) < 100)){
	      if(ballDir >= 0)
	        super.player.dribble(100,70);
	      else
	        super.player.dribble(100,-70);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B2ZNo$J%I%j%V%k!*!*(B");
              // 1-2$B%j%?!<%s7QB3(B
              super.infoManager.setSelfStrategy("1-2Return",
                                            super.infoManager.getSelfSType());
	    }
            else if(super.infoManager.getSelfSType().equals("plan") &&
              (super.infoManager.getReceivePartner("1-2Return","reply",
		     super.infoManager.getBodyFaceDir(),100,20,recXY) < 1.7)){
              //  1-2$B%j%?!<%s;?F1<T$K%Q%9$,DL$k>l9g(B,1-2$B%j%?!<%s7QB3(B
              super.infoManager.setSelfStrategy("throughPass","plan");

	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              //  System.out.println("1-2 $B$/$l!*(B");
	    }
            else if(super.infoManager.getSelfSType().equals("reply") &&

              (super.infoManager.getReceivePlanner("1-2Return",absGoalDir,
		                                   10,20,5,openDirNS) < 1.7)){
              //  1-2$B%j%?!<%sDs<(<T$K%Q%9$,DL$k>l9g(B,1-2$B%j%?!<%s7QB3(B
              super.infoManager.setSelfStrategy("throughPass","reply");

	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("1-2 $B$$$C$?$>!*(B");
	    }
            else if(super.infoManager.getPosOfReceiver(absGoalDir,45,20,
                                                               recXY) < 1.7){
              super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
            }
            else {
              //  $B;vA0$K$5$,$7$?%*!<%W%s%9%Z!<%9$K=3$k(B
              if(super.infoManager.forwardFW(absGoalDir,90,0) >= 1){
                //  $B%4!<%kJ}8~$K%*!<%W%s%9%Z!<%9$,$"$jL#J}$,$$$k(B,$B%9%k!<%Q%9(B 
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("$B6/0z$J%9%k!<%Q%9!*!*(B");
	      }
	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
            }
          }
          else {  //  $B%U%j!<$8$c$J$$$H$-(B
	    if(goalDist < 20){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B%?%$%,!<%7%g%C%H!*!*(B");
	    }
            else if(super.infoManager.getSelfSType().equals("plan") &&
              (super.infoManager.getReceivePartner("1-2Return","reply",
		     super.infoManager.getBodyFaceDir(),100,20,recXY) < 1.7)){
              //  1-2$B%j%?!<%s;?F1<T$K%Q%9$,DL$k>l9g(B,1-2$B%j%?!<%s7QB3(B
              super.infoManager.setSelfStrategy("throughPass","plan");

	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("1-2 $B$/$l!*(B");
	    }
            else if(super.infoManager.getSelfSType().equals("reply") &&
              (super.infoManager.getReceivePlanner("1-2Return",absGoalDir,
		                                   10,20,5,openDirNS) < 1.7)){
              //  1-2$B%j%?!<%sDs<(<T$K%Q%9$,DL$k>l9g(B,1-2$B%j%?!<%s7QB3(B
              super.infoManager.setSelfStrategy("throughPass","reply");

	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("1-2 $B$$$C$?$>!*(B");
	    }
  	    else if(super.infoManager.getPosOfReceiver(absGoalDir,45,20,recXY)
                    < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else if(super.infoManager.getDirOfOpenSpace(absGoalDir,30,25,5,
                                                           openDirNS) < 1.7){
              if(super.infoManager.forwardFW(absGoalDir,90,0) >= 1){
                //  $B%4!<%kJ}8~$K%*!<%W%s%9%Z!<%9$,$"$jL#J}$,$$$k(B,$B%9%k!<%Q%9!*(B
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("$B6/0z$J%9%k!<%Q%9!*!*(B");
	      }
	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      15);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
  	    else if(super.infoManager.getPosOfReceiver(absGoalDir,100,20,recXY)
                    < 1.7){
              if(super.infoManager.getStamina() > staminaThreshold){
                //  $B%U%)%m!<$G$-$kL#J}$,$$$F(B,$B%9%?%_%J$,$"$k>l9g(B,1-2$B%j%?!<%s!*(B
                super.infoManager.setSelfStrategy("1-2Return","plan");
	      }
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else {
              //  $B;vA0$K$5$,$7$?%*!<%W%s%9%Z!<%9$K6/0z$K%I%j%V%k(B
	      if(Math.abs(Generic.normDir(openDirNS.getNS() - 
                                    super.infoManager.getBodyFaceDir())) < 3){
	         if(Math.abs(ballDir) < 100){
	           if(ballDir >= 0)
		     super.player.dribble(100,70);
	           else
		     super.player.dribble(100,-70);
                   // System.out.println("$B6/0z$J%I%j%V%k!*!*(B");
	         }
                 else {
	           if(ballDir >= 0)
		     super.player.turnBall(90);
	           else
		   super.player.turnBall(-90);
	         }
	      }
              else {
	        super.player.turn(super.infoManager.
                                   getMoment(Generic.normDir(openDirNS.getNS()
                                   - super.infoManager.getBodyFaceDir())));
	        super.player.lookAtBall();
	      }
              sayBallString = Generic.NS_TRUE;
	    }
	  }
	}
        else { //  $B$=$NB>$N@oN,(B        
          if(freeFlag){  //  $B<~$j$KE($,$$$J$$(B
	    if(goalDist < 18){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      if(super.infoManager.isBallTrap()){
	        super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	        super.player.lookAtBall();
	      }
              else {
	        super.player.trapBall();
	        super.player.lookAtBall();
	      } 
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B$&$*$*$)$*!*!*(B");
	    }
            else if(super.infoManager.
		     getDirOfOpenSpace(absGoalDir,15,7,3,openDirNS) < 1.7){
	      if(Math.abs(Generic.normDir(openDirNS.getNS() - 
                                    super.infoManager.getBodyFaceDir())) < 5){
	        if(Math.abs(ballDir) < 100){
	          if(ballDir >= 0)
	            super.player.dribble(100,70);
	          else
	            super.player.dribble(100,-70);
                  // System.out.println("$B6/0z$J%I%j%V%k!*!*(B");
                  if(super.infoManager.getStamina() > staminaThreshold){
                    //  $B%9%?%_%J$,$"$k>l9g(B,1-2$B%j%?!<%s!*(B
                    super.infoManager.setSelfStrategy("1-2Return","plan");
		  }
	        }
                else {
	          if(ballDir >= 0)
	            super.player.turnBall(90);
	          else
	          super.player.turnBall(-90);
	        }
	      }
              else {
	        super.player.turn(super.infoManager.
                                   getMoment(Generic.normDir(openDirNS.getNS()
                                   - super.infoManager.getBodyFaceDir())));
	        super.player.lookAtBall();
	      }
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B6/0z$J%I%j%V%k!*!*(B");
	    }
  	    else if(super.infoManager.getPosOfReceiver(absGoalDir,45,20,recXY)
                    < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else {
              super.infoManager.getDirOfOpenSpace(absGoalDir,30,15,2,
                                                           openDirNS);
              if(super.infoManager.forwardFW(absGoalDir,90,0) >= 1){
                //  $B%4!<%kJ}8~$K%*!<%W%s%9%Z!<%9$,$"$jL#J}$,$$$k(B,$B%9%k!<%Q%9!*(B
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("$B6/0z$J%9%k!<%Q%9!*!*(B");
	      }
	      super.player.shot(super.infoManager.getX() + 15 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 15 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	  }
          else {  //  $B%U%j!<$8$c$J$$$H$-(B
	    if(goalDist < 20){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("$B%?%$%,!<%7%g%C%H!*!*(B");
	    }
  	    else if(super.infoManager.getPosOfReceiver(absGoalDir,45,20,recXY)
                    < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else if(super.infoManager.getDirOfOpenSpace(absGoalDir,30,15,2,
                                                           openDirNS) < 1.7){
              if(super.infoManager.forwardFW(absGoalDir,90,0) >= 1){
                //  $B%4!<%kJ}8~$K%*!<%W%s%9%Z!<%9$,$"$jL#J}$,$$$k(B,$B%9%k!<%Q%9!*(B
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("$B6/0z$J%9%k!<%Q%9!*!*(B");
	      }
	      super.player.shot(super.infoManager.getX() + 15 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 15 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
  	    else if(super.infoManager.getPosOfReceiver(absGoalDir,100,20,recXY)
                    < 1.7){
              if(super.infoManager.getStamina() > staminaThreshold){
                //  $B%U%)%m!<$G$-$kL#J}$,$$$F(B,$B%9%?%_%J$,$"$k>l9g(B,1-2$B%j%?!<%s!*(B
                super.infoManager.setSelfStrategy("1-2Return","plan");
	      }
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else {
              //  $B;vA0$K$5$,$7$?%*!<%W%s%9%Z!<%9$K6/0z$K%I%j%V%k(B
	      if(Math.abs(Generic.normDir(openDirNS.getNS() - 
                                    super.infoManager.getBodyFaceDir())) < 3){
	        if(Math.abs(ballDir) < 100){
	          if(ballDir >= 0)
	            super.player.dribble(100,70);
	          else
	            super.player.dribble(100,-70);
                  // System.out.println("$B6/0z$J%I%j%V%k!*!*(B");
                  if(super.infoManager.getStamina() > staminaThreshold){
                    //  $B%9%?%_%J$,$"$k>l9g(B,1-2$B%j%?!<%s!*(B
                    super.infoManager.setSelfStrategy("1-2Return","plan");
		  }
	        }
                else {
	          if(ballDir >= 0)
	            super.player.turnBall(90);
	          else
	          super.player.turnBall(-90);
	        }
	      }
              else {
	        super.player.turn(super.infoManager.
                                   getMoment(Generic.normDir(openDirNS.getNS()
                                   - super.infoManager.getBodyFaceDir())));
	        super.player.lookAtBall();
	      }
              sayBallString = Generic.NS_TRUE;
	    }
	  }
	}
      }
      else {        //  $B%\!<%k$r;}$C$F$$$J$$>l9g(B
        if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  $B%+%&%s%?!<$r:NMQCf$K%\!<%k%/%j%"$rJ9$$$?$i(B,$B%+%&%s%?!<GK4~(B
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter") &&
                                        super.infoManager.getX() > ballX){
            //  $B%\!<%k$h$jA0$K$$$F%+%&%s%?!<$N9g?^$rJ9$$$?$i(B,$B%+%&%s%?!<:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"show");
            this.lastSearchTime = currentTime;
	  }
          else if(strategyPlan.equals("throughPass") &&
                                        super.infoManager.getX() > ballX){
            //  $B%\!<%k$h$jA0$K$$$F%9%k!<%Q%9$N9g?^$rJ9$$$?$i(B,$B%9%k!<%Q%9:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"show");
            this.lastSearchTime = currentTime;
	  }
          else if(strategyPlan.equals("1-2Return") && 
                          freeFlag && distBallPos <15 &&
            super.infoManager.hasPlayerInfo(super.infoManager.getSPlanner())){ 
            //  1-2$B%j%?!<%s$NDs<(<T$,3NG'$G$-$F$$$F%U%j!<$N>l9g(B,1-2$B%j%?!<%s:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"reply");
            sayStrategy = Generic.NS_TRUE;  //  $BJVEz(B
	  }
        }
        else { //  $B<+8J$N@oN,$N:FI>2A(B
          if(currentStrategy.equals("counter") && ballX > 0
                                   &&  super.infoManager.getX() > ballX){ 
            //  $B%\!<%k$h$jA0$K$$$k>l9g(B,$B%+%&%s%?!<7QB3(B
            super.infoManager.setSelfStrategy(currentStrategy,"show");
	  }
          else if(currentStrategy.equals("throughPass") &&
                          (super.infoManager.getX() < ballX || ballX < 0)){
            //  $B%\!<%k$h$j8e$m$K$$$k$^$?$O96$a$i$l$F$$$k>l9g(B,$B%9%k!<%Q%9GK4~(B
            super.infoManager.setSelfStrategy("private","show");
	  }
        }
	
        selfStrategy = super.infoManager.getSelfStrategy();

        if(selfStrategy.equals("counter")){  //  $B6(D4@oN,(B"counter"
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 25){
	    if(ballX > 15  && Math.abs(ballY) < 24 && distBallPos < 15)
              super.player.chaseBall(100);
            else if(super.infoManager.getStamina() > staminaThreshold)
              if(super.infoManager.needToChaseBall(1) && ballX > 0)
	        super.player.chaseBall(100);
              else
	        super.player.chaseBall(dashPower * 1.5);
	    else
	      super.player.chaseBall(dashPower * 0.5);
	    super.player.lookAtBall();
          }
          else {
	    if(currentTime - this.lastSearchTime > 30){  //  NS99$B$N%P%0!)(B
	      double headAngle = super.infoManager.getHeadAngle();
	      if(this.searchStep != 0){
                super.player.turn2(45);
                this.searchStep--;
              }
              else {
                super.player.turn2(45);
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
            }
            else if(super.infoManager.getX() <  offSideLine - 1){
              if(super.infoManager.getStamina() > staminaThreshold)
                dashPower *= 1.5;
	      super.infoManager.getDirOfOpenSpace(0,15,10,2,openDirNS);
	      super.player.goToThePos(super.infoManager.getX() + 10 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 10 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
                              dashPower);
	      super.player.lookAtBall();
	    }
            else {
              //  $B%*%U%5%$%I%i%$%s$r1[$($?$i(B,$B%+%&%s%?!<GK4~(B
              super.infoManager.setSelfStrategy("private","show");
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();    
	    }
          }
	}
        else if(selfStrategy.equals("throughPass")){ //  $B6(D4@oN,(B"throughPass"
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 25){
	    if(ballX > Generic.FIELD_X_WIDTH * 0.25  && Math.abs(ballY) < 24
	                                             && distBallPos < 15)
              super.player.chaseBall(100);
            else if(super.infoManager.getStamina() > staminaThreshold)
              if(super.infoManager.needToChaseBall(1) && ballX > 0)
	        super.player.chaseBall(100);
              else
	        super.player.chaseBall(dashPower);
	    else
	      super.player.chaseBall(dashPower * 0.5);
	    super.player.lookAtBall();
          }
          else {
	    if(currentTime - this.lastSearchTime > 30){  //  NS99$B$N%P%0!)(B
	      double headAngle = super.infoManager.getHeadAngle();
	      if(this.searchStep != 0){
                super.player.turn2(45);
                this.searchStep--;
              }
              else {
                super.player.turn2(45);
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
            }
            else if(super.infoManager.getX() <  offSideLine - 1){
	      super.infoManager.getDirOfOpenSpace(absGoalDir,15,10,2,
                                                                  openDirNS);
	      super.player.goToThePos(super.infoManager.getX() + 10 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 10 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
                              dashPower * 2);
	      super.player.lookAtBall();
	    }
            else {
              //  $B%*%U%5%$%I%i%$%s$r1[$($?$i(B,$B%9%k!<%Q%9GK4~(B
              super.infoManager.setSelfStrategy("private","show");
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();    
	    }
          }
	}
        else if(selfStrategy.equals("1-2Return")){ //  $B6(D4@oN,(B"1-2Return"
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 22){
            if(ballX > Generic.FIELD_X_WIDTH * 0.25  && Math.abs(ballY) < 20
                                                    &&  distBallPos < 10)
              super.player.chaseBall(100);
            else if(super.infoManager.getStamina() > staminaThreshold){
              if(super.infoManager.needToChaseBall(1) && ballX > 0)
	        super.player.chaseBall(100);
              else
	        super.player.chaseBall(dashPower);
	    }
	    else {
              super.player.chaseBall(dashPower * 0.5);
	    }
            super.player.lookAtBall();
          }
          else {
            if(currentTime - this.lastSearchTime > 30){  //  NS99$B$N%P%0!)(B
                double headAngle = super.infoManager.getHeadAngle();
              if(this.searchStep != 0){
                super.player.turn2(45);
                this.searchStep--;
              }
              else {
                super.player.turn2(45);
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
	    }
            else if(super.infoManager.getSelfSType().equals("plan")){
              super.infoManager.getDirOfOpenSpace(absGoalDir,15,5,2,
                                                                  openDirNS);
              super.player.goToThePos(super.infoManager.getX() + 5 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 5 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
                              dashPower * 2);
              super.player.lookAtBall();
	    }
            else {
              super.infoManager.getDirOfOpenSpace(Math.abs(ballDir),60,5,2,
                                                                  openDirNS);
              super.player.goToThePos(super.infoManager.getX() + 5 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 5 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
                              dashPower * 2);
              super.player.lookAtBall();
	    }
	  }
	}
        else { //  $B$=$NB>$N@oN,(B
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 22){
	    if(ballX > Generic.FIELD_X_WIDTH * 0.25 && Math.abs(ballY) < 20
	                                            && distBallPos < 10)
              super.player.chaseBall(100);
            else if(super.infoManager.getStamina() > staminaThreshold
                                                  || currentTime > 5500)
              if(super.infoManager.needToChaseBall(1) && ballX > 0)
	         super.player.chaseBall(100);
              else 
                 super.player.chaseBall(dashPower * 1.5);
	    else
	      super.player.chaseBall(dashPower * 0.5);
	    super.player.lookAtBall();
          }
          else {
	    if(currentTime - this.lastSearchTime > 30){  //  NS99$B$N%P%0!)(B
	      double headAngle = super.infoManager.getHeadAngle();
	      if(this.searchStep != 0){
                super.player.turn2(45);
                this.searchStep--;
              }
              else {
                super.player.turn2(45);
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
            }
            else if(!super.infoManager.onThePos(super.tmpXY.getPointX(),
                                                        super.stdY)){
	      // $B%9%?%_%J$,>/$J$$$H$-$OL5M}$7$FLa$i$J$$(B
	      if (super.infoManager.getStamina() < 
		   Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR){
		// $B%\!<%k$r$_$h(B
		//System.out.println("$BBNNO2sI|Cf(B : $B%W%l!<%dHV9f(B,$B%9%?%_%J(B " + super.player.getUniformNum() +"," + super.infoManager.getStamina());  
		super.player.turn(super.infoManager.getMoment(ballDir));
		super.player.lookAtBall();  
	      }
	      else{
		super.player.goToThePos(super.tmpXY.getPointX(),
					super.stdY,dashPower);
		super.player.lookAtBall();
	      }
	    }
            else {
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();    
	    }
          }
	}
      }
    } else {  //  $B%\!<%k>pJs$r$b$C$F$$$J$$>l9g(B
      if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
        String strategyPlan = super.infoManager.getStrategyPlan();
        int planner = super.infoManager.getSPlanner();

        if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
          //  $B%+%&%s%?!<$r:NMQCf$K%\!<%k%/%j%"$rJ9$$$?$i(B,$B%+%&%s%?!<GK4~(B
          super.infoManager.setSelfStrategy("private","show");
        }
        else if(strategyPlan.equals("counter")){
         //  $B9g?^$,$"$C$?>l9g(B,$B%+%&%s%?!<:NMQ(B
          super.infoManager.setSelfStrategy(strategyPlan,"show");
	}
      }
      else { //  $B<+8J$N@oN,$N:FI>2A(B
      }

      super.player.turn2(super.infoManager.searchBall());
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
if(!currentStrategy.equals(super.infoManager.getSelfStrategy()))
  System.out.println("[" + currentTime + "] (" + super.player.getUniformNum()
                    + ") " + super.infoManager.getStrategyString());

System.out.println("[" + currentTime + "] (" + super.player.getUniformNum()
                    + ") <" + super.infoManager.getStrategyCertainty() + "> " 
                    + super.infoManager.getStrategyString());
*/
  }
}
