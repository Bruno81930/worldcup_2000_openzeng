// Player.java

package playerP;

import java.net.*;
import genericP.*;
import stokenizerP.*;
import info_managerP.*;

/********************************************************/
/*         Player   サッカーエージェント用基本クラス    */
/********************************************************/

public class Player{
  String teamname;
  int uniformNum;
  String servName;
  boolean goalie;
  String side;
  float version;

  String commands;

  int port;
  InetAddress servAddress;   // サーバーのIPアドレス  取得は init
  DatagramSocket dgsock;     // データグラムソケットの作成は init で

  public InfoManager infoManager;   // 生成はインスタンス生成時

  static int lastCycleTime1 = 0;
  static int lastCycleTime2 = 0;
  static double lastAvoidTime1 = 0;
  static double lastAvoidTime2 = 0;

  // ダッシュ力調整用
  double dashPowerRate = 1.0;
  double kickPowerRate = 1.0;

/***************************************************************************/

  public Player(String tname,String sname,int p,float ver,boolean g){
    goalie = g;
    version = ver;
    init(tname,sname,p);
    connect(ver);

    infoManager = new InfoManager(teamname,uniformNum,side);
  }

/***************************************************************************/

  public Player(String tname,int unum,String sname,int p){    // reconnect用
    uniformNum = unum;
    init(tname,sname,p);
    reconnect();

    infoManager = new InfoManager(teamname,uniformNum,side);
  }

/***************************************************************************/

  void init(String tname,String sname,int p){
    this.teamname = tname;
    this.servName = sname;

    this.port = p;

    try {                      // 例外検出  いるかな？ 
      // サーバーのIPアドレス取得
      this.servAddress = InetAddress.getByName(this.servName);
      // DatagramSocketをバインド。 パケットの生成は送信時
      this.dgsock = new DatagramSocket();
      //     this.dgsock.connect(this.servAddress,this.port);  // 宛先固定
    } catch (Exception e) {
        System.err.println(e);
        System.exit(1);
    }
  }

/***************************************************************************/
  // サーバに接続。versionによる接続を追加 00/6/19
  void connect(float ver){
    StringBuffer strCommands;
    if(this.goalie){
      strCommands = new StringBuffer("(init ");
      strCommands.append(teamname).append(" (version ").append(ver)
                 .append(") (goalie))");
    }
    else {
      strCommands = new StringBuffer("(init ");
      strCommands.append(teamname).append(" (version ").append(ver)
                 .append("))");
    }

    this.commands = strCommands.toString();

    sendCommand();                 // パケット生成して送信

    receiveInit();
  }

/***************************************************************************/
  // リコネクト用
  void reconnect(){
    StringBuffer strCommands = new StringBuffer("(reconnect ");
    strCommands.append(teamname).append(" ").append(uniformNum).append(")");

    this.commands = strCommands.toString();

    sendCommand();

    receiveInit();
  }

/***************************************************************************/
  // init メッセージを受け取り ポート変更
  void receiveInit(){
    byte[] buff = new byte[Generic.MAX_DGPACKET_SIZE];
    String recvMassage;

    DatagramPacket dgpackR =
                    new DatagramPacket(buff,Generic.MAX_DGPACKET_SIZE,
                                                this.servAddress,this.port);
    
    try{

      // System.out.println(" receive : ");

      this.dgsock.receive(dgpackR);           // サーバーからのパケット受信

      this.port = dgpackR.getPort();          // 接続ポート変更
      this.servAddress = dgpackR.getAddress();
      //   this.dgsockR.disconnect();
      //   this.dgsockR.connect(this.servAddress,this.port);

    } catch (Exception e) {
        System.err.println(e);
        System.exit(1);
    }    

    recvMassage = new String(dgpackR.getData());

    STokenizer tokenizer = new STokenizer(recvMassage);
    String token = tokenizer.nextSToken();
    
    if(!token.equals("init") && !token.equals("reconnect")){
      System.out.println("Error : init failed.");
      System.exit(1);
    }
    
    this.side = tokenizer.nextSToken();
    this.uniformNum = Integer.valueOf(tokenizer.nextSToken()).intValue();
    token = tokenizer.nextSToken();
    
    // delete dgpack;
  }

/***************************************************************************/
  // コマンドの送信
  public void sendCommand(){

    byte[] sendBuff = this.commands.getBytes();

    DatagramPacket dgpackS =
           new DatagramPacket(sendBuff,sendBuff.length,
                                            this.servAddress,this.port);
    
    try {          
      // System.out.println(" send");

      this.dgsock.send(dgpackS);
    } catch (Exception e) {
        System.err.println(e);
        System.exit(1);
    }

    this.commands = "";
  }

/***************************************************************************/

