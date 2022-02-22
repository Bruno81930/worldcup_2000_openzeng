// DefensiveHalfSR.java

package intercepterP;

import genericP.*;
import brainP.*;
import playerP.*;


/********************************************************/
/*         DefensiveHalfSR    守備的MFエージェント      */
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

    if(super.infoManager.hasBallInfo()){  //  ボール情報をもっている
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

      if(super.infoManager.isKickable()){ //  ボールを持っている
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&  pointDiff < 0 && ballX < 0){ 
            //  チームが負けていて攻められている場合,ボールクリア採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
            super.infoManager.setSelfStrategy("private","show");
          }
          else if(strategyPlan.equals("counter") && 
                                   super.infoManager.forwardFW(0,45,5) >= 1){ 
            //  前方にFWがいる場合,カウンター採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("ChangeTop") && 
                                      super.infoManager.parseSideChange()){ 
            //  ボールのあるフィールドに敵が集中している場合,サイドチェンジ採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("ballClear")
                                              && pointDiff < 0 && ballX < 0){ 
            //  チームが負けていて攻められている場合,ボールクリア継続
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else if(currentStrategy.equals("counter") && 
                                   super.infoManager.forwardFW(0,40,5) >= 1){ 
            //  前方にFWが2人以上いる場合,カウンター継続
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else if(currentStrategy.equals("sideChange") && 
                                      super.infoManager.parseSideChange()){ 
            //  ボールのあるフィールドに敵が集中している場合,サイドチェンジ採用
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else { //  その他の戦略を提案
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25 && pointDiff < 0){
              //  フィールドの1/4こえていて負けている場合,ボールクリア提案
              super.infoManager.setSelfStrategy("ballClear","plan");
	    }
	  }
        }

        selfStrategy = super.infoManager.getSelfStrategy();
        sayStrategy = Generic.NS_TRUE;  //  発言権あり

        if(selfStrategy.equals("ballClear")){  //  協調戦略"ballClear"
          if((super.infoManager
                        .getDirOfOpenSpace(0,30,25,5,openDirNS) < 1.7) && 
                  (super.infoManager.forwardFW(0,40,5) > 1)){
            //  前方に味方がいてオープンスペースがある場合,カウンター！
            super.infoManager.setSelfStrategy("counter","plan");
	    super.player.shot(super.infoManager.getX() + 30 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                 //  とにかく前に強く蹴れ！
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		       .getPosOfReceiver(0,45,35,recXY) < 1.5){
                                      //  すこしシビアにしてみた(1.7 -> 1.5)
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
			      10);                 //  とにかく前に強く蹴れ！
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
        else if(selfStrategy.equals("counter")){  //  協調戦略"counter"
	  if(goalDist < 35){
	    super.infoManager.getDirOfOpenSpace(absGoalDir,10,15,2,openDirNS);
              //  ゴール方向のオープンスペースへショット！スルーパス！
              super.infoManager.setSelfStrategy("throughPass","plan");

	    super.player.shot(super.infoManager.getX() + 35 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 35 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                //  とにかく前に強く蹴れ！
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager
		     .getDirOfOpenSpace(0,30,25,5,openDirNS) < 1.7){
            if(super.infoManager.forwardFW(openDirNS.getNS(),45,5) >= 1){
              //  前方のオープンスペースへショット！スルーパス！
              super.infoManager.setSelfStrategy("throughPass","plan");
	    }
	    super.player.shot(super.infoManager.getX() + 30 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                 //  とにかく前に強く蹴れ！
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
        else if(selfStrategy.equals("sideChange")){  //  協調戦略"sideChange"
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
        else { //  その他の戦略
	  if(goalDist < 35){
	    super.infoManager.getDirOfOpenSpace(absGoalDir,10,15,2,openDirNS);
              //  ゴール方向のオープンスペースへショット！スルーパス！
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
      else {        //  ボールを持っていない場合
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&
                                               pointDiff < 0 && ballX < 0){ 
            //  チームが負けていて攻められている場合,ボールクリア採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
          else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter") && 
                  super.infoManager.forwardFW(0,45,5) >= 1 &&
                  this.infoManager.getX() > ballX){ 
            //  前方にFWがいて自分がボールより前方にいる場合,カウンター採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
          else if(strategyPlan.equals("sideChange") && 
                                       ballY * this.infoManager.getY() >= 0){ 
            //  ボールと同じサイドにいる場合,サイドチェンジ採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("ballClear")
                                              && pointDiff < 0 && ballX < 0){ 
            //  チームが負けていて攻められている場合,ボールクリア継続
            super.infoManager.setSelfStrategy(currentStrategy,"show");
	  }
          else { //  その他の戦略を採用
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25 && pointDiff < 0)
              //  フィールドの1/4こえていて負けている場合,ボールクリア採用
              super.infoManager.setSelfStrategy("ballClear","show");
	  }
        }
	
        selfStrategy = super.infoManager.getSelfStrategy();

        if(selfStrategy.equals("counter")){  //  協調戦略"counter"
          if(super.infoManager.needToChaseBall(2) && predDistBallPos < 20){
            if(selfStrategy.equals("counter") && 
	       ballX < - Generic.FIELD_X_WIDTH * 0.25){
              //  フィールドの1/4こえてる場合,カウンター破棄
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
        else if(selfStrategy.equals("sideChange")){  //  協調戦略"sideChange"
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
        else { //  その他の戦略,"ballClear"も同じ
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
    else {  //  ボール情報をもっていない場合
      if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
        String strategyPlan = super.infoManager.getStrategyPlan();
        int planner = super.infoManager.getSPlanner();

        if(strategyPlan.equals("ballClear") && pointDiff < 0){ 
          //  チームが負けている場合,ボールクリア採用
          super.infoManager.setSelfStrategy(strategyPlan,"show");
	}
        else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
          //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
          super.infoManager.setSelfStrategy("private","show");
        }
      }
      else { //  自己の戦略の再評価
        if(currentStrategy.equals("ballClear") && pointDiff < 0){ 
          //  チームが負けている場合,ボールクリア継続
          super.infoManager.setSelfStrategy(currentStrategy,"show");
	}
      }

      super.player.turn(45);
    }

    //  自分の持つ戦略とボール情報を流す
    if(sayBallString || sayStrategy){
      StringBuffer str = new StringBuffer("(");  // sprintf!!

      //  ボール情報はshotの後のみ流す
      if(sayBallString)
        str.append(super.infoManager.getBallString());
      if(sayBallString && sayStrategy)
        str.append(" ");
      //  自分の戦略をブロードキャスト
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






