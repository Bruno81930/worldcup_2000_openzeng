// BomberSR.java

package brainP;

import genericP.*;
// import brainP.*;
import playerP.*;

/********************************************************/
/*         BomberSR   FW エージェント                   */
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

    // スタミナ管理用。急造仕様のためバランスわるいかも。
    if(currentTime > 5500){                   // ガンバルシステム
      staminaThreshold = 0;
      dashPower = Generic.STAMINA_INC_MAX * 2;
    }
    else if(pointDiff < -1){                  // 負けている
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

      // absShootDirの算出は無理やりだけどこれでOKか？
      shootAngle = Math.abs(shootAngle);

      // フリーかどうかの判定。
      // 視界内(距離指定=3.0m)＋Neighborhood -0.5m(2.5m)内に敵がいるかどうか
      freeFlag = super.infoManager.getFreeFlag(3.0);
      /*
      if(freeFlag)
        System.out.println(super.player.getUniformNum() + " free!");
      */
      
      /*  戦況により座標変化させたい。これはいまいち
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

      //  FWは反撃の機会を待つべし
      if(ballX < -10){
	dashPower = Generic.STAMINA_INC_MAX * 0.6;
      }
      else if(ballX > Generic.FIELD_X_WIDTH * 0.25 && Math.abs(ballY) < 20
                    && pointDiff <= 0){ // チャンスには気合いで！
        staminaThreshold = Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR - 400;
      }
      

      if(super.infoManager.isKickable()){
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter")){ 
            //  カウンターの合図を聞いたら,カウンター採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
          else if(strategyPlan.equals("1-2Return") && 
            super.infoManager.hasPlayerInfo(super.infoManager.getSPlanner())){ 
            //  1-2リターンの提示者が確認できている場合,1-2リターン採用
            super.infoManager.setSelfStrategy(strategyPlan,"reply");
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("counter") && 
                          super.infoManager.forwardFW(absGoalDir,60,0) >= 1){ 
            //  前方に味方FWがいる場合,カウンター継続
            super.infoManager.setSelfStrategy(currentStrategy,"reply");
	  }
          else if(currentStrategy.equals("sideChange") && 
                                      super.infoManager.parseSideChange()){ 
            //  ボールのあるフィールドに敵が集中している場合,サイドチェンジ採用
            super.infoManager.setSelfStrategy(currentStrategy,"plan");
	  }
          else { //  その他の戦略を提案
            if(ballX < Generic.FIELD_X_WIDTH * 0.25 && 
                         super.infoManager.forwardFW(absGoalDir,60,0) >= 1){
              //  敵陣中盤で味方FWが前方にいる場合,カウンター提案
              super.infoManager.setSelfStrategy("counter","plan");
	    }
	  }
        }

        selfStrategy = super.infoManager.getSelfStrategy();
        sayStrategy = Generic.NS_TRUE;  //  発言権あり

        // フリーでトラップできていない場合
        if(freeFlag && !super.infoManager.isBallTrap()){
          // まずボールの速度を殺す。
          if(ballRelV >= 0.2){
            super.player.trapBall();
            // System.out.println(currentTime+" trapBall");
	  // 敵ゴール方向にボールを
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

        if(selfStrategy.equals("counter")){  //  協調戦略"counter"
	  if(goalDist < 20){
	    super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                goalDist,2,openDirNS);
	    if(super.infoManager.isBallTrap()){
	      super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  とにかく前に強く蹴れ！
	      super.player.lookAtBall();
	    }
            else {
	      super.player.trapBall();
	      super.player.lookAtBall();
	    } 
            sayBallString = Generic.NS_TRUE;
            // System.out.println("カウンターシュート！！");
	  }
          else if(freeFlag){  //  周りに敵がいない
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
              // System.out.println("華麗なドリブル！！");
              if(super.infoManager.getStamina() > staminaThreshold){
                //  スタミナがある場合,1-2リターン！
                super.infoManager.setSelfStrategy("1-2Return","plan");
	      }
	    }
	    else if(super.infoManager.getPosOfReceiver(absGoalDir,40,25,recXY)
                                                                      < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("つばさぁ！！");
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
          else {  //  フリーじゃないとき
  	    if(super.infoManager.getPosOfReceiver(absGoalDir,40,20,recXY)
                                                                      < 1.7){
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("みすぎ！！");
	    }
	    else if(super.infoManager.getDirOfOpenSpace(absGoalDir,15,15,2,
                                                           openDirNS) < 1.7){
              if(super.infoManager.forwardFW(0,90,0) >= 1){
                //  ゴール方向にオープンスペースがあり味方がいる,スルーパス！
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("強烈なスルーパス！！");
	      }
	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);                
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("シュート！！");
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
                   // System.out.println("絶妙なドリブル！！");
                   if(super.infoManager.getStamina() > staminaThreshold){
                     //  スタミナがある場合,1-2リターン！
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
        else if(selfStrategy.equals("1-2Return")){  //  協調戦略"1-2Return"
          if(freeFlag){  //  周りに敵がいない
	    if(goalDist < 18){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      if(super.infoManager.isBallTrap()){
	        super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  とにかく前に強く蹴れ！
	        super.player.lookAtBall();
	      }
              else {
	        super.player.trapBall();
	        super.player.lookAtBall();
	      } 
              sayBallString = Generic.NS_TRUE;
              // System.out.println("うおおぉお！！");
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
              // System.out.println("華麗なドリブル！！");
              // 1-2リターン継続
              super.infoManager.setSelfStrategy("1-2Return",
                                            super.infoManager.getSelfSType());
	    }
            else if(super.infoManager.getSelfSType().equals("plan") &&
              (super.infoManager.getReceivePartner("1-2Return","reply",
		     super.infoManager.getBodyFaceDir(),100,20,recXY) < 1.7)){
              //  1-2リターン賛同者にパスが通る場合,1-2リターン継続
              super.infoManager.setSelfStrategy("throughPass","plan");

	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              //  System.out.println("1-2 くれ！");
	    }
            else if(super.infoManager.getSelfSType().equals("reply") &&

              (super.infoManager.getReceivePlanner("1-2Return",absGoalDir,
		                                   10,20,5,openDirNS) < 1.7)){
              //  1-2リターン提示者にパスが通る場合,1-2リターン継続
              super.infoManager.setSelfStrategy("throughPass","reply");

	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("1-2 いったぞ！");
	    }
            else if(super.infoManager.getPosOfReceiver(absGoalDir,45,20,
                                                               recXY) < 1.7){
              super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
            }
            else {
              //  事前にさがしたオープンスペースに蹴る
              if(super.infoManager.forwardFW(absGoalDir,90,0) >= 1){
                //  ゴール方向にオープンスペースがあり味方がいる,スルーパス 
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("強引なスルーパス！！");
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
          else {  //  フリーじゃないとき
	    if(goalDist < 20){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  とにかく前に強く蹴れ！
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("タイガーショット！！");
	    }
            else if(super.infoManager.getSelfSType().equals("plan") &&
              (super.infoManager.getReceivePartner("1-2Return","reply",
		     super.infoManager.getBodyFaceDir(),100,20,recXY) < 1.7)){
              //  1-2リターン賛同者にパスが通る場合,1-2リターン継続
              super.infoManager.setSelfStrategy("throughPass","plan");

	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("1-2 くれ！");
	    }
            else if(super.infoManager.getSelfSType().equals("reply") &&
              (super.infoManager.getReceivePlanner("1-2Return",absGoalDir,
		                                   10,20,5,openDirNS) < 1.7)){
              //  1-2リターン提示者にパスが通る場合,1-2リターン継続
              super.infoManager.setSelfStrategy("throughPass","reply");

	      super.player.shot(super.infoManager.getX() + 20 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 20 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      10);
              super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("1-2 いったぞ！");
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
                //  ゴール方向にオープンスペースがあり味方がいる,スルーパス！
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("強引なスルーパス！！");
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
                //  フォローできる味方がいて,スタミナがある場合,1-2リターン！
                super.infoManager.setSelfStrategy("1-2Return","plan");
	      }
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else {
              //  事前にさがしたオープンスペースに強引にドリブル
	      if(Math.abs(Generic.normDir(openDirNS.getNS() - 
                                    super.infoManager.getBodyFaceDir())) < 3){
	         if(Math.abs(ballDir) < 100){
	           if(ballDir >= 0)
		     super.player.dribble(100,70);
	           else
		     super.player.dribble(100,-70);
                   // System.out.println("強引なドリブル！！");
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
        else { //  その他の戦略        
          if(freeFlag){  //  周りに敵がいない
	    if(goalDist < 18){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      if(super.infoManager.isBallTrap()){
	        super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  とにかく前に強く蹴れ！
	        super.player.lookAtBall();
	      }
              else {
	        super.player.trapBall();
	        super.player.lookAtBall();
	      } 
              sayBallString = Generic.NS_TRUE;
              // System.out.println("うおおぉお！！");
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
                  // System.out.println("強引なドリブル！！");
                  if(super.infoManager.getStamina() > staminaThreshold){
                    //  スタミナがある場合,1-2リターン！
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
              // System.out.println("強引なドリブル！！");
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
                //  ゴール方向にオープンスペースがあり味方がいる,スルーパス！
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("強引なスルーパス！！");
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
          else {  //  フリーじゃないとき
	    if(goalDist < 20){
	      super.infoManager.getDirOfOpenSpace(absShootDir,shootAngle,
                                                  goalDist,2,openDirNS);
	      super.player.shot(super.infoManager.getX() + 25 
                              * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	      super.infoManager.getY() + 25 
                              * Math.sin(Generic.digToRad(openDirNS.getNS())),
			      25);                 //  とにかく前に強く蹴れ！
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
              // System.out.println("タイガーショット！！");
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
                //  ゴール方向にオープンスペースがあり味方がいる,スルーパス！
                super.infoManager.setSelfStrategy("throughPass","plan");
                // System.out.println("強引なスルーパス！！");
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
                //  フォローできる味方がいて,スタミナがある場合,1-2リターン！
                super.infoManager.setSelfStrategy("1-2Return","plan");
	      }
	      super.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	      super.player.lookAtBall();
              sayBallString = Generic.NS_TRUE;
	    }
	    else {
              //  事前にさがしたオープンスペースに強引にドリブル
	      if(Math.abs(Generic.normDir(openDirNS.getNS() - 
                                    super.infoManager.getBodyFaceDir())) < 3){
	        if(Math.abs(ballDir) < 100){
	          if(ballDir >= 0)
	            super.player.dribble(100,70);
	          else
	            super.player.dribble(100,-70);
                  // System.out.println("強引なドリブル！！");
                  if(super.infoManager.getStamina() > staminaThreshold){
                    //  スタミナがある場合,1-2リターン！
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
      else {        //  ボールを持っていない場合
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
            //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
            super.infoManager.setSelfStrategy("private","show");
	  }
          else if(strategyPlan.equals("counter") &&
                                        super.infoManager.getX() > ballX){
            //  ボールより前にいてカウンターの合図を聞いたら,カウンター採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
            this.lastSearchTime = currentTime;
	  }
          else if(strategyPlan.equals("throughPass") &&
                                        super.infoManager.getX() > ballX){
            //  ボールより前にいてスルーパスの合図を聞いたら,スルーパス採用
            super.infoManager.setSelfStrategy(strategyPlan,"show");
            this.lastSearchTime = currentTime;
	  }
          else if(strategyPlan.equals("1-2Return") && 
                          freeFlag && distBallPos <15 &&
            super.infoManager.hasPlayerInfo(super.infoManager.getSPlanner())){ 
            //  1-2リターンの提示者が確認できていてフリーの場合,1-2リターン採用
            super.infoManager.setSelfStrategy(strategyPlan,"reply");
            sayStrategy = Generic.NS_TRUE;  //  返答
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("counter") && ballX > 0
                                   &&  super.infoManager.getX() > ballX){ 
            //  ボールより前にいる場合,カウンター継続
            super.infoManager.setSelfStrategy(currentStrategy,"show");
	  }
          else if(currentStrategy.equals("throughPass") &&
                          (super.infoManager.getX() < ballX || ballX < 0)){
            //  ボールより後ろにいるまたは攻められている場合,スルーパス破棄
            super.infoManager.setSelfStrategy("private","show");
	  }
        }
	
        selfStrategy = super.infoManager.getSelfStrategy();

        if(selfStrategy.equals("counter")){  //  協調戦略"counter"
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
	    if(currentTime - this.lastSearchTime > 30){  //  NS99のバグ？
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
              //  オフサイドラインを越えたら,カウンター破棄
              super.infoManager.setSelfStrategy("private","show");
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();    
	    }
          }
	}
        else if(selfStrategy.equals("throughPass")){ //  協調戦略"throughPass"
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
	    if(currentTime - this.lastSearchTime > 30){  //  NS99のバグ？
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
              //  オフサイドラインを越えたら,スルーパス破棄
              super.infoManager.setSelfStrategy("private","show");
	      super.player.turn(super.infoManager.getMoment(ballDir));
	      super.player.lookAtBall();    
	    }
          }
	}
        else if(selfStrategy.equals("1-2Return")){ //  協調戦略"1-2Return"
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
            if(currentTime - this.lastSearchTime > 30){  //  NS99のバグ？
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
        else { //  その他の戦略
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
	    if(currentTime - this.lastSearchTime > 30){  //  NS99のバグ？
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
	      // スタミナが少ないときは無理して戻らない
	      if (super.infoManager.getStamina() < 
		   Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR){
		// ボールをみよ
		//System.out.println("体力回復中 : プレーヤ番号,スタミナ " + super.player.getUniformNum() +"," + super.infoManager.getStamina());  
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
    } else {  //  ボール情報をもっていない場合
      if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
        String strategyPlan = super.infoManager.getStrategyPlan();
        int planner = super.infoManager.getSPlanner();

        if(strategyPlan.equals("ballClear") &&
                                        currentStrategy.equals("counter")){ 
          //  カウンターを採用中にボールクリアを聞いたら,カウンター破棄
          super.infoManager.setSelfStrategy("private","show");
        }
        else if(strategyPlan.equals("counter")){
         //  合図があった場合,カウンター採用
          super.infoManager.setSelfStrategy(strategyPlan,"show");
	}
      }
      else { //  自己の戦略の再評価
      }

      super.player.turn2(super.infoManager.searchBall());
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