  public String receiveMessage(){
    byte[] recvBuff = new byte[Generic.MAX_DGPACKET_SIZE];
    String recvMassage;

    DatagramPacket dgpackR =
                    new DatagramPacket(recvBuff,Generic.MAX_DGPACKET_SIZE,
                                                 this.servAddress,this.port);
    
    try {
      this.dgsock.receive(dgpackR);           // サーバーからのパケット受信
    } catch (Exception e) {
        System.err.println(e);
        System.exit(1);
    }

    recvMassage = new String(dgpackR.getData());
    return recvMassage;
  }

/***************************************************************************/

  public void move(double x,double y){
    StringBuffer str = new StringBuffer("(move ");  // sprintf!!
    str.append(x).append(" ").append(y).append(")");

    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByMove(x,y);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void dash(double power){
    double inputPower = dashPowerRate * power;
    StringBuffer str = new StringBuffer("(dash ");  // sprintf!!
    str.append(Double.toString(inputPower)).append(")");
    
    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByDash(power);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void turn(double moment){
    StringBuffer str = new StringBuffer("(turn ");  // sprintf!!
    str.append(moment).append(")");

    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByTurn(moment);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void turn2(double absDir){

    /*
    double moment = this.infoManager.getMoment(Generic.normDir(absDir
                                        - this.infoManager.getBodyFaceDir()));
    */

    double moment = this.infoManager.getMoment(absDir);

    StringBuffer str = new StringBuffer("(turn ");  // sprintf!!
    str.append(moment).append(")");

    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByTurn(moment);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void kickBall(double power,double direction){
    double inputKickPower = power * kickPowerRate;

    StringBuffer str = new StringBuffer("(kick ");  // sprintf!!

    str.append(Double.toString(inputKickPower));
    str.append(" ").append(Double.toString(direction)).append(")");

    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByKick(power,direction);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void catchBall(double direction){
    StringBuffer str = new StringBuffer("(catch ");  // sprintf!!
    str.append(direction).append(")");

    this.commands = this.commands.concat(str.toString());

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void turnNeck(double angle){
    StringBuffer str = new StringBuffer("(turn_neck ");  // sprintf!!
    str.append(angle).append(")");

    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByTurnNeck(angle);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void turnNeck2(double angle){

    angle = Generic.normDir(angle - this.infoManager.getHeadAngle());

    StringBuffer str = new StringBuffer("(turn_neck ");  // sprintf!!
    str.append(angle).append(")");

    this.commands = this.commands.concat(str.toString());

    this.infoManager.updateByTurnNeck(angle);

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void say(String message){
    StringBuffer str = new StringBuffer("(say ");  // sprintf!!
    str.append(teamname).append(" ").append(message).append(")");

    this.commands = this.commands.concat(str.toString());

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("say : " + str.toString());
    }
    */

  }

/***************************************************************************/

  public void changeView(String angleWidth,String quality){
    StringBuffer str = new StringBuffer("(change_view ");  // sprintf!!
    str.append(angleWidth).append(" ").append(quality).append(")");

    this.commands = this.commands.concat(str.toString());

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println(str.toString());
    }
    */

  }

/***************************************************************************/

  public void bye(){
    this.commands = this.commands.concat("(bye)");
    System.exit(0);
  }

/***************************************************************************/

  public void dribble(double power,double angle){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Dribble.");
    }
    */

    double bodyFaceDir = this.infoManager.getBodyFaceDir();
    double x = this.infoManager.getX();
    double y = this.infoManager.getY();
    double vx = this.infoManager.getVX();
    double vy = this.infoManager.getVY();

    double keepDist = 0.65;  // 適当に決めてある

    double stamina = this.infoManager.getStamina();
    if(power > stamina)
      power = stamina;

    if(lastCycleTime1 == this.infoManager.getBodyTime() - 1){
      dash(power);
    }
    else {
      lastCycleTime1 = this.infoManager.getBodyTime();
      double predX = x + vx
	    + (vx * Generic.PLAYER_DECAY + power * Generic.DASH_POWER_RATE
	       * Math.cos(Generic.digToRad(bodyFaceDir)));
      double predY = y + vy
	    + (vy * Generic.PLAYER_DECAY + power * Generic.DASH_POWER_RATE
	       * Math.sin(Generic.digToRad(bodyFaceDir)));

      /*
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("dribble predX : " + predX);
        System.out.println("dribble predY : " + predY);
      }
      */
	
      double destX = predX + keepDist 
	    * Math.cos(Generic.digToRad(Generic.normDir(angle + bodyFaceDir)));
      double destY = predY + keepDist 
	    * Math.sin(Generic.digToRad(Generic.normDir(angle + bodyFaceDir)));

      /*
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("dribble destX : " + destX);
        System.out.println("dribble destY : " + destY);
	System.out.println("dribble ballX : " + this.infoManager.getBallX());
	System.out.println("dribble ballY : " + this.infoManager.getBallY());
      }
      */

      double mx = destX - this.infoManager.getBallX();
      double my = destY - this.infoManager.getBallY();

      /*
      if(Generic.DEBUG){   // デバッグ用
	System.out.println("dribble mx : " + mx);
	System.out.println("dribble my : " + my);
      }
      */

      double ax = ((mx - (1 + Generic.BALL_DECAY)
                     * this.infoManager.getBallVX()))
                     / (1 + Generic.BALL_DECAY);
      double ay = ((my - (1 + Generic.BALL_DECAY)
                     * this.infoManager.getBallVY()))
	             / (1 + Generic.BALL_DECAY);

      /*
      if(Generic.DEBUG){   // デバッグ用
	System.out.println("dribble ax : " + ax);
	System.out.println("dribble ay : " + ay);      
      }
      */

      NSobject kickPowerNS = new NSobject();  // NSobject!!
      NSobject kickDirNS = new NSobject();

      this.infoManager.getKickArgByAccel(ax,ay,kickPowerNS,kickDirNS);

      kickBall(kickPowerNS.getNS(),kickDirNS.getNS());
    }

  }

/***************************************************************************/
  //(X,Y) に 速度 speed で到達するボールを放つ
  public void powerShot(double X,double Y,double speed){
    int currentTime = this.infoManager.getBodyTime();

    NSobject powerNS = new NSobject();  // NSobject!!
    NSobject dirNS = new NSobject();

    /*
    if(lastCycleTime2 == currentTime - 1){
      trapBall();

      if(Generic.DEBUG){   // デバッグ用
        System.out.println("PowerShot Phase 2.");
      }

    }
    else if(lastCycleTime2 == currentTime - 2){
      this.infoManager.getKickArgForPowerShot(X,Y,powerNS,dirNS,
                                              Generic.NS_FALSE);
      kickBall(powerNS.getNS(),dirNS.getNS());

      if(Generic.DEBUG){   // デバッグ用
        System.out.println("PowerShot Phase 3.");
      }

    }
    else if(lastCycleTime2 == currentTime - 3){
      this.infoManager.getKickArgByDest(X,Y,10,powerNS,dirNS);
      kickBall(powerNS.getNS(),dirNS.getNS());

      if(Generic.DEBUG){   // デバッグ用
        System.out.println("PowerShot Phase 4.");
      }

    }
    else {
      this.infoManager.getKickArgForPowerShot(X,Y,powerNS,dirNS,
                                              Generic.NS_TRUE);
      kickBall(powerNS.getNS(),dirNS.getNS());
      lastCycleTime2 = currentTime;

      if(Generic.DEBUG){   // デバッグ用
        System.out.println("PowerShot Phase 1.");
      }

    }
    */

    if(lastCycleTime2 == currentTime - 1){
      this.infoManager.getKickArgByDest(X,Y,speed,powerNS,dirNS);
      kickBall(powerNS.getNS(),dirNS.getNS());

      /*
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("PowerShot Phase 4.");
      }
      */

    }
    else {
      this.infoManager.getKickArgForPowerShot(X,Y,powerNS,dirNS,
                                              Generic.NS_FALSE);
      kickBall(powerNS.getNS(),dirNS.getNS());
      lastCycleTime2 = currentTime;

      /*
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("PowerShot Phase 3.");
      }
      */

    }

  }

/***************************************************************************/

  public void shot(double X,double Y,double speed){
    NSobject powerNS = new NSobject();  // NSobject!!
    NSobject dirNS = new NSobject();

    this.infoManager.getKickArgByDest(X,Y,speed,powerNS,dirNS);
    kickBall(powerNS.getNS(),dirNS.getNS());
  }

/***************************************************************************/

  public void goToThePos(double X,double Y,double power){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Positioning.");
    }
    */

    boolean avoidFlag = Generic.NS_FALSE;  // 障害物を避ける動作をするとTrue
    double dirMargin = 5;  // 1 -> 5 シビアすぎ
    
    if(this.infoManager.onThePos(X,Y))
      return;

    if(lastAvoidTime1 == this.infoManager.getBodyTime() - 1){
      // 前に障害物を避けるための turn をしている場合
      dash(power);
      return;
    }

    NSobject dirNS = new NSobject(this.infoManager.getDirOfThePos(X,Y));

    avoidFlag = this.infoManager. 
	adjustDirToAvoidPlayer(this.infoManager.getDistOfThePos(X,Y),dirNS);
    avoidFlag = this.infoManager.
    	adjustDirToAvoidBall(this.infoManager.getDistOfThePos(X,Y),dirNS);

    if(avoidFlag)
      lastAvoidTime1 = this.infoManager.getBodyTime();

    if(Math.abs(dirNS.getNS()) > dirMargin){
      // if(avoidFlag)
      //  lastAvoidTime1 = this.infoManager.getBodyTime();
      turn(this.infoManager.getMoment(dirNS.getNS()));
    }
    else
      dash(power);
  }

/***************************************************************************/

  public void chaseBall(double power){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Chasing Ball.");
    }
    */

    boolean avoidFlag = Generic.NS_FALSE;  // 障害物を避ける動作をするとTrue

    int t = 0;
    NSobject kickPowerNS = new NSobject();  // NSobject!!
    NSobject kickDirNS = new NSobject();

    /*
    if(this.infoManager.isKickable()){
      this.infoManager.getKickArgByMoveDist(0,0,kickPowerNS,kickDirNS);
      kickBall(kickPowerNS.getNS(),kickDirNS.getNS());
      return;
    }
    */
    
    // この判定ってもしかしたらミスする可能性大！？
    // avoidturnした瞬間にけられてたらどうなるの？だめだよね。
    if(lastAvoidTime2 == this.infoManager.getBodyTime() - 1){
      // 前に障害物を避けるための turn をしている場合
      dash(power);
      return;
    }
    
    // ここのLoopは無駄だなあ
    while(t < 100){
      if(this.infoManager.getMovableDist(t,power,this.infoManager.getBallDir())
                                         > this.infoManager.getPredBallDist(t))
	break;
      t++;
    }
    /*
    while(t < 100){
      if(this.infoManager.getMovableDist(t,power) >
	   this.infoManager.getPredBallDist(t))
	break;
        t++;
    }
    */
    //t += 3;  // ちょっと余裕を持たせる

    double X = this.infoManager.getPredBallX(t);
    double Y = this.infoManager.getPredBallY(t);
    double predBallDir = this.infoManager.getDirOfThePos(X,Y);
    NSobject dirNS = new NSobject(predBallDir);
                                                               // NSobject!!
    /*
    avoidFlag = this.infoManager. 
	adjustDirToAvoidPlayer(this.infoManager.getPredBallDist(t),dirNS);
    */
    avoidFlag = this.infoManager. 
	adjustDirToAvoidPlayer(X,Y,this.infoManager.getPredBallDist(t),dirNS);

    double dirMargin = this.infoManager.getPredBallDist(t) / 4.0 + 20;
    // double dirMargin = 5;
    /*
    if(avoidFlag)
       lastAvoidTime2 = this.infoManager.getBodyTime();
    */

    if(avoidFlag){
      if(Math.abs(dirNS.getNS()) <= dirMargin)
	dash(power);
      else{
	lastAvoidTime2 = this.infoManager.getBodyTime();
        // System.out.println("avoidTurn!");
        turn(this.infoManager.getMoment(dirNS.getNS()));
      }
    }
    else{
      if(Math.abs(predBallDir) <= dirMargin)
	dash(power);
      else{
        // System.out.println("ballTurn!");
        turn(this.infoManager.getMoment(predBallDir));
      }
    }
    /*  
    if(Math.abs(dirNS.getNS()) <= dirMargin){
      dash(power);
    }
    else {
      if(avoidFlag){
        lastAvoidTime2 = this.infoManager.getBodyTime();
        System.out.println("avoidTurn!");
        turn(this.infoManager.getMoment(dirNS.getNS()));
      }
      else{
        System.out.println("ballTurn!");
        turn(this.infoManager.getMoment(predBallDir));
      }
    }
    */
  }

/***************************************************************************/

  public void turnBall(double angle){
    NSobject powerNS = new NSobject();  // NSobject!!
    NSobject dirNS = new NSobject();

    double keepDist = 0.65;  // 適当に
    
    this.infoManager.getKickArgByKeepDir(angle,keepDist,powerNS,dirNS);
    kickBall(powerNS.getNS(),dirNS.getNS());
  }

/***************************************************************************/

  public void trapBall(){
    NSobject powerNS = new NSobject();  // NSobject!!
    NSobject dirNS = new NSobject();

    this.infoManager.getKickArgByMoveDist(0,0,powerNS,dirNS);
    kickBall(powerNS.getNS(),dirNS.getNS());
  }

/***************************************************************************/

  public void lookAtBall(){
    //turnNeck2(this.infoManager.getBallDir());
    turnNeck2(this.infoManager.getPredBallDir(1));
  }

/***************************************************************************/

  public void lookForward(){
    turnNeck(-1 * this.infoManager.getHeadAngle());
  }

/***************************************************************************/

  public int getUniformNum(){
    return this.uniformNum;
  }

/***************************************************************************/

  public String getTeamname(){
    return this.teamname;
  }

/***************************************************************************/

  String getSide(){
    return this.side;
  }

/***************************************************************************/

  public void updateSeeInfo(String si){
    this.infoManager.parseSeeInfo(si);
    this.infoManager.updateSeeInfo();
  }

/***************************************************************************/

  public void updateBodyInfo(String bi){
    this.infoManager.parseBodyInfo(bi);
    this.infoManager.updateBodyInfo();
  }

/***************************************************************************/

  public void updateHearInfo(String hi){
    this.infoManager.parseHearInfo(hi);
    this.infoManager.updateHearInfo();
  }

/***************************************************************************/

  public void updateEtc(){
    this.infoManager.updateEtc();
  }

/***************************************************************************/
  /*
  public virtual void decideCommand(){
  }
  */

/***************************************************************************/

  // キーパー用 goToThePos．出来るだけボールを見失わないように
  // 後退等をしながら移動する．

  public void goaliegoToThePos(double dist_x,double dist_y){
    // 移動したい場所への絶対方向を得る
    double dirXY = this.infoManager.getDirOfThePos2(dist_x,dist_y);

    if (Math.abs(dist_x - this.infoManager.getX()) > 1){
      // 目標とする x 座標との差が 1 より大きい
      if (dist_x < this.infoManager.getX()){
	// 目標角度の基本軸の変換
	if (dirXY > 0){
	  dirXY = dirXY - 180;
	}
	else{
	  dirXY = dirXY + 180;
	}

	if (Math.abs(dirXY - this.infoManager.getBodyFaceDir())< 10){
	  dash(-50);lookAtBall();
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // 後ろ走り 1");   
	}
	else{
	  // ホントはここで,turn によりボールが見えなくなるなら止めるを
	  // 入れたい
	  turn2(Generic.normDir(dirXY - this.infoManager.getBodyFaceDir()));
	  lookAtBall();
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // 後ろ走り前準備 1");
	}
      }	      
      else {
	if (Math.abs(dirXY-this.infoManager.getBodyFaceDir()) < 10){
	  dash(100);lookAtBall();
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // 前走り");
	}
	else{
	  // ホントはここで,turn によりボールが見えなくなるなら止めるを
	  // 入れたい
	  turn2(Generic.normDir(dirXY-this.infoManager.getBodyFaceDir()));
	  lookAtBall();
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // 前走り前準備");
	}	  
      }
    }
    else if (Math.abs(dist_x - this.infoManager.getX()) < 1){
      if (dist_y > this.infoManager.getY()){
	if (Math.abs(this.infoManager.getBodyFaceDir() - 90) > 10){
	  // 角度修正
	  turn2(Generic.normDir(90-this.infoManager.getBodyFaceDir()));
	  lookAtBall();
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // 角度修正１");
	}
	else{
	  // 移動
	  dash(100);
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // ダッシュ１");	    
	}
      }
      else{
	if (Math.abs(this.infoManager.getBodyFaceDir() + 90) > 10){
	  // 角度修正
	  turn2(Generic.normDir(-90-this.infoManager.getBodyFaceDir()));
	  lookAtBall();
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // 角度修正２");
	}
	else{
	  // 移動
	  dash(100);
	  // System.out.println("#Debug# Time Unum :" + this.infoManager.getBodyTime() + " " + this.infoManager.getMyUnum() + " // ダッシュ２");	    
	}
      }
    }
  }

/***************************************************************************/

  public void setDashPowerRate(double val){
    if(val>1.0 || val <= 0.0){
      return;
    };
    dashPowerRate = val;
  }

/***************************************************************************/

  public void setKickPowerRate(double val){
    if(val>1.0 || val <= 0.0){
      return;
    };
    kickPowerRate = val;
  }



}



