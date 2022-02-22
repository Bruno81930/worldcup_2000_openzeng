// Defender_GSR.java

package brainP;

import genericP.*;
// import brainP.*;
import playerP.*;


/********************************************************/
/*         Defender_GSR    DF_G エージェント            */
/********************************************************/

public class Defender_GSR extends Brain{

  int lastCatchTime,lastGoalKickTime;
  static boolean search  = Generic.NS_FALSE;
  static boolean needMove = true; // 5/19 move のチェックの変更
  static boolean backPoint = false; // FreeKick の後は必ず定位置に強制移動
  static boolean goToThePos = false; // 5/26 移動中
  static String turnStartDir="";          // ボールを見失ったときの探索方向

  String selfStrategy;
  double ballCatchAngle = 0;       // 5/17 ボールの飛んできた方向（上か下）
  int myLevel = 0;
  int turnNumber = 0;
  int goalieKick = 0;              // FreeKick をした次の瞬間に catch の
                                   // 発行をするのを押さえるため
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

    // キャッチは 5 サイクルに 1 回しかできないので
    if (catchFlag > 0) catchFlag--;

    if (FreeKickWait > 0)
      FreeKickWait = 0;    

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if (goalieFreeKick){
      // !! Second Step フリーキック後はボールを一時的に見失っても
      //    いいから,基準となりそうな位置に速攻戻りましょう
      
      if (super.infoManager.getY() > 0){
	// (-47,5) へ緊急移動
	if (super.infoManager.onThePos(-47,5)){
	  // 次回からは通常の行動．正面を見ておく
	  goalieFreeKick = false;
	  super.player.turn2(-super.infoManager.getBodyFaceDir());
	  super.player.turnNeck(-super.infoManager.getHeadFaceDir());
	}
	else{
	  double dirXY = super.infoManager.getDirOfThePos2(-47,5);
	  
	  if (Math.abs(dirXY - super.infoManager.getBodyFaceDir()) > 5){
	    // 角度修正
	    super.player.turn2(dirXY-super.infoManager.getBodyFaceDir());
	  }
	  else{
	    // 猛ダッシュ
	    super.player.dash(100);
	  }
	}
      }
      else{
	// (-47,-5) へ緊急移動
	if (super.infoManager.onThePos(-47,-5)){
	  // 次回からは通常の行動．正面を見ておく
	  goalieFreeKick = false;
	  super.player.turn2(-super.infoManager.getBodyFaceDir());
	  super.player.turnNeck(-super.infoManager.getHeadFaceDir());
	}
	else{
	  double dirXY = super.infoManager.getDirOfThePos2(-47,-5);
	  
	  if (Math.abs(dirXY - super.infoManager.getBodyFaceDir()) > 5){
	    // 角度修正
	    super.player.turn2(dirXY-super.infoManager.getBodyFaceDir());
	  }
	  else{
	    // 猛ダッシュ
	    super.player.dash(100);
	  }	  
	}
      }
    }
    else if(super.infoManager.hasBallInfo()){
      //  ボール情報をもっている
      double ballDir = super.infoManager.getBallDir();  // 絶対方向
      double ballX = super.infoManager.getBallX();
      double ballY = super.infoManager.getBallY();
      
      // 適正位置を探す
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

      // バグとり用
      if(super.tmpXY.getPointX() < - Generic.FIELD_X_WIDTH * 0.5 + 1)
        super.tmpXY.setPointX(- Generic.FIELD_X_WIDTH * 0.5 + 1.5);

      /*
      System.out.println("dist_x" + super.tmpXY.getPointX());
      System.out.println("dist_y" + super.tmpXY.getPointY());
      */

      // 一時的に変更 
      if(ballX < -25)
        dashPower = Generic.STAMINA_INC_MAX * 3;

      // 行動決定
      if (super.infoManager.isCatchable() 
           && super.infoManager.inPenaltyArea()
	   && currentTime - this.lastGoalKickTime > 10
	   && goalieKick == 0
	   && catchFlag == 0){
      // !!! First Step "ボールキャッチ"
	double nextBallDir = super.infoManager.getPredBallDirForGoalie(1);
        super.player.catchBall(nextBallDir);
	ballCatchAngle = ballY;
	catchFlag = 5; 
      }
      else if(super.infoManager.isKickable()){ 
	// !!! Second Step "ボールを蹴れる"
        if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
          String strategyPlan = super.infoManager.getStrategyPlan();
          int planner = super.infoManager.getSPlanner();

          if((strategyPlan.equals("ballClear") && pointDiff <= 0) || 
             (strategyPlan.equals("ballClear") && 
                                   ballX < - Generic.FIELD_X_WIDTH * 0.25)){ 
            //  チームが同点または負けている,または
            //  フィールドの1/4こえてる場合,ボールクリア採用
            super.infoManager.setSelfStrategy(strategyPlan,"plan");
	  }
        }
        else { //  自己の戦略の再評価
          if(currentStrategy.equals("ballClear") && pointDiff <= 0){ 
            //  チームが同点または負けている場合,ボールクリア継続
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
        else { //  その他の戦略
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
	// !!! Third Step "ペナルティエリア内"
	// 5/26 とりあえず書いた状態．中身は見直す必要絶大
	
	// 実装に向けて
	/*
	double a = (g_y - ballY)/(-52.5 - ballX);
	double b = g_y + a * 52.5;
	double Y1 = a * super.infoManager.getX() + b;
	double a = (g_y - ballY)/(-52.5 - ballX);
	double b = g_y + a * 52.5;
	double Y2 = a * super.infoManager.getX() + b;
	*/
	// 実装に向けて

	if (super.infoManager.judgeCriticalCase()){
	  super.player.chaseBall(100);
	  super.player.lookAtBall();
	}
	else if (!super.infoManager.getNearSide()){
	  // 敵の方がボールの近くにいる
	  if (super.infoManager.getBallX() < super.infoManager.getX()){
	    // 自分より後ろにボールがある（緊急事態）
	    super.player.chaseBall(100);
	    super.player.lookAtBall();
	  }
	  else if (super.infoManager.getBallDist() < 5){
	    if (super.infoManager.getOpponentEnableShot()){
	      if (!super.infoManager.onThePos(super.tmpXY.getPointX(),
					      super.tmpXY.getPointY())){
		// 適正位置に移動してキャッチ準備
		
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
		  // 適正位置に移動してキャッチ準備
		  
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
		    // 適正位置に移動してキャッチ準備
		    
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
	    // 適正位置に移動してキャッチ準備
	    
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
	// !!! fourth Step "移動の必要がある"
	
	goToThePos = true;
	super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());	
      }
      else {
	// !!! LAST Step "ボールを捕捉する"
	
	// 移動の必要がなくなったので
	goToThePos = false;
	
	super.player.turn(super.infoManager.getMoment(ballDir));
	super.player.lookAtBall();      
      }
    }
  else {
    // !!! ボール情報をもっていない場合
    if(super.infoManager.getStrategyFlag()){ //  戦略の提示があった
      String strategyPlan = super.infoManager.getStrategyPlan();
      int planner = super.infoManager.getSPlanner();
      
      if(strategyPlan.equals("ballClear") && pointDiff < 0){ 
	//  チームが負けている場合,ボールクリア採用
	super.infoManager.setSelfStrategy(strategyPlan,"show");
      }
    }
    else { //  自己の戦略の再評価
      if(currentStrategy.equals("ballClear") && pointDiff < 0){ 
	  //  チームが負けている場合,ボールクリア継続
	super.infoManager.setSelfStrategy(currentStrategy,"show");
      }
    }

    // ボール情報を持っていないときの指定位置
    super.tmpXY.setPointX(super.startX);
    super.tmpXY.setPointY(super.startY);

    // ボールを探す
    if (goToThePos || !super.infoManager.onThePos(super.tmpXY.getPointX(),
					       super.tmpXY.getPointY())){
      super.player.goaliegoToThePos(super.tmpXY.getPointX(),super.tmpXY.getPointY());
      
      // goToThePos の再評価
      goToThePos = !super.infoManager.onThePos(super.tmpXY.getPointX(),
					       super.tmpXY.getPointY());      
    }
    else {
      // ボールを探すときの回転方向を相手のゴール側から回る
      
      if (turnStartDir.equals("Right")){
	super.player.turn2(+45);
      }
      else if (turnStartDir.equals("Left")){
	super.player.turn2(-45);
      }
      else{
	// まずあり得ない if 文だが
	super.player.turn2(45);
      }
      
      // とりあえず身体の向きに正対しておく
      //super.player.turnNeck(super.infoManager.getHeadFaceDir()-super.infoManager.getBodyFaceDir());

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
    double kickMargin = 0.4;       // キックの精度向上用,値は適当(0.35以上)	
    // System.out.println("BodyFace " + super.infoManager.getBodyFaceDir());
    goalieKick = 3;
    goalieFreeKick = true;

    turnNumber++;

    /*
      FreeKick の流れ
       Level Action
         1.   move を行う
	 3.   蹴る方向を探すA(今はパス)
	 4.   蹴る方向を探すB(今はパス)
	 5.   ボールに近づく
	 6.   ボールを蹴る
    */

    if (myLevel == 0){
      if(super.infoManager.onThePos(-42,12) || 
	   super.infoManager.onThePos(-42,-12)){
	// 所定の位置にいるなら
	myLevel++;
      }
    }
    else if (myLevel == 1){
      // System.out.println("BodyFace " + super.infoManager.getBodyFaceDir());
      if(Math.abs(super.infoManager.getBodyFaceDir()) < 5 ){
	// 前をちゃんと見ているなら
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
      // まず自分の位置を合わす．
      
      if (ballCatchAngle > 0){
	// ボールが下から飛んできた．少なくても下には敵がいるので
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
	
	// kick の方向等を決める
	NSobject openDirNS = new NSobject();  // NS object! openDir
	Point recXY = new Point();  // Point! recX, recY
	Point oppXY = new Point();  // Point! recX, recY
	
	super.infoManager.getPosOfOpponentsForward(oppXY);
	if(oppXY.getPointX() < - Generic.FIELD_X_WIDTH * 0.25){ 
	  // 敵FWがフィールド1/4以上攻めている場合,ボールクリア提案
	  super.infoManager.setSelfStrategy("ballClear","plan");
	  sayStrategy = Generic.NS_TRUE;
	}
	
	double value = super.infoManager.getPosOfReceiver(super.infoManager.getBodyFaceDir(),45,35,recXY,10);
	
	if (value < sairyo){
	  sairyo = value;
	  Sairyo = recXY;
	}

	if (turnNumber > 13){
	  if(sairyo < 1.5){  //  すこしシビアにしてみた(1.7 -> 1.5)
	    super.infoManager.getPosOfOpponentsForward(oppXY);
	    if(recXY.getPointX() > oppXY.getPointY()){
	      //  パス相手が敵FWより前方にいる場合,カウンター提案
	      super.infoManager.setSelfStrategy("counter","plan");
	      sayStrategy = Generic.NS_TRUE;
	    }
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),100);
	    // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // 見方にパスを蹴る");
	    sayBallString = Generic.NS_TRUE;
	  }
	  else {
	    //  パス相手やオープンスペースがないとき.ボールクリア提案
	    super.infoManager.setSelfStrategy("ballClear","plan");
	    sayStrategy = Generic.NS_TRUE;
	    value = super.infoManager.getDirOfOpenSpace(0,70,25,2,openDirNS);
	    super.player.shot(super.infoManager.getX() + 30
			      * Math.cos(Generic.digToRad(openDirNS.getNS())),
			      super.infoManager.getY() + 30
			      * Math.sin(Generic.digToRad(openDirNS.getNS())),100);
	    //  とにかく前に強く蹴れ！
	    // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // とにかく前に強く蹴る");
	    sayBallString = Generic.NS_TRUE;
	  }
	}
    }
    else {
      // ボール情報がないのならボールを探す
      this.player.turn(this.infoManager.getMoment(30));
      // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // ballを探す");
    }
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
      // 敵FWがフィールド1/4以上攻めている場合,ボールクリア提案
      super.infoManager.setSelfStrategy("ballClear","plan");
      sayStrategy = Generic.NS_TRUE;
    }

    if(!super.infoManager.hasBallInfo()){
      super.player.turn2(45);
    }
    else if(super.infoManager.isKickable2()){ //  ボールを持っている
      NSobject openDirNS = new NSobject();  // NS object! openDir
      Point recXY = new Point();  // Point! recX, recY

      if(super.infoManager.getPosOfReceiver(0,30,35,recXY) < 1.5){
                                        //  すこしシビアにしてみた(1.7 -> 1.5)
        if(recXY.getPointX() > oppXY.getPointX()){
          //  パス相手が敵FWより前方にいる場合,カウンター提案
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
        //  パス相手やオープンスペースがないとき,ボールクリア提案
        super.infoManager.setSelfStrategy("ballClear","plan");
        sayStrategy = Generic.NS_TRUE;
        super.infoManager.getDirOfOpenSpace(0,45,25,2,openDirNS);
       	super.player.shot(super.infoManager.getX() + 30 *
                          Math.cos(Generic.digToRad(openDirNS.getNS())),
       	                  super.infoManager.getY() + 30 * 
                          Math.sin(Generic.digToRad(openDirNS.getNS())),100);
	                                           //  とにかく前に強く蹴れ！
        sayBallString = Generic.NS_TRUE;
        this.lastGoalKickTime = super.infoManager.getBodyTime();
      }
    }
    else {
      super.player.chaseBall(dashPower);
      super.player.lookAtBall();
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
