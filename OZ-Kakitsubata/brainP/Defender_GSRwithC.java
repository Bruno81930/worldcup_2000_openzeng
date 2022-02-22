// コーチからの指示をうけるDefender_GSR.java

package brainP;

import genericP.*;
// import brainP.*;
import playerP.*;

/********************************************************/
/*         Defender_GSR    DF_G エージェントwith coach  */
/********************************************************/

public class Defender_GSRwithC extends Defender_GSR{

  //add 0801 for freekick 
  int countForCoachSaying = -1;
  protected final int COACH_WAITING_TIME = 10;  

  int myLevelForCoach = 0;

  double kx, ky, pow;	

/***************************************************************************/

  public Defender_GSRwithC(Player p){

    super(p);

  }

/***************************************************************************/

  public void playOn(){
      
      myLevelForCoach = 0;
      super.playOn();

  }
    
/***************************************************************************/
  public void ourFreeKick(){

    //
    //コーチが指示する,しないに関係ない初期化処理
    //(まちがっているかもしれないです)
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    int pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint();

    backPoint=true;
    goalieKick = 3;
    goalieFreeKick = true;

    turnNumber++;

    //System.out.println("level = "+myLevelForCoach);

    //System.out.println("stamina :" + super.infoManager.getStamina());
    //System.out.println("FreeKickWait :" + FreeKickWait);
    //System.out.println("point :" + pointDiff);

    // 同点か,勝っているときはフリーキックをゆっくりと
    if ((super.infoManager.getStamina() > 
	Generic.STAMINA_MAX * Generic.EFFORT_DEC_THR || FreeKickWait > 20)
	&& (pointDiff < 0 || FreeKickWait > 20))
      {

      switch(myLevelForCoach){
	
      case -1:
	
	//
	//コーチからの指示がなかった	  
	//
	
	ourFreeKickNoCoach();	  
	break;
	
      case 0: 
	
	//
	//FreeKickではじめてこのメソッドにやってきた
	//
	
	myLevelForCoach = 1;
	
	countForCoachSaying = COACH_WAITING_TIME;
	
	break;
	
      case 1:
	
	//
	//コーチから指示を待っている状態
	//
	
	countForCoachSaying--;
	
	if (super.infoManager.hasCoachGKFreekickPos()){
	  
	  //指示があった
	  myLevelForCoach = 2;
	  super.infoManager.setCoachPosGKFreeKickFlag(Generic.NS_FALSE);

	  //receiveFreekickInformation();
	  
	}else if(countForCoachSaying == 0){
	  
	  //時間切れ。自ら判断する
	  myLevelForCoach = -1;
	  
	}
	break;
	
      case 2:
	
	//
	//指示された位置に移動する
	//
	
	super.player.move(super.infoManager.getCoachGKFreekickMyPosX(),
			  super.infoManager.getCoachGKFreekickMyPosY());
	super.player.lookAtBall();
	myLevelForCoach = 3;
	
	break;
	
      case 3: myLevelForCoach++; super.player.lookAtBall(); break;
	
      case 4: 
	
	double ballDir = super.infoManager.getBallDir();
	
	if(ballDir > 110)
	  super.player.turnBall(90);
	else if(ballDir < -110)
	  super.player.turnBall(-90);
	
	super.player.lookAtBall();
	
	myLevelForCoach++; 
	break;
	
      case 5: 
	//
	//指示された位置にキック！
	//(playerである程度判断するように変更)
	
	double mx, my;
	// pow = 30;   //default

	// mx = super.infoManager.getCoachGKFreekickMyPosX();
	// my = super.infoManager.getCoachGKFreekickMyPosY();
	// 自分の座標の方が正確かな
	mx = super.infoManager.getX();
	my = super.infoManager.getY();
	kx = super.infoManager.getCoachGKFreekickPosX();
	ky = super.infoManager.getCoachGKFreekickPosY();
	
	System.out.println(mx+"\t"+my+"\t"+kx+"\t"+ky);
	
	
	// ここから変更(8/22)ある程度キーパー側で判断できるように。
	
	// kick の方向等を決める
	NSobject openDirNS = new NSobject();  // NS object! openDir
	Point recXY = new Point();  // Point! recX, recY
	Point oppXY = new Point();  // Point! recX, recY
	
	double dir = super.infoManager.getDirOfThePos2(kx,ky);
	double dist = super.infoManager.getDistOfThePos(kx,ky);
	
	// まず味方へのパスを試みる
	double value = super.infoManager.getPosOfReceiver(dir,20,dist + 5,
							  recXY,5);
	if(value < 1.5){  //  すこしシビアにしてみた(1.7 -> 1.5)
	  super.infoManager.getPosOfOpponentsForward(oppXY);
	  if(recXY.getPointX() > oppXY.getPointY()){
	    //  パス相手が敵FWより前方にいる場合,カウンター提案
	    super.infoManager.setSelfStrategy("counter","plan");
	    sayStrategy = Generic.NS_TRUE;
	  }
	  super.player.shot(recXY.getPointX(),recXY.getPointY(),15);
	  sayBallString = Generic.NS_TRUE;
	}
	else {
	  //  パス相手やオープンスペースがないとき.ボールクリア提案
	  super.infoManager.setSelfStrategy("ballClear","plan");
	  sayStrategy = Generic.NS_TRUE;
	  value = super.infoManager.getDirOfOpenSpace(dir,45,30,5,openDirNS);
	  super.player.shot(super.infoManager.getX() + 30
			    * Math.cos(Generic.digToRad(openDirNS.getNS())),
			    super.infoManager.getY() + 30
			    * Math.sin(Generic.digToRad(openDirNS.getNS())),25);
	  sayBallString = Generic.NS_TRUE;
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
	// ここまで変更
	
	break;
      }
    }
    else{
      // ちょっと休んで体力回復
      FreeKickWait++;
    }
  }
    
  public void ourFreeKickNoCoach(){
    //
    //FreeKick時の初期化
    //
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;
    double pointDiff = super.infoManager.getOurPoint()
	               - super.infoManager.getOpponentsPoint(); 

    double kickMargin = 0.4;       // キックの精度向上用,値は適当(0.35以上)	
    // System.out.println("BodyFace " + super.infoManager.getBodyFaceDir());

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
	
	double value = super.infoManager.getPosOfReceiver(super.infoManager.getBodyFaceDir(),45,30,recXY,5);
	
	/* 時間の経過によってよりシビアになっていく？つまり蹴れない？
	if (value < sairyo){
	  sairyo = value;
	  Sairyo = recXY;
	}
	*/

	if (turnNumber > 13){
	  if(value < 1.5){  //  すこしシビアにしてみた(1.7 -> 1.5)
	    super.infoManager.getPosOfOpponentsForward(oppXY);
	    if(recXY.getPointX() > oppXY.getPointY()){
	      //  パス相手が敵FWより前方にいる場合,カウンター提案
	      super.infoManager.setSelfStrategy("counter","plan");
	      sayStrategy = Generic.NS_TRUE;
	    }
	    super.player.shot(recXY.getPointX(),recXY.getPointY(),15);
	    // System.out.println("#Debug# Time Unum :" + super.infoManager.getBodyTime() + " " + super.infoManager.getMyUnum() + " // 見方にパスを蹴る");
	    sayBallString = Generic.NS_TRUE;
	  }
	  else {
	    //  パス相手やオープンスペースがないとき.ボールクリア提案
	    super.infoManager.setSelfStrategy("ballClear","plan");
	    sayStrategy = Generic.NS_TRUE;
	    value = super.infoManager.getDirOfOpenSpace(0,70,30,5,openDirNS);
	    super.player.shot(super.infoManager.getX() + 30
			  * Math.cos(Generic.digToRad(openDirNS.getNS())),
			  super.infoManager.getY() + 30
			  * Math.sin(Generic.digToRad(openDirNS.getNS())),25);
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
