// InfoManager.java

package info_managerP;


import genericP.*;
import java.awt.geom.*;
import objectP.*;
import stokenizerP.*;
import parserP.*;

/********************************************************/
/*         InfoManager          情報の管理,加工         */
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
    
  // ビューモード
  String angleWidth;
  String quality;

  // スタミナ関係
  double stamina;
  double effort;
  double recovery;

  // 位置関係
  double x,y;
  double bodyFaceDir,headFaceDir; // 体,首の向き ***** 絶対座標 *****
  double headAngle; // 首の向き  ***** 体中心座標 *****
  double speed,vx,vy;
  double ax,ay,pvx,pvy;
  final int puSize = 5;
  double ux,uy;
  double[] pux = new double[puSize];
  double[] puy = new double[puSize];
  // ux[],uy[] としてもよいのだけど,ここでは現在の情報と過去の情報が
  // ごっちゃにならないように
  // ux[],uy[] にする場合はメソッドを新しく定義してそれを介して
  // 現在の情報と過去の情報とをはっきり区別して取り出せるように
  // すること
  double pHeadFaceDir;  // 一つ前のSee情報を得たときに首が面していた方向
  // この部分は後付けになってしまったため無理やり

  double pBallVX,pBallVY;

  // 得点
  int ourPoint,opponentsPoint;

  // カウンタ
  int kickCount;
  int dashCount;
  int turnCount;
  int sayCount;
  int turnNeckCount;

  // see と sense の同期を取る為
  private double nextBodyAbsDir;
  private double nextHeadRelDir;
  private double nextHeadAbsDir;

  private final int UNKNOWN = -999; // UNKOWN 識別子

  SeeInfoParser seeInfoParser = new SeeInfoParser();
  BodyInfoParser bodyInfoParser = new BodyInfoParser();
  HearInfoParser hearInfoParser = new HearInfoParser();

  PlayerTable playerTable = new PlayerTable();  //  オブジェクトの生成は？？
  StrategyTable strategyTable = new StrategyTable(playerTable);

  BallInfo ball = new BallInfo();
  BallInfo ballShadow = new BallInfo();;
  boolean knowsBallInfo;  //  ボールの情報を持っているかどうか
  int searchFlag;

  String selfString;
  String oppForwardString;

  String strategy;               //  採用している協調戦略
  String strategyType;           //  戦略のタイプ
  double strategyCertainty;      //  戦略の信頼度
  boolean strategyFlag;          //  新規の戦略を聞いたかどうか？
  int planner;                   //  戦略提案者
  int planTime = 0;              //  

  double ctStX,ctStY,ctSdX,ctSdY;                // コーチからのポジション
  boolean stdPosChanged = Generic.NS_FALSE;
  boolean evalSRFlag = Generic.NS_FALSE;

/***************************************************************************/

  //**************************************//
  // add for coach 0801 by Shinji Futamase//
  //**************************************//
  double ctkX, ctkY;               //FK時のコーチから指示されたキック位置
                                    //現在はGKのみが採用

  double ctmyX, ctmyY;              //FK時のコーチから指示された自位置
                                    //現在はGKのみが採用
 
  boolean freekickPosChanged = false;//コーチからの指示があったかどうか
  


