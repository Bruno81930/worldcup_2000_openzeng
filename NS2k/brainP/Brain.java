// Brain.java

package brainP;

import genericP.*;
import playerP.*;
import parserP.*;
import info_managerP.*;

/********************************************************/
/*         Brain    行動決定用クラス                    */
/********************************************************/

public class Brain implements PlaymodeTable{

  protected String sayBuff;
  protected double startX,startY;
  protected double stdX,stdY;

  protected Point tmpXY = new Point();  // Point!! tmpX,tmpY
    
  protected int uniformNum;
    
  protected int ourOffsideCount,opponentsOffsideCount;

  protected Player player;                 // インスタンス生成は？
  protected InfoManager infoManager;

  static boolean ready1 = Generic.NS_FALSE;
  static boolean ready2 = Generic.NS_FALSE;
  static boolean ready3 = Generic.NS_FALSE;
  static int lastSearchTime1 = 0;
  static int lastSearchTime2 = 0;
  static int lastSearchTime3 = 0;
  static int lastSearchTime4 = 0;
  static int searchStep1 = 3;
  static int searchStep2 = 3;
  static int searchStep3 = 3;
  static int searchStep4 = 3;

  PositionTable positiontable = new PositionTable();
  protected boolean sayConnect = true;

/***************************************************************************/

  public Brain(){

  }

/***************************************************************************/

  public Brain(Player p){
    player = p;
    infoManager = player.infoManager;

    ourOffsideCount = opponentsOffsideCount = 0;

    uniformNum = player.getUniformNum();

    startX = positiontable.startPos[0][uniformNum - 1][0];
    startY = positiontable.startPos[0][uniformNum - 1][1];
    stdX = positiontable.stdPos[0][uniformNum -1][0];
    stdY = positiontable.stdPos[0][uniformNum -1][1];
    tmpXY.setPointX(stdX);
    tmpXY.setPointY(stdY);

  }

/***************************************************************************/

  public void decideCommand(){

    int playmode = this.infoManager.getPlaymode();

    // switch case にしてもよい 僕は嫌いなので使わないけど
    if(playmode == PM_BEFORE_KICK_OFF){
      beforeKickOff();
    }
    else if(playmode == PM_OUR_KICK_OFF){
      ourKickOff();
    }
    else if(playmode == PM_OPPONENTS_KICK_OFF){
      opponentsKickOff();
    }
    else if(playmode == PM_OUR_KICK_IN){
      ourKickIn();
    }
    else if(playmode == PM_OPPONENTS_KICK_IN){
      opponentsKickIn();
    }
    else if(playmode == PM_OUR_CORNER_KICK){
      ourCornerKick();
    }
    else if(playmode == PM_OPPONENTS_CORNER_KICK){
      opponentsCornerKick();
    }
    else if(playmode == PM_OUR_GOAL_KICK){
      ourGoalKick();
    }
    else if(playmode == PM_OPPONENTS_GOAL_KICK){
      opponentsGoalKick();
    }
    else if(playmode == PM_OUR_FREE_KICK){
      ourFreeKick();
    }
    else if(playmode == PM_OPPONENTS_FREE_KICK){
      opponentsFreeKick();
    }
    else if(playmode == PM_OUR_OFFSIDE){
      ourOffSide();
    }
    else if(playmode == PM_OPPONENTS_OFFSIDE){
      opponentsOffSide();
    }
    else if(playmode == PM_PLAY_ON){
      playOn();
    }
    else if(playmode == PM_HALF_TIME){
      halfTime();
    }
    else if(playmode == PM_TIME_UP){
      timeUp();
    }
    else if(playmode == PM_EXTEND){
      extend();
    }
    else if(playmode == PM_OUR_FOUL){
      ourFoul();
    }
    else if(playmode == PM_OPPONENTS_FOUL){
      opponentsFoul();
    }
    else if(playmode == PM_OUR_GOAL){
      ourGoal();
    }
    else if(playmode == PM_OPPONENTS_GOAL){
      opponentsGoal();
    }
  }

/***************************************************************************/

