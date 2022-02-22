// DefenderSR.java

package brainP;

import genericP.*;
// import brainP.*;
import playerP.*;


/********************************************************/
/*         DefenderSR    DF エージェント                */
/********************************************************/

public class DefenderSR extends Brain{
  public static final int X_CELL_NUM = 7;
  public static final double X_CELL_WIDTH = ((Generic.FIELD_X_WIDTH / 2.0)
                              / (double)X_CELL_NUM);
  public static final int V_NUM = 8;
  public static final double KICK_REWARD = 1.0;
  public static final int REWARD_INTERVAL = 5;
  public static final double DEC_RATE = 0.8;

  float learningRate;
  int learningNum;
  String tablename;

  static int lastSearchTime = 0;
  static int lastSearchTime4 = 0;
  static int lastKickTime = 0;
  static int searchStep = 3;
  static int searchStep4 = 3;

  String selfStrategy;

/***************************************************************************/

  public DefenderSR (Player p){
    super(p);
  }

/***************************************************************************/

  public void playOn(){   
    int currentTime = super.infoManager.getBodyTime();
    boolean markFlag = Generic.NS_FALSE;
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    boolean emergency;
    String currentStrategy = super.infoManager.getSelfStrategy();
    // double strategyCertainty = super.infoManager.getStrategyCertainty();
    NSobject openDirNS = new NSobject();     // NSobject!!
    Point recXY = new Point();               // Point!!
    Point oppXY = new Point();               // Point!!
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    double staminaThreshold;
    double dashPower;

    // スタミナ管理用。急造仕様のためバランスわるいかも。
    if(currentTime > 5800 && pointDiff < 0){  // ガンバルシステム
      staminaThreshold = 0;
      dashPower = Generic.STAMINA_INC_MAX * 2;
    }
    else if(pointDiff < 0){                  // 負けている
      staminaThreshold = Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR;
      dashPower = Generic.STAMINA_INC_MAX * 1.2;
    }
    else {
      staminaThreshold = Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR + 400;
      dashPower = Generic.STAMINA_INC_MAX;
    }

    if(super.infoManager.getPosOfNearestOpponent(0,180,15,oppXY) < 25)
      markFlag = Generic.NS_TRUE;

    if(super.infoManager.hasBallInfo()){  //  ボール情報をもっている
      double ballX = super.infoManager.getBallX();
      double ballY = super.infoManager.getBallY();
      double predBallX = super.infoManager.getPredBallX(20);
      double predBallY = super.infoManager.getPredBallY(20);
      double ballDir = super.infoManager.getBallDir();
      double absGoalDir = super.infoManager.getAbsGoalDir();

      if(ballX < super.stdX + 10){
        tmpXY.setPointX(ballX - 10);
        tmpXY.setPointY(super.stdY);
        emergency = Generic.NS_FALSE;
        dashPower = Generic.STAMINA_INC_MAX * 1.5;
	if(ballX - 13 < - Generic.FIELD_X_WIDTH * 0.5){
	  tmpXY.setPointX(- Generic.FIELD_X_WIDTH * 0.5 + 7);
          if(Math.abs(ballY) < 20){
            tmpXY.setPointY(super.stdY - super.stdY / 3);
            emergency = Generic.NS_TRUE;
            dashPower = Generic.STAMINA_INC_MAX * 2.5;
	  }
	}
      }
      /* イマイチバランスが悪いので削除
      else if(ballX > 5){
        tmpXY.setPointX(super.stdX + 5);
        tmpXY.setPointY(super.stdY);
	emergency = Generic.NS_FALSE;
      }
      */
      else{
        tmpXY.setPointX(super.stdX);
        tmpXY.setPointY(super.stdY);
	emergency = Generic.NS_FALSE;
      }

      // こういうはコードはどうでしょう？ from ごしょ
      if (super.infoManager.getStamina() > Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR){
	if (ballY > 22){
	  tmpXY.setPointY(super.stdY + 10);
	}
	else if (ballY > 12){
	  tmpXY.setPointY(super.stdY + ballY - 12);
	}
	
	if (ballY < -22){
	  tmpXY.setPointY(super.stdY - 10);
	}
	else if (ballY < -12){
	  tmpXY.setPointY(super.stdY + ballY + 12);
	}
      }

      /*
      double distBallPos = Generic.DIST(ballX,ballY,
                           super.infoManager.getX(),super.infoManager.getY());
      */
      double predDistBallPos = Generic.DIST(predBallX,predBallY,
                             super.tmpXY.getPointX(),super.tmpXY.getPointY());

      /*
      // バグとり用
      if(super.tmpXY.getPointX() < - Generic.FIELD_X_WIDTH * 0.5 + 1)
        super.tmpXY.setPointX(- Generic.FIELD_X_WIDTH * 0.5 + 1.5);
      */

      if(super.infoManager.isKickable()){ //  ボールを持っている
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") && 
                       ((pointDiff <= 0 && ballX < 0) || 
                       ballX < - Generic.FIELD_X_WIDTH * 0.25)){ 
            //  チームが同点または負けていて攻められている,または
            //  フィールドの1/4こえてる場合,ボールクリア採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter") && 
                                  super.infoManager.forwardFW(0,45,10) >= 1){ 
            //  前方にFWがいる場合,カウンター採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("ballClear")
                                              && pointDiff <= 0 && ballX < 0){ 
            //  チームが同点または負けていて攻められている場合,ボールクリア継続
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else { //  その他の戦略を提案
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25)
              //  フィールドの1/4こえてる場合,ボールクリア提案
              super.infoManager.setSelfStrategy("ballClear","plan");
	  }
        }

        selfStrategy = super.infoManager.getSelfStrategy();
        sayStrategy = Generic.NS_TRUE;  //  発言権あり

        if(selfStrategy.equals("ballClear")){  //  協調戦略"ballClear"
	  if(super.infoManager.getPosOfReceiver(0,45,35,recXY) < 1.5){
                                      //  すこしシビアにしてみた(1.7 -> 1.5)
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
          }
          else if(super.infoManager.
                  getDirOfClearSpace(absGoalDir,45,25,5,openDirNS) < 1.7){
            super.player.shot(super.infoManager.getX() + 30 * 
                              Math.cos(Generic.digToRad(openDirNS.getNS())),
      		              super.infoManager.getY() + 30 *
                              Math.sin(Generic.digToRad(openDirNS.getNS())),
                              10);                 //  とにかく前に強く蹴れ！
            super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
          }
          else {
	    super.infoManager.getDirOfClearSpace(0,45,15,5,openDirNS);
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
	      super.player.turn(super.infoManager.
                                getMoment(Generic.normDir(openDirNS.getNS()
                                - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
        }
        if(selfStrategy.equals("counter")){  //  協調戦略"counter"
          if(super.infoManager.
                  getDirOfOpenSpace(0,30,25,5,openDirNS) < 1.7){
            super.player.shot(super.infoManager.getX() + 30 * 
                              Math.cos(Generic.digToRad(openDirNS.getNS())),
      		              super.infoManager.getY() + 30 *
                              Math.sin(Generic.digToRad(openDirNS.getNS())),
                              10);                 //  とにかく前に強く蹴れ！
            super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
          }
	  else if(super.infoManager.getPosOfReceiver(0,45,35,recXY) < 1.7){
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
          }
          else {
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25)
              //  パス相手がいなくてフィールドの1/4こえてる場合,カウンター破棄
              super.infoManager.setSelfStrategy("private","show");

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
	      super.player.turn(super.infoManager.
                                getMoment(Generic.normDir(openDirNS.getNS()
                                - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
        }
        else { //  その他の戦略
	  if(super.infoManager.getPosOfReceiver(0,45,35,recXY) < 1.7){
            super.infoManager.getPosOfOpponentsForward(oppXY);
            if(super.infoManager.getX() < oppXY.getPointX()){
              //  自分が敵FWより前方にいるとき,カウンター提案
              super.infoManager.setSelfStrategy("counter","plan");
	    }

	    super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    super.player.lookAtBall();

            sayBallString = Generic.NS_TRUE;
	  }
	  else if(super.infoManager.
		    getDirOfOpenSpace(absGoalDir,45,25,5,openDirNS) < 1.7){
	    super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 25
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			       10);
	    super.player.lookAtBall();

            sayBallString = Generic.NS_TRUE;
	  }
          else {
	    super.infoManager.getDirOfOpenSpace(absGoalDir,45,15,5,openDirNS);
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
	      super.player.turn(super.infoManager.
                                getMoment(Generic.normDir(openDirNS.getNS()
                                - super.infoManager.getBodyFaceDir())));
	      super.player.lookAtBall();
	    }
            sayBallString = Generic.NS_TRUE;
	  }
	}

        lastKickTime = currentTime;
      }
      else {        //  ボールを持っていない場合
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") && 
              (pointDiff <= 0 && ballX < 0 ||
               ballX < - Generic.FIELD_X_WIDTH * 0.25)){ 
            //  チームが同点または負けていて攻められている,または
            //  フィールドの1/4こえてる場合,ボールクリア採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
          else if(strategyPlan.equals("ballClear") && 
                                      currentStrategy.equals("counter")){ 
            //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter") && 
                  super.infoManager.forwardFW(0,45,10) > 1 &&
                  this.infoManager.getX() > ballX){ 
            //  前方にFWがいて自分がボールより前方にいる場合,カウンター採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("ballClear")
                                              && pointDiff <= 0 && ballX < 0){ 
            //  チームが同点または負けていて攻められている場合,ボールクリア継続
            super.infoManager.setSelfStrategy(currentStrategy,"show");
	  }
          else { //  その他の戦略を採用
            if(ballX < - Generic.FIELD_X_WIDTH * 0.25)
              //  フィールドの1/4こえてる場合,ボールクリア採用
              super.infoManager.setSelfStrategy("ballClear","show");
	  }


        }

        selfStrategy = super.infoManager.getSelfStrategy();

        if(selfStrategy.equals("ballClear")){  //  協調戦略"ballClear"
          if(super.infoManager.needToChaseBall(2) &&
                              (predDistBallPos < 22 || ballX < super.stdX)){
	    if(super.infoManager.getStamina() > staminaThreshold ||
              (ballX < - Generic.FIELD_X_WIDTH * 0.25 && Math.abs(ballY) < 20))
	      super.player.chaseBall(100);
	    else
	      super.player.chaseBall(dashPower);

	    player.lookAtBall();

          }
	  else if(markFlag && super.infoManager.needToMark(oppXY.getPointX(),
                                                         oppXY.getPointY(),1)){
	    mark(oppXY.getPointX(),oppXY.getPointY());
	    super.player.lookAtBall();

	  }
          else {
	    if(!super.infoManager.onThePos(super.tmpXY.getPointX(),
                                                    super.tmpXY.getPointY())){
	      super.player.goToThePos(super.tmpXY.getPointX(),
                                            super.tmpXY.getPointY(),dashPower);
	      super.player.lookAtBall();
	    }
            else {
              super.player.turn(super.infoManager.getMoment(ballDir));
              super.player.lookAtBall();		    
            }
	  }
	}
        else { //  その他の戦略
          if(super.infoManager.needToChaseBall(2) &&
                              (predDistBallPos < 18 || ballX < super.stdX)){
            if(selfStrategy.equals("counter") && 
	       ballX < - Generic.FIELD_X_WIDTH * 0.25){
              //  フィールドの1/4こえてる場合,カウンター破棄
              super.infoManager.setSelfStrategy("private","show");
	    }

	    if(super.infoManager.getStamina() > staminaThreshold)
	      super.player.chaseBall(100);
	    else
	      super.player.chaseBall(dashPower);

	    player.lookAtBall();

          }
	  else if(markFlag && super.infoManager.needToMark(oppXY.getPointX(),
                                                         oppXY.getPointY(),1)){
	    mark(oppXY.getPointX(),oppXY.getPointY());
	    super.player.lookAtBall();

	  }
          else {
	    if(!super.infoManager.onThePos(super.tmpXY.getPointX(),
                                                    super.tmpXY.getPointY())){
	      super.player.goToThePos(super.tmpXY.getPointX(),
                                            super.tmpXY.getPointY(),dashPower);
	      super.player.lookAtBall();
            }
            else {
              super.player.turn(super.infoManager.getMoment(ballDir));
              super.player.lookAtBall();		    
            }
	  }
	}
      }
    }  //  ボール情報をもっていない場合
    else {
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
      super.player.turn2(super.infoManager.searchBall());
      //updateValueTable(0.0,super.tmpXY);
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

/***************************************************************************/

  public void mark(double oppX, double oppY){
    Point markXY = new Point();        // Point!!
    double dashPower = Generic.STAMINA_INC_MAX * 1.5;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower *= 2;

    markXY.setPointX(super.tmpXY.getPointX());

    Generic.getDivisionPointByDX(oppX,oppY,-52.5,0,markXY);

    if(!super.infoManager.onThePos(markXY.getPointX(),markXY.getPointY())){
      super.player.goToThePos(markXY.getPointX(),markXY.getPointY(),
                                                    Generic.STAMINA_INC_MAX);
    }
  }

/***************************************************************************/

  public void opponentsFreeKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(infoManager.hasCoachPos()){
      startX = infoManager.getCoachStartPosX();
      startY = infoManager.getCoachStartPosY();
      stdX = infoManager.getCoachStdPosX();
      stdY = infoManager.getCoachStdPosY();
      infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    int currentTime = this.infoManager.getBodyTime();
    double headAngle = this.infoManager.getHeadAngle();
    Point oppXY = new Point();             // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;
    
    if(currentTime - lastSearchTime4 > 30){
      if(searchStep4 != 0){
	this.player.turn2(90);
	searchStep4--;
      }
      else {
	this.player.turn2(90);
	searchStep4 = 3;
	lastSearchTime4 = currentTime;
      }
    }
    else if(this.infoManager.hasBallInfo()){
      double ballX = super.infoManager.getBallX();
      double ballY = super.infoManager.getBallY();
      if(ballX < super.stdX + 8){ 
        tmpXY.setPointX(ballX - 8);
      }
      else {
        tmpXY.setPointX(super.stdX);
      }

      if(this.infoManager.getPosOfNearestOpponent(0,180,15,oppXY) < 25){
	if(this.infoManager.needToMark(oppXY.getPointX(),oppXY.getPointY(),1)){
	  mark(oppXY.getPointX(),oppXY.getPointY());
	  this.player.lookAtBall();
	}
        else 
          this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY()
                                                                  ,dashPower);
      }
      else if(!this.infoManager.onThePos(tmpXY.getPointX(),tmpXY.getPointY()))
	this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),dashPower);
      else
	this.player.turn(this.infoManager
                             .getMoment(this.infoManager.getBallDir()));
    }
    else {
      this.player.turn(this.infoManager.getMoment(30));
    }

    //  自分の持つ戦略とボール情報を流す
    if(sayBallString || sayStrategy){
      StringBuffer str = new StringBuffer("(");  // sprintf!!

      //  ボール情報はshotの後のみ流す
      if(sayBallString)
        str.append(this.infoManager.getBallString());
      if(sayBallString && sayStrategy)
        str.append(" ");
      //  自分の戦略をブロードキャスト
      if(sayStrategy)
        str.append(this.infoManager.getStrategyString());

      str.append(")");

      this.sayBuff = str.toString(); 
      this.player.say(this.sayBuff);    
    }

  }

}    