/***************************************************************************/
  public InfoManager(String tname,int uNum,String s){
    /*
    if(Generic.DEBUG){   // デバッグ用
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

    // 位置関係
    x = 0;y = 0;
    bodyFaceDir = 0;headFaceDir = 0; // 体,首の向き ***** 絶対座標 *****
    headAngle = 0; // 首の向き  ***** 体中心座標 *****
    speed = vx = vy = pvx = pvy = 0;
    ax = ay = ux = uy = 0;
    pBallVX = pBallVY = 0;


    // カウンタ
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

    // see と sense の同期を取るための制御の初期化
    nextBodyAbsDir = UNKNOWN;
    nextHeadRelDir = UNKNOWN;
    nextHeadAbsDir = UNKNOWN;
  }

/***************************************************************************/

  void calcFaceDir(){  // headFaceDir,bodyFaceDir の計算
    double tmpDir = 0;

    //ライン情報を持ってなければ更新しない
    // updateByTurn() だけで十分でしょ
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

    if(this.side.equals("r"))      // 自サイドが右のとき
      tmpDir += ((tmpDir > 0) ? -180.0 : 180.0);


    // 予測される角度と大幅に異なるときは更新をしない
    // ためのコードを後から追加…するといいかもね

    this.headFaceDir = tmpDir;

    /*
    if(Generic.DEBUG){    // デバッグ用
      System.out.println("headFaceDir : " + this.headFaceDir);
    }
    */

    this.bodyFaceDir = Generic.normDir(this.headFaceDir - this.headAngle);

    /*
    if(Generic.DEBUG){    // デバッグ用
      System.out.println("bodyFaceDir : " + this.bodyFaceDir);
    }
    */

  }

/***************************************************************************/

  void calcPos(){ // 現在位置の計算
    FlagInfo[] flags = this.seeInfoParser.getFlags();
    int flagNumber = this.seeInfoParser.getFlagNum();
    // int goalNum = this.seeInfoParser.getGoalNum();

    // フラグもゴールも見えないときは更新しない
    // updateEtc() で十分だと思う
    if(flagNumber == 0)
      return;

    if (flagNumber > 2){  // 10?
      // 三角測量による計算
      double myX,myY;          // 自分の座標
      double xa = 0,ya = 0;    // 1 番目に近いフラグの x 座標 y 座標
      double xb = 0,yb = 0;    // 2 番目に近いフラグの x 座標 y 座標

      xa = flags[0].getX();
      ya = flags[0].getY();

      xb = flags[1].getX();
      yb = flags[1].getY();

      double angle; //フラグの相対角度

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
      // フラグ情報が 1 本しかない時の解析 一本測量

      double xa = 0,ya = 0;    //解析するフラグの x 座標 y 座標
      
      xa = flags[0].getX();
      ya = flags[0].getY();
      
      double angle; //フラグの絶対角
      
      angle = Generic.normDir(headFaceDir + flags[0].getDir());
      
      this.x = xa - flags[0].getDist() * Math.cos(angle * Math.PI / 180);
      this.y = ya - flags[0].getDist() * Math.sin(angle * Math.PI / 180);
    }

    // 上記の計算はラインぎわで非常に怪しいので強制修正
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

  void calcV(){   // 速度の計算
    this.vx = this.speed * Math.cos(Generic.digToRad(this.bodyFaceDir));
    this.vy = this.speed * Math.sin(Generic.digToRad(this.bodyFaceDir));

    /*
    if(Generic.DEBUG){   // デバッグ用
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
  public double normDir(double direction){  // 角度の正規化(Genericと同じ)
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

    // 目標とするスピードに達するための DashPower
    // ただし,そのスピードm/cycle で移動するわけではないことに注意
    // 役にたたないかも?
  public double getDashPowerBySpeed(double targetSpeed){
    double actualPower;

    actualPower = (targetSpeed / Generic.PLAYER_DECAY - this.speed)
                   / Generic.DASH_POWER_RATE;

    return actualPower / this.effort;
  }

/***************************************************************************/

  // 距離 distance メートルを移動するための DashPower を計算
  // つまり m/cycle で移動したいならこっちを使う
  public double getDashPowerByMoveDist(double distance){
    double actualPower;
    
    actualPower = (distance - this.speed) / Generic.DASH_POWER_RATE;
    
    return actualPower / this.effort;
  }

/***************************************************************************/


  // actualAngle 度回転するための moment を計算
  public double getMoment(double actualAngle){
    return actualAngle * (1.0 + Generic.INERTIA_MOMENT * this.speed);
  }

/***************************************************************************/

  public void getKickArgByAccel(double AX,double AY,
                                NSobject power,NSobject dir){ // NSobject!!
    double adir = Generic.RADIAN * Math.atan2(AY, AX);
                                               //必要な加速度の方向 絶対座標系
    
    dir.setNS(Generic.normDir(adir - this.bodyFaceDir)); // 体中心座標系に直す

    double a = Math.sqrt( AX * AX + AY * AY ); // 必要な加速度の大きさを求める

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

  // ボールを現在位置から X,Y メートル移動させるための力と方向を求める
  // 結果は名前渡しであることに注意  X,Y は絶対座標系
  public void getKickArgByMoveDist(double X,double Y,
                                   NSobject power,NSobject dir){ //NSobject!!

    double ax = X - this.ball.getVX();  // とりあえず絶対座標系での
    double ay = Y - this.ball.getVY();  // 必要な加速度を求める

    /*
    double adir = Generic.RADIAN * Math.atan2(ay, ax);
                                           //必要な加速度の方向 絶対座標系
    
    dir.setNS(Generic.normDir(adir - this.bodyFaceDir); // 体中心座標系に直す

    double a = Math.sqrt( ax * ax + ay * ay ); // 必要な加速度の大きさを求める

    power.setNS(a / (Generic.KICK_POWER_RATE *
		 (1 - 0.25 * Math.abs((Generic.normDir(getAbsDir(this.ball)
                                                              - bodyFaceDir)))
		  / 180.0 - 0.25 * (getBallDist()
                                    - Generic.PLAYER_SIZE - Generic.BALL_SIZE)
		  / Generic.KICKABLE_MARGIN));

    if(power.getNS() > 100)
      power.setNS(100);
    */

    getKickArgByAccel(ax,ay,power,dir);  // 結果は名前渡しであることに注意 ?? 
  }

/***************************************************************************/
  // speed は初速 X,Yは目標の絶対座標
  public  void getKickArgByDest(double X,double Y,double speed,
			  NSobject power,NSobject dir){
    dir.setNS(Generic.RADIAN * Math.atan2
                                (Y - this.ball.getY(),X - this.ball.getX()));
    double ballVX = speed * Math.cos(Generic.digToRad(dir.getNS()));
    double ballVY = speed * Math.sin(Generic.digToRad(dir.getNS()));
    
    double ax = ballVX / Generic.BALL_DECAY - this.ball.getVX();
    double ay = ballVY / Generic.BALL_DECAY - this.ball.getVY();

    getKickArgByAccel(ax,ay,power,dir); // 結果は名前渡しであることに注意??
  }

/***************************************************************************/

  // (X,Y)へPowerShotを撃つためのKickの引数を求める
  // save は溜め動作かどうか
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
                           // 結果は名前渡しであることに注意?? NSobject!!
      return;
    }

    Generic.getCrossPoint(r,destRelX,destRelY,centRelX,centRelY,
		          tmpXY1,tmpXY2);                          // Point !!

    /*
    if(Generic.DEBUG){   // デバッグ用
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
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("InfoManager::getKickArgForPowerShot() keepDir : "
	                  + keepDir); 
      System.out.println("InfoManager::getKickArgForPowerShot() keepDist : "
	                  + keepDist);
    }
    */

    getKickArgByKeepDir(keepDir,keepDist,power,dir);  // NSobject??

  }

/***************************************************************************/

  // ボールを相対方向 keepDir, 相対距離 keepDist に移動させる
  // 直接移動できない場合は接線方向へ
  public void getKickArgByKeepDir(double keepDir,double keepDist,
			          NSobject power,NSobject dir){ // NSobject!!

    double margin = Generic.BALL_SIZE + 0.2;  // ちょっと適当
                                              // 相対角度  ballのままでOK?
    double ballDir = Generic.normDir(getAbsDir(this.ball) - this.bodyFaceDir);

    double ballDist = getBallDist();          // ballのままでOK?

    // 目的の場所の X,Y 相対座標
    double destRelX = vx + 
	keepDist * Math.cos(Generic.digToRad(Generic.normDir(keepDir 
                                                        + this.bodyFaceDir)));
    double destRelY = vy +
	keepDist * Math.sin(Generic.digToRad(Generic.normDir(keepDir
                                                        + this. bodyFaceDir)));
    // ボールの X,Y 相対座標
    double ballRelX = this.ball.getX() - x;
    double ballRelY = this.ball.getY() - y;

    /*
    if(Generic.DEBUG){   // デバッグ用
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

    // ボールから目的の場所への距離  変数名紛らわしいので注意!
    double destDist = Math.sqrt(Math.pow(destRelX - ballRelX,2)
			   + Math.pow(destRelY - ballRelY,2));

    // ボールから目的の場所への方向を計算
    double destDir = Generic.RADIAN * Math.atan2(destRelY - ballRelY,
                                                 destRelX - ballRelX);

    double mx,my;  // ボールを移動させる距離 絶対座標

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
   if(Generic.DEBUG){   // デバッグ用
     System.out.println("InfoManager::getKickArgByKeepDir() mx : " + mx
	                 + " my : " + my);
   }
   */

    getKickArgByMoveDist(mx,my,power,dir);  // NSobject??

  }

/***************************************************************************/
  // destDir,destDist は ボールからの絶対角度,相対距離であることに注意
  public boolean adjustDestToAvoidBody(NSobject destDir,NSobject destDist){
                                                                // NSobject??
    boolean direct = Generic.NS_TRUE;
    double upperDir,lowerDir;
    double margin = Generic.BALL_SIZE + 0.15;  // ちょっと適当
                                               // 相対角度
    double ballDir = Generic.normDir(getAbsDir(this.ball) - this.bodyFaceDir);
    double ballDist = getBallDist();

    // 接線方向を計算する
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
   if(Generic.DEBUG){   // デバッグ用
     System.out.println("InfoManager::adjustDestToAvoidBody() ballDir : "
	                 + ballDir);
     System.out.println("InfoManager::adjustDestToAvoidBody() upperDir : "
	                 + upperDir);
     System.out.println("InfoManager::adjustDestToAvoidBody() lowerDir : "
	                 + lowerDir);
   }
   */

    // 今の位置から直接目的の場所に蹴れるかどうか
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

    // 直接蹴れない場合は前方に近い接線方向に蹴る
    if(!direct){
      if(Math.abs(upperDir) <= Math.abs(lowerDir))
	destDir.setNS(Generic.normDir(upperDir + this.bodyFaceDir));
      else
	destDir.setNS(Generic.normDir(lowerDir + this.bodyFaceDir));

      destDist.setNS(1.5 * Math.sqrt(ballDist * ballDist
                                 - Math.pow(Generic.PLAYER_SIZE + margin,2)));
    }

    /*
   if(Generic.DEBUG){   // デバッグ用
     if(direct)
       System.out.println("InfoManager::adjustDestToAvoidBody() Direct.");
     else
       System.out.println("InfoManager::adjustDestToAvoidBody() NonDirect!");
   }
   */

    return direct;

  }

/***************************************************************************/

  // フィールド内かどうか
  public boolean isInField(){
    // 見えているラインが1本ならフィールド内 0,2本ならフィールド外
    if((this.seeInfoParser.getLineNum() % 2) == 1){
      return Generic.NS_TRUE;
    }
    else {
      return Generic.NS_FALSE;
    }
  }

/***************************************************************************/

  // 与えられたオブジェクトの方向(絶対座標)を返す
  // プレイヤーの(X,Y) オブジェクトの(X,Y) から計算
  public double getAbsDir(ObjectInfo obj){
    return Generic.RADIAN * Math.atan2(obj.getY() - y, obj.getX() - x);
  }

/***************************************************************************/

    // 与えられたオブジェクトへの距離を返す
    // プレイヤーの(X,Y) オブジェクトの(X,Y) から計算
    // これあやしいかも...大塚説
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

  // 身体情報を更新
  public void updateBodyInfo(){
    bodyTime = this.bodyInfoParser.getTime();
    this.angleWidth = this.bodyInfoParser.getAngleWidth();
    this.quality = this.bodyInfoParser.getQuality();
    this.stamina =this. bodyInfoParser.getStamina();
    this.effort = this.bodyInfoParser.getEffort();
    this.speed = this.bodyInfoParser.getSpeed();
                                  // 5.18以降はここを有効に
    this.headAngle = this.bodyInfoParser.getHeadAngle();

    this.kickCount = this.bodyInfoParser.getKickCount();
    this.dashCount = this.bodyInfoParser.getDashCount();
    this.turnCount = this.bodyInfoParser.getTurnCount();
    this.sayCount = this.bodyInfoParser.getSayCount();
    this.turnNeckCount = this.bodyInfoParser.getTurnNeckCount();

    calcV();

    // See情報の方がSence情報より先に来ることを考慮して
    if(bodyTime > seeTime){
      this.x += this.vx / Generic.PLAYER_DECAY;
      this.y += this.vy / Generic.PLAYER_DECAY; 
    }

    // ここでターンとSeeの同期をとる

    if (nextBodyAbsDir != UNKNOWN){
      this.bodyFaceDir = Generic.normDir(this.bodyFaceDir + nextBodyAbsDir);
      // self.setBodyAbsDir(Generic.normDir(getBodyAbsDir() + nextBodyAbsDir));
      // 体の回転を考慮に入れて,ボールの角度を修正
      ball.adjustBallDir(nextBodyAbsDir);
      nextBodyAbsDir = UNKNOWN;
    }
    if (nextHeadRelDir != UNKNOWN){
      // HeadRelDir は毎ターン更新されているので,修正の必要なし
      // 体の回転を考慮に入れて,ボールの角度を修正
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

  // 視覚情報を更新
  public void updateSeeInfo(){
    this.prevSeeTime = this.seeTime;
    this.seeTime = this.seeInfoParser.getTime();

    // ここでターンとSeeの同期をとる
    // See情報の方がSence情報より先に来ることを考慮して
    if(seeTime > bodyTime){
      if(nextHeadRelDir != UNKNOWN)
        headAngle += nextHeadRelDir;
      nextBodyAbsDir = nextHeadRelDir = nextHeadAbsDir = UNKNOWN;
    }

    calcFaceDir();
    calcPos();

    // ボールの情報を持っているときはボールの情報を更新
    if(this.seeInfoParser.hasBallInfo()){
      this.knowsBallInfo = Generic.NS_TRUE;
      this.ballShadow = this.ball;  // 前に見たボールの情報は残しておく
      this.ball = this.seeInfoParser.getBallInfo(); // ballの中身生成
      this.ball.calcPos(this.x,this.y,this.headFaceDir);

      // 前に見たボールの情報と今のボールの情報との間にどれだけ時間が
      // 経っているか調べる    うまくいかないわこれ
      /*
      int blinkTime = this.ball.getTime() - this.ballShadow.getTime();
	if(blinkTime <= this.puSize && blinkTime != 0){
	  double ballMX,ballMY;
	  double totalUX = 0,totalUY = 0;
	  //まず自分がこの間にどれだけ移動したか計算
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
	  // seeTime 時の updateEtc() が行われているかどうか
	  if(this.lastUpdateEtcTime == this.seeTime){
	    this.ball.calcV(this.pvx,this.pvy);
	  } else {
	      this.ball.calcV(this.vx,this.vy);
	  }
	  if(getBallDist() < Generic.KICKABLE_AREA){ 
                                                     // Change情報は信用しない
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
        if(Generic.DEBUG){   // デバッグ用
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
    // プレイヤー情報の更新
    int playerNum = this.seeInfoParser.getPlayerNum();
    PlayerInfo player;
    
    this.playerTable.makeShadows(); // 残像を残す

    for(int i = 0; i < playerNum; i++){
      player = this.seeInfoParser.getPlayerInfo(i);
      player.calcPos(this.x,this.y,this.headFaceDir);
      String tname = player.getTeamname();
      int uNum = player.getUniformNum();
      if(tname.equals(""))
	this.playerTable.addUnknownPlayer(player);
      else if(tname.equals(teamname)){
	if(uNum != 0)   //  背番号が 0 でなければ
	  this.playerTable.setTeammateWithUNum(player,uNum);
	else
	  this.playerTable.addTeammate(player);
      }
      else {
	if(uNum != 0)   //  背番号が 0 でなければ
	  this.playerTable.setOpponentWithUNum(player,uNum);
	else
	  this.playerTable.addOpponent(player);
      }
    }

    this.playerTable.classifyUnknownPlayers(this.seeTime - prevSeeTime,
                                            this.x,this.y,
                                            this.headFaceDir,this.angleWidth);

    /*    
    if(Generic.DEBUG2){   // デバッグ用2
      playerTable.printInfo();
    }
    */

  }


 /***************************************************************************/

   // 聴覚情報を更新
   public void updateHearInfo(){
     int playmodeOld = this.playmode;

     this.hearTime = this.hearInfoParser.getTime();

     if(this.hearInfoParser.fromReferee()){
       this.playmode = this.hearInfoParser.getPlaymode();

       //  playmodeがplay_on以外へ移行したら戦略テーブルを初期化 
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

         // ボールの情報 どうやら lookAtBall のおかげで役に立ってない
         if(token.equals("NIT_ball")){
           int t = Integer.valueOf(subTokenizer.nextSToken()).intValue();
	   // 自分の見たボール情報より新しければ採用
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

         /*  よくわからないのでとりあえず除く。視覚情報とかぶる？
         else if(token.equals("player")){
           if(Integer.valueOf(subTokenizer.nextSToken()).intValue()
		 - this.hearTime < 3){
             // 味方のポジション情報  学習用…ともかぎらんか
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

	       if(token.equals("opponent"))            // Array Error防ぎ用
	         this.playerTable.addOpponent(opponent);
	       else
	         this.playerTable.addUnknownPlayer(opponent);
	     }
	   }
         }
         */

         //  チームメイトの協調戦略に関する情報
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
       //ここにコーチからの命令による行動をかく
       String message;
       message = this.hearInfoParser.getMessage();

       STokenizer tokenizer = new STokenizer(message);

       while(tokenizer.hasMoreSTokens()){
         String token;
         token = tokenizer.nextSToken();
         STokenizer subTokenizer = new STokenizer(token);
         token = subTokenizer.nextSToken();
       
	 // ポジション情報
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
           // とりあえずローカル変数に格納。まだ利用はしてない。
           // int time;
           String sName;
           String eval;
           int unum;
   
           // time = Integer.valueOf(subTokenizer.nextSToken()).intValue();
	   unum = Integer.valueOf(subTokenizer.nextSToken()).intValue();
           if(unum == uniformNum){
	     sName = subTokenizer.nextSToken();
             eval = subTokenizer.nextSToken();
	     // あとtimeも保持しないといかんね。
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

  // Move による位置情報の更新
  public void updateByMove(double X,double Y){
    this.x = X;
    this.y = Y;
    this.vx = 0.0;
    this.vy = 0.0;
  }
    
/***************************************************************************/

    // キックによるボール情報の更新
  public void updateByKick(double power, double dir){
    if(getBallDist() <= Generic.KICKABLE_AREA){
      double adir = Generic.normDir(bodyFaceDir + dir);// 蹴る方向の絶対座標系

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
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("ballAX : " + this.ball.getAX());
        System.out.println("ballVY : " + this.ball.getAY());
      }
      */

    }
  }

/***************************************************************************/

  // ダッシュによる加速度とスタミナの更新
  public void updateByDash(double power){
    if(power > this.stamina)       // パワーよりスタミナが少ない場合は
      power = this.stamina;        // パワーをスタミナ量に制限

    this.stamina -= power;
    
    power *= Generic.DASH_POWER_RATE;

    this.ax = power * Math.cos(Generic.digToRad(this.bodyFaceDir));
    this.ay = power * Math.sin(Generic.digToRad(this.bodyFaceDir));
  }
    
/***************************************************************************/

  // ターンによる bodyFaceDir,headFaceDir の更新
  public void updateByTurn(double moment){
    double actualAngle = moment / (1.0 + Generic.INERTIA_MOMENT * this.speed);
    
    // this.bodyFaceDir = Generic.normDir(this.bodyFaceDir + actualAngle);
    // this.headFaceDir = Generic.normDir(this.headFaceDir + actualAngle);

    nextBodyAbsDir = actualAngle;
    nextHeadAbsDir = actualAngle;
  }
    
/***************************************************************************/

  // 首回しによる headFaceDir,headAngle の更新
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

    // headAngle = nextHeadRelDirです
  }

/***************************************************************************/

  // 現在位置,ボール情報の更新   コマンドを発行した後に行うこと
  // (8/22)↑を変更して,sense情報の更新直前にやる。
  // ただしsenseよりもseeが先にくることも考慮する。
  public void updateEtc(){
    for(int i = this.puSize - 1; i > 0; i--){
      this.pux[i] = this.pux[i - 1];
      this.puy[i] = this.puy[i - 1];
    }
    this.pux[0] = this.ux;
    this.puy[0] = this.uy;
    
    this.ux = this.vx + this.ax;
    this.uy = this.vy + this.ay;

    this.pvx = this.vx;  // 一つ前の速度を残しておく
    this.pvy = this.vy;

    this.ax = 0;
    this.ay = 0;

    //this.x = this.x + this.ux;
    //this.y = this.y + this.uy;
     
    // 速度はsenseで更新される
    this.vx = this.ux * Generic.PLAYER_DECAY;
    this.vy = this.uy * Generic.PLAYER_DECAY;

    this.pBallVX = this.ball.getVX();
    this.pBallVY = this.ball.getVY();


    //  チームメイトの協調戦略テーブルの更新
    this.strategyTable.updateStrategyTable();
    //  自分の協調戦略の更新
    strategyNextState();

    // sense情報の更新直前にやる。
    this.lastUpdateEtcTime = this.bodyTime; // 最後に更新した時間


    // ワールドモデルの更新。senseよりもseeが先に来たときはやらなくてよし
    // lastUpdateEtcTime < seeTime ならreturnとしたいところだが、
    // beforeKickOffなど時間が進まない場合があるので、無理やり。
    // いい考えが浮かばないので毎回更新する。誤差は気にしない。
      //  周囲のプレイヤー情報の更新
      playerTable.updatePlayerTable();

    // ボール情報の更新。無理やり。
    // 問題はbeforeKickOffなど時間が進まない場合。でもボールはそのとき
    // 停止している。たぶん。
    if(this.ball.getTime() < lastUpdateEtcTime){
      this.ball.calcNextState();

      // ボールとの衝突の処理
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

  // 首の向きの絶対方向が欲しい
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
     // 1サイクル後に0.985m以内にある場合Kickable
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
     // 1サイクル後0.685m以内にある場合Kickable
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
    // this.x + this.vx = 1サイクル後の自分の座標
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

  // ボールを追う必要があるかどうか
  // n 人以上の味方が自分よりボールに近ければ必要なし
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

  // 指定した座標に自分がいるかどうか
  public boolean onThePos(double X,double Y){
    double margin = 1.0;  // 誤差範囲
    
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
    // ちょっと Y 軸をせばめてみた
    if(this.ball.getPredX(t) <= - Generic.PENALTY_AREA_X_WIDTH * 0.5 &&
       Math.abs(this.ball.getPredY(t))
                                 <= Generic.PENALTY_AREA_Y_WIDTH * 0.5 - 4.0)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }
    
/***************************************************************************/

  // 指定した座標の方向を求める 相対
  public double getDirOfThePos(double X,double Y){
    double absDir = Generic.RADIAN * Math.atan2(Y - this.y,X - this.x);
    
    return Generic.normDir(absDir - this.bodyFaceDir);
  }

/***************************************************************************/

  // 指定した座標の距離を求める 相対
  public double getDistOfThePos(double X,double Y){
    return Math.sqrt(Math.pow(X - this.x,2) + Math.pow(Y - this.y,2));
  }

/***************************************************************************/

  // t サイクル後のボールとの距離
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

  // t サイクルで進める距離
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
    // distはtサイクル後のボールの距離

    double margin = 50.0;

    int playerNum = this.seeInfoParser.getPlayerNum();
    for(int i = 0; i < playerNum; i++){
      PlayerInfo player = this.seeInfoParser.getPlayerInfo(i);
      player.calcPos(this.x,this.y,this.headFaceDir);
	
      double playerDir = Generic.normDir(getAbsDir(player) - this.bodyFaceDir);
      double playerDist = Generic.DIST(this.x,this.y,
                                       player.getX(),player.getY());

      /*	
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("InfoManager::adjustDirToAvoidPlayer() dirDiff : "
  	                    + Generic.normDir(playerDir - dir.getNS()));
        System.out.println("InfoManager::adjustDirToAvoidPlayer() dir : "
	                    + dir.getNS());
        System.out.println("InfoManager::adjustDirToAvoidPlayer() playerDir : "
	                    + playerDir);
      }
      */

      // 敵プレイヤーとの距離 < tサイクル後のボールの距離 &&
      // 敵プレイヤーとの距離 < 5m &&
      // 敵プレイヤーとボールとの相対角(絶対値) < margin / 敵との距離
      // 最後が謎なんだよなあ
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
    if(Generic.DEBUG){   // デバッグ用
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

  public String getSelfString(){  // sprintfの置き換え
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

  public String getOpponentsForwardString(){  // sprintfの置き換え
    Point oppXY = new Point();
    this.playerTable.getPosOfOpponentsForward(oppXY);

    StringBuffer oppForwardString = new StringBuffer("(player ");
    oppForwardString.append(this.bodyTime).append(" ").append("opponent")
           .append(" ").append(oppXY.getPointX()).append(" ")
           .append(oppXY.getPointY()).append(")");
    return oppForwardString.toString();
  }

/*********************ここから追加メソッド**********************************/

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
     // 自分との相対速度が2m/s,ボールの角度(絶対値)が100度以下であれば,
     // trapできているとする。
     // ボールとの距離はisKickableで判定済みとする。00/6/15
     if(this.ball.getRelV() < 0.2 && getAbsDir(this.ball) < 100)
     //if(this.ball.getV() < 1.0)
       return Generic.NS_TRUE;
     else
       return Generic.NS_FALSE;
   }

/***************************************************************************/

   // 与えられたオブジェクトのtサイクル後の方向(絶対座標)を返す
   public double getAbsPredDir(int t,ObjectInfo obj){
     return Generic.RADIAN * Math.atan2(obj.getPredY(t) - this.y,
                                        obj.getPredX(t) - this.x);
   }

/***************************************************************************/

   public double getBallRelV(){
     return this.ball.getRelV();
   }

/***************************************************************************/

  public double getPredX(int t){     // 時間 t 後のX座標を予測
    double predX = x;
    double predVX = vx;
    
    for(int i = 0; i < t; i++){
      predX += predVX;
      predVX *= Generic.PLAYER_DECAY;
    }
    
    return predX;
  }

/***************************************************************************/

  public double getPredY(int t){     // 時間 t 後のY座標を予測
    double predY = y;
    double predVY = vy; 

    for(int i = 0; i < t; i++){
      predY += predVY;
      predVY *= Generic.PLAYER_DECAY;
    }
    
    return predY;
  }

 /***************************************************************************/

   // フリーの判定。視界内(距離指定)＋Neighborhood -0.5m内に敵がいるかどうか？
   public boolean getFreeFlag(double dist){
     double width;
     // viewModeに合わせて判定する視界範囲を決定
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
 //GK FreeKick用 add 0803 by Shinji Futamase
 //ここから 


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
  //ここまで
  //

/**************************************************************************/

   // t サイクルで進める距離
   public double getMovableDist(int t,double power,double dir){
     double dist = 0;
     double u = 0;
     // dir方向への速度成分を初期値に設定  00/6/19
     double V = this.speed * Math.cos(Generic.digToRad(dir));

     for(int i = 0; i < t; i++){
       u = V + power * Generic.DASH_POWER_RATE;
       if(u > Generic.PLAYER_SPEED_MAX)
         u = 1.0;
       dist += u;
       V = Generic.PLAYER_DECAY * u;
     }
     /*  // 速度の計算おかしいんじゃない？
     for(int i = 0; i < t; i++){
       dist = dist + V + power * Generic.DASH_POWER_RATE;
       V *= Generic.PLAYER_DECAY;
     }
     */

     return dist;
   }

/***************************************************************************/

  // 指定した座標の方向を求める 絶対
  public double getDirOfThePos2(double X,double Y){
    double absDir = Generic.RADIAN * Math.atan2(Y - this.y,X - this.x);
    
    return absDir;
  }

/***************************************************************************/

  // 5/20 敵がシュートを打ってくる可能性があるかどうか
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

  // キーパー用
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

  // キーパー用 相対でなく絶対を返す
  // １サイクル後の自分の座標に変更(8/24)
  public double getPredBallDir2(int t){
    return Generic.normDir(Generic.RADIAN 
                     * Math.atan2(this.ball.getPredY(t)- this.y,
                               this.ball.getPredX(t) - this.x));
  }

  /***************************************************************************/

  // キーパー用
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
    /*  // 一サイクル後の自分の位置は (x,y) + (vx,vy)でいいと思う。
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
    // distはtサイクル後のボールの距離
    // X,Yはtサイクル後のボールの座標
    double margin = 50.0;

    int playerNum = this.seeInfoParser.getPlayerNum();
    for(int i = 0; i < playerNum; i++){
      PlayerInfo player = this.seeInfoParser.getPlayerInfo(i);
      player.calcPos(this.x,this.y,this.headFaceDir);
	
      double playerDir = Generic.normDir(getAbsDir(player) - this.bodyFaceDir);
      double playerDist = Generic.DIST(this.x,this.y,
                                       player.getX(),player.getY());
      double playerToBall = Generic.DIST(X,Y,player.getX(),player.getY());

      // 敵プレイヤーとの距離 < tサイクル後のボールの距離 &&
      // 敵プレイヤーとの距離 < 5m &&
      // 敵プレイヤーとボールとの相対角(絶対値) < margin / 敵との距離
      // 最後が謎なんだよなあ
      // というわけで敵が自分よりボールに近かったらを追加。
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

/**********************ここから協調戦略情報*********************************/

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
      // ボールの信頼度が0.1以上あればその残像を追うようにターンする。
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
        // とりあえず時計回りに75度回転
        searchFlag = 1;
        return 75;
      }
    }
    // 時計回りに回転していた場合,継続して回転
    else if(searchFlag == 1){
      return 75;
    }
    // 逆
    else{
      return -75;
    }
  }

}