  public void beforeKickOff(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    if(this.infoManager.onThePos(this.startX,this.startY)){
      if(this.infoManager.hasBallInfo()){
	this.player.turn(this.infoManager.getBallDir());
	this.player.lookAtBall();
      }
      else {
	this.player.turn(30);
      }
    }
    else {
      this.player.move(this.startX,this.startY);
      this.player.changeView("normal","high");
    }

    //  自分の持つ戦略とボール情報を流す
    if(sayBallString || sayStrategy){
      StringBuffer str = new StringBuffer("(");  // sprintf!!
      if(sayConnect){
	str.append("(Connect OZ No.").append(this.infoManager.getMyUnum())
           .append(" : NITStones)");
        sayConnect = false;
      }

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

/***************************************************************************/

  public void ourKickOff(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    this.tmpXY.setPointX(this.stdX);  //  初期化する
    this.tmpXY.setPointY(this.stdY);

    if(this.infoManager.hasBallInfo()){
      if(this.infoManager.isKickable2()){
        NSobject openDirNS = new NSobject();     // NSobject!!
        Point recXY = new Point();               // Point!!

        if(this.infoManager.getPosOfReceiver(0,90,25,recXY) < 1.7){
	  this.player.shot(recXY.getPointY(),recXY.getPointY(),10);
	  this.player.lookAtBall();
          sayBallString = Generic.NS_TRUE;
        }
        else {
	  this.infoManager.getDirOfOpenSpace(0,90,25,5,openDirNS);
	  this.player.shot(this.infoManager.getX() + 15 
                           * Math.cos(Generic.digToRad(openDirNS.getNS())),
		  	   this.infoManager.getY() + 15
                           * Math.sin(Generic.digToRad(openDirNS.getNS())),10);
 	  this.player.lookAtBall();
          sayBallString = Generic.NS_TRUE;
        }
      }
      else if(this.infoManager.needToChaseBall(1)){
        if(this.infoManager.getOurPoint()
                           - this.infoManager.getOpponentsPoint() < 0){
	  this.player.chaseBall(2 * Generic.STAMINA_INC_MAX);
        }
        else {
	  this.player.chaseBall(Generic.STAMINA_INC_MAX);
        }
	this.player.lookAtBall();
      }
      else {
        this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                               Generic.STAMINA_INC_MAX);
	this.player.lookAtBall();
      }
    }
    else{      
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

/***************************************************************************/

  public void opponentsKickOff(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    this.tmpXY.setPointX(this.stdX);  //  初期化する
    this.tmpXY.setPointY(this.stdY);

    if(this.infoManager.onThePos(this.startX,this.startY)){
      if(this.infoManager.hasBallInfo()){
	this.player.turn(this.infoManager.getBallDir());
      }
      else {
        this.player.turn(30);
      }
    }
    else {
      this.player.move(this.startX,this.startY);
      this.player.changeView("normal","high");
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

/***************************************************************************/

  public void ourKickIn(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    double ballX = this.infoManager.getBallX();
    double ballY = this.infoManager.getBallY();
    double ballDir = this.infoManager.getBallDir();
    NSobject openDirNS  = new NSobject();    // NSobject!!
    Point recXY = new Point();               // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();
    double absGoalDir = this.infoManager.getAbsGoalDir();
    double kickDir = absGoalDir;
    double kickAngle = Math.abs(absGoalDir) - 5;

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(this.infoManager.hasBallInfo()){
      if(ready1){
	if(this.infoManager.isKickable2()){
	  double value;
          value = this.infoManager.
                       getPosOfReceiver(kickDir,kickAngle,35,recXY);

	  if(value < 1.7){
	    this.player.shot(recXY.getPointX(),recXY.getPointY(),10);
 	    this.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
	    this.infoManager.getDirOfOpenSpace(kickDir,kickAngle,25,3,
                                                                 openDirNS);
		    
	    this.player.shot(this.infoManager.getX() + 25
                             * Math.cos(Generic.digToRad(openDirNS.getNS())),
			     this.infoManager.getY()+ 25 
                             * Math.sin(Generic.digToRad(openDirNS.getNS())),
                             10);
 	    this.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
		
	  ready1 = Generic.NS_FALSE;
	}
        else {
	    this.player.chaseBall(dashPower);
            //this.player.lookAtBall();
	}
      }
      else if(this.infoManager.needToChaseBall(1)){
	double destX = ballX;
        double destY;
        if(ballY > 0)
          destY = ballY + 3;
        else
          destY = ballY - 3;

	if(this.infoManager.onThePos(destX,destY)){
	  ready1 = Generic.NS_TRUE;
          player.chaseBall(dashPower);
          player.lookAtBall();
	  //this.player.turn(this.infoManager.getMoment(ballDir));
          //player.chaseBall(dashPower);
        }
        else {
          this.player.goToThePos(destX,destY,dashPower);
          this.player.lookAtBall();
	}
      }
      else {
        this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                            dashPower);
        this.player.lookAtBall();
      }
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

/***************************************************************************/

  public void opponentsKickIn(){

    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    int currentTime = this.infoManager.getBodyTime();
    double headAngle = this.infoManager.getHeadAngle();
    Point oppXY = new Point();                     // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;
    
    if(currentTime - lastSearchTime1 > 30){
      if(searchStep1 != 0){
	this.player.turn(90);
	searchStep1--;
      }
      else {
        this.player.turn(90);
	searchStep1 = 3;
	lastSearchTime1 = currentTime;
      }
    }
    else {
      if(this.infoManager.getPosOfNearestOpponent(0,180,20,oppXY) < 25){
	if(this.infoManager.needToMark(oppXY.getPointX(),oppXY.getPointY(),1)){
	  mark(oppXY.getPointX(),oppXY.getPointY());
	  this.player.lookAtBall();
	}
        else {
	  this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                               dashPower);
	}
      }
      else if(!this.infoManager.onThePos(tmpXY.getPointX(),tmpXY.getPointY()))
	this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),dashPower);
      else{
	this.player.turn(this.infoManager
                             .getMoment(this.infoManager.getBallDir()));
	this.player.lookAtBall();
      }
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

/***************************************************************************/

  public void ourCornerKick(){ // ほとんど ourKickIn と同じ
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    double ballX = this.infoManager.getBallX();
    double ballY = this.infoManager.getBallY();
    double ballDir = this.infoManager.getBallDir();
    NSobject openDirNS = new NSobject(); // NSobject!!
    Point recXY = new Point();         // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();
    double centerDir = Generic.normDir(this.infoManager.getDirOfThePos(0,0) +
			               this.infoManager.getBodyFaceDir());

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(this.infoManager.hasBallInfo()){
      if(ready2){
	if(this.infoManager.isKickable2()){
	  double value;
	  value = this.infoManager 
		      .getPosOfReceiver(centerDir,30,35,recXY);

	  if(value < 1.7){
	    this.player.shot(recXY.getPointX(),recXY.getPointY(),10);
 	    this.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
          else {
	    this.infoManager.getDirOfOpenSpace(centerDir,25,25,5,openDirNS);
		    
	    this.player.shot(this.infoManager.getX() + 25
                             * Math.cos(Generic.digToRad(openDirNS.getNS())),
			     this.infoManager.getY() + 25
                             * Math.sin(Generic.digToRad(openDirNS.getNS())),
			     10);
 	    this.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
		
	  ready2 = Generic.NS_FALSE;
        } else {
	    this.player.chaseBall(dashPower);
            // this.player.lookAtBall();
	}
      }
      else if(this.infoManager.needToChaseBall(1)){
	double destX = ballX;
        double destY;
	if(ballY > 0)
	  destY = ballY + 3;
	else
	  destY = ballY - 3;

	if(this.infoManager.onThePos(destX,destY)){
	  ready2 = Generic.NS_TRUE;
	  this.player.chaseBall(dashPower);
	  this.player.lookAtBall();
	  // this.player.turn(this.infoManager.getMoment(ballDir));
	  // this.player.chaseBall(dashPower);
	}
        else {
	  this.player.goToThePos(destX,destY,dashPower);
          this.player.lookAtBall();
	}
      }
      else {
	this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),dashPower);
	this.player.lookAtBall();
      }
    } else {
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

/***************************************************************************/

  public void opponentsCornerKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    int currentTime = this.infoManager.getBodyTime();
    double headAngle = this.infoManager.getHeadAngle();
    Point oppXY = new Point();          // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();

    tmpXY.setPointX(this.stdX - 20);

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;
    
    if(currentTime - lastSearchTime2 > 30){
      if(searchStep2 != 0){
	this.player.turn(90);
	searchStep2--;
      }
      else {
	this.player.turn(90);
	searchStep2 = 3;
	lastSearchTime2 = currentTime;
      }
    }
    else {
      if(this.infoManager.getPosOfNearestOpponent(0,180,15,oppXY) < 25){
	if(this.infoManager.needToMark(oppXY.getPointX(),oppXY.getPointY(),1)){
	  mark(oppXY.getPointX(),oppXY.getPointY());
	  this.player.lookAtBall();
	}
        else {
	  this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                               dashPower);
	}
      }
      if(!this.infoManager.onThePos(tmpXY.getPointX(),tmpXY.getPointY()))
	this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),dashPower);
      else{
	this.player.turn(this.infoManager
			   .getMoment(this.infoManager.getBallDir()));
	this.player.lookAtBall();
      }
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

/***************************************************************************/

  public void ourGoalKick(){
    Point oppXY = new Point();          // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    if(this.infoManager.getPosOfNearestOpponent(0,180,15,oppXY) < 25){
      if(this.infoManager.needToMark(oppXY.getPointX(),oppXY.getPointY(),1)){
        mark(oppXY.getPointX(),oppXY.getPointY());
        this.player.lookAtBall();
      }
      else {
        this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                 Generic.STAMINA_INC_MAX);
      }
    }
    else {
      if(!this.infoManager.onThePos(tmpXY.getPointX(),tmpXY.getPointY()))
        this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                 Generic.STAMINA_INC_MAX);
      else{
        this.player.turn(this.infoManager
			   .getMoment(this.infoManager.getBallDir()));
	this.player.lookAtBall();
      }
    }

  }

/***************************************************************************/

  public void opponentsGoalKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    int currentTime = this.infoManager.getBodyTime();
    double headAngle = this.infoManager.getHeadAngle();
    Point oppXY = new Point();        // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;
    
    if(currentTime - lastSearchTime3 > 30){
      if(searchStep3 != 0){
	this.player.turn(90);
	searchStep3--;
      }
      else {
	this.player.turn(90);
	searchStep3 = 3;
	lastSearchTime3 = currentTime;
      }
    }
    else {
      if(this.infoManager.getPosOfNearestOpponent(0,180,15,oppXY) < 25){
	if(this.infoManager.needToMark(oppXY.getPointX(),oppXY.getPointY(),1)){
	  mark(oppXY.getPointX(),oppXY.getPointY());
	  this.player.lookAtBall();
	}
        else {
	  this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                              dashPower);
        }
      }
      if(!this.infoManager.onThePos(tmpXY.getPointX(),tmpXY.getPointY()))
	this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),dashPower);
      else{
	this.player.turn(this.infoManager
                             .getMoment(this.infoManager.getBallDir()));
	this.player.lookAtBall();
      }
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

/***************************************************************************/

  public void ourFreeKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();
    double absGoalDir = this.infoManager.getAbsGoalDir();
    double ballDir = this.infoManager.getBallDir();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    if(this.infoManager.hasBallInfo()){

      double ballX = this.infoManager.getBallX();
      double ballY = this.infoManager.getBallY();
      Point recXY = new Point();          // Point!!
      NSobject openDirNS = new NSobject(); // NSobject!!

      if(ready3){
	if(this.infoManager.isKickable2()){
	  if(this.infoManager.
	              getPosOfReceiver(absGoalDir,70,35,recXY) < 1.7){
	    this.player.shot(recXY.getPointX(),recXY.getPointY(),10);
	    ready3 = Generic.NS_FALSE;
 	    this.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	  else {
	    this.infoManager.getDirOfOpenSpace(absGoalDir,45,25,5,openDirNS);
	    this.player.shot(this.infoManager.getX() + 25 
                             * Math.cos(Generic.digToRad(openDirNS.getNS())),
			     this.infoManager.getY()+ 25
                             * Math.sin(Generic.digToRad(openDirNS.getNS())),
			     10);
	    ready3 = Generic.NS_FALSE;
 	    this.player.lookAtBall();
            sayBallString = Generic.NS_TRUE;
	  }
	} else {
	      this.player.chaseBall(dashPower);
	      // this.player.lookAtBall();
	}
      }
      else if(this.infoManager.needToChaseBall(1)){
        double destX = ballX - 3;
        double destY = ballY;

        if(this.infoManager.onThePos(destX,destY)){
          ready3 = Generic.NS_TRUE;
          this.player.chaseBall(dashPower);
	  this.player.lookAtBall();
	}
        else {
          this.player.goToThePos(destX,destY,dashPower);
          this.player.lookAtBall();
	}
      }
      else {
        this.player.goToThePos(tmpXY.getPointX(),tmpXY.getPointY(),
                                                              dashPower);
        this.player.lookAtBall();
      }
    } else {
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

/***************************************************************************/

  public void opponentsFreeKick(){
    boolean sayBallString = Generic.NS_FALSE;
    boolean sayStrategy = Generic.NS_FALSE;

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
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
	this.player.turn(90);
	searchStep4--;
      }
      else {
	this.player.turn(90);
	searchStep4 = 3;
	lastSearchTime4 = currentTime;
      }
    }
    else if(this.infoManager.hasBallInfo()){
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
      else{
	this.player.turn(this.infoManager
                             .getMoment(this.infoManager.getBallDir()));
	this.player.lookAtBall();
      }
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

/***************************************************************************/

  public void ourOffSide(){
    this.ourOffsideCount++;
  }

/***************************************************************************/

  public void opponentsOffSide(){
    this.opponentsOffsideCount++;
    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }
  }

/***************************************************************************/

  public void playOn(){
  }

/***************************************************************************/

  public void halfTime(){
    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }
  }

/***************************************************************************/

  public void timeUp(){
    this.player.bye();
  }

/***************************************************************************/

  public void extend(){

   }

/***************************************************************************/

  public void ourFoul(){

  }

/***************************************************************************/

  public void opponentsFoul(){

  }

/***************************************************************************/

  public void ourGoal(){

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    if(this.infoManager.onThePos(this.startX,this.startY)){
      if(this.infoManager.hasBallInfo()){
	this.player.turn(this.infoManager.getBallDir());
      }
      else {
	this.player.turn(30);
      }
    }
    else {
      this.player.move(this.startX,this.startY);
      this.player.changeView("normal","high");
    }
  }

/***************************************************************************/

  public void opponentsGoal(){

    if(this.infoManager.hasCoachPos()){
      this.startX = this.infoManager.getCoachStartPosX();
      this.startY = this.infoManager.getCoachStartPosY();
      this.stdX = this.infoManager.getCoachStdPosX();
      this.stdY = this.infoManager.getCoachStdPosY();
      this.infoManager.setCoachPosFlag(Generic.NS_FALSE);
    }

    if(this.infoManager.onThePos(this.startX,this.startY)){
      if(this.infoManager.hasBallInfo()){
	this.player.turn(this.infoManager.getBallDir());
      }
      else {
	this. player.turn(30);
      }
    }
    else {
      this.player.move(this.startX,this.startY);
      this.player.changeView("normal","high");
    }
  }

/***************************************************************************/
    
  public void mark(double oppX,double oppY){
    Point markXY = new Point();            // Point!!
    double dashPower = Generic.STAMINA_INC_MAX;
    double pointDiff = this.infoManager.getOurPoint()
	               - this.infoManager.getOpponentsPoint();

    if(pointDiff < 0)
      dashPower = Generic.STAMINA_INC_MAX * 2;

    Generic.getDivisionPoint(oppX,oppY,
		     this.infoManager.getBallX(),
		     this.infoManager.getBallY(),
		     1,9,markXY);

    if(!this.infoManager.onThePos(markXY.getPointX(),markXY.getPointY())){
      this.player.goToThePos(markXY.getPointX(),markXY.getPointY(),dashPower);
    }
  }

/***************************************************************************/

  public void parseStrategy(){

  }

/***************************************************************************/
  // ポジションの設定

  public void setPosition(int formationNum,int uNum){
    startX = positiontable.startPos[formationNum][uNum - 1][0];
    startY = positiontable.startPos[formationNum][uNum - 1][1];
    stdX = positiontable.stdPos[formationNum][uNum -1][0];
    stdY = positiontable.stdPos[formationNum][uNum -1][1];
    tmpXY.setPointX(stdX);
    tmpXY.setPointY(stdY);

  }
/***************************************************************************/

  public void setPosition(double sx,double sy,double tx,double ty){
    startX = sx;
    startY = sy;
    stdX = tx;
    stdY = ty;
    tmpXY.setPointX(stdX);
    tmpXY.setPointY(stdY);

  }

}
