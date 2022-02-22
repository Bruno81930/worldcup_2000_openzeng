// DefensiveHalfSR.java

package intercepterP;

import genericP.*;
import brainP.*;
import playerP.*;


/********************************************************/
/*         DefensiveHalfSR    $B<iHwE*(BMF$B%(!<%8%'%s%H(B      */
/********************************************************/

public class DefensiveHalfSR extends Brain{

  static int lastSearchTime = 0;    
  static int searchStep = 3;

  String selfStrategy;

/***************************************************************************/

  public DefensiveHalfSR(Player p){
    super(p);
  }

/***************************************************************************/

  public void playOn(){
    int currentTime = super.infoManager.getBodyTime();
    boolean markFlag = Generic.NS_FALSE;
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    String currentStrategy = super.infoManager.getSelfStrategy();
    // double strategyCertainty = super.infoManager.getStrategyCertainty();
    double dashPower = Generic.STAMINA_INC_MAX;
    NSobject openDirNS = new NSobject();     // NSobject!!
    Point recXY = new Point();               // Point!!
    Point oppXY = new Point();               // Point!!
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(super.infoManager.hasBallInfo()){  //  $B%\!<%k>pJs$r$b$C$F$$$k(B
      double ballX = super.infoManager.getBallX();
      double ballY = super.infoManager.getBallY();
      double predBallX = super.infoManager.getPredBallX(20);
      double predBallY = super.infoManager.getPredBallY(20);
      double ballDir = super.infoManager.getBallDir();
      double predDistBallPos = Generic.DIST(predBallX,predBallY,
                           super.tmpXY.getPointX(),super.tmpXY.getPointY());
      double distBallPos = Generic.DIST(ballX,ballY,
                           super.tmpXY.getPointX(),super.tmpXY.getPointY());
      double absGoalDir = super.infoManager.getAbsGoalDir();
      double goalDist = super.infoManager.getGoalDist();

      if(ballX < 0)
	dashPower = Generic.STAMINA_INC_MAX * 2;

      if(ballX < super.stdX + 5 && 
                                ballX - 13 > - Generic.FIELD_X_WIDTH * 0.5)
	tmpXY.setPointX(ballX - 2);
      else if(ballX > 10)
        tmpXY.setPointX(super.stdX + 5);
      else
        tmpXY.setPointX(super.stdX);

      /*
      if(ballX < super.stdX - 10)
	super.tmpXY.setPointX(ballX + 10);
      else if(ballX > Generic.FIELD_X_WIDTH * 0.25)
	super.tmpXY.setPointX(super.stdX + 7);
      else
	super.tmpXY.setPointX(super.stdX);
      */

      if(super.infoManager.isKickable()){ //  $B%\!<%k$r;}$C$F$$$k(B
        if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&  pointDiff < 0 && ballX < 0){ 
            //  $B%A!<%`$,Ii$1$F$$$F96$a$i$l$F$$$k>l9g(B,$B%\!<%k%/%j%":NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  $B%+%&%s%?!<$r:NMQCf$K%\!<%k%/%j%"$rJ9$$$?$i(B,$B%+%&%s%?!<GK4~(B
            super.infoManager.setSelfStrategy("private","show");
          }
          else if(strategyPlan.equals("counter") && 
                                   super.infoManager.forwardFW(0,45,5) >= 1){ 
            //  $BA0J}$K(BFW$B$,$$$k>l9g(B,$B%+%&%s%?!<:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("ChangeTop") && 
                                      super.infoManager.parseSideChange()){ 
            //  $B%\!<%k$N$"$k%U%#!<%k%I$KE($,=8Cf$7$F$$$k>l9g(B,$B%5%$%I%A%'%s%8:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
        }
        else { //  $B<+8J$N@oN,$N:FI>2A(B
          if(currentStrategy.equals("ballClear")
                                              && pointDiff < 0 && ballX < 0){ 
            //  $B%A!<%`$,Ii$1$F$$$F96$a$i$l$F$$$k>l9g(B,$B%\!<%k%/%j%"7QB3(B
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else if(currentStrategy.equals("counter") && 
                                   super.infoManager.forwardFW(0,40,5) >= 1){ 
            //  $BA0J}$K(BFW$B$,(B2$B?M0J>e$$$k>l9g(B,$B%+%&%s%?!<7QB3(B
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else if(currentStrategy.equals("sideChange") && 
                                      super.infoManager.parseSideChange()){ 
            //  $B%\!<%k$N$"$k%U%#!<%k%I$KE($,=8Cf$7$F$$$k>l9g(B,$B%5%$%I%A%'%s%8:NMQ(B
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else { //  $B$=$NB>$N@oN,$rDs0F(B
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25 && pointDiff < 0){
              //  $B%U%#!<%k%I$N(B1/4$B$3$($F$$$FIi$1$F$$$k>l9g(B,$B%\!<%k%/%j%"Ds0F(B
              super.infoManager.setSelfStrategy("ballClear","plan");
	    }
	  }
        }

        selfStrategy = super.infoManager.getSelfStrategy();
        sayStrategy = Generic.NS_TRUE;  //  $BH/8@8"$"$j(B

        if(selfStrategy.equals("ballClear")){  //  $B6(D4@oN,(B"ballClear"
          if((super.infoManager
                        .getDirOfOpenSpace(0,30,25,5,openDirNS) < 1.7) && 
                  (super.infoManager.forwardFW(0,40,5) > 1)){
            //  $BA0J}$KL#J}$,$$$F%*!<%W%s%9%Z!<%9$,$"$k>l9g(B,$B%+%&%s%?!<!*(B
            super.infoManager.setSelfStrategy("counter","plan");
	    super.player.shot(super.infoManager.getX() + 30 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getPosOfReceiver(0,45,35,recXY) < 1.5){
                                      //  $B$9$3$7%7%S%"$K$7$F$_$?(B(1.7 -> 1.5)
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		     .getDirOfClearSpace(absGoalDir,30,25,5,openDirNS) < 1.7){
	    super.player.shot(super.infoManager.getX() + 30 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
	    super.infoManager.getDirOfClearSpace(absGoalDir,30,15,5,openDirNS);
	    if(Math.abs(Generic.normDir(openDirNS.getNS()
                                  - super.infoManager.getBodyFaceDir())) < 3){
	      if(Math.abs(ballDir) < 100){
		if(ballDir >= 0)
	          super.player.dribble(100,70);
	        else
	          super.player.dribble(100,-70);
	      }
              else {
		if(ballDir >= 0)
		  super.player.turnBall(90);
		else
		  super.player.turnBall(-90);
	      }
	    }
            else {
	      super.player.turn(super.infoManager
			  .getMoment(Generic.normDir(openDirNS.getNS()
                                     - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
        }
        else if(selfStrategy.equals("counter")){  //  $B6(D4@oN,(B"counter"
	  if(goalDist < 35){
	    super.infoManager.getDirOfOpenSpace(absGoalDir,10,15,2,openDirNS);
              //  $B%4!<%kJ}8~$N%*!<%W%s%9%Z!<%9$X%7%g%C%H!*%9%k!<%Q%9!*(B
              super.infoManager.setSelfStrategy("throughPass","plan");

	    super.player.shot(super.infoManager.getX() + 35 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 35 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		     .getDirOfOpenSpace(0,30,25,5,openDirNS) < 1.7){
            if(super.infoManager.forwardFW(openDirNS.getNS(),45,5) >= 1){
              //  $BA0J}$N%*!<%W%s%9%Z!<%9$X%7%g%C%H!*%9%k!<%Q%9!*(B
              super.infoManager.setSelfStrategy("throughPass","plan");
	    }
	    super.player.shot(super.infoManager.getX() + 30 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                 //  $B$H$K$+$/A0$K6/$/=3$l!*(B
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getPosOfReceiver(absGoalDir,45,35,recXY) < 1.7){
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else if(super.infoManager
		      .getDirOfOpenSpace(absGoalDir,30,15,5,openDirNS) < 1.7){
	    if(Math.abs(Generic.normDir(openDirNS.getNS()
                                  - super.infoManager.getBodyFaceDir())) < 3){
	      if(Math.abs(ballDir) < 100){
		if(ballDir >= 0)
	          super.player.dribble(100,70);
	        else
	          super.player.dribble(100,-70);
	      }
              else {
		if(ballDir >= 0)
		  super.player.turnBall(90);
		else
		  super.player.turnBall(-90);
	      }
	    }
            else {
	      super.player.turn(super.infoManager
			  .getMoment(Generic.normDir(openDirNS.getNS()
                                     - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
	  else {
            super.infoManager.getDirOfOpenSpace(absGoalDir,30,20,5,openDirNS);
	    super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
        }
        else if(selfStrategy.equals("sideChange")){  //  $B6(D4@oN,(B"sideChange"
          if(goalDist < 35){
	    super.infoManager.getDirOfOpenSpace(absGoalDir,10,15,2,openDirNS);
	    super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getPosOfReceiver(absGoalDir,30,35,recXY) < 1.7){
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getSideChangeReceiver(absGoalDir,90,35,recXY) < 1.7){
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		     .getDirOfChangeSpace(absGoalDir,45,25,5,openDirNS) < 1.7){
	    super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
	    super.infoManager.getDirOfOpenSpace(absGoalDir,30,15,5,openDirNS);
		
	    if(Math.abs(Generic.normDir(openDirNS.getNS()
                                  - super.infoManager.getBodyFaceDir())) < 3){
	      if(Math.abs(ballDir) < 100){
		if(ballDir >= 0)
	          super.player.dribble(100,70);
	        else
	          super.player.dribble(100,-70);
	      }
              else {
		if(ballDir >= 0)
		  super.player.turnBall(90);
		else
		  super.player.turnBall(-90);
	      }
	    }
            else {
	      super.player.turn(super.infoManager
			  .getMoment(Generic.normDir(openDirNS.getNS()
                                     - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
	}
        else { //  $B$=$NB>$N@oN,(B
	  if(goalDist < 35){
	    super.infoManager.getDirOfOpenSpace(absGoalDir,10,15,2,openDirNS);
              //  $B%4!<%kJ}8~$N%*!<%W%s%9%Z!<%9$X%7%g%C%H!*%9%k!<%Q%9!*(B
              super.infoManager.setSelfStrategy("throughPass","plan");

	    super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getPosOfReceiver(absGoalDir,45,35,recXY) < 1.7){
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getDirOfOpenSpace(absGoalDir,30,25,5,openDirNS) < 1.7){
	    super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
	    super.infoManager.getDirOfOpenSpace(absGoalDir,30,15,5,openDirNS);
		
	    if(Math.abs(Generic.normDir(openDirNS.getNS()
                                  - super.infoManager.getBodyFaceDir())) < 3){
	      if(Math.abs(ballDir) < 100){
		if(ballDir >= 0)
	          super.player.dribble(100,70);
	        else
	          super.player.dribble(100,-70);
	      }
              else {
		if(ballDir >= 0)
		  super.player.turnBall(90);
		else
		  super.player.turnBall(-90);
	      }
	    }
            else {
	      super.player.turn(super.infoManager
			  .getMoment(Generic.normDir(openDirNS.getNS()
                                     - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
	}
      }
      else {        //  $B%\!<%k$r;}$C$F$$$J$$>l9g(B
        if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&
                                               pointDiff < 0 && ballX < 0){ 
            //  $B%A!<%`$,Ii$1$F$$$F96$a$i$l$F$$$k>l9g(B,$B%\!<%k%/%j%":NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
          else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  $B%+%&%s%?!<$r:NMQCf$K%\!<%k%/%j%"$rJ9$$$?$i(B,$B%+%&%s%?!<GK4~(B
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter") && 
                  super.infoManager.forwardFW(0,45,5) >= 1 &&
                  this.infoManager.getX() > ballX){ 
            //  $BA0J}$K(BFW$B$,$$$F<+J,$,%\!<%k$h$jA0J}$K$$$k>l9g(B,$B%+%&%s%?!<:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
          else if(strategyPlan.equals("sideChange") && 
                                       ballY * this.infoManager.getY() >= 0){ 
            //  $B%\!<%k$HF1$8%5%$%I$K$$$k>l9g(B,$B%5%$%I%A%'%s%8:NMQ(B
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
        }
        else { //  $B<+8J$N@oN,$N:FI>2A(B
          if(currentStrategy.equals("ballClear")
                                              && pointDiff < 0 && ballX < 0){ 
            //  $B%A!<%`$,Ii$1$F$$$F96$a$i$l$F$$$k>l9g(B,$B%\!<%k%/%j%"7QB3(B
            super.infoManager.setSelfStrategy(currentStrategy,"show");
	  }
          else { //  $B$=$NB>$N@oN,$r:NMQ(B
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25 && pointDiff < 0)
              //  $B%U%#!<%k%I$N(B1/4$B$3$($F$$$FIi$1$F$$$k>l9g(B,$B%\!<%k%/%j%":NMQ(B
              super.infoManager.setSelfStrategy("ballClear","show");
	  }
        }
	
        selfStrategy = super.infoManager.getSelfStrategy();

        if(selfStrategy.equals("counter")){  //  $B6(D4@oN,(B"counter"
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 20){
            if(selfStrategy.equals("counter") && 
	       ballX < - Generic.FIELD_X_WIDTH * 0.25){
              //  $B%U%#!<%k%I$N(B1/4$B$3$($F$k>l9g(B,$B%+%&%s%?!<GK4~(B
              super.infoManager.setSelfStrategy("private","show");
	    }

	    if(super.infoManager.getStamina() > 
                                 Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR)
	      super.player.chaseBall(100);
	    else
	      super.player.chaseBall(Generic.STAMINA_INC_MAX);

	    super.player.lookAtBall();
	  }
          else {
	    if(currentTime - this.lastSearchTime > 30){
	      double headAngle = super.infoManager.getHeadAngle();
	      if(this.searchStep != 0){
                super.player.turn(45);
                this.searchStep--;
              }
              else {
                super.player.turn(45);		    
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
            }
            else if(super.infoManager.getX() < super.stdX){
	      super.player.goToThePos(super.tmpXY.getPointX() + 5,
                                                      super.stdY,dashPower);
              super.player.lookAtBall();
	    }
            else {
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();
            }
	  }
	}
        else if(selfStrategy.equals("sideChange")){  //  $B6(D4@oN,(B"sideChange"
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 22){
	    if(super.infoManager.getStamina() > 
                                 Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR)
	      super.player.chaseBall(100);
	    else
	      super.player.chaseBall(Generic.STAMINA_INC_MAX);

	    super.player.lookAtBall();
	  }
          else {
	    if(currentTime - this.lastSearchTime > 30){
	      double headAngle = super.infoManager.getHeadAngle();
	      if(this.searchStep != 0){
                super.player.turn(45);
                this.searchStep--;
              }
              else {
                super.player.turn(45);		    
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
            }
            else if(!super.infoManager.onThePos(super.tmpXY.getPointX(),
                                                               super.stdY)){
	      super.player.goToThePos(super.tmpXY.getPointX(),
                                                      super.stdY,dashPower);
              super.player.lookAtBall();
	    }
            else {
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();
	    }
	  }
	}
        else { //  $B$=$NB>$N@oN,(B,"ballClear"$B$bF1$8(B
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 24){
	    if(super.infoManager.getStamina() > 
                                 Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR)
	      super.player.chaseBall(100);
	    else
	      super.player.chaseBall(Generic.STAMINA_INC_MAX);

	    super.player.lookAtBall();
	  }
          else {
	    if(currentTime - this.lastSearchTime > 30){
	      double headAngle = super.infoManager.getHeadAngle();
	      if(this.searchStep != 0){
                super.player.turn(45);
                this.searchStep--;
              }
              else {
                super.player.turn(45);		    
                this.lastSearchTime = currentTime;
                this.searchStep = 3;
              }
            }
            else if(!super.infoManager.onThePos(super.tmpXY.getPointX(),
                                                               super.stdY)){
	      super.player.goToThePos(super.tmpXY.getPointX(),
                                                      super.stdY,dashPower);
              super.player.lookAtBall();
	    }
            else {
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();
	    }	    
	  }
	}
      }
    }
    else {  //  $B%\!<%k>pJs$r$b$C$F$$$J$$>l9g(B
      if(super.infoManager.getStrategyFlag()){ //  $B@oN,$NDs<($,$"$C$?(B
        String strategyPlan = super.infoManager.getStrategyPlan();
        int planner = super.infoManager.getSPlanner();

        if(strategyPlan.equals("ballClear") && pointDiff < 0){ 
          //  $B%A!<%`$,Ii$1$F$$$k>l9g(B,$B%\!<%k%/%j%":NMQ(B
          super.infoManager.setSelfStrategy(strategyPlan,"show");
	}
        else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
          //  $B%+%&%s%?!<$r:NMQCf$K%\!<%k%/%j%"$rJ9$$$?$i(B,$B%+%&%s%?!<GK4~(B
          super.infoManager.setSelfStrategy("private","show");
        }
      }
      else { //  $B<+8J$N@oN,$N:FI>2A(B
        if(currentStrategy.equals("ballClear") && pointDiff < 0){ 
          //  $B%A!<%`$,Ii$1$F$$$k>l9g(B,$B%\!<%k%/%j%"7QB3(B
          super.infoManager.setSelfStrategy(currentStrategy,"show");
	}
      }

      super.player.turn(45);
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






